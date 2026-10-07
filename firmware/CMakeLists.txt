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
if(EXISTS ${picoVscode})
    include(${picoVscode})
endif()
# ====================================================================================

list(APPEND PICO_BOARD_HEADER_DIRS
    ${CMAKE_CURRENT_LIST_DIR}/boards)
set(PICO_BOARD rp2354A_USBA CACHE STRING "Board type")
add_compile_options(-Wno-error=attributes)

cmake_minimum_required(VERSION 3.13)

include(FetchContent)

function(apply_dependency_overrides SOURCE_DIR OVERRIDE_DIR)
    file(GLOB_RECURSE override_files
        RELATIVE "${OVERRIDE_DIR}"
        CONFIGURE_DEPENDS
        "${OVERRIDE_DIR}/*")
    foreach(relative_path IN LISTS override_files)
        if(NOT IS_DIRECTORY "${OVERRIDE_DIR}/${relative_path}")
            get_filename_component(destination_dir
                "${SOURCE_DIR}/${relative_path}" DIRECTORY)
            file(MAKE_DIRECTORY "${destination_dir}")
            configure_file(
                "${OVERRIDE_DIR}/${relative_path}"
                "${SOURCE_DIR}/${relative_path}"
                COPYONLY)
        endif()
    endforeach()
endfunction()

FetchContent_Declare(tinyusb_021
    GIT_REPOSITORY https://github.com/hathach/tinyusb.git
    GIT_TAG dae3f9a366bfcddbf9dcf1b48d7500286a849539
    GIT_SHALLOW TRUE
    GIT_SUBMODULES ""
    UPDATE_DISCONNECTED TRUE
    SOURCE_SUBDIR cmake-not-used)
FetchContent_MakeAvailable(tinyusb_021)
apply_dependency_overrides(
    "${tinyusb_021_SOURCE_DIR}"
    "${CMAKE_CURRENT_LIST_DIR}/tinyusb-overrides")
set(PICO_TINYUSB_PATH "${tinyusb_021_SOURCE_DIR}" CACHE PATH "TinyUSB source" FORCE)

FetchContent_Declare(pico_jxglib
    GIT_REPOSITORY https://github.com/ypsitau/pico-jxglib.git
    GIT_TAG 42b7411694b918f7e42e2743bb583665f43f4fc6
    GIT_SHALLOW TRUE
    GIT_SUBMODULES src/FS/LFS/littlefs
    UPDATE_DISCONNECTED TRUE
    SOURCE_SUBDIR cmake-not-used)
FetchContent_MakeAvailable(pico_jxglib)
apply_dependency_overrides(
    "${pico_jxglib_SOURCE_DIR}"
    "${CMAKE_CURRENT_LIST_DIR}/pico-jxglib-overrides")
set(PICO_JXGLIB_OVERRIDE_DIR
    "${CMAKE_CURRENT_LIST_DIR}/pico-jxglib-overrides"
    CACHE PATH "pico-jxglib source overrides" FORCE)

include(pico_sdk_import.cmake)
project(keyboard_host_firmware C CXX ASM)
set(FAMILY rp2040)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

pico_sdk_init()

set(PICO_PIO_USB_DIR ${PICO_SDK_PATH}/lib/tinyusb/hw/mcu/raspberry_pi/Pico-PIO-USB)
add_subdirectory(${PICO_PIO_USB_DIR} pico_pio_usb)
add_subdirectory(${pico_jxglib_SOURCE_DIR} pico-jxglib)

add_subdirectory(KBD-13_EMU)
add_subdirectory(FMSX_USBKEYUNIT)
