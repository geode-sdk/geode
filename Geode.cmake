#[[
	geode-exported variables
		GEODE_SDK_PATH            path of geode sdk
		GEODE_TARGET_PLATFORM     target platform
		GEODE_CODEGEN_DIR         location of codegen directory
	
	geode-imported variables
		OUTPUT                    													name of the output binary
		BUILD_CODEGEN             													whether to build
		NO_GEODE_FILE             													dont produce geode file
		NO_LOADER                 													dont link loader
		DISABLE_THE_LINKING_OF_THE_EXTERNAL_DEPENDENCY_KNOWN_AS_FMTLIB				dont add fmt subdirectory
		
]]

cmake_minimum_required(VERSION 3.13.4)

set(GEODE_SDK_PATH ${CMAKE_CURRENT_LIST_DIR})

include(CMakeParseArguments)
include(${GEODE_SDK_PATH}/cmake/GeodeFile.cmake)

function(setup_geode_mod)
	set(bools BUILD_CODEGEN NO_GEODE_FILE NO_LOADER NO_PRECOMPILED_HEADER NO_ENTRY DISABLE_THE_LINKING_OF_THE_EXTERNAL_DEPENDENCY_KNOWN_AS_FMTLIB)
    cmake_parse_arguments(
        PARSED_ARGS
        "${bools}"
        "OUTPUT"
        ""
        ${ARGN}
    )

    set(GEODE_NO_LOADER ${PARSED_ARGS_NO_LOADER})
    set(GEODE_DO_CODEGEN ${PARSED_ARGS_BUILD_CODEGEN})
    set(GEODE_NO_PRECOMPILED_HEADER ${PARSED_ARGS_NO_PRECOMPILED_HEADER})
    set(GEODE_NO_ENTRY ${PARSED_ARGS_NO_ENTRY})
	set(GEODE_DISABLE_THE_LINKING_OF_THE_EXTERNAL_DEPENDENCY_KNOWN_AS_FMTLIB ${PARSED_ARGS_DISABLE_THE_LINKING_OF_THE_EXTERNAL_DEPENDENCY_KNOWN_AS_FMTLIB})
	if(PARSED_ARGS_OUTPUT)
		set_target_properties(${PROJECT_NAME} PROPERTIES PREFIX "" OUTPUT_NAME ${PARSED_ARGS_OUTPUT})
	endif()

	include(${GEODE_SDK_PATH}/cmake/Platform.cmake)
	include(CheckIPOSupported)
	check_ipo_supported(RESULT supported OUTPUT error)
	
	if (NOT ${GEODE_NO_ENTRY})
		target_sources(${PROJECT_NAME} PRIVATE ${GEODE_SDK_PATH}/entry.cpp)
	endif()

	if (${GEODE_DO_CODEGEN})
		add_definitions(-DGEODE_EXPORTING_CODEGEN)

		# only 1 codegen dir
		get_property(GEODE_CODEGEN_DIR GLOBAL PROPERTY GEODE_CODEGEN_DIR)
		if (NOT GEODE_CODEGEN_DIR)
			set(GEODE_CODEGEN_BUILD 1)
			set_property(GLOBAL PROPERTY GEODE_CODEGEN_DIR ${CMAKE_CURRENT_BINARY_DIR}/codegen)
			get_property(GEODE_CODEGEN_DIR GLOBAL PROPERTY GEODE_CODEGEN_DIR)
		endif()

		file(MAKE_DIRECTORY ${GEODE_CODEGEN_DIR})

		set_source_files_properties(${GEODE_CODEGEN_DIR}/GeneratedSource.cpp PROPERTIES GENERATED 1)
		target_sources(${PROJECT_NAME} PRIVATE ${GEODE_CODEGEN_DIR}/GeneratedSource.cpp)
		target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_BINARY_DIR})
		#target_link_libraries(${PROJECT_NAME} fmt)

		if (DEFINED GEODE_CODEGEN_BUILD)
			add_subdirectory(${GEODE_SDK_PATH}/codegen ${GEODE_CODEGEN_DIR})
		endif()
	endif()

	target_compile_definitions(${PROJECT_NAME} PUBLIC -DPROJECT_NAME=${PROJECT_NAME} -DEXPORT_${PROJECT_NAME}=1)
	target_include_directories(${PROJECT_NAME} PUBLIC
		${GEODE_SDK_PATH}/include
		${GEODE_SDK_PATH}/include/codegen-base
		${GEODE_SDK_PATH}/include/c++stl
		${GEODE_SDK_PATH}/include/loader
		${GEODE_SDK_PATH}/include/utils
		${GEODE_SDK_PATH}/include/meta
		${GEODE_SDK_PATH}/include/cocos/
		${GEODE_SDK_PATH}/include/cocos/cocos2dx
		${GEODE_SDK_PATH}/include/cocos/cocos2dx/include
		${GEODE_SDK_PATH}/include/cocos/cocos2dx/support/zip_support
		${GEODE_SDK_PATH}/include/cocos/cocos2dx/kazmath/include
		${GEODE_SDK_PATH}/include/cocos/extensions
		${GEODE_SDK_PATH}/include/fmod
	)

	if (APPLE)
		file(GLOB_RECURSE GEODE_NO_PCH ${CMAKE_SOURCE_DIR}/**/*.mm ${CMAKE_SOURCE_DIR}/**/*.m ${CMAKE_SOURCE_DIR}/*.m ${CMAKE_SOURCE_DIR}/*.mm)
		set_source_files_properties(${GEODE_NO_PCH} PROPERTIES SKIP_PRECOMPILE_HEADERS ON)
		if (NOT GEODE_NO_PRECOMPILED_HEADER)
			target_precompile_headers(${PROJECT_NAME} PUBLIC
				"$<$<COMPILE_LANGUAGE:CXX>:${GEODE_SDK_PATH}/include/Geode.hpp>"
			)
		endif()
	endif()

	if (NOT PARSED_ARGS_NO_GEODE_FILE)
		create_geode_file(${PROJECT_NAME})
	endif()

endfunction()
