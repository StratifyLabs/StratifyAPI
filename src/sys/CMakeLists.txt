
set(SOURCELIST
	Auth.cpp
	Appfs.cpp
	Cli.cpp
	Sys.cpp
	TaskManager.cpp
	Thread.cpp
	Mutex.cpp
	JsonPrinter.cpp
	Signal.cpp
	ProgressCallback.cpp
	Printer.cpp
	MarkdownPrinter.cpp
	YamlPrinter.cpp
	)


if( ${SOS_BUILD_CONFIG} STREQUAL arm )
	set(SOURCELIST ${SOURCELIST}
		Assets.cpp
		Sem.cpp
		Sched.cpp
		Mq.cpp)
endif()

if( ${SOS_BUILD_CONFIG} STREQUAL link )
	set(SOURCELIST ${SOURCELIST}
		Link.cpp)
endif()


set(SOURCES ${SOURCELIST} PARENT_SCOPE)  
