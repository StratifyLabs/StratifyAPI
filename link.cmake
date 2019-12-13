

include(${SOS_TOOLCHAIN_CMAKE_PATH}/sos-sdk.cmake)

#Add sources to the project
sos_sdk_add_subdirectory(SOS_SOURCELIST ${CMAKE_SOURCE_DIR}/src)

file(GLOB_RECURSE HEADER_SOURCES ${CMAKE_SOURCE_DIR}/include/*)
list(APPEND SOS_SOURCELIST ${SOURCES} ${HEADER_SOURCES})

set(SOS_OPTION "")
set(SOS_CONFIG release)
set(SOS_ARCH link)
include(${SOS_TOOLCHAIN_CMAKE_PATH}/sos-lib.cmake)
