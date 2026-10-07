cmake_minimum_required(VERSION 3.17)

project(jxglib C CXX ASM)

# Only configure the components used by KBD-13_EMU and KBD-13_HOST. This
# avoids fetching unrelated pico-jxglib submodules such as LVGL and TFLMicro.
add_subdirectory(Common)
add_subdirectory(Mouse)
add_subdirectory(LineBuff)
add_subdirectory(Terminal)
add_subdirectory(Serial)
add_subdirectory(RTC)
add_subdirectory(Flash)
add_subdirectory(FS)
add_subdirectory(JSON)
add_subdirectory(USBDevice)
add_subdirectory(USBHost)

function(jxglib_set_if_undefined VAR VALUE)
	if(NOT DEFINED ${VAR})
		set(${VAR} "${VALUE}" PARENT_SCOPE)
	endif()
endfunction()

function(jxglib_set01 VAR)
	if(${VAR})
		set(${VAR} 1 PARENT_SCOPE)
	else()
		set(${VAR} 0 PARENT_SCOPE)
	endif()
endfunction()
