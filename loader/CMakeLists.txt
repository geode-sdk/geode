cmake_minimum_required(VERSION 3.21 FATAL_ERROR)

project(geode-loader VERSION ${GEODE_VERSION} LANGUAGES C CXX)
set(PROJECT_VERSION_TYPE Alpha)

# Package info file for internal representation
configure_file(resources/mod.json.in ${CMAKE_CURRENT_SOURCE_DIR}/resources/mod.json)
file(READ resources/mod.json LOADER_MOD_JSON)
file(READ resources/about.md LOADER_ABOUT_MD)
configure_file(src/internal/about.hpp.in ${CMAKE_CURRENT_SOURCE_DIR}/src/internal/about.hpp)

# Source files
file(GLOB CORE_SOURCES
	src/cocos2d-ext/*.cpp
	src/core/*.cpp
	src/hooks/*.cpp
	src/internal/*.cpp
	src/internal/windows/*.cpp
	src/internal/mac/*.cpp
	src/internal/ios/*.cpp
	src/load/*.cpp
	src/load/windows/*.cpp
	src/load/mac/*.cpp
	src/mac/*.cpp
	src/main.cpp
	src/utils/*.cpp
	src/utils/windows/*.cpp
	src/utils/zip/*.cpp
	src/index/*.cpp
	src/ui/nodes/*.cpp
	src/ui/internal/*.cpp
	src/ui/internal/credits/*.cpp
	src/ui/internal/dev/*.cpp
	src/ui/internal/info/*.cpp
	src/ui/internal/list/*.cpp
	src/ui/internal/settings/*.cpp
)

file(GLOB OBJC_SOURCES
	src/internal/ios/*.mm
	src/internal/mac/*.mm
	src/load/ios/*.mm
	src/load/mac/*.mm
	src/utils/ios/*.mm
	src/utils/mac/*.mm
)

# Embed version info in binary
if (WIN32)
	configure_file(src/internal/windows/info.rc.in info.rc)
	set(CORE_SOURCES ${CORE_SOURCES} ${CMAKE_CURRENT_BINARY_DIR}/info.rc)
endif()

add_library(${PROJECT_NAME} SHARED
	${CORE_SOURCES}
	${OBJC_SOURCES}
)

make_directory("${GEODE_BIN_PATH}/nightly")

# Prevent SDK from activating a loader build
set_target_properties(${PROJECT_NAME} PROPERTIES 
	PREFIX "" 
	OUTPUT_NAME "Geode"
	# i'm going to say a slur that not a single soul has uttered in the last 200 years
	LIBRARY_OUTPUT_DIRECTORY_RELWITHDEBINFO "${GEODE_BIN_PATH}/nightly"
	RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO "${GEODE_BIN_PATH}/nightly"
	ARCHIVE_OUTPUT_DIRECTORY_RELWITHDEBINFO "${GEODE_BIN_PATH}/nightly"
	LIBRARY_OUTPUT_DIRECTORY_RELEASE "${GEODE_BIN_PATH}/nightly"
	RUNTIME_OUTPUT_DIRECTORY_RELEASE "${GEODE_BIN_PATH}/nightly"
	ARCHIVE_OUTPUT_DIRECTORY_RELEASE "${GEODE_BIN_PATH}/nightly"
	LIBRARY_OUTPUT_DIRECTORY_MINSIZEREL "${GEODE_BIN_PATH}/nightly"
	RUNTIME_OUTPUT_DIRECTORY_MINSIZEREL "${GEODE_BIN_PATH}/nightly"
	ARCHIVE_OUTPUT_DIRECTORY_MINSIZEREL "${GEODE_BIN_PATH}/nightly"
	LIBRARY_OUTPUT_DIRECTORY "${GEODE_BIN_PATH}/nightly"
	RUNTIME_OUTPUT_DIRECTORY "${GEODE_BIN_PATH}/nightly"
	ARCHIVE_OUTPUT_DIRECTORY "${GEODE_BIN_PATH}/nightly"
)

if (GEODE_NO_UNDEFINED_VIRTUALS)
	target_compile_definitions(${PROJECT_NAME} PUBLIC GEODE_NO_UNDEFINED_VIRTUALS)
endif()

if (NOT GEODE_DISABLE_CLI_CALLS)
	# Package resources for UI
	package_geode_resources(
		${PROJECT_NAME}
		${CMAKE_CURRENT_SOURCE_DIR}/resources
		${GEODE_BIN_PATH}/nightly/resources
	)
endif()

target_include_directories(${PROJECT_NAME} PRIVATE
	src/internal/
	src/platform/
	src/gui/
	src/index/
	md4c/src/
	hash/
	./ # lilac
)

# For profiling
set_property(TARGET ${PROJECT_NAME} PROPERTY RULE_LAUNCH_COMPILE "${CMAKE_COMMAND} -E time")

target_compile_definitions(${PROJECT_NAME} PUBLIC GEODE_EXPORTING)

# Markdown support
add_subdirectory(md4c)
target_link_libraries(${PROJECT_NAME} md4c)

# Lilac (hooking)
add_subdirectory(lilac)
target_link_libraries(${PROJECT_NAME} z lilac_hook geode-sdk)

# Use precompiled headers for faster builds
set_source_files_properties(${OBJC_SOURCES} PROPERTIES SKIP_PRECOMPILE_HEADERS ON)
target_precompile_headers(${PROJECT_NAME} PRIVATE
	"${CMAKE_CURRENT_SOURCE_DIR}/include/Geode/DefaultInclude.hpp"
	# "${CMAKE_CURRENT_SOURCE_DIR}/include/Geode/Utils.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/Geode/Loader.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/Geode/UI.hpp"
	# "${CMAKE_CURRENT_SOURCE_DIR}/include/Geode/Bindings.hpp"
	# "${CMAKE_CURRENT_SOURCE_DIR}/include/Geode/Modify.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/Geode/cocos/cocos2dx/include/cocos2d.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/Geode/cocos/extensions/cocos-ext.h"
)

# Create launcher
if (APPLE) 
	add_subdirectory(launcher/mac)

	if(CMAKE_SYSTEM_PROCESSOR MATCHES "arm" OR GEODE_TARGET_PLATFORM STREQUAL "iOS")
		add_custom_command(TARGET geode-loader
		    POST_BUILD COMMAND 
		    ${CMAKE_INSTALL_NAME_TOOL} -id \"/Library/MobileSubstrate/DynamicLibraries/Geode.dylib\"
		    $<TARGET_FILE:geode-loader>)
	endif()
elseif (WIN32)
	add_subdirectory(launcher/windows)

	add_subdirectory(minhook)
	target_link_libraries(${PROJECT_NAME} minhook)

	# target_include_directories(${PROJECT_NAME} PRIVATE dobby/include)
	# target_link_directories(${PROJECT_NAME} PRIVATE dobby)
	# target_link_libraries(${PROJECT_NAME} dobby)

	# set(DOBBY_GENERATE_SHARED OFF CACHE BOOL "Build dobby shared library" FORCE)
	# # set(DOBBY_DEBUG ON CACHE BOOL "Build dobby shared library" FORCE)
	# add_subdirectory(dobby)

	target_link_libraries(${PROJECT_NAME} dbghelp)

	# disable warnings about CCNode::setID
	if (MSVC)
		target_link_options(${PROJECT_NAME} PUBLIC /ignore:4217)
	endif()
endif()

# Build test mods if needed
if(NOT GEODE_DONT_BUILD_TEST_MODS)
	add_subdirectory(test)
endif()

# Build index hashing algorithm test program
add_subdirectory(hash)
