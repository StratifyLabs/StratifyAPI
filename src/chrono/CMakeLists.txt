
if( ${SOS_BUILD_CONFIG} STREQUAL arm )
set(SOURCES
	${SOURCES_PREFIX}/Timer.cpp
	${SOURCES_PREFIX}/ChronoObject.cpp
	${SOURCES_PREFIX}/Clock.cpp
	${SOURCES_PREFIX}/ClockTime.cpp
	PARENT_SCOPE)
endif()
