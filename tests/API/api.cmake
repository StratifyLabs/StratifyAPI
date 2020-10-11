macro(api_test_executable NAME DIRECTORIES)

	string(COMPARE EQUAL ${SOS_BUILD_CONFIG} link IS_LINK)

	if(IS_LINK)
		set(ARCH link)
	else()
		set(ARCH v7m)
	endif()

	sos_sdk_app_target(RELEASE ${NAME} "" release ${ARCH})
	add_executable(${RELEASE_TARGET})
	target_sources(${RELEASE_TARGET}
		PRIVATE
		${CMAKE_CURRENT_LIST_DIR}/src/main.cpp
		${CMAKE_CURRENT_LIST_DIR}/src/sl_config.h
		)

	target_compile_options(${RELEASE_TARGET}
		PRIVATE
		-Os
		)

	set_property(TARGET ${RELEASE_TARGET} PROPERTY CXX_STANDARD 17)

	foreach(DIRECTORY ${DIRECTORIES})
		target_include_directories(${RELEASE_TARGET}
			PRIVATE
			${CMAKE_SOURCE_DIR}/libraries/${DIRECTORY}/include
			)
	endforeach(DIRECTORY)

	sos_sdk_app_add_arch_targets("${RELEASE_OPTIONS}" "${DIRECTORIES}" ${RAM_SIZE})

endmacro()
