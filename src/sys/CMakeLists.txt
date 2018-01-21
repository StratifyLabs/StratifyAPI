
set(SOURCES
  ${SOURCES_PREFIX}/Appfs.cpp
  ${SOURCES_PREFIX}/Dir.cpp
  ${SOURCES_PREFIX}/File.cpp
  ${SOURCES_PREFIX}/FileInfo.cpp
  ${SOURCES_PREFIX}/Signal.cpp
  ${SOURCES_PREFIX}/Sys.cpp
  ${SOURCES_PREFIX}/Time.cpp
  ${SOURCES_PREFIX}/Timer.cpp
  PARENT_SCOPE)


if( ${SOS_BUILD_CONFIG} STREQUAL arm )
    set(SOURCES ${SOURCES}
      ${SOURCES_PREFIX}/Assets.cpp
      ${SOURCES_PREFIX}/Cli.cpp
      ${SOURCES_PREFIX}/Sem.cpp
      ${SOURCES_PREFIX}/Thread.cpp
      ${SOURCES_PREFIX}/Sched.cpp
      ${SOURCES_PREFIX}/Mutex.cpp
      ${SOURCES_PREFIX}/Mq.cpp
      ${SOURCES_PREFIX}/Messenger.cpp
      PARENT_SCOPE)
  endif()
