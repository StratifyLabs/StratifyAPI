

set(SOURCES
	${SOURCES_PREFIX}/WorkObject.cpp
	PARENT_SCOPE)

if( ${SOS_BUILD_CONFIG} STREQUAL arm )
		set(SOURCELIST ${SOURCELIST}
			${SOURCES_PREFIX}/DspWorkObject.cpp)
endif()
