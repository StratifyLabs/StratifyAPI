
set(SOURCES
	Thread.cpp
	Mutex.cpp
	Signal.cpp
	Sem.cpp
	PARENT_SCOPE
	)


if(SOS_IS_ARM)
	list(APPEND ${SOURCES}
		Sched.cpp
		PARENT_SCOPE
		)
endif()
