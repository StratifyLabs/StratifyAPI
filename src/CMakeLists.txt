

set(SRC_SOURCES_PREFIX ${SOURCES_PREFIX})

if( ${SOS_BUILD_CONFIG} STREQUAL arm )
  set(SOURCES_PREFIX ${SRC_SOURCES_PREFIX}/draw CACHE INTERNAL "src prefix")
  add_subdirectory(draw)
  list(APPEND SOURCELIST ${SOURCES})

  set(SOURCES_PREFIX ${SRC_SOURCES_PREFIX}/sgfx CACHE INTERNAL "src prefix")
  add_subdirectory(sgfx)
  list(APPEND SOURCELIST ${SOURCES})

  set(SOURCES_PREFIX ${SRC_SOURCES_PREFIX}/ui CACHE INTERNAL "src prefix")
  add_subdirectory(ui)
  list(APPEND SOURCELIST ${SOURCES})

endif()

set(SOURCES_PREFIX ${SRC_SOURCES_PREFIX}/calc CACHE INTERNAL "src prefix")
add_subdirectory(calc)
list(APPEND SOURCELIST ${SOURCES})

set(SOURCES_PREFIX ${SRC_SOURCES_PREFIX}/fmt CACHE INTERNAL "src prefix")
add_subdirectory(fmt)
list(APPEND SOURCELIST ${SOURCES})

set(SOURCES_PREFIX ${SRC_SOURCES_PREFIX}/hal CACHE INTERNAL "src prefix")
add_subdirectory(hal)
list(APPEND SOURCELIST ${SOURCES})

set(SOURCES_PREFIX ${SRC_SOURCES_PREFIX}/sys CACHE INTERNAL "src prefix")
add_subdirectory(sys)
list(APPEND SOURCELIST ${SOURCES})

set(SOURCES_PREFIX ${SRC_SOURCES_PREFIX}/var CACHE INTERNAL "src prefix")
add_subdirectory(var)
list(APPEND SOURCELIST ${SOURCES})

set(SOURCES ${SOURCELIST} PARENT_SCOPE)
