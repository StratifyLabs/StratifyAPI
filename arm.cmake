
include(${SOS_TOOLCHAIN_CMAKE_PATH}/sos-sdk.cmake)

#Add sources to the project
sos_sdk_add_subdirectory(SOS_SOURCELIST ${CMAKE_SOURCE_DIR}/src)

#add headers
file(GLOB_RECURSE HEADER_SOURCES ${CMAKE_SOURCE_DIR}/include/*)
list(APPEND SOS_SOURCELIST ${SOURCES} ${HEADER_SOURCES} doxyfile)

set(SOS_OPTION "")
set(SOS_CONFIG release)
set(SOS_BUILD_FLAGS -mlong-calls)
include(${SOS_TOOLCHAIN_CMAKE_PATH}/sos-lib-std.cmake)
