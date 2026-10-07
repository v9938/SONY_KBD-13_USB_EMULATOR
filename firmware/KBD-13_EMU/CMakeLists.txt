set(target_name KBD-13_EMU)
add_executable(${target_name})

target_sources(${target_name} PRIVATE
  KBD-13_EMU.cpp
  ws2812.pio
  hbf500_dat_bus.pio
  # can use 'tinyusb_pico_pio_usb' library later when pico-sdk is updated
#  ${PICO_TINYUSB_PATH}/src/portable/raspberrypi/pio_usb/hcd_pio_usb.c
)

# UART output only
pico_enable_stdio_uart(${target_name} 1)
pico_enable_stdio_usb(${target_name} 0)

pico_set_program_name(${target_name} ${target_name})
pico_set_program_version(${target_name} "0.1")

target_link_options(${target_name} PRIVATE -Xlinker --print-memory-usage)
target_link_options(${target_name} PRIVATE -Wl,-Map=${PROJECT_NAME}.map)
target_compile_options(${target_name} PRIVATE -Wall -Wextra)

# USB host lifecycle diagnostics. Enable for hot-plug investigation with:
#   cmake -DJXGLIB_USBHOST_DIAGNOSTICS=1 ..
set(JXGLIB_USBHOST_DIAGNOSTICS 0 CACHE STRING
    "Enable USB host mount/report/reentry diagnostic logs (0, 1, or 2)" FORCE)
target_compile_definitions(${target_name} PRIVATE
  JXGLIB_USBHOST_DIAGNOSTICS=${JXGLIB_USBHOST_DIAGNOSTICS})

# Keep TinyUSB silent in normal firmware. Raise this only for transport
# diagnostics, which intentionally change USB host timing.
set(CFG_TUSB_DEBUG 0 CACHE STRING
    "TinyUSB diagnostic level (0 for release firmware)" FORCE)
target_compile_options(${target_name} PRIVATE
  -UCFG_TUSB_DEBUG
  -DCFG_TUSB_DEBUG=${CFG_TUSB_DEBUG})

# Records RP2350 host events in RAM without UART traffic. The trace freezes and
# is dumped after a USB disconnect, allowing timing analysis without logging.
set(RP2USB_TRACE 0 CACHE STRING
    "Enable RAM-only RP2350 USB host event trace (0 or 1)" FORCE)
target_compile_definitions(${target_name} PRIVATE
  RP2USB_TRACE=${RP2USB_TRACE})
target_compile_definitions(jxglib_USBHost INTERFACE
  RP2USB_TRACE=${RP2USB_TRACE})

#target_compile_definitions(${target_name} PRIVATE PIO_USB_USE_TINYUSB)
target_include_directories(${target_name} PRIVATE ${CMAKE_CURRENT_LIST_DIR})
target_include_directories(${target_name} PRIVATE ${CMAKE_CURRENT_BINARY_DIR})

pico_generate_pio_header(${target_name} ${CMAKE_CURRENT_LIST_DIR}/ws2812.pio)
pico_generate_pio_header(${target_name} ${CMAKE_CURRENT_LIST_DIR}/hbf500_dat_bus.pio)

target_link_libraries(${target_name} PRIVATE
  pico_stdlib
  pico_multicore
  jxglib_USBHost  # GamePad uses jxglib_USBHost HID support
  jxglib_Serial
  jxglib_JSON
  jxglib_LFS_Flash
  jxglib_FAT_USBMSC
  tinyusb_host
  hardware_pio
  hardware_pwm
)

pico_add_extra_outputs(${target_name})

target_compile_definitions(jxglib_USBHost INTERFACE
  CFG_TUH_ENABLED=1
  CFG_TUH_HID=8
  CFG_TUH_MSC=1
)

jxglib_configure_FAT(${target_name}
    FF_VOLUMES      2
    FF_USE_FASTSEEK 1
    FF_FS_NOFSINFO  1
)
