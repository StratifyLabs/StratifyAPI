cmake_minimum_required (VERSION 3.12)


include(${CMAKE_CURRENT_LIST_DIR}/../API/api.cmake)

set(DIRECTORIES
	${CMAKE_SOURCE_DIR}/libraries/VarAPI/include
	${CMAKE_SOURCE_DIR}/libraries/FsAPI/include
	${CMAKE_SOURCE_DIR}/libraries/ChronoAPI/include
	${CMAKE_SOURCE_DIR}/libraries/PrinterAPI/include
	${CMAKE_SOURCE_DIR}/libraries/SysAPI/include
	${CMAKE_SOURCE_DIR}/libraries/ThreadAPI/include
	)

api_target(TestAPI "${DIRECTORIES}")


target_link_libraries(${RELEASE_TARGET} PUBLIC API)
