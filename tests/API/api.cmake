
macro(api_test_executable NAME DIRECTORIES)

	set(IS_COVERAGE OFF)

	if(SOS_IS_LINK AND IS_COVERAGE)
		set(CONFIG coverage)
		set(ELF_NAME_PREFIX ${NAME}_${CONFIG})
	else()
		set(CONFIG release)
		set(ELF_NAME_PREFIX ${NAME})

	endif()


	sos_sdk_app_target(RELEASE ${NAME} "" ${CONFIG} ${SOS_ARCH})
	add_executable(${RELEASE_TARGET})
	target_sources(${RELEASE_TARGET}
		PRIVATE
		${CMAKE_SOURCE_DIR}/tests/common/main.cpp
		${CMAKE_SOURCE_DIR}/tests/${NAME}/src/sl_config.h
		${CMAKE_SOURCE_DIR}/tests/${NAME}/src/UnitTest.hpp
		)

	if(SOS_IS_LINK AND IS_COVERAGE)
		set_target_properties(${RELEASE_TARGET}
			PROPERTIES
			LINK_FLAGS --coverage)
	else()
		target_compile_options(${RELEASE_TARGET}
			PRIVATE
			-Os
			)
	endif()

	set_property(TARGET ${RELEASE_TARGET} PROPERTY CXX_STANDARD 17)

	foreach(DIRECTORY ${DIRECTORIES})
		target_include_directories(${RELEASE_TARGET}
			PRIVATE
			${CMAKE_SOURCE_DIR}/libraries/${DIRECTORY}/include
			)
	endforeach(DIRECTORY)

	#target_link_libraries(${RELEASE_TARGET}
	#	PRIVATE
	#	mbedtls
	#	)

	target_include_directories(${RELEASE_TARGET}
		PRIVATE
		${CMAKE_SOURCE_DIR}/tests/${NAME}/src
		)

	sos_sdk_app_add_arch_targets("${RELEASE_OPTIONS}" "${DIRECTORIES}" ${RAM_SIZE})

	get_target_property(MY_DIR ${RELEASE_TARGET} BINARY_DIR)
	message(STATUS "BINARY DIR for ${RELEASE_TARGET} is ${MY_DIR}")

	include(CTest)

	set(CTEST_OUTPUT_ON_FAILURE ON)
	if(SOS_IS_LINK)

		add_test(NAME tests${NAME}
			COMMAND "../build_${CONFIG}_link/${ELF_NAME_PREFIX}_link.elf" --api
			WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/tmp
			)


		set_tests_properties(
			tests${NAME}
			PROPERTIES
			PASS_REGULAR_EXPRESSION "___finalResultPass___"
			)

	endif()


endmacro()
