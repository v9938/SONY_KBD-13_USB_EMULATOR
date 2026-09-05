# == DO NOT EDIT THE FOLLOWING LINES for the Raspberry Pi Pico VS Code Extension to work ==
if(WIN32)
    set(USERHOME $ENV{USERPROFILE})
else()
    set(USERHOME $ENV{HOME})
endif()
set(sdkVersion 2.3.0)
set(toolchainVersion 15_2_Rel1)
set(picotoolVersion 2.3.0)
set(picoVscode ${USERHOME}/.pico-sdk/cmake/pico-vscode.cmake)
if (EXISTS ${picoVscode})
    include(${picoVscode})
endif()
# ====================================================================================
# 独自ボード定義の検索先
list(APPEND PICO_BOARD_HEADER_DIRS
    ${CMAKE_CURRENT_LIST_DIR}/boards
)
set(PICO_BOARD rp2354A_USBA CACHE STRING "Board type")

add_compile_options(-Wno-error=attributes)

# Clone the pico-jxglib submodule before configuring this project.
if (NOT EXISTS "${CMAKE_CURRENT_LIST_DIR}/pico-jxglib/CMakeLists.txt")
    message(FATAL_ERROR
        "Missing pico-jxglib submodule. Run: git submodule update --init --recursive")
endif()

cmake_minimum_required(VERSION 3.13)

# Keep the SDK's bundled TinyUSB untouched while building this project against
# the RP2350-capable TinyUSB 0.21.0 checkout.
set(PICO_TINYUSB_PATH
    "${USERHOME}/.pico-sdk/sdk/2.3.0/lib/tinyusb021"
    CACHE PATH "Path to the TinyUSB source tree" FORCE)

include (pico_sdk_import.cmake)
project(pio_usb C CXX ASM)
set(FAMILY rp2040)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

pico_sdk_init()

# TinyUSB 0.21.0 does not vendor Pico-PIO-USB. Keep using the SDK's existing
# copy for optional PIO-USB examples; host_hid_msc itself uses the native HCD.
set(PICO_PIO_USB_DIR ${PICO_SDK_PATH}/lib/tinyusb/hw/mcu/raspberry_pi/Pico-PIO-USB)
# the second argument to add_subdirectory is needed here because this isn't
# a subdirectory, it's out of tree.
add_subdirectory(${PICO_PIO_USB_DIR} pico_pio_usb)

add_subdirectory(host_hid_msc)
