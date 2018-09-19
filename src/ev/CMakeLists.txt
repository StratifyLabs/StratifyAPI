
set(SOURCELIST "")

if( ${SOS_BUILD_CONFIG} STREQUAL arm )

	set(SOURCELIST
		${SOURCES_PREFIX}/Button.cpp
		${SOURCES_PREFIX}/PinButton.cpp
		${SOURCES_PREFIX}/DeviceButton.cpp
		${SOURCES_PREFIX}/Event.cpp
		${SOURCES_PREFIX}/EventLoop.cpp
		${SOURCES_PREFIX}/EventHandler.cpp)

endif()

set(SOURCES ${SOURCELIST} PARENT_SCOPE)  
