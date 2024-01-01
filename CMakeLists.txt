cmake_minimum_required(VERSION 3.21 FATAL_ERROR)

set(BUILD_SHARED_LIBS OFF CACHE BOOL "Build libraries static" FORCE)

# Docs flags
if (GEODE_BUILDING_DOCS)
	set(GEODE_DISABLE_CLI_CALLS On)
	set(CMAKE_EXPORT_COMPILE_COMMANDS On)
	set(GEODE_DISABLE_PRECOMPILED_HEADERS On)
endif()

set(GEODE_GD_VERSION 2.200)

# Read version
file(READ VERSION GEODE_VERSION)
string(STRIP "${GEODE_VERSION}" GEODE_VERSION)

# Check if version has a tag like v1.0.0-alpha
string(FIND ${GEODE_VERSION} "-" GEODE_VERSION_HAS_TAG)
if (NOT ${GEODE_VERSION_HAS_TAG} EQUAL "-1")
	string(REGEX MATCH "[a-z]+(\.[0-9]+)?$" GEODE_VERSION_TAG ${GEODE_VERSION})
	string(SUBSTRING "${GEODE_VERSION}" 0 ${GEODE_VERSION_HAS_TAG} GEODE_VERSION)
	string(FIND ${GEODE_VERSION_TAG} "." GEODE_VERSION_TAG_HAS_NUMBER)

	# Extract tag type and number from tag
	if (NOT ${GEODE_VERSION_TAG_HAS_NUMBER} EQUAL "-1")
		string(SUBSTRING "${GEODE_VERSION_TAG}" 0 ${GEODE_VERSION_TAG_HAS_NUMBER} GEODE_VERSION_TAG_TYPE)
		math(EXPR GEODE_VERSION_TAG_HAS_NUMBER "${GEODE_VERSION_TAG_HAS_NUMBER} + 1")
		string(SUBSTRING "${GEODE_VERSION_TAG}" ${GEODE_VERSION_TAG_HAS_NUMBER} -1 GEODE_VERSION_TAG_NUMBER)
	else()
		set(GEODE_VERSION_TAG_TYPE "${GEODE_VERSION_TAG}")
		set(GEODE_VERSION_TAG_NUMBER "")
	endif()

	# Capitalize first letter of tag type
	string(SUBSTRING ${GEODE_VERSION_TAG_TYPE} 0 1 FIRST_LETTER)
	string(TOUPPER ${FIRST_LETTER} FIRST_LETTER)
	string(REGEX REPLACE "^.(.*)" "${FIRST_LETTER}\\1" GEODE_VERSION_TAG_TYPE "${GEODE_VERSION_TAG_TYPE}")

	message(STATUS "Version: ${GEODE_VERSION}, tag: ${GEODE_VERSION_TAG} (type: ${GEODE_VERSION_TAG_TYPE}, number: ${GEODE_VERSION_TAG_NUMBER})")
else()
	set(GEODE_VERSION_TAG "")
	set(GEODE_VERSION_TAG_TYPE "")
	set(GEODE_VERSION_TAG_NUMBER "")

	message(STATUS "Version: ${GEODE_VERSION}")
endif()

project(geode-sdk VERSION ${GEODE_VERSION} LANGUAGES CXX C)

add_library(${PROJECT_NAME} INTERFACE)

if (CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)
	set(GEODE_ALWAYS_BUILD_CODEGEN ON)
endif()

if (NOT DEFINED GEODE_DEBUG AND (CMAKE_BUILD_TYPE STREQUAL Debug OR CMAKE_BUILD_TYPE STREQUAL RelWithDebInfo))
	set(GEODE_DEBUG ON)
endif()

# define it in command line or your project system
if (GEODE_DEBUG) 
	target_compile_definitions(${PROJECT_NAME} INTERFACE -DGEODE_DEBUG)
endif()

# Rerun CMake on VERSION file change
set_target_properties(${PROJECT_NAME} PROPERTIES CMAKE_CONFIGURE_DEPENDS VERSION)

target_compile_definitions(${PROJECT_NAME} INTERFACE -DPROJECT_NAME=${CMAKE_PROJECT_NAME})

set(GEODE_CODEGEN_PATH ${CMAKE_CURRENT_BINARY_DIR}/codegenned)
set(GEODE_BIN_PATH ${CMAKE_CURRENT_SOURCE_DIR}/bin)
set(GEODE_LOADER_PATH ${CMAKE_CURRENT_SOURCE_DIR}/loader)
set(GEODE_ROOT_PATH ${CMAKE_CURRENT_SOURCE_DIR})

include(cmake/Platform.cmake)
include(cmake/GeodeFile.cmake)
include(cmake/CPM.cmake)

if (PROJECT_IS_TOP_LEVEL AND NOT GEODE_BUILDING_DOCS)
	set(MAT_JSON_AS_INTERFACE ON)
endif()
CPMAddPackage("gh:geode-sdk/json#a47f570")
CPMAddPackage("gh:fmtlib/fmt#10.1.1")
CPMAddPackage("gh:gulrak/filesystem#3e5b930")

target_compile_definitions(${PROJECT_NAME} INTERFACE MAT_JSON_DYNAMIC=1)

# this is needed for cross compilation on linux,
# since fmtlib will fail to compile otherwise
if (GEODE_DISABLE_FMT_CONSTEVAL)
	message(VERBOSE "Disabling FMT_CONSTEVAL")
	target_compile_definitions(fmt PUBLIC -DFMT_CONSTEVAL=)
endif()

# Tulip hook (hooking)
if (PROJECT_IS_TOP_LEVEL AND NOT GEODE_BUILDING_DOCS)
	set(TULIP_LINK_SOURCE ON)
endif()
set(CMAKE_WARN_DEPRECATED OFF CACHE BOOL "" FORCE)
CPMAddPackage("gh:geode-sdk/TulipHook#80b1f57")
set(CMAKE_WARN_DEPRECATED ON CACHE BOOL "" FORCE)

# Silence warnings from dependencies
include(CheckCXXCompilerFlag)
check_cxx_compiler_flag(-Wno-everything SUPPORTS_W_NO_EVERYTHING)
if (SUPPORTS_W_NO_EVERYTHING)
	if (TARGET capstone)
		target_compile_options(capstone PRIVATE -Wno-everything)
	endif()
endif()

target_sources(${PROJECT_NAME} INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/entry.cpp)

# Filesystem implementation in a separate target because i couldnt disable pch
add_library(GeodeFilesystemImpl ${CMAKE_CURRENT_SOURCE_DIR}/FilesystemImpl.cpp)
target_compile_features(GeodeFilesystemImpl PUBLIC cxx_std_20)
target_link_libraries(GeodeFilesystemImpl PUBLIC ghc_filesystem)

# Allow users to have their own copy of bindings that can be overwritten with a CMake option.
# If the option is not provided, by default just clone bindings with CPM and use that
if (DEFINED ENV{GEODE_BINDINGS_REPO_PATH})
	set(temp $ENV{GEODE_BINDINGS_REPO_PATH})
	# this is so stupid i hate windows paths
	string(REPLACE "\\" "/" GEODE_BINDINGS_REPO_PATH ${temp})
endif()

if (NOT GEODE_BINDINGS_REPO_PATH)
	message(STATUS 
		"No override path for bindings provided, using CPM to clone default. "
		"If you would like to use a separate clone of the bindings repo "
		"(for example in order to be able to efficiently change and "
		"contribute new bindings) then set GEODE_BINDINGS_REPO_PATH to where you have "
		"cloned the repository (system environment variables are supported)."
	)
	CPMAddPackage("gh:geode-sdk/bindings#main")
	set(GEODE_BINDINGS_REPO_PATH ${GeodeBindings_SOURCE_DIR})
else()
	message(STATUS "Using ${GEODE_BINDINGS_REPO_PATH} for bindings repo")
endif()

include(ExternalProject)
set(GEODE_CODEGEN_BINARY_OUT ${CMAKE_CURRENT_BINARY_DIR}/codegen)
ExternalProject_Add(CodegenProject
	BUILD_ALWAYS ON
	SOURCE_DIR ${GEODE_BINDINGS_REPO_PATH}
	# manually set configure command as to not inherit generator used by geode,
	# this should hopefully fix generator cache mismatch between different projects, however
	# it causes a warning to be shown every time. if you know a better solution please tell us ok thx
	CONFIGURE_COMMAND ${CMAKE_COMMAND} ${GEODE_CODEGEN_CMAKE_ARGS} -DCMAKE_INSTALL_PREFIX:STRING=${GEODE_CODEGEN_BINARY_OUT}
		-DCMAKE_C_COMPILER_LAUNCHER=${CMAKE_C_COMPILER_LAUNCHER} -DCMAKE_CXX_COMPILER_LAUNCHER=${CMAKE_CXX_COMPILER_LAUNCHER}
		-DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
		-S <SOURCE_DIR> -B <BINARY_DIR>
	BUILD_COMMAND ${CMAKE_COMMAND} --build <BINARY_DIR> --config $<CONFIGURATION>
	INSTALL_COMMAND ${CMAKE_COMMAND} --install <BINARY_DIR> --config $<CONFIGURATION>
)

if (NOT GEODE_BINDINGS_PATH)
	set(GEODE_BINDINGS_PATH ${GEODE_BINDINGS_REPO_PATH}/bindings/${GEODE_GD_VERSION})
endif()

file(GLOB CODEGEN_DEPENDS CONFIGURE_DEPENDS 
	${GEODE_BINDINGS_PATH}/*.bro
	${GEODE_BINDINGS_REPO_PATH}/codegen/src/*.cpp
	${GEODE_BINDINGS_REPO_PATH}/codegen/src/*.hpp
)


file(GLOB CODEGEN_OUTPUTS CONFIGURE_DEPENDS 
	${GEODE_CODEGEN_PATH}/Geode/binding/*.hpp
)

add_custom_command(
	DEPENDS ${CODEGEN_DEPENDS}
	DEPENDS CodegenProject
	COMMAND ${GEODE_CODEGEN_BINARY_OUT}/Codegen ${GEODE_TARGET_PLATFORM} ${GEODE_BINDINGS_PATH} ${GEODE_CODEGEN_PATH}
	COMMAND echo codegen > ${GEODE_CODEGEN_PATH}/.stamp
	WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
	COMMENT "Run Codegen"
	OUTPUT ${GEODE_CODEGEN_PATH}/Geode/GeneratedSource.cpp ${GEODE_CODEGEN_PATH}/Geode/GeneratedAddress.cpp ${GEODE_CODEGEN_PATH}/.stamp ${CODEGEN_OUTPUTS}
)

add_custom_target(CodegenRun
	DEPENDS ${GEODE_CODEGEN_PATH}/.stamp
)

add_dependencies(${PROJECT_NAME} CodegenRun)

# Hacky way to supress the not generated error
if (NOT EXISTS ${GEODE_CODEGEN_PATH}/Geode/GeneratedSource.cpp)
	make_directory(${GEODE_CODEGEN_PATH})
	make_directory(${GEODE_CODEGEN_PATH}/Geode)
	file(TOUCH ${GEODE_CODEGEN_PATH}/Geode/GeneratedSource.cpp)
endif()

if (NOT EXISTS ${GEODE_CODEGEN_PATH}/Geode/GeneratedAddress.cpp)
	make_directory(${GEODE_CODEGEN_PATH})
	make_directory(${GEODE_CODEGEN_PATH}/Geode)
	file(TOUCH ${GEODE_CODEGEN_PATH}/Geode/GeneratedAddress.cpp)
endif()

add_library(GeodeCodegenSources ${GEODE_CODEGEN_PATH}/Geode/GeneratedSource.cpp ${GEODE_CODEGEN_PATH}/Geode/GeneratedAddress.cpp)
target_link_directories(GeodeCodegenSources PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/loader/include/link)
target_link_libraries(GeodeCodegenSources PRIVATE ghc_filesystem GeodeFilesystemImpl fmt TulipHookInclude mat-json)
target_include_directories(GeodeCodegenSources PRIVATE
	${GEODE_CODEGEN_PATH}
	${GEODE_LOADER_PATH}/include
	${GEODE_LOADER_PATH}/include/Geode/cocos/include
	${GEODE_LOADER_PATH}/include/Geode/cocos/extensions
	${GEODE_LOADER_PATH}/include/Geode/fmod
)
set_target_properties(GeodeCodegenSources PROPERTIES CXX_VISIBILITY_PRESET hidden)
target_compile_features(GeodeCodegenSources PUBLIC cxx_std_20)

if (APPLE)
	target_compile_options(GeodeCodegenSources PUBLIC -ffunction-sections -fdata-sections)
	target_link_options(GeodeCodegenSources PUBLIC -dead_strip)
elseif(ANDROID)
	target_compile_options(GeodeCodegenSources PUBLIC -ffunction-sections -fdata-sections)
	target_link_options(GeodeCodegenSources PUBLIC -Wl,--gc-sections)
endif()

if (NOT GEODE_DISABLE_PRECOMPILED_HEADERS)
	target_precompile_headers(GeodeCodegenSources INTERFACE
		"${GEODE_LOADER_PATH}/include/Geode/Bindings.hpp"
	)
endif()

target_include_directories(${PROJECT_NAME} INTERFACE
	${GEODE_CODEGEN_PATH}
	${GEODE_LOADER_PATH}/include
	${GEODE_LOADER_PATH}/include/Geode/cocos/include
	${GEODE_LOADER_PATH}/include/Geode/cocos/extensions
	${GEODE_LOADER_PATH}/include/Geode/fmod
)
target_link_directories(${PROJECT_NAME} INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/loader/include/link)

target_link_libraries(${PROJECT_NAME} INTERFACE ghc_filesystem fmt TulipHookInclude GeodeCodegenSources mat-json GeodeFilesystemImpl)


if (NOT EXISTS ${GEODE_BIN_PATH})
	make_directory(${GEODE_BIN_PATH})
endif()

if (NOT EXISTS ${GEODE_BIN_PATH}/${PROJECT_VERSION} AND EXISTS ${GEODE_BIN_PATH}/nightly/)
	set(GEODE_LINK_NIGHTLY 1)
endif()

if (${GEODE_LINK_NIGHTLY})
	set(GEODE_PLATFORM_BIN_PATH ${GEODE_BIN_PATH}/nightly/${GEODE_PLATFORM_BINARY})
else()
	set(GEODE_PLATFORM_BIN_PATH ${GEODE_BIN_PATH}/${PROJECT_VERSION}/${GEODE_PLATFORM_BINARY})
endif()

if (WIN32)
	# This allows you to compile in debug mode
	add_compile_definitions(_HAS_ITERATOR_DEBUGGING=0)
endif()


if (PROJECT_IS_TOP_LEVEL)
	add_subdirectory(loader)
	target_link_libraries(${PROJECT_NAME} INTERFACE geode-loader)
elseif(EXISTS ${GEODE_PLATFORM_BIN_PATH})
	target_link_libraries(${PROJECT_NAME} INTERFACE "${GEODE_PLATFORM_BIN_PATH}")
	target_precompile_headers(${PROJECT_NAME} INTERFACE
		"${GEODE_LOADER_PATH}/include/Geode/DefaultInclude.hpp"
		"${GEODE_LOADER_PATH}/include/Geode/Geode.hpp"
		# please stop adding modify here its not here because it makes windows compilation take longer than geode 1.0 release date
	)
else()
	message(FATAL_ERROR
		"No valid loader binary to link to! Install prebuilts with `geode sdk install-binaries`, "
		"or build Geode from source and add `set(GEODE_LINK_NIGHTLY ON)` to your CMakeLists.txt "
		"in the line before calling add_subdirectory for Geode."
	)
endif()
