
set(SOURCELIST
	${SOURCES_PREFIX}/Appfs.cpp
	${SOURCES_PREFIX}/Cli.cpp
	${SOURCES_PREFIX}/Dir.cpp
	${SOURCES_PREFIX}/File.cpp
	${SOURCES_PREFIX}/FileInfo.cpp
	${SOURCES_PREFIX}/Sys.cpp
	${SOURCES_PREFIX}/Task.cpp
	${SOURCES_PREFIX}/Thread.cpp
	${SOURCES_PREFIX}/Mutex.cpp
	${SOURCES_PREFIX}/JsonPrinter.cpp
	${SOURCES_PREFIX}/dirent_windows.h
	${SOURCES_PREFIX}/Signal.cpp
	${SOURCES_PREFIX}/Printer.cpp)


if( ${SOS_BUILD_CONFIG} STREQUAL arm )
	set(SOURCELIST ${SOURCELIST}
		${SOURCES_PREFIX}/Assets.cpp
		${SOURCES_PREFIX}/Sem.cpp
		${SOURCES_PREFIX}/Sched.cpp
		${SOURCES_PREFIX}/Mq.cpp
		${SOURCES_PREFIX}/Messenger.cpp)
endif()

if( ${SOS_BUILD_CONFIG} STREQUAL link )
	set(SOURCELIST ${SOURCELIST}
		${SOURCES_PREFIX}/Link.cpp)
endif()


set(SOURCES ${SOURCELIST} PARENT_SCOPE)  
