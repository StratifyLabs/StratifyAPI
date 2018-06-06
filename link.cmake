

#Add sources to the project
set(SOURCES_PREFIX ${CMAKE_SOURCE_DIR}/src)
add_subdirectory(src)
file(GLOB_RECURSE HEADER_SOURCES ${CMAKE_SOURCE_DIR}/include/*)
list(APPEND SOS_LIB_SOURCELIST ${SOURCES} ${HEADER_SOURCES})

set(SOS_LIB_OPTION "")
set(SOS_LIB_TYPE release)
set(SOS_LIB_ARCH link)
include(${SOS_TOOLCHAIN_CMAKE_PATH}/sos-lib.cmake)
