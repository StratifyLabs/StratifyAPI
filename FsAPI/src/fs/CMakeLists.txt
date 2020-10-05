
set(SOURCELIST
	Dir.cpp
	File.cpp
	Stat.cpp)

if( ${SOS_BUILD_CONFIG} STREQUAL arm )

endif()

if( ${SOS_BUILD_CONFIG} STREQUAL link )

endif()


set(SOURCES ${SOURCELIST} PARENT_SCOPE)  
