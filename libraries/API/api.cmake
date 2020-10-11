

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
	sos_sdk_add_subdirectory(PUBLIC_SOURCES	${CMAKE_CURRENT_SOURCE_DIR}/include)
	set(FORMAT_LIST ${PRIVATE_SOURCES} ${PUBLIC_SOURCES})

	sos_sdk_library_target(RELEASE ${API_NAME} "" release ${ARCH})

	add_library(${RELEASE_TARGET} STATIC)

	set_property(TARGET ${RELEASE_TARGET} PROPERTY CXX_STANDARD 17)

	target_sources(${RELEASE_TARGET}
		PRIVATE
		${PUBLIC_SOURCES}
		${PRIVATE_SOURCES}
		)

	target_compile_options(${RELEASE_TARGET}
		PRIVATE
		-Os
		)

	if(IS_LINK)
		target_include_directories(${RELEASE_TARGET}
			PUBLIC
			${CMAKE_INSTALL_PREFIX}/include
			)
	endif()

	foreach(DIRECTORY ${DIRECTORIES})
		target_include_directories(${RELEASE_TARGET}
			INTERFACE
			$<INSTALL_INTERFACE:include/${DIRECTORY}>
			PRIVATE
			$<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/libraries/${DIRECTORY}/include>
			)

	endforeach(DIRECTORY)

	message(STATUS "build include ${BUILD_INCLUDE_DIRECTORIES}")
	message(STATUS "install include ${INSTALL_INCLUDE_DIRECTORIES}")

	target_include_directories(${RELEASE_TARGET}
		PUBLIC
		$<INSTALL_INTERFACE:include/${API_NAME}>
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
		)

	message(STATUS "Adding dependencies ${DEPENDENCIES}")
	sos_sdk_library_add_arch_targets("${RELEASE_OPTIONS}" ${SOS_ARCH} "${DIRECTORIES}")


	add_custom_target(
		${API_NAME}_format
		COMMAND /usr/local/bin/clang-format
		-i
		--verbose
		${FORMAT_LIST}
		)

endmacro()
