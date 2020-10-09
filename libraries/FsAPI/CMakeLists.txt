cmake_minimum_required (VERSION 3.12)


include(${CMAKE_CURRENT_LIST_DIR}/../API/api.cmake)

set(DIRECTORIES
	PrinterAPI
	ChronoAPI
	SysAPI
	LinkAPI
	)

api_target(FsAPI "${DIRECTORIES}")

target_link_libraries(${RELEASE_TARGET} PUBLIC API)
