cmake_minimum_required (VERSION 3.12)

if(NOT SOS_SDK_PATH)
	set(SOS_SDK_PATH $ENV{SOS_SDK_PATH})
endif()

set(CMAKE_MODULE_PATH ${SOS_SDK_PATH}/Tools/gcc/cmake)
set(CMAKE_TOOLCHAIN_FILE ${CMAKE_MODULE_PATH}/sos-toolchain.cmake)
include(sos-sdk)

add_subdirectory(API testsAPI)
add_subdirectory(VarAPI testsVarAPI)
add_subdirectory(ChronoAPI testsChronoAPI)
add_subdirectory(FsAPI testsFsAPI)


add_custom_target(coverage
	DEPENDS coverage_FsAPI_coverage_link coverage_ChronoAPI_coverage_link coverage_VarAPI_coverage_link
	)
