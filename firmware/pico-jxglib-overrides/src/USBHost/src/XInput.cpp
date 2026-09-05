//==============================================================================
// XInput.cpp - TinyUSB application class driver for Xbox 360 wire protocol.
//==============================================================================
#include "jxglib/USBHost/HID.h"

extern "C" {
#include "host/usbh_pvt.h"
}

namespace {

struct XInputInterface {
	uint8_t epIn;
	uint8_t epOut;
	uint8_t itfNum;
	uint8_t buffer[32];
};

static XInputInterface s_interfaces[CFG_TUH_DEVICE_MAX + 1] = {};

static bool xinput_init() { return true; }
static bool xinput_deinit() { return true; }

static uint16_t xinput_open(uint8_t rhport, uint8_t devAddr, const tusb_desc_interface_t* itfDesc, uint16_t maxLen)
{
	(void) rhport;
	// Xbox 360 wired protocol, including Logitech F710 when switched to X mode.
	if (itfDesc->bInterfaceClass != TUSB_CLASS_VENDOR_SPECIFIC ||
		itfDesc->bInterfaceSubClass != 0x5d || itfDesc->bInterfaceProtocol != 0x01) return 0;

	XInputInterface& itf = s_interfaces[devAddr];
	itf = {};
	itf.itfNum = itfDesc->bInterfaceNumber;
	const uint8_t* desc = reinterpret_cast<const uint8_t*>(itfDesc);
	uint16_t consumed = itfDesc->bLength;
	desc = tu_desc_next(desc);
	while (consumed < maxLen && (itf.epIn == 0 || itf.epOut == 0)) {
		if (tu_desc_type(desc) == TUSB_DESC_ENDPOINT) {
			const tusb_desc_endpoint_t* epDesc = reinterpret_cast<const tusb_desc_endpoint_t*>(desc);
			if (!tuh_edpt_open(devAddr, epDesc)) return 0;
			if (tu_edpt_dir(epDesc->bEndpointAddress) == TUSB_DIR_IN) itf.epIn = epDesc->bEndpointAddress;
			else itf.epOut = epDesc->bEndpointAddress;
		}
		const uint8_t len = tu_desc_len(desc);
		if (len == 0) return 0;
		consumed += len;
		desc = tu_desc_next(desc);
	}
	return (itf.epIn && itf.epOut) ? consumed : 0;
}

static bool xinput_set_config(uint8_t devAddr, uint8_t itfNum)
{
	XInputInterface& itf = s_interfaces[devAddr];
	if (itf.itfNum != itfNum || itf.epIn == 0) return false;
	uint16_t vid, pid;
	tuh_vid_pid_get(devAddr, &vid, &pid);
	jxglib::USBHost::GamePad::OnXInputMount(devAddr, vid, pid);
	if (!usbh_edpt_claim(devAddr, itf.epIn)) return false;
	if (!usbh_edpt_xfer(devAddr, itf.epIn, itf.buffer, sizeof(itf.buffer))) {
		usbh_edpt_release(devAddr, itf.epIn);
		return false;
	}
	usbh_driver_set_config_complete(devAddr, itfNum);
	return true;
}

static bool xinput_xfer_cb(uint8_t devAddr, uint8_t epAddr, xfer_result_t result, uint32_t xferredBytes)
{
	XInputInterface& itf = s_interfaces[devAddr];
	if (epAddr != itf.epIn) return false;
	if (result == XFER_RESULT_SUCCESS) jxglib::USBHost::GamePad::OnXInputReport(devAddr, itf.buffer, static_cast<uint16_t>(xferredBytes));
	if (!usbh_edpt_claim(devAddr, itf.epIn)) return false;
	if (!usbh_edpt_xfer(devAddr, itf.epIn, itf.buffer, sizeof(itf.buffer))) usbh_edpt_release(devAddr, itf.epIn);
	return true;
}

static void xinput_close(uint8_t devAddr)
{
	jxglib::USBHost::GamePad::OnXInputUmount(devAddr);
	s_interfaces[devAddr] = {};
}

static const usbh_class_driver_t s_xinputDriver = {
	.name = "XInput",
	.init = xinput_init,
	.deinit = xinput_deinit,
	.open = xinput_open,
	.set_config = xinput_set_config,
	.xfer_cb = xinput_xfer_cb,
	.close = xinput_close,
};

} // namespace

extern "C" const usbh_class_driver_t* usbh_app_driver_get_cb(uint8_t* driverCount)
{
	*driverCount = 1;
	return &s_xinputDriver;
}
