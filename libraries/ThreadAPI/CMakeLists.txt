cmake_minimum_required (VERSION 3.12)


include(${CMAKE_CURRENT_LIST_DIR}/../API/api.cmake)

set(DIRECTORIES
	${CMAKE_CURRENT_LIST_DIR}/../PrinterAPI/include
	${CMAKE_CURRENT_LIST_DIR}/../VarAPI/include
	${CMAKE_CURRENT_LIST_DIR}/../ChronoAPI/include
	)

api_target(ThreadAPI "${DIRECTORIES}")


target_link_libraries(${RELEASE_TARGET} PUBLIC API)
