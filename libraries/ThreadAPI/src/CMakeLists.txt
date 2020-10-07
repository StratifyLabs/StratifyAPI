
set(SOURCES
	Thread.cpp
	Mutex.cpp
	Signal.cpp
	Sem.cpp
	PARENT_SCOPE
	)


if( ${SOS_BUILD_CONFIG} STREQUAL arm )
	list(APPEND ${SOURCES}
		Sched.cpp
		PARENT_SCOPE
		)
endif()
