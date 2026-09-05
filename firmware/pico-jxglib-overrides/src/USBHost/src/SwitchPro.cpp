//==============================================================================
// SwitchPro.cpp - Nintendo Switch Pro Controller USB HID support.
//==============================================================================
#include "jxglib/USBHost/HID.h"

#if CFG_TUH_HID > 0
namespace jxglib::USBHost {

namespace {

constexpr uint16_t kNintendoVid = 0x057e;
constexpr uint16_t kSwitchProPid = 0x2009;
constexpr uint8_t kReportUsbReply = 0x81;
constexpr uint8_t kReportInputFull = 0x30;
constexpr uint8_t kReportInputAck = 0x21;
constexpr uint8_t kReportUsbCommand = 0x80;
constexpr uint8_t kUsbEnable = 0x04;
constexpr uint8_t kUsbHandshake = 0x02;
constexpr uint8_t kHidReportTypeOutput = 0x02;

GamePad* s_switchProGamePad = nullptr;

float normalize_switch_axis(uint16_t value, bool invert)
{
	float normalized = (static_cast<float>(value) - 2048.0f) / 2047.0f;
	if (normalized < -1.0f) normalized = -1.0f;
	if (normalized > 1.0f) normalized = 1.0f;
	return invert ? -normalized : normalized;
}

uint32_t switch_hat(bool up, bool down, bool left, bool right)
{
	if (up && right) return 2;
	if (right && down) return 4;
	if (down && left) return 6;
	if (left && up) return 8;
	if (up) return 1;
	if (right) return 3;
	if (down) return 5;
	if (left) return 7;
	return 0;
}

bool send_switch_pro_report(GamePad& gamePad, const uint8_t* data, uint16_t len, const char* label)
{
	if (gamePad.switchProControlPending_ || len < 2 || data[0] != kReportUsbCommand) return false;
	if (!::tuh_hid_set_report(gamePad.switchProDeviceAddress_, gamePad.switchProInstance_,
		kReportUsbCommand, kHidReportTypeOutput, const_cast<uint8_t*>(data), len)) {
		::printf("[SWPRO] %s submit failed\n", label);
		return false;
	}
	gamePad.switchProControlPending_ = true;
	::printf("[SWPRO] %s sent (report=80 data=%u bytes)\n", label, static_cast<unsigned>(len));
	return true;
}

void switch_pro_initialize(GamePad& gamePad)
{
	static const uint8_t kEnable[64] = { kReportUsbCommand, kUsbEnable };
	static const uint8_t kHandshake[64] = { kReportUsbCommand, kUsbHandshake };
	static const uint8_t kSetFullMode[64] = {
		0x80, 0x92, 0x00, 0x31, 0x00, 0x00, 0x00, 0x00,
		0x01, 0x00, 0x00, 0x10, 0x40, 0x40, 0x00, 0x10,
		0x40, 0x40, 0x03, 0x30
	};
	static const uint8_t kSetPlayerLed[64] = {
		0x80, 0x92, 0x00, 0x31, 0x00, 0x00, 0x00, 0x00,
		0x01, 0x00, 0x00, 0x10, 0x40, 0x40, 0x00, 0x10,
		0x40, 0x40, 0x30, 0x01
	};

	switch (gamePad.switchProInitStep_) {
	case 0:
		if (send_switch_pro_report(gamePad, kEnable, sizeof(kEnable), "USB enable")) ++gamePad.switchProInitStep_;
		break;
	case 1:
		if (send_switch_pro_report(gamePad, kHandshake, sizeof(kHandshake), "USB handshake")) ++gamePad.switchProInitStep_;
		break;
	case 2:
		if (send_switch_pro_report(gamePad, kSetFullMode, sizeof(kSetFullMode), "full input mode")) {
			++gamePad.switchProInitStep_;
		}
		break;
	case 3:
		if (send_switch_pro_report(gamePad, kSetPlayerLed, sizeof(kSetPlayerLed), "player LED 1")) {
			++gamePad.switchProInitStep_;
			::printf("[SWPRO] initialization complete; waiting for report 0x30\n");
		}
		break;
	default:
		break;
	}
}

} // namespace

void GamePad::OnSwitchProMount(uint8_t devAddr, uint8_t iInstance, uint16_t vid, uint16_t pid)
{
	if (vid != kNintendoVid || pid != kSwitchProPid) return;
	s_switchProGamePad = this;
	switchProMounted_ = true;
	switchProReportChanged_ = false;
	switchProVID_ = vid;
	switchProPID_ = pid;
	switchProDeviceAddress_ = devAddr;
	switchProInstance_ = iInstance;
	switchProReportActive_ = false;
	switchProReady_ = false;
	switchProInitStep_ = 0;
	switchProInitStarted_ = false;
	switchProControlPending_ = false;
	::memset(switchProButton_, 0x00, sizeof(switchProButton_));
	::memset(switchProAxis_, 0x00, sizeof(switchProAxis_));
	switchProHatSwitch_ = 0;
	::printf("[SWPRO] mounted addr=%u inst=%u vid=%04x pid=%04x\n", devAddr, iInstance, vid, pid);
	::printf("[SWPRO] waiting for the controller USB-ready reply\n");
}

void GamePad::OnSwitchProUmount(uint8_t devAddr)
{
	if (!s_switchProGamePad || s_switchProGamePad->switchProDeviceAddress_ != devAddr) return;
	::printf("[SWPRO] unmounted addr=%u\n", devAddr);
	s_switchProGamePad->switchProMounted_ = false;
	s_switchProGamePad->switchProReportActive_ = false;
	s_switchProGamePad->switchProReady_ = false;
	s_switchProGamePad->switchProReportChanged_ = false;
	s_switchProGamePad->switchProInitStarted_ = false;
	s_switchProGamePad->switchProControlPending_ = false;
	s_switchProGamePad = nullptr;
}

void GamePad::OnSwitchProReport(uint8_t devAddr, const uint8_t* report, uint16_t len)
{
	if (!s_switchProGamePad || !report || len == 0 || s_switchProGamePad->switchProDeviceAddress_ != devAddr) return;
	GamePad& gamePad = *s_switchProGamePad;
	if (!gamePad.switchProInitStarted_) {
		gamePad.switchProInitStarted_ = true;
		::printf("[SWPRO] controller reply received; starting initialization\n");
		switch_pro_initialize(gamePad);
	}

	if (report[0] == kReportUsbReply) {
		::printf("[SWPRO] USB reply raw:");
		for (uint16_t i = 0; i < len && i < 16; ++i) ::printf(" %02x", report[i]);
		::printf("\n");
		if (len <= 10) {
			::printf("[SWPRO] short USB reply len=%u\n", static_cast<unsigned>(len));
			return;
		}
		report += 10;
		len -= 10;
	}

	if (report[0] == kReportInputAck) {
		::printf("[SWPRO] command ACK len=%u cmd=%02x\n", static_cast<unsigned>(len), len > 14 ? report[14] : 0xff);
		return;
	}
	if (report[0] != kReportInputFull) {
		::printf("[SWPRO] ignored report id=%02x len=%u data:", report[0], static_cast<unsigned>(len));
		for (uint16_t i = 0; i < len && i < 16; ++i) ::printf(" %02x", report[i]);
		::printf("\n");
		return;
	}
	if (len < 12) {
		::printf("[SWPRO] short full report len=%u\n", static_cast<unsigned>(len));
		return;
	}

	const uint8_t buttonsRight = report[3];
	const uint8_t buttonsShared = report[4];
	const uint8_t buttonsLeft = report[5];
	gamePad.switchProButton_[0] = buttonsRight & 0x08; // A
	gamePad.switchProButton_[1] = buttonsRight & 0x04; // B
	gamePad.switchProButton_[2] = buttonsRight & 0x02; // X
	gamePad.switchProButton_[3] = buttonsRight & 0x01; // Y
	gamePad.switchProButton_[4] = buttonsLeft & 0x40;  // L
	gamePad.switchProButton_[5] = buttonsRight & 0x40; // R
	gamePad.switchProButton_[6] = buttonsLeft & 0x80;  // ZL
	gamePad.switchProButton_[7] = buttonsRight & 0x80; // ZR
	gamePad.switchProButton_[8] = buttonsShared & 0x08;  // L stick
	gamePad.switchProButton_[9] = buttonsShared & 0x04;  // R stick
	gamePad.switchProButton_[10] = buttonsShared & 0x01; // Minus
	gamePad.switchProButton_[11] = buttonsShared & 0x02; // Plus
	gamePad.switchProButton_[12] = buttonsShared & 0x10; // Home
	gamePad.switchProHatSwitch_ = switch_hat(buttonsLeft & 0x02, buttonsLeft & 0x01,
		buttonsLeft & 0x08, buttonsLeft & 0x04);

	const uint16_t leftX = static_cast<uint16_t>(report[6] | ((report[7] & 0x0f) << 8));
	const uint16_t leftY = static_cast<uint16_t>((report[7] >> 4) | (report[8] << 4));
	const uint16_t rightX = static_cast<uint16_t>(report[9] | ((report[10] & 0x0f) << 8));
	const uint16_t rightY = static_cast<uint16_t>((report[10] >> 4) | (report[11] << 4));
	gamePad.switchProAxis_[0] = normalize_switch_axis(leftX, false);
	gamePad.switchProAxis_[1] = normalize_switch_axis(leftY, true);
	gamePad.switchProAxis_[2] = normalize_switch_axis(rightX, false);
	gamePad.switchProAxis_[3] = normalize_switch_axis(rightY, true);
	gamePad.switchProAxis_[4] = gamePad.switchProAxis_[5] = gamePad.switchProAxis_[6] = 0.0f;
	gamePad.switchProAxis_[7] = gamePad.switchProAxis_[8] = 0.0f;
	gamePad.switchProReportActive_ = true;
	gamePad.switchProReady_ = true;
	gamePad.switchProReportChanged_ = true;
	::printf("[SWPRO] input lx=%u ly=%u rx=%u ry=%u hat=%u\n", leftX, leftY, rightX, rightY,
		static_cast<unsigned>(gamePad.switchProHatSwitch_));
}

void GamePad::OnSwitchProSetReportComplete(uint8_t devAddr, uint8_t iInstance, uint8_t reportId, uint16_t len)
{
	if (!s_switchProGamePad || s_switchProGamePad->switchProDeviceAddress_ != devAddr ||
		s_switchProGamePad->switchProInstance_ != iInstance || reportId != kReportUsbCommand ||
		!s_switchProGamePad->switchProControlPending_) return;
	s_switchProGamePad->switchProControlPending_ = false;
	if (len == 0) {
		::printf("[SWPRO] control SET_REPORT failed at init step=%u\n",
			static_cast<unsigned>(s_switchProGamePad->switchProInitStep_));
		return;
	}
	::printf("[SWPRO] control SET_REPORT complete step=%u len=%u\n",
		static_cast<unsigned>(s_switchProGamePad->switchProInitStep_), static_cast<unsigned>(len));
	switch_pro_initialize(*s_switchProGamePad);
}

} // namespace jxglib::USBHost

extern "C" void tuh_hid_set_report_complete_cb(uint8_t devAddr, uint8_t iInstance,
	uint8_t reportId, uint8_t reportType, uint16_t len)
{
	if (reportType == 0x02) {
		jxglib::USBHost::GamePad::OnSwitchProSetReportComplete(devAddr, iInstance, reportId, len);
	}
}
#endif
