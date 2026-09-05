/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2019 Ha Thach (tinyusb.org)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
 */

#ifndef _TUSB_CONFIG_H_
#define _TUSB_CONFIG_H_

#ifdef __cplusplus
extern "C" {
#endif

//--------------------------------------------------------------------
// COMMON CONFIGURATION
//--------------------------------------------------------------------

#define CFG_TUSB_OS               OPT_OS_PICO
#define CFG_TUD_ENABLED           0
#define CFG_TUH_ENABLED           1
//#define CFG_TUH_RPI_PIO_USB       0

// Prefer the build-system setting; use level 2 only when none is supplied.
#ifndef CFG_TUSB_DEBUG
#define CFG_TUSB_DEBUG             0
#endif


#ifndef CFG_TUSB_MEM_SECTION
#define CFG_TUSB_MEM_SECTION
#endif

#ifndef CFG_TUSB_MEM_ALIGN
#define CFG_TUSB_MEM_ALIGN        __attribute__ ((aligned(4)))
#endif

//--------------------------------------------------------------------
// HOST CONFIGURATION
//--------------------------------------------------------------------

#define CFG_TUH_ENUMERATION_BUFSIZE              1024
#define CFG_TUH_ENUM_DEBOUNCING_DELAY_MS         1500
#define CFG_TUH_ENUM_ATTEMPT_COUNT_MAX           8
#define CFG_TUH_ENUM_ATTEMPT_DELAY_MS            150
#define CFG_TUH_ENUM_RESET_AND_RESTART_ON_ADDR_FAIL 0
#define CFG_TUH_ENUM_SET_ADDR_RECOVERY_DELAY_MS  50

#define BOARD_TUH_RHPORT                         1
#define CFG_TUH_HUB                              1
#define CFG_TUH_DEVICE_MAX                       (CFG_TUH_HUB ? 4 : 1)

#define CFG_TUH_HID                              8
#define CFG_TUH_MSC                              1
#define CFG_TUH_HID_EPIN_BUFSIZE                 64
#define CFG_TUH_HID_EPOUT_BUFSIZE                64

// MSC Buffer size of Device Mass storage
#define CFG_TUD_MSC_EP_BUFSIZE   4096

// USBH task event queue size (default 16).
// HID(1ms毎) + MSC(3イベント/セクタ)の同時使用でオーバーフローを防ぐため拡張
#define CFG_TUH_TASK_QUEUE_SZ    32



#ifdef __cplusplus
}
#endif

#endif /* _TUSB_CONFIG_H_ */
