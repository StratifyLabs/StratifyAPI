

macro(api_target NAME DIRECTORIES)
	include(sos-sdk)

	project(
		${NAME}
		VERSION ${STRATIFYAPI_PROJECT_VERSION}
		LANGUAGES CXX)

	install(DIRECTORY include/ DESTINATION include/${NAME})

	sos_sdk_add_subdirectory(PRIVATE_SOURCES src)
	sos_sdk_add_subdirectory(PUBLIC_SOURCES	${CMAKE_CURRENT_SOURCE_DIR}/include)
	set(FORMAT_LIST ${PRIVATE_SOURCES} ${PUBLIC_SOURCES})

	sos_sdk_library_target(RELEASE ${NAME} "" release ${SOS_ARCH})

	add_library(${RELEASE_TARGET} STATIC)

	set_property(TARGET ${RELEASE_TARGET} PROPERTY CXX_STANDARD 17)

	target_sources(${RELEASE_TARGET}
		PRIVATE
		${PUBLIC_SOURCES}
		${PRIVATE_SOURCES}
		)

	target_compile_definitions(${RELEASE_TARGET}
		PRIVATE
		__PROJECT_VERSION_MAJOR=${PROJECT_VERSION_MAJOR}
		__PROJECT_VERSION_MINOR=${PROJECT_VERSION_MINOR}
		__PROJECT_VERSION_PATCH=${PROJECT_VERSION_PATCH}
		)

	target_compile_options(${RELEASE_TARGET}
		PRIVATE
		-Os
		)

	if(SOS_IS_LINK)
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


	target_include_directories(${RELEASE_TARGET}
		PUBLIC
		$<INSTALL_INTERFACE:include/${NAME}>
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
		)


	if(SOS_IS_LINK)
		sos_sdk_library_target(COVERAGE ${NAME} "" coverage ${SOS_ARCH})
		add_library(${COVERAGE_TARGET} STATIC)
		sos_sdk_copy_target(${RELEASE_TARGET} ${COVERAGE_TARGET})



		target_compile_options(${COVERAGE_TARGET}
			PUBLIC
			--coverage
			)

		sos_sdk_library_add_arch_targets("${COVERAGE_OPTIONS}" ${SOS_ARCH} "${DIRECTORIES}")

		get_target_property(MY_DIR ${COVERAGE_TARGET} BINARY_DIR)
		message(STATUS "BINARY DIR for ${COVERAGE_TARGET} is ${MY_DIR}")

		get_target_property(SOURCES ${COVERAGE_TARGET} SOURCES)



		foreach(SOURCE ${SOURCES})
			get_filename_component(FILE_NAME ${SOURCE} NAME)
			list(APPEND GCOV_SOURCES ${MY_DIR}/CMakeFiles/${COVERAGE_TARGET}.dir/src/${FILE_NAME}.gcda)
		endforeach()

		#list(APPEND GCOV_SCRUB_SOURCES ${GCOV_SOURCES})

		message(STATUS "GCOV SOURCES ${GCOV_SOURCES}")

		add_custom_target(coverage_mkdir_${COVERAGE_TARGET}
			COMMAND mkdir -p ${CMAKE_SOURCE_DIR}/coverage/${COVERAGE_TARGET}
			)

		add_custom_target(coverage_${COVERAGE_TARGET}
			COMMAND gcov ${GCOV_SOURCES}
			WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/coverage/${COVERAGE_TARGET}
			DEPENDS coverage_mkdir_${COVERAGE_TARGET}
			)

		add_custom_target(clean_coverage_${COVERAGE_TARGET}
			COMMAND rm -f ${GCOV_SOURCES}
			DEPENDS coverage_mkdir_${COVERAGE_TARGET}
			)

	endif()


	#message(STATUS "Adding dependencies ${DIRECTORIES}")
	sos_sdk_library_add_arch_targets("${RELEASE_OPTIONS}" ${SOS_ARCH} "${DIRECTORIES}")

	add_custom_target(
		${NAME}_format
		COMMAND /usr/local/bin/clang-format
		-i
		--verbose
		${FORMAT_LIST}
		)

endmacro()
