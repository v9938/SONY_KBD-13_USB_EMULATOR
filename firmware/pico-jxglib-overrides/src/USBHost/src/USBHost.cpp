//==============================================================================
// USBHost.cpp
//==============================================================================
#include "jxglib/USBHost.h"

#if RP2USB_TRACE
extern "C" void rp2usb_trace_dump(void);
#endif

// Set to 1 with -DJXGLIB_USBHOST_DIAGNOSTICS=1 to trace host task nesting
// and device-level connect/disconnect callbacks. Keep disabled in normal builds.
#ifndef JXGLIB_USBHOST_DIAGNOSTICS
#define JXGLIB_USBHOST_DIAGNOSTICS 0
#endif

namespace jxglib::USBHost {

static Controller Instance;
static EventHandler* pEventHandler = nullptr;
static bool s_tuhTaskActive = false;
static uint32_t s_tuhTaskReentryCount = 0;

//------------------------------------------------------------------------------
// Functions
//------------------------------------------------------------------------------
void Initialize(uint8_t rhport, EventHandler* eventHandler)
{
	::tuh_init(rhport);
	pEventHandler = eventHandler;
	#if JXGLIB_USBHOST_DIAGNOSTICS
	::printf("[USBH] init rhport=%u handler=%p\n", rhport, static_cast<void*>(eventHandler));
	#endif
}

//------------------------------------------------------------------------------
// USBHost::Controller
//------------------------------------------------------------------------------
Controller::Controller()
{
}

void Controller::OnTick()
{
	if (s_tuhTaskActive) {
		s_tuhTaskReentryCount++;
		#if JXGLIB_USBHOST_DIAGNOSTICS
		::printf("[USBH] tuh_task reentry skipped count=%lu tickDepth=%d\n",
			static_cast<unsigned long>(s_tuhTaskReentryCount), Tickable::GetTickCalledDepth());
		#endif
		return;
	}
	s_tuhTaskActive = true;
	::tuh_task();
	s_tuhTaskActive = false;
}

}

//------------------------------------------------------------------------------
// Callback functions
//------------------------------------------------------------------------------
extern "C" void tuh_mount_cb(uint8_t devAddr)
{
	using namespace jxglib::USBHost;
	#if JXGLIB_USBHOST_DIAGNOSTICS
	::printf("[USBH] device mount addr=%u\n", devAddr);
	#endif
	if (pEventHandler) pEventHandler->OnMount(devAddr);
}

extern "C" void tuh_umount_cb(uint8_t devAddr)
{
	using namespace jxglib::USBHost;
	#if JXGLIB_USBHOST_DIAGNOSTICS
	::printf("[USBH] device unmount addr=%u\n", devAddr);
	#endif
	#if RP2USB_TRACE
	rp2usb_trace_dump();
	#endif
	if (pEventHandler) pEventHandler->OnUmount(devAddr);
}
