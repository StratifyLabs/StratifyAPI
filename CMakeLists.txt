cmake_minimum_required (VERSION 3.12)

if(NOT SOS_SDK_PATH)
	set(SOS_SDK_PATH $ENV{SOS_SDK_PATH})
endif()

set(CMAKE_MODULE_PATH ${SOS_SDK_PATH}/Tools/gcc/cmake)
set(CMAKE_TOOLCHAIN_FILE ${CMAKE_MODULE_PATH}/sos-toolchain.cmake)
include(sos-sdk)


set(SOS_NAME api)
project(StratifyAPI CXX C)

include(CheckIncludeFiles)
check_include_files("mbedtls_api.h" HAVE_MBEDTLS_H)

install(DIRECTORY include/ DESTINATION include/sapi)
set(SOS_INCLUDE_DIRECTORIES include)

include( ${CMAKE_SOURCE_DIR}/${SOS_BUILD_CONFIG}.cmake )

add_custom_target(
	format
	COMMAND /usr/local/bin/clang-format
	-i
	--verbose
	${FORMAT_LIST}
	)
