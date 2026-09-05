//==============================================================================
// GamePad.cpp
//==============================================================================
#include "jxglib/USBHost/HID.h"

#if CFG_TUH_HID > 0
namespace jxglib::USBHost {

GamePad* GamePad::pXInputGamePad_ = nullptr;

namespace {

constexpr uint16_t XINPUT_DPAD_UP = 0x0001;
constexpr uint16_t XINPUT_DPAD_DOWN = 0x0002;
constexpr uint16_t XINPUT_DPAD_LEFT = 0x0004;
constexpr uint16_t XINPUT_DPAD_RIGHT = 0x0008;
constexpr uint16_t XINPUT_START = 0x0010;
constexpr uint16_t XINPUT_BACK = 0x0020;
constexpr uint16_t XINPUT_LEFT_THUMB = 0x0040;
constexpr uint16_t XINPUT_RIGHT_THUMB = 0x0080;
constexpr uint16_t XINPUT_LEFT_SHOULDER = 0x0100;
constexpr uint16_t XINPUT_RIGHT_SHOULDER = 0x0200;
constexpr uint16_t XINPUT_GUIDE = 0x0400;
constexpr uint16_t XINPUT_A = 0x1000;
constexpr uint16_t XINPUT_B = 0x2000;
constexpr uint16_t XINPUT_X = 0x4000;
constexpr uint16_t XINPUT_Y = 0x8000;

uint16_t read_le16(const uint8_t* p) {
	return static_cast<uint16_t>(p[0] | (static_cast<uint16_t>(p[1]) << 8));
}

int16_t read_le16s(const uint8_t* p) {
	return static_cast<int16_t>(read_le16(p));
}

float normalize_axis(int16_t value) {
	return value < 0 ? static_cast<float>(value) / 32768.0f : static_cast<float>(value) / 32767.0f;
}

uint32_t xinput_hat(uint16_t buttons) {
	const bool up = buttons & XINPUT_DPAD_UP;
	const bool down = buttons & XINPUT_DPAD_DOWN;
	const bool left = buttons & XINPUT_DPAD_LEFT;
	const bool right = buttons & XINPUT_DPAD_RIGHT;
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

} // namespace

GamePad::GamePad()
{
	ClearUsageAccessor();
	pXInputGamePad_ = this;
}

void GamePad::ClearUsageAccessor()
{
	xinputMounted_ = false;
	xinputReportChanged_ = false;
	xinputVID_ = 0;
	xinputPID_ = 0;
	xinputReportActive_ = false;
	::memset(xinputButton_, 0x00, sizeof(xinputButton_));
	::memset(xinputAxis_, 0x00, sizeof(xinputAxis_));
	xinputHatSwitch_ = 0;
	switchProMounted_ = false;
	switchProReportChanged_ = false;
	switchProVID_ = 0;
	switchProPID_ = 0;
	switchProDeviceAddress_ = 0;
	switchProInstance_ = 0;
	switchProReportActive_ = false;
	switchProReady_ = false;
	switchProInitStep_ = 0;
	switchProInitStarted_ = false;
	switchProControlPending_ = false;
	::memset(switchProButton_, 0x00, sizeof(switchProButton_));
	::memset(switchProAxis_, 0x00, sizeof(switchProAxis_));
	switchProHatSwitch_ = 0;
	pUsage_Button0 = pUsage_Button1 = pUsage_Button2 = pUsage_Button3 = &HID::UsageAccessor::None;
	pUsage_Button4 = pUsage_Button5 = pUsage_Button6 = pUsage_Button7 = &HID::UsageAccessor::None;
	pUsage_Button8 = pUsage_Button9 = pUsage_Button10 = pUsage_Button11 = pUsage_Button12 = &HID::UsageAccessor::None;
	pUsage_Axis0 = pUsage_Axis1 = pUsage_Axis2 = pUsage_Axis3 = pUsage_Axis4 = &HID::UsageAccessor::None;
	pUsage_Axis5 = pUsage_Axis6 = pUsage_Axis7 = pUsage_Axis8 = &HID::UsageAccessor::None;
	pUsage_HatSwitch = &HID::UsageAccessor::None;
	pUsage_ButtonA = pUsage_ButtonB = pUsage_ButtonX = pUsage_ButtonY = &HID::UsageAccessor::None;
	pUsage_ButtonLB = pUsage_ButtonRB = pUsage_ButtonLT = pUsage_ButtonRT = &HID::UsageAccessor::None;
	pUsage_ButtonLeft = pUsage_ButtonRight = pUsage_ButtonBack = pUsage_ButtonStart = pUsage_ButtonHome = &HID::UsageAccessor::None;
	pUsage_LeftX = pUsage_LeftY = pUsage_RightX = pUsage_RightY = &HID::UsageAccessor::None;
}

void GamePad::OnMount()
{
	const uint16_t vid = GetHID().GetVID();
	const uint16_t pid = GetHID().GetPID();
	if (vid == 0x057e && pid == 0x2009) {
		OnSwitchProMount(GetHID().GetDeviceAddress(), GetHID().GetInstance(), vid, pid);
	}
	pUsage_Button0 = &GetApplication().FindUsageAccessorRecursive(0x00090001);
	pUsage_Button1 = &GetApplication().FindUsageAccessorRecursive(0x00090002);
	pUsage_Button2 = &GetApplication().FindUsageAccessorRecursive(0x00090003);
	pUsage_Button3 = &GetApplication().FindUsageAccessorRecursive(0x00090004);
	pUsage_Button4 = &GetApplication().FindUsageAccessorRecursive(0x00090005);
	pUsage_Button5 = &GetApplication().FindUsageAccessorRecursive(0x00090006);
	pUsage_Button6 = &GetApplication().FindUsageAccessorRecursive(0x00090007);
	pUsage_Button7 = &GetApplication().FindUsageAccessorRecursive(0x00090008);
	pUsage_Button8 = &GetApplication().FindUsageAccessorRecursive(0x00090009);
	pUsage_Button9 = &GetApplication().FindUsageAccessorRecursive(0x0009000a);
	pUsage_Button10 = &GetApplication().FindUsageAccessorRecursive(0x0009000b);
	pUsage_Button11 = &GetApplication().FindUsageAccessorRecursive(0x0009000c);
	pUsage_Button12 = &GetApplication().FindUsageAccessorRecursive(0x0009000d);
	pUsage_Axis0 = &GetApplication().FindUsageAccessorRecursive(0x00010030);
	pUsage_Axis1 = &GetApplication().FindUsageAccessorRecursive(0x00010031);
	pUsage_Axis2 = &GetApplication().FindUsageAccessorRecursive(0x00010032);
	pUsage_Axis3 = &GetApplication().FindUsageAccessorRecursive(0x00010035);
	pUsage_HatSwitch = &GetApplication().FindUsageAccessorRecursive(0x00010039);
	pUsage_ButtonA = pUsage_Button0; pUsage_ButtonB = pUsage_Button1;
	pUsage_ButtonX = pUsage_Button2; pUsage_ButtonY = pUsage_Button3;
	pUsage_ButtonLB = pUsage_Button4; pUsage_ButtonRB = pUsage_Button5;
	pUsage_ButtonLT = pUsage_Button6; pUsage_ButtonRT = pUsage_Button7;
	pUsage_ButtonLeft = pUsage_Button8; pUsage_ButtonRight = pUsage_Button9;
	pUsage_ButtonBack = pUsage_Button10; pUsage_ButtonStart = pUsage_Button11; pUsage_ButtonHome = pUsage_Button12;
	pUsage_LeftX = pUsage_Axis0; pUsage_LeftY = pUsage_Axis1;
	pUsage_RightX = pUsage_Axis2; pUsage_RightY = pUsage_Axis3;
}

void GamePad::OnUmount()
{
	if (switchProMounted_) OnSwitchProUmount(switchProDeviceAddress_);
	ClearUsageAccessor();
}

void GamePad::OnReport()
{
	const HID::Report& report = GetReport();
	if (!report.buff) return;
	if (switchProMounted_) {
		OnSwitchProReport(switchProDeviceAddress_, report.buff, report.len);
		return;
	}
	OnXInputReport(0, report.buff, report.len);
}

void GamePad::OnXInputMount(uint8_t devAddr, uint16_t vid, uint16_t pid)
{
	(void) devAddr;
	if (!pXInputGamePad_) return;
	GamePad& gamePad = *pXInputGamePad_;
	gamePad.xinputMounted_ = true;
	gamePad.xinputReportChanged_ = true;
	gamePad.xinputVID_ = vid;
	gamePad.xinputPID_ = pid;
	gamePad.xinputReportActive_ = false;
	::memset(gamePad.xinputButton_, 0x00, sizeof(gamePad.xinputButton_));
	::memset(gamePad.xinputAxis_, 0x00, sizeof(gamePad.xinputAxis_));
	gamePad.xinputHatSwitch_ = 0;
}

void GamePad::OnXInputUmount(uint8_t devAddr)
{
	(void) devAddr;
	if (!pXInputGamePad_) return;
	pXInputGamePad_->xinputMounted_ = false;
	pXInputGamePad_->xinputReportChanged_ = false;
	pXInputGamePad_->xinputReportActive_ = false;
}

void GamePad::OnXInputReport(uint8_t devAddr, const uint8_t* reportBuff, uint16_t reportLen)
{
	(void) devAddr;
	if (!pXInputGamePad_ || !reportBuff || reportLen < 14) return;
	GamePad& gamePad = *pXInputGamePad_;

	// Xbox 360 wired reports are 00 14 followed by the fixed XInput gamepad layout.
	// Some HID-wrapped receivers expose 14 as a HID report ID instead, so accept both.
	const uint8_t* packet = nullptr;
	if (reportLen >= 14 && reportBuff[0] == 0x00 && reportBuff[1] == 0x14) {
		packet = reportBuff;
	} else if (reportLen >= 15 && reportBuff[0] == 0x14) {
		packet = reportBuff + 1;
	}
	if (!packet) return;

	const uint16_t buttons = read_le16(packet + 2);
	gamePad.xinputButton_[0] = buttons & XINPUT_A;
	gamePad.xinputButton_[1] = buttons & XINPUT_B;
	gamePad.xinputButton_[2] = buttons & XINPUT_X;
	gamePad.xinputButton_[3] = buttons & XINPUT_Y;
	gamePad.xinputButton_[4] = buttons & XINPUT_LEFT_SHOULDER;
	gamePad.xinputButton_[5] = buttons & XINPUT_RIGHT_SHOULDER;
	gamePad.xinputButton_[6] = packet[4] != 0;
	gamePad.xinputButton_[7] = packet[5] != 0;
	gamePad.xinputButton_[8] = buttons & XINPUT_LEFT_THUMB;
	gamePad.xinputButton_[9] = buttons & XINPUT_RIGHT_THUMB;
	gamePad.xinputButton_[10] = buttons & XINPUT_BACK;
	gamePad.xinputButton_[11] = buttons & XINPUT_START;
	gamePad.xinputButton_[12] = buttons & XINPUT_GUIDE;
	gamePad.xinputHatSwitch_ = xinput_hat(buttons);
	gamePad.xinputAxis_[0] = normalize_axis(read_le16s(packet + 6));
	gamePad.xinputAxis_[1] = -normalize_axis(read_le16s(packet + 8));
	gamePad.xinputAxis_[2] = normalize_axis(read_le16s(packet + 10));
	gamePad.xinputAxis_[3] = -normalize_axis(read_le16s(packet + 12));
	gamePad.xinputAxis_[4] = static_cast<float>(packet[4]) / 255.0f;
	gamePad.xinputAxis_[5] = static_cast<float>(packet[5]) / 255.0f;
	gamePad.xinputAxis_[6] = gamePad.xinputAxis_[7] = gamePad.xinputAxis_[8] = 0.0f;
	gamePad.xinputReportActive_ = true;
	gamePad.xinputReportChanged_ = true;
}

float GamePad::GetCookedAxis(const HID::UsageAccessor& usageAccessor) const
{
	if (!usageAccessor.IsValid()) return 0.0f;
	const int32_t minimum = usageAccessor.GetLogicalMinimum();
	const int32_t maximum = usageAccessor.GetLogicalMaximum();
	const int32_t value = usageAccessor.GetVariable(GetHID().GetReport());
	if (minimum < 0) return value < 0 ? static_cast<float>(value) / -minimum : static_cast<float>(value) / maximum;
	if (maximum == 0) return 0.0f;
	return minimum == 0 ? (static_cast<float>(value) / maximum) * 2.0f - 1.0f : static_cast<float>(value) / maximum;
}

} // namespace jxglib::USBHost
#endif
