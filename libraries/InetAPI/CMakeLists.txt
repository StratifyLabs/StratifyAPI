cmake_minimum_required (VERSION 3.12)


include(${CMAKE_CURRENT_LIST_DIR}/../API/api.cmake)

set(DIRECTORIES
	PrinterAPI
	VarAPI
	FsAPI
	CryptoAPI
	ChronoAPI
	ThreadAPI
	)

api_target(InetAPI "${DIRECTORIES}")


target_link_libraries(${RELEASE_TARGET} PUBLIC API)
