

if( ${SOS_BUILD_CONFIG} STREQUAL arm )
    set(SOURCELIST
			${SOURCES_PREFIX}/Socket.cpp
			${SOURCES_PREFIX}/Http.cpp
	${SOURCES_PREFIX}/SecureSocket.cpp)
endif()


set(SOURCES ${SOURCELIST} PARENT_SCOPE)  
