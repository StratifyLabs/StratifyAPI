
set(SOURCELIST "")

if( ${SOS_BUILD_CONFIG} STREQUAL arm )

	set(SOURCELIST
		${SOURCES_PREFIX}/StateMachine.cpp
		)

endif()

set(SOURCES ${SOURCELIST} PARENT_SCOPE)  
