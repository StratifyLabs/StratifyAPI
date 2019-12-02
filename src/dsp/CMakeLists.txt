
set(SOURCELIST "")

if( ${SOS_BUILD_CONFIG} STREQUAL arm )

	set(SOURCELIST
#  	${SOURCES_PREFIX}/SignalQ15.cpp
#		${SOURCES_PREFIX}/SignalQ31.cpp
#		${SOURCES_PREFIX}/SignalF32.cpp
#		${SOURCES_PREFIX}/Transform.cpp
#		${SOURCES_PREFIX}/Filter.cpp
#		${SOURCES_PREFIX}/SignalDataGeneric.h
		)

endif()

set(SOURCES ${SOURCELIST} PARENT_SCOPE)  
