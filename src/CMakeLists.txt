

set(SOURCES_PREFIX "")

if( ${SOS_BUILD_CONFIG} STREQUAL arm )
	sos_sdk_add_subdirectory(SOURCELIST draw)
	sos_sdk_add_subdirectory(SOURCELIST dsp)
	sos_sdk_add_subdirectory(SOURCELIST ui)
endif()

sos_sdk_add_subdirectory(SOURCELIST api)
sos_sdk_add_subdirectory(SOURCELIST calc)
sos_sdk_add_subdirectory(SOURCELIST chrono)
sos_sdk_add_subdirectory(SOURCELIST crypto)
sos_sdk_add_subdirectory(SOURCELIST ev)
sos_sdk_add_subdirectory(SOURCELIST fmt)
sos_sdk_add_subdirectory(SOURCELIST fs)
sos_sdk_add_subdirectory(SOURCELIST hal)
sos_sdk_add_subdirectory(SOURCELIST inet)
sos_sdk_add_subdirectory(SOURCELIST sgfx)
sos_sdk_add_subdirectory(SOURCELIST sm)
sos_sdk_add_subdirectory(SOURCELIST sys)
sos_sdk_add_subdirectory(SOURCELIST test)
sos_sdk_add_subdirectory(SOURCELIST var)

#[[

set(SOURCES_PREFIX ${SRC_SOURCES_PREFIX}/api)
add_subdirectory(api)
list(APPEND SOURCELIST ${SOURCES})

set(SOURCES_PREFIX ${SRC_SOURCES_PREFIX}/fs)
add_subdirectory(fs)
list(APPEND SOURCELIST ${SOURCES})

set(SOURCES_PREFIX ${SRC_SOURCES_PREFIX}/chrono)
add_subdirectory(chrono)
list(APPEND SOURCELIST ${SOURCES})

set(SOURCES_PREFIX ${SRC_SOURCES_PREFIX}/ev)
add_subdirectory(ev)
list(APPEND SOURCELIST ${SOURCES})

set(SOURCES_PREFIX ${SRC_SOURCES_PREFIX}/sm)
add_subdirectory(sm)
list(APPEND SOURCELIST ${SOURCES})

set(SOURCES_PREFIX ${SRC_SOURCES_PREFIX}/sgfx)
add_subdirectory(sgfx)
list(APPEND SOURCELIST ${SOURCES})

set(SOURCES_PREFIX ${SRC_SOURCES_PREFIX}/inet)
add_subdirectory(inet)
list(APPEND SOURCELIST ${SOURCES})

set(SOURCES_PREFIX ${SRC_SOURCES_PREFIX}/calc)
add_subdirectory(calc)
list(APPEND SOURCELIST ${SOURCES})

set(SOURCES_PREFIX ${SRC_SOURCES_PREFIX}/crypto)
add_subdirectory(crypto)
list(APPEND SOURCELIST ${SOURCES})

set(SOURCES_PREFIX ${SRC_SOURCES_PREFIX}/fmt)
add_subdirectory(fmt)
list(APPEND SOURCELIST ${SOURCES})

set(SOURCES_PREFIX ${SRC_SOURCES_PREFIX}/hal)
add_subdirectory(hal)
list(APPEND SOURCELIST ${SOURCES})

set(SOURCES_PREFIX ${SRC_SOURCES_PREFIX}/sys)
add_subdirectory(sys)
list(APPEND SOURCELIST ${SOURCES})

set(SOURCES_PREFIX ${SRC_SOURCES_PREFIX}/test)
add_subdirectory(test)
list(APPEND SOURCELIST ${SOURCES})

set(SOURCES_PREFIX ${SRC_SOURCES_PREFIX}/var)
add_subdirectory(var)
list(APPEND SOURCELIST ${SOURCES})

#]]

set(SOURCES ${SOURCELIST} PARENT_SCOPE)
