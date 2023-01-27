cmake_minimum_required(VERSION 3.21 FATAL_ERROR)

project(geode-loader VERSION ${GEODE_VERSION} LANGUAGES C CXX)
set(PROJECT_VERSION_TYPE geode::VersionTag::Beta)
set(PROJECT_VERSION_SUFFIX -beta)

# Package info file for internal representation
configure_file(resources/mod.json.in ${CMAKE_CURRENT_SOURCE_DIR}/resources/mod.json)
file(READ resources/mod.json LOADER_MOD_JSON)
file(READ resources/about.md LOADER_ABOUT_MD)
configure_file(src/internal/about.hpp.in ${CMAKE_CURRENT_SOURCE_DIR}/src/internal/about.hpp)

# Source files
file(GLOB SOURCES CONFIGURE_DEPENDS 
	src/cocos2d-ext/*.cpp
	src/cocos2d-ext/zip/*.cpp
	src/core/*.cpp
	src/hooks/*.cpp
	src/ids/*.cpp
	src/internal/*.cpp
	src/platform/mac/*.cpp
	src/platform/ios/*.cpp
	src/loader/*.cpp
	src/main.cpp
	src/utils/*.cpp
	src/ui/*.cpp
	src/ui/nodes/*.cpp
	src/ui/internal/*.cpp
	src/ui/internal/credits/*.cpp
	src/ui/internal/dev/*.cpp
	src/ui/internal/info/*.cpp
	src/ui/internal/list/*.cpp
	src/ui/internal/settings/*.cpp
)

# Obj-c sources
file(GLOB OBJC_SOURCES
	src/platform/ios/*.mm
	src/platform/mac/*.mm
)
set_source_files_properties(${OBJC_SOURCES} PROPERTIES SKIP_PRECOMPILE_HEADERS ON)

# Add platform sources
if (WIN32)

	file(GLOB WIN_SOURCES CONFIGURE_DEPENDS 
		src/platform/windows/*.cpp
	)
	list(APPEND SOURCES ${WIN_SOURCES})

elseif(IOS)

	file(GLOB IOS_SOURCES CONFIGURE_DEPENDS 
		src/platform/ios/*.cpp
	)
	list(APPEND SOURCES ${IOS_SOURCES})
	list(APPEND SOURCES ${OBJC_SOURCES})

elseif(APPLE)

	file(GLOB MAC_SOURCES CONFIGURE_DEPENDS 
		src/platform/mac/*.cpp
	)
	list(APPEND SOURCES ${MAC_SOURCES})
	list(APPEND SOURCES ${OBJC_SOURCES})

endif()

# Embed version info in binary
if (WIN32)
	configure_file(src/platform/windows/info.rc.in info.rc)
	set(SOURCES ${SOURCES} ${CMAKE_CURRENT_BINARY_DIR}/info.rc)
endif()

add_library(${PROJECT_NAME} SHARED ${SOURCES})

target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_20)

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

# Package resources for UI
package_geode_resources_now(
	${PROJECT_NAME}
	${CMAKE_CURRENT_SOURCE_DIR}/resources
	${GEODE_BIN_PATH}/nightly/resources
	${CMAKE_CURRENT_SOURCE_DIR}/src/internal/resources.hpp
)

target_include_directories(${PROJECT_NAME} PRIVATE
	src/
	src/loader/
	src/internal/
	src/platform/
	src/gui/
	hash/
	./
)

if (APPLE)
	# For profiling
	target_compile_options(${PROJECT_NAME} PUBLIC "-ftime-trace")
	# target_link_options(${PROJECT_NAME} PRIVATE "-Wl,-e,_dynamicInit")
	#set_property(TARGET ${PROJECT_NAME} PROPERTY RULE_LAUNCH_COMPILE "${CMAKE_COMMAND} -E time")
endif()

target_compile_definitions(${PROJECT_NAME} PUBLIC GEODE_EXPORTING)

# These are only needed for building source :-)
if (NOT GEODE_BUILDING_DOCS)
	# Markdown support
	CPMAddPackage("gh:mity/md4c#e9ff661")

	# Regex support
	CPMAddPackage("gh:google/re2#954656f")

	target_include_directories(${PROJECT_NAME} PRIVATE ${md4c_SOURCE_DIR}/src)

	target_link_libraries(${PROJECT_NAME} md4c re2)
endif()

target_link_libraries(${PROJECT_NAME} z TulipHook geode-sdk mat-json)


# Use precompiled headers for faster builds
if (NOT GEODE_DISABLE_PRECOMPILED_HEADERS)
	target_precompile_headers(${PROJECT_NAME} PRIVATE
		"${CMAKE_CURRENT_SOURCE_DIR}/include/Geode/DefaultInclude.hpp"
		# "${CMAKE_CURRENT_SOURCE_DIR}/include/Geode/Utils.hpp"
		"${CMAKE_CURRENT_SOURCE_DIR}/include/Geode/Loader.hpp"
		"${CMAKE_CURRENT_SOURCE_DIR}/include/Geode/UI.hpp"
		# "${CMAKE_CURRENT_SOURCE_DIR}/include/Geode/Bindings.hpp"
		"${CMAKE_CURRENT_SOURCE_DIR}/include/Geode/cocos/include/cocos2d.h"
		"${CMAKE_CURRENT_SOURCE_DIR}/include/Geode/cocos/extensions/cocos-ext.h"
	)
endif()

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

	CPMAddPackage("gh:TsudaKageyu/minhook#49d03ad")
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
