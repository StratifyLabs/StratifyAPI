#Copy this file to the application project folder as CMakeLists.txt
cmake_minimum_required (VERSION 3.12)

if(NOT SOS_SDK_PATH)
	set(SOS_SDK_PATH $ENV{SOS_SDK_PATH})
endif()

set(CMAKE_MODULE_PATH ${SOS_SDK_PATH}/Tools/gcc/cmake)
set(CMAKE_TOOLCHAIN_FILE ${CMAKE_MODULE_PATH}/sos-toolchain.cmake)
include(sos-sdk)

get_filename_component(SOS_NAME ${CMAKE_CURRENT_LIST_DIR} NAME)
project(${SOS_NAME} CXX C)

# This will set the default RAM used by the application
set(RAM_SIZE 16384)

#Add sources to the project
sos_sdk_add_subdirectory(SOURCELIST src)
list(
	APPEND
	SOURCELIST
	sl_settings.json)

sos_sdk_app_target(RELEASE ${SOS_NAME} "" release v7m)
add_executable(${RELEASE_TARGET})
target_sources(${RELEASE_TARGET}
	PRIVATE
	${SOURCELIST})

target_compile_options(${RELEASE_TARGET}
	PRIVATE
	-Os
	)

set_property(TARGET ${RELEASE_TARGET} PROPERTY CXX_STANDARD 17)

target_include_directories(${RELEASE_TARGET}
	PRIVATE
	${CMAKE_SOURCE_DIR}/libraries/API/include
	${CMAKE_SOURCE_DIR}/libraries/VarAPI/include
	)

target_link_libraries(${RELEASE_TARGET}
	PRIVATE
	VarAPI_release_v7m
	API_release_v7m
	stdc++
	supc++
	StratifyOS_crt
	)


sos_sdk_app("${RELEASE_OPTIONS}" ${RAM_SIZE})
#sos_sdk_app_add_arm_targets("${RELEASE_OPTIONS}" ${RAM_SIZE})


#[[
if( ${SOS_BUILD_CONFIG} STREQUAL arm )
	#This will cross compile for all supported ARM architetures
	include(${SOS_TOOLCHAIN_CMAKE_PATH}/sos-app-std.cmake)
elseif( ${SOS_BUILD_CONFIG} STREQUAL link )
	#This will compile natively. If the program uses hardware, it can "link" to a board over USB
	set(SOS_ARCH link)
	set(SOS_CONFIG release)
	include(${SOS_TOOLCHAIN_CMAKE_PATH}/sos-app.cmake)
endif()
#]]
