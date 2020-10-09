cmake_minimum_required (VERSION 3.12)


include(${CMAKE_CURRENT_LIST_DIR}/../API/api.cmake)

set(DIRECTORIES
	VarAPI
	FsAPI
	ChronoAPI
	PrinterAPI
	SysAPI
	ThreadAPI
	)

api_target(TestAPI "${DIRECTORIES}")


target_link_libraries(${RELEASE_TARGET} PUBLIC API)
