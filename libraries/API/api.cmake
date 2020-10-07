

macro(api_target NAME DIRECTORIES)
	include(sos-sdk)

	set(API_NAME ${NAME})
	project(${API_NAME} CXX C)

	install(DIRECTORY include/ DESTINATION include/${API_NAME})

	string(COMPARE EQUAL ${SOS_BUILD_CONFIG} link IS_LINK)

	if(IS_LINK)
		set(ARCH link)
	else()
		set(ARCH v7m)
	endif()

	sos_sdk_add_subdirectory(PRIVATE_SOURCES src)
	sos_sdk_add_subdirectory(PUBLIC_SOURCES ${CMAKE_SOURCE_DIR}/libraries/${API_NAME}/include)
	set(FORMAT_LIST ${PRIVATE_SOURCES} ${PUBLIC_SOURCES})

	sos_sdk_library_target(RELEASE ${API_NAME} "" release ${ARCH})

	add_library(${RELEASE_TARGET} STATIC)

	set_property(TARGET ${RELEASE_TARGET} PROPERTY CXX_STANDARD 17)

	target_sources(${RELEASE_TARGET}
		PUBLIC
		${PUBLIC_SOURCES}
		PRIVATE
		${PRIVATE_SOURCES}
		)

	target_compile_options(${RELEASE_TARGET}
		PUBLIC
		-Os
		)

	if(IS_LINK)
		target_include_directories(${RELEASE_TARGET}
			PUBLIC
			${SOS_SDK_PATH}/Tools/gcc/include
			)
	endif()

	target_include_directories(${RELEASE_TARGET}
		PUBLIC
		${CMAKE_SOURCE_DIR}/libraries/API/include
		${DIRECTORIES}
		${CMAKE_SOURCE_DIR}/libraries/${API_NAME}/include
		)

	sos_sdk_library("${RELEASE_OPTIONS}")


	add_custom_target(
		${API_NAME}_format
		COMMAND /usr/local/bin/clang-format
		-i
		--verbose
		${FORMAT_LIST}
		)

endmacro()
