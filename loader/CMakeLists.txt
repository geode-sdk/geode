cmake_minimum_required(VERSION 3.21 FATAL_ERROR)

project(geode-loader VERSION 0.2.0 LANGUAGES C CXX)
set(PROJECT_VERSION_TYPE Alpha)

# Package info file for internal representation
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
	LIBRARY_OUTPUT_DIRECTORY "${GEODE_BIN_PATH}"
	RUNTIME_OUTPUT_DIRECTORY "${GEODE_BIN_PATH}"
	ARCHIVE_OUTPUT_DIRECTORY "${GEODE_BIN_PATH}"
)

# Move compiled binary into `bin/nightly` directory
if (APPLE)
	add_custom_command(
		COMMAND 
			${CMAKE_COMMAND} -E rename "$<TARGET_FILE:${PROJECT_NAME}>" "${GEODE_BIN_PATH}/nightly/$<TARGET_FILE_NAME:${PROJECT_NAME}>"
		DEPENDS $<TARGET_FILE:${PROJECT_NAME}>
		VERBATIM
		TARGET ${PROJECT_NAME}
	)
elseif(WIN32)
	add_custom_command(
		COMMAND ${CMAKE_COMMAND} -E rename "$<TARGET_FILE:${PROJECT_NAME}>" "${GEODE_BIN_PATH}/nightly/$<TARGET_FILE_NAME:${PROJECT_NAME}>"
		COMMAND ${CMAKE_COMMAND} -E rename "$<TARGET_LINKER_FILE:${PROJECT_NAME}>" "${GEODE_BIN_PATH}/nightly/$<TARGET_LINKER_FILE_NAME:${PROJECT_NAME}>"
		DEPENDS $<TARGET_FILE:${PROJECT_NAME}>
		VERBATIM
		TARGET ${PROJECT_NAME}
	)
endif()

if (NOT GEODE_DONT_PACKAGE_RESOURCES)
	# Package resources for UI
	package_geode_resources(
		${PROJECT_NAME}
		${CMAKE_CURRENT_SOURCE_DIR}/resources
		${GEODE_BIN_PATH}/nightly/resources
		"geode.loader"
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

target_compile_definitions(${PROJECT_NAME} PUBLIC GEODE_EXPORTING GEODE_PLATFORM_CONSOLE)

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
	"${CMAKE_CURRENT_SOURCE_DIR}/include/Geode/Modify.hpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/Geode/cocos/cocos2dx/include/cocos2d.h"
)

# Create launcher
if (APPLE) 
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
endif()

# Build test mods if needed
if(NOT GEODE_DONT_BUILD_TEST_MODS)
	if(CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
		add_subdirectory(test)
	endif()
endif()

# Build index hashing algorithm test program
add_subdirectory(hash)
