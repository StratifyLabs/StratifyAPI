cmake_minimum_required (VERSION 3.12)

if(NOT SOS_SDK_PATH)
	set(SOS_SDK_PATH $ENV{SOS_SDK_PATH})
endif()

set(CMAKE_MODULE_PATH ${SOS_SDK_PATH}/Tools/gcc/cmake)
set(CMAKE_TOOLCHAIN_FILE ${CMAKE_MODULE_PATH}/sos-toolchain.cmake)
include(sos-sdk)

set(SOS_NAME api)
project(StratifyAPI CXX C)

add_subdirectory(API api)
add_subdirectory(VarAPI var)
add_subdirectory(PrinterAPI printer)
add_subdirectory(ChronoAPI chrono)
add_subdirectory(ThreadAPI thread)
