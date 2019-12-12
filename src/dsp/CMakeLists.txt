
set(SOURCELIST "")

if( ${SOS_BUILD_CONFIG} STREQUAL arm )

	set(SOURCELIST
#  	SignalQ15.cpp
#		SignalQ31.cpp
#		SignalF32.cpp
#		Transform.cpp
#		Filter.cpp
#		SignalDataGeneric.h
		)

endif()

set(SOURCES ${SOURCELIST} PARENT_SCOPE)  
