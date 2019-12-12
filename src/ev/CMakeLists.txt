
set(SOURCELIST "")

if( ${SOS_BUILD_CONFIG} STREQUAL arm )

	set(SOURCELIST
		Button.cpp
		PinButton.cpp
		DeviceButton.cpp
		Event.cpp
		EventLoop.cpp
		EventHandler.cpp)

endif()

set(SOURCES ${SOURCELIST} PARENT_SCOPE)  
