cmake_minimum_required(VERSION 3.25 FATAL_ERROR)

set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT Embedded)
cmake_policy(SET CMP0141 NEW)

set(BUILD_SHARED_LIBS OFF CACHE BOOL "Build libraries static" FORCE)

# Docs flags
if (GEODE_BUILDING_DOCS)
	set(GEODE_DISABLE_CLI_CALLS ON)
	set(GEODE_DISABLE_PRECOMPILED_HEADERS ON)
	set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
	set(GEODE_DONT_BUILD_TEST_MODS ON)
endif()

option(GEODE_USE_BREAKPAD "Enables the use of the Breakpad library for crash dumps." ON)

# Read version
file(READ VERSION GEODE_VERSION)
string(STRIP "${GEODE_VERSION}" GEODE_VERSION)
set(GEODE_VERSION "${GEODE_VERSION}" CACHE INTERNAL "Geode version")
set(GEODE_VERSION_FULL "${GEODE_VERSION}" CACHE INTERNAL "Geode version full")

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

include(cmake/CPM.cmake)

if (NOT DEFINED CPM_SOURCE_CACHE)
	message(NOTICE "Not using CPM cache (CPM_SOURCE_CACHE). "
		"It is recommended to use it to improve configure times.")
endif()

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
	if ("${CMAKE_SYSTEM_NAME}" STREQUAL "iOS")
	message(STATUS "Using ios branch for bindings")
        CPMAddPackage(NAME "bindings"
                GITHUB_REPOSITORY "geode-ios/bindings"
                GIT_TAG "main"
                DOWNLOAD_ONLY YES
                NO_CACHE YES
        )
	else()
	CPMAddPackage(NAME "bindings"
		GITHUB_REPOSITORY "geode-sdk/bindings"
		GIT_TAG "main"
		DOWNLOAD_ONLY YES
		NO_CACHE YES
	)
	endif()
	set(GEODE_BINDINGS_REPO_PATH ${bindings_SOURCE_DIR})
else()
	message(STATUS "Using ${GEODE_BINDINGS_REPO_PATH} for bindings repo")
endif()

project(geode-sdk VERSION ${GEODE_VERSION} LANGUAGES CXX C)

message(STATUS "Looking for ccache/sccache")
if (DEFINED CMAKE_C_COMPILER_LAUNCHER OR DEFINED CMAKE_CXX_COMPILER_LAUNCHER)
	message(STATUS "Looking for ccache/sccache - detecting variant")
	if (DEFINED CMAKE_C_COMPILER_LAUNCHER AND DEFINED CMAKE_CXX_COMPILER_LAUNCHER AND
		CMAKE_C_COMPILER_LAUNCHER STREQUAL CMAKE_CXX_COMPILER_LAUNCHER)
		if (CMAKE_CXX_COMPILER_LAUNCHER MATCHES "sccache(|.exe)$")
			message(STATUS "Looking for ccache/sccache - detected sccache")
			set(GEODE_CCACHE_VARIANT "sccache")
		elseif(CMAKE_CXX_COMPILER_LAUNCHER MATCHES "ccache(|.exe)$")
			message(STATUS "Looking for ccache/sccache - detected ccache")
			set(GEODE_CCACHE_VARIANT "ccache")
		else()
			message(STATUS "Looking for ccache/sccache - none (custom compiler launcher: {CMAKE_C_COMPILER_LAUNCHER}, {CMAKE_CXX_COMPILER_LAUNCHER})")
		endif()
	else()
		message(STATUS "Looking for ccache/sccache - skipped (custom compiler launcher: {CMAKE_C_COMPILER_LAUNCHER}, {CMAKE_CXX_COMPILER_LAUNCHER})")
	endif()
elseif (CMAKE_GENERATOR MATCHES "Visual Studio")
	message(STATUS "Looking for ccache/sccache - skipped (Visual Studio generator)")
else()
	find_program(GEODE_CCACHE NAMES sccache ccache)
	if (GEODE_CCACHE)
		if (${GEODE_CCACHE} MATCHES "sccache(|.exe)$")
			set(GEODE_CCACHE_VARIANT "sccache")
		else()
			set(GEODE_CCACHE_VARIANT "ccache")
		endif()
		message(STATUS "Looking for ccache/sccache - found ${GEODE_CCACHE_VARIANT}")
		message(NOTICE "Compiler launcher not set but ccache/sccache was found. "
			"Setting compiler launcher to that")
		set(CMAKE_C_COMPILER_LAUNCHER ${GEODE_CCACHE})
		set(CMAKE_CXX_COMPILER_LAUNCHER ${GEODE_CCACHE})
	else()
		message(STATUS "Looking for ccache/sccache - not found")
	endif()
	unset(GEODE_CCACHE)
endif()

if (DEFINED GEODE_CCACHE_VARIANT)
	if (NOT DEFINED GEODE_DISABLE_PRECOMPILED_HEADERS)
		if (${GEODE_CCACHE_VARIANT} STREQUAL "sccache" AND CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
			message(NOTICE "Using ${GEODE_CCACHE_VARIANT} with ${CMAKE_CXX_COMPILER_ID}, PCH will be enabled.")
			set(GEODE_DISABLE_PRECOMPILED_HEADERS OFF)
		else()
			message(NOTICE "Using ${GEODE_CCACHE_VARIANT} with ${CMAKE_CXX_COMPILER_ID}, PCH will be disabled.")
			set(GEODE_DISABLE_PRECOMPILED_HEADERS ON)
		endif()
	endif()
else()
	message(NOTICE "Not using a caching compiler (ccache/sccache). "
		"It is recommended to install one to improve build times.")
	message(NOTICE "We recommend sccache, check its README for installation instructions, "
		"normally you can just use your usual package manager (e.g. 'scoop install sccache').")
	if (NOT DEFINED GEODE_DISABLE_PRECOMPILED_HEADERS)
		message(NOTICE "Because of this, PCH will be enabled.")
		set(GEODE_DISABLE_PRECOMPILED_HEADERS OFF)
	endif()
endif()

if (NOT DEFINED GEODE_DISABLE_PRECOMPILED_HEADERS)
	set(GEODE_DISABLE_PRECOMPILED_HEADERS OFF)
endif()

if (GEODE_DISABLE_PRECOMPILED_HEADERS)
	message(STATUS "Pre-compiled headers - OFF")
else()
	message(STATUS "Pre-compiled headers - ON")
endif()

add_library(${PROJECT_NAME} INTERFACE)

# Rerun CMake on VERSION file change
set_target_properties(${PROJECT_NAME} PROPERTIES CMAKE_CONFIGURE_DEPENDS VERSION)

target_compile_definitions(${PROJECT_NAME} INTERFACE -DPROJECT_NAME=${CMAKE_PROJECT_NAME})

set(GEODE_BIN_PATH ${CMAKE_CURRENT_SOURCE_DIR}/bin)
set(GEODE_LOADER_PATH ${CMAKE_CURRENT_SOURCE_DIR}/loader)
set(GEODE_ROOT_PATH ${CMAKE_CURRENT_SOURCE_DIR})

include(cmake/Platform.cmake)
include(cmake/GeodeFile.cmake)

if (NOT DEFINED GEODE_GD_VERSION)
	set(GEODE_GD_VERSION 2.206)
	set(GEODE_COMP_GD_VERSION 22060)
endif()

target_compile_definitions(${PROJECT_NAME} INTERFACE GEODE_GD_VERSION=${GEODE_GD_VERSION} GEODE_COMP_GD_VERSION=${GEODE_COMP_GD_VERSION})

if (WIN32)
	# This allows you to compile in debug mode
	# add_compile_definitions(_HAS_ITERATOR_DEBUGGING=0)
	add_definitions(-D_HAS_ITERATOR_DEBUGGING=0)
	target_compile_definitions(${PROJECT_NAME} INTERFACE _HAS_ITERATOR_DEBUGGING=0)
	
	target_link_libraries(${PROJECT_NAME} INTERFACE delayimp ws2_32)
	if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang" AND
		CMAKE_CXX_COMPILER_FRONTEND_VARIANT STREQUAL "GNU")
		# target x86 on windows with clang
		target_compile_options(${PROJECT_NAME} INTERFACE --target=x86_64-pc-windows-msvc)
		target_link_options(${PROJECT_NAME} INTERFACE --target=x86_64-pc-windows-msvc)
		add_compile_options(--target=x86_64-pc-windows-msvc)
		add_link_options(--target=x86_64-pc-windows-msvc)
	endif()
endif()

# if (APPLE AND CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
# 	set_property(TARGET ${PROJECT_NAME} PROPERTY LINKER_TYPE LLD)
# 	target_link_options(${PROJECT_NAME} INTERFACE -fuse-ld=lld)
# endif ()

# We do not support anything below SDK 23
if (ANDROID)
	string(REGEX MATCH "[0-9]+" ANDROID_PLATFORM_NUMBER "${ANDROID_PLATFORM}")
	if (ANDROID_PLATFORM_NUMBER LESS 23)
		message(FATAL_ERROR "Specified target Android SDK version is too low (${ANDROID_PLATFORM_NUMBER}), must be 23 or higher")
	endif()
endif()

set(MAT_JSON_AS_INTERFACE ON)
CPMAddPackage("gh:geode-sdk/json#3a79c51")
CPMAddPackage("gh:fmtlib/fmt#10.1.1")

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

# Allow users to have their own copy of TulipHook that can be overwritten with a CMake option.
# If the option is not provided, by default just clone TulipHook with CPM and use that
if (DEFINED ENV{GEODE_TULIPHOOK_REPO_PATH})
	set(temp $ENV{GEODE_TULIPHOOK_REPO_PATH})
	# this is so stupid i hate windows paths
	string(REPLACE "\\" "/" GEODE_TULIPHOOK_REPO_PATH ${temp})
endif()

if (DEFINED GEODE_TULIPHOOK_REPO_PATH)
	message(STATUS "Using ${GEODE_TULIPHOOK_REPO_PATH} for TulipHook")
	add_subdirectory(${GEODE_TULIPHOOK_REPO_PATH} ${GEODE_TULIPHOOK_REPO_PATH}/build)
else()
	CPMAddPackage("gh:geode-sdk/TulipHook#b8357de")
endif()
set(CMAKE_WARN_DEPRECATED ON CACHE BOOL "" FORCE)

include(CheckCXXCompilerFlag)
check_cxx_compiler_flag(-Wno-everything SUPPORTS_W_NO_EVERYTHING)
if (SUPPORTS_W_NO_EVERYTHING)
	# Silence warnings from dependencies
	if (TARGET capstone)
		target_compile_options(capstone PRIVATE -Wno-everything)
	endif()
endif()

target_sources(${PROJECT_NAME} INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/entry.cpp)

add_subdirectory(${GEODE_BINDINGS_REPO_PATH} ${CMAKE_BINARY_DIR}/bindings)

if (NOT GEODE_DISABLE_PRECOMPILED_HEADERS)
	target_precompile_headers(GeodeBindings INTERFACE
		"$<$<COMPILE_LANGUAGE:CXX>:${GEODE_LOADER_PATH}/include/Geode/Bindings.hpp>"
	)
endif()

target_include_directories(GeodeBindings PUBLIC
	${GEODE_LOADER_PATH}/include
	${GEODE_LOADER_PATH}/include/Geode/cocos/include
	${GEODE_LOADER_PATH}/include/Geode/cocos/extensions
	${GEODE_LOADER_PATH}/include/Geode/fmod
)
target_link_directories(GeodeBindings PUBLIC ${GEODE_LOADER_PATH}/include/link)
target_link_libraries(GeodeBindings PUBLIC fmt TulipHookInclude mat-json)
target_link_libraries(${PROJECT_NAME} INTERFACE GeodeBindings)

if (NOT EXISTS ${GEODE_BIN_PATH})
	file(MAKE_DIRECTORY ${GEODE_BIN_PATH})
endif()

if (NOT EXISTS ${GEODE_BIN_PATH}/${PROJECT_VERSION} AND EXISTS ${GEODE_BIN_PATH}/nightly/)
	set(GEODE_LINK_NIGHTLY 1)
endif()

if (${GEODE_LINK_NIGHTLY})
	set(GEODE_PLATFORM_BIN_PATH ${GEODE_BIN_PATH}/nightly/${GEODE_PLATFORM_BINARY})
else()
	set(GEODE_PLATFORM_BIN_PATH ${GEODE_BIN_PATH}/${PROJECT_VERSION}/${GEODE_PLATFORM_BINARY})
endif()

if (PROJECT_IS_TOP_LEVEL)
	add_subdirectory(loader)
	target_link_libraries(${PROJECT_NAME} INTERFACE geode-loader)
	if (SUPPORTS_W_NO_EVERYTHING)
		target_compile_options(geode-loader PRIVATE -Wno-inconsistent-missing-override)
	endif()
elseif (EXISTS ${GEODE_PLATFORM_BIN_PATH})
	target_link_libraries(${PROJECT_NAME} INTERFACE "${GEODE_PLATFORM_BIN_PATH}")
	if (NOT GEODE_DISABLE_PRECOMPILED_HEADERS)
		target_precompile_headers(${PROJECT_NAME} INTERFACE
			"$<$<COMPILE_LANGUAGE:CXX>:${GEODE_LOADER_PATH}/include/Geode/DefaultInclude.hpp>"
			"$<$<COMPILE_LANGUAGE:CXX>:${GEODE_LOADER_PATH}/include/Geode/Geode.hpp>"
			# please stop adding modify here its not here because it makes windows compilation take longer than geode 1.0 release date
		)
	endif()
else()
	message(FATAL_ERROR
		"No valid loader binary to link to! Install pre-built binaries with `geode sdk install-binaries`, "
		"or build Geode from source and add `set(GEODE_LINK_NIGHTLY ON)` to your CMakeLists.txt "
		"in the line before calling add_subdirectory for Geode."
	)
endif()
