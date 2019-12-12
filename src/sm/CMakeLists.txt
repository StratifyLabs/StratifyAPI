
set(SOURCELIST "")

if( ${SOS_BUILD_CONFIG} STREQUAL arm )

	set(SOURCELIST
		StateMachine.cpp
		)

endif()

set(SOURCES ${SOURCELIST} PARENT_SCOPE)  
