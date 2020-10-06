cmake_minimum_required (VERSION 3.12)


include(${CMAKE_SOURCE_DIR}/API/api.cmake)

set(DIRECTORIES
	${CMAKE_SOURCE_DIR}/PrinterAPI/include
	${CMAKE_SOURCE_DIR}/VarAPI/include
	${CMAKE_SOURCE_DIR}/ChronoAPI/include
	${CMAKE_SOURCE_DIR}/LinkAPI/include
	)

api_target(FsAPI "${DIRECTORIES}")

target_link_libraries(${RELEASE_TARGET} PUBLIC API)
