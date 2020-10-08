cmake_minimum_required (VERSION 3.12)

if(NOT SOS_SDK_PATH)
	set(SOS_SDK_PATH $ENV{SOS_SDK_PATH})
endif()

set(CMAKE_MODULE_PATH ${SOS_SDK_PATH}/Tools/gcc/cmake)
set(CMAKE_TOOLCHAIN_FILE ${CMAKE_MODULE_PATH}/sos-toolchain.cmake)
include(sos-sdk)

set(SOS_NAME api)
project(StratifyAPI CXX C)

add_subdirectory(libraries/API api)
add_subdirectory(libraries/VarAPI var)
add_subdirectory(libraries/PrinterAPI printer)
add_subdirectory(libraries/ChronoAPI chrono)
add_subdirectory(libraries/FsAPI fs)
add_subdirectory(libraries/ThreadAPI thread)
add_subdirectory(libraries/CryptoAPI crypto)
add_subdirectory(libraries/HalAPI hal)
add_subdirectory(libraries/SysAPI sys)
add_subdirectory(libraries/TestAPI test)
add_subdirectory(libraries/InetAPI inet)
add_subdirectory(libraries/LinkAPI link)


#add_subdirectory(tests tests)
