set(target_name FMSX_USBKEYUNIT)

add_executable(${target_name}
  FMSX_USBKEYUNIT.cpp
  ws2812.pio
  hbf500_dat_bus.pio
)

pico_enable_stdio_uart(${target_name} 1)
pico_enable_stdio_usb(${target_name} 0)
pico_set_program_name(${target_name} ${target_name})
pico_set_program_version(${target_name} "0.1")

target_compile_options(${target_name} PRIVATE
  -Wall -Wextra
  -UCFG_TUSB_DEBUG -DCFG_TUSB_DEBUG=0)
target_compile_definitions(${target_name} PRIVATE
  FMSX_I2C_KEYUNIT=1
  JXGLIB_USBHOST_DIAGNOSTICS=0
  RP2USB_TRACE=0)
target_include_directories(${target_name} PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}
  ${CMAKE_CURRENT_BINARY_DIR})

pico_generate_pio_header(${target_name} ${CMAKE_CURRENT_LIST_DIR}/ws2812.pio)
pico_generate_pio_header(${target_name} ${CMAKE_CURRENT_LIST_DIR}/hbf500_dat_bus.pio)

target_link_libraries(${target_name} PRIVATE
  pico_stdlib
  pico_multicore
  pico_i2c_slave
  hardware_i2c
  hardware_pio
  hardware_pwm
  jxglib_USBHost
  jxglib_Serial
  jxglib_JSON
  jxglib_LFS_Flash
  jxglib_FAT_USBMSC
  tinyusb_host)

target_compile_definitions(jxglib_USBHost INTERFACE
  CFG_TUH_ENABLED=1
  CFG_TUH_HID=8
  CFG_TUH_MSC=1)

jxglib_configure_FAT(${target_name}
  FF_VOLUMES 2
  FF_USE_FASTSEEK 1
  FF_FS_NOFSINFO 1)

pico_add_extra_outputs(${target_name})
