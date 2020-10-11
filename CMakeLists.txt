cmake_minimum_required (VERSION 3.12)

if(NOT SOS_SDK_PATH)
	set(SOS_SDK_PATH $ENV{SOS_SDK_PATH})
endif()

set(CMAKE_MODULE_PATH ${SOS_SDK_PATH}/Tools/gcc/cmake)
set(CMAKE_TOOLCHAIN_FILE ${CMAKE_MODULE_PATH}/sos-toolchain.cmake)
include(sos-sdk)


project(StratifyAPI CXX C)
include(CTest)

add_subdirectory(libraries/API API)
add_subdirectory(libraries/VarAPI VarAPI)
add_subdirectory(libraries/PrinterAPI PrinterAPI)
add_subdirectory(libraries/ChronoAPI ChronoAPI)
add_subdirectory(libraries/FsAPI FsAPI)
add_subdirectory(libraries/ThreadAPI ThreadAPI)
add_subdirectory(libraries/CryptoAPI CryptoAPI)
add_subdirectory(libraries/HalAPI HalAPI)
add_subdirectory(libraries/SysAPI SysAPI)
add_subdirectory(libraries/TestAPI TestAPI)
add_subdirectory(libraries/InetAPI InetAPI)
add_subdirectory(libraries/LinkAPI LinkAPI)


add_subdirectory(tests tests)
