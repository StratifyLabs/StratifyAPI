cmake_minimum_required (VERSION 3.12)


include(${CMAKE_SOURCE_DIR}/API/api.cmake)
set(DIRECTORIES
	${CMAKE_SOURCE_DIR}/PrinterAPI/include ${CMAKE_SOURCE_DIR}/VarAPI/include
	)

api_target(ChronoAPI "${DIRECTORIES}")
