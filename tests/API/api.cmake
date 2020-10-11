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
		${CMAKE_SOURCE_DIR}/tests/common/main.cpp
		${CMAKE_SOURCE_DIR}/tests/${NAME}/src/sl_config.h
		${CMAKE_SOURCE_DIR}/tests/${NAME}/src/UnitTest.hpp
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

	target_include_directories(${RELEASE_TARGET}
		PRIVATE
		${CMAKE_SOURCE_DIR}/tests/${NAME}/src
		)

	sos_sdk_app_add_arch_targets("${RELEASE_OPTIONS}" "${DIRECTORIES}" ${RAM_SIZE})

	include(CTest)

	if(SOS_IS_LINK)
	add_test(NAME tests${NAME}
		COMMAND ../build_release_link/${NAME}_link.elf --api
		WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/tmp
		)

	set_tests_properties(
		tests${NAME}
		PROPERTIES
		PASS_REGULAR_EXPRESSION "___finalResultPass___"
		)

	endif()

endmacro()
