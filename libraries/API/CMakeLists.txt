
include(${CMAKE_CURRENT_LIST_DIR}/api.cmake)

api_target(API "")

target_sources(${RELEASE_TARGET}
	PRIVATE
	api.cmake
	StratifyAPI.cmake
	StratifyAPI_arm.cmake
	StratifyAPI_link.cmake
	)

install(FILES
	StratifyAPI.cmake
	StratifyAPI_arm.cmake
	StratifyAPI_link.cmake
	DESTINATION
	cmake/targets)
