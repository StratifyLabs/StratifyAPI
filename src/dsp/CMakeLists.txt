
set(SOURCELIST "")

if( ${SOS_BUILD_CONFIG} STREQUAL arm )

	set(SOURCELIST
		${SOURCES_PREFIX}/SignalData.cpp
		${SOURCES_PREFIX}/Transform.cpp
		${SOURCES_PREFIX}/Filter.cpp
		)

endif()

set(SOURCES ${SOURCELIST} PARENT_SCOPE)  
