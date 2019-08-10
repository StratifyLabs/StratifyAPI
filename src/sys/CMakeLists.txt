
set(SOURCELIST
	${SOURCES_PREFIX}/Auth.cpp
	${SOURCES_PREFIX}/Appfs.cpp
	${SOURCES_PREFIX}/Cli.cpp
	${SOURCES_PREFIX}/Sys.cpp
	${SOURCES_PREFIX}/TaskManager.cpp
	${SOURCES_PREFIX}/Thread.cpp
	${SOURCES_PREFIX}/Mutex.cpp
	${SOURCES_PREFIX}/JsonPrinter.cpp
	${SOURCES_PREFIX}/Signal.cpp
	${SOURCES_PREFIX}/ProgressCallback.cpp
	${SOURCES_PREFIX}/Printer.cpp)


if( ${SOS_BUILD_CONFIG} STREQUAL arm )
	set(SOURCELIST ${SOURCELIST}
		${SOURCES_PREFIX}/Assets.cpp
		${SOURCES_PREFIX}/Sem.cpp
		${SOURCES_PREFIX}/Sched.cpp
		${SOURCES_PREFIX}/Mq.cpp)
endif()

if( ${SOS_BUILD_CONFIG} STREQUAL link )
	set(SOURCELIST ${SOURCELIST}
		${SOURCES_PREFIX}/Link.cpp)
endif()


set(SOURCES ${SOURCELIST} PARENT_SCOPE)  
