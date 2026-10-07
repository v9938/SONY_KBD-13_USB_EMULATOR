#ifndef _TUSB_CONFIG_H_
#define _TUSB_CONFIG_H_

#ifdef __cplusplus
extern "C" {
#endif

// Native USB host configuration for the RP2350 USB controller.
#define CFG_TUSB_OS OPT_OS_PICO
#define CFG_TUD_ENABLED 0
#define CFG_TUH_ENABLED 1

#ifndef CFG_TUSB_DEBUG
#define CFG_TUSB_DEBUG 0
#endif

#ifndef CFG_TUSB_MEM_SECTION
#define CFG_TUSB_MEM_SECTION
#endif

#ifndef CFG_TUSB_MEM_ALIGN
#define CFG_TUSB_MEM_ALIGN __attribute__((aligned(4)))
#endif

#define CFG_TUH_ENUMERATION_BUFSIZE 1024
#define CFG_TUH_ENUM_DEBOUNCING_DELAY_MS 1500
#define CFG_TUH_ENUM_ATTEMPT_COUNT_MAX 8
#define CFG_TUH_ENUM_ATTEMPT_DELAY_MS 150
#define CFG_TUH_ENUM_RESET_AND_RESTART_ON_ADDR_FAIL 0
#define CFG_TUH_ENUM_SET_ADDR_RECOVERY_DELAY_MS 50

#define BOARD_TUH_RHPORT 1
#define CFG_TUH_HUB 1
#define CFG_TUH_DEVICE_MAX (CFG_TUH_HUB ? 4 : 1)
#define CFG_TUH_HID 8
#define CFG_TUH_MSC 1
#define CFG_TUH_HID_EPIN_BUFSIZE 64
#define CFG_TUH_HID_EPOUT_BUFSIZE 64
#define CFG_TUH_TASK_QUEUE_SZ 32

#ifdef __cplusplus
}
#endif

#endif
