cmake_minimum_required(VERSION 3.21 FATAL_ERROR)

project(geode-loader VERSION ${GEODE_VERSION} LANGUAGES C CXX)
if (GEODE_VERSION_TAG_TYPE)
	if (GEODE_VERSION_TAG_NUMBER)
		set(PROJECT_VERSION_TAG_CONSTR "geode::VersionTag(geode::VersionTag::${GEODE_VERSION_TAG_TYPE}, ${GEODE_VERSION_TAG_NUMBER})")
	else()
		set(PROJECT_VERSION_TAG_CONSTR "geode::VersionTag::${GEODE_VERSION_TAG_TYPE}")
	endif()
else()
	set(PROJECT_VERSION_TAG_CONSTR "std::nullopt")
endif()

if (GEODE_VERSION_TAG)
	set(PROJECT_VERSION_SUFFIX "-${GEODE_VERSION_TAG}")
else()
	set(PROJECT_VERSION_SUFFIX "")
endif()

# https://stackoverflow.com/a/63924044/9124836
execute_process(
	COMMAND git rev-parse --short HEAD
	WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
	OUTPUT_VARIABLE GEODE_COMMIT_HASH
	OUTPUT_STRIP_TRAILING_WHITESPACE
	ERROR_QUIET
)

if (NOT GEODE_COMMIT_HASH)
	set(GEODE_COMMIT_HASH "(?)")
endif()

execute_process(
	COMMAND git rev-parse --short HEAD
	WORKING_DIRECTORY ${GEODE_BINDINGS_REPO_PATH}
	OUTPUT_VARIABLE GEODE_BINDINGS_COMMIT_HASH
	OUTPUT_STRIP_TRAILING_WHITESPACE
	ERROR_QUIET
)

if (NOT GEODE_BINDINGS_COMMIT_HASH)
	set(GEODE_BINDINGS_COMMIT_HASH "(?)")
endif()

# Package info file for internal representation
set(GEODE_RESOURCES_PATH ${CMAKE_CURRENT_SOURCE_DIR}/resources)
configure_file(resources/mod.json.in ${CMAKE_CURRENT_SOURCE_DIR}/resources/mod.json)
file(READ resources/mod.json LOADER_MOD_JSON)
configure_file(${GEODE_ROOT_PATH}/VERSION ${GEODE_RESOURCES_PATH}/version COPYONLY)
configure_file(${GEODE_RESOURCES_PATH}/about.md.in ${GEODE_RESOURCES_PATH}/about.md NEWLINE_STYLE LF)
configure_file(${GEODE_ROOT_PATH}/CHANGELOG.md ${GEODE_RESOURCES_PATH}/changelog.md NEWLINE_STYLE LF)
configure_file(${GEODE_RESOURCES_PATH}/support.md.in ${GEODE_RESOURCES_PATH}/support.md NEWLINE_STYLE LF)
configure_file(src/internal/about.hpp.in ${CMAKE_CURRENT_SOURCE_DIR}/src/internal/about.hpp)

# Source files
file(GLOB SOURCES CONFIGURE_DEPENDS 
	src/cocos2d-ext/*.cpp
	src/cocos2d-ext/zip/*.cpp
	src/core/*.cpp
	src/hooks/*.cpp
	src/ids/*.cpp
	src/internal/*.cpp
	src/loader/*.cpp
	src/load.cpp
	src/utils/*.cpp
	src/ui/*.cpp
	src/ui/nodes/*.cpp
	src/ui/internal/*.cpp
	src/ui/internal/info/*.cpp
	src/ui/internal/list/*.cpp
	src/ui/internal/settings/*.cpp
	src/c++stl/*.cpp
	hash/hash.cpp
)

# Obj-c sources
file(GLOB OBJC_SOURCES
	src/platform/Objcpp.mm
)
set_source_files_properties(${OBJC_SOURCES} PROPERTIES SKIP_PRECOMPILE_HEADERS ON)

if (WIN32)
	# This allows you to compile in debug mode
	add_compile_definitions(_HAS_ITERATOR_DEBUGGING=0)
	add_definitions(-D_HAS_ITERATOR_DEBUGGING=0)
endif()


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

elseif(ANDROID)

	file(GLOB ANDROID_SOURCES CONFIGURE_DEPENDS
		src/platform/android/*.cpp
	)
	list(APPEND SOURCES ${ANDROID_SOURCES})

endif()

# Embed version info in binary
if (WIN32)
	configure_file(src/platform/windows/info.rc.in info.rc)
	set(SOURCES ${SOURCES} ${CMAKE_CURRENT_BINARY_DIR}/info.rc)
endif()


if (ANDROID AND GEODE_USE_BREAKPAD)
	CPMAddPackage(
		NAME "Breakpad"
		VERSION "2023.06.01"
		GIT_REPOSITORY "https://chromium.googlesource.com/breakpad/breakpad.git"
	)

	if (Breakpad_ADDED)
		# uses fetchcontent to store the library in an arbitrary path (the binary dir)
		include(FetchContent)

		FetchContent_Declare(lss
			GIT_REPOSITORY https://chromium.googlesource.com/linux-syscall-support.git
			GIT_TAG v2022.10.12
			SOURCE_DIR ${Breakpad_BINARY_DIR}/src/third_party/lss
		)

		FetchContent_MakeAvailable(lss)

		add_library(Breakpad STATIC
			${Breakpad_SOURCE_DIR}/src/client/linux/crash_generation/crash_generation_client.cc
			${Breakpad_SOURCE_DIR}/src/client/linux/dump_writer_common/thread_info.cc
			${Breakpad_SOURCE_DIR}/src/client/linux/dump_writer_common/ucontext_reader.cc
			${Breakpad_SOURCE_DIR}/src/client/linux/handler/exception_handler.cc
			${Breakpad_SOURCE_DIR}/src/client/linux/handler/minidump_descriptor.cc
			${Breakpad_SOURCE_DIR}/src/client/linux/log/log.cc
			${Breakpad_SOURCE_DIR}/src/client/linux/microdump_writer/microdump_writer.cc
			${Breakpad_SOURCE_DIR}/src/client/linux/minidump_writer/linux_dumper.cc
			${Breakpad_SOURCE_DIR}/src/client/linux/minidump_writer/linux_ptrace_dumper.cc
			${Breakpad_SOURCE_DIR}/src/client/linux/minidump_writer/minidump_writer.cc
			${Breakpad_SOURCE_DIR}/src/client/linux/minidump_writer/pe_file.cc
			${Breakpad_SOURCE_DIR}/src/client/minidump_file_writer.cc
			${Breakpad_SOURCE_DIR}/src/common/convert_UTF.cc
			${Breakpad_SOURCE_DIR}/src/common/md5.cc
			${Breakpad_SOURCE_DIR}/src/common/string_conversion.cc
			${Breakpad_SOURCE_DIR}/src/common/linux/breakpad_getcontext.S
			${Breakpad_SOURCE_DIR}/src/common/linux/elfutils.cc
			${Breakpad_SOURCE_DIR}/src/common/linux/file_id.cc
			${Breakpad_SOURCE_DIR}/src/common/linux/guid_creator.cc
			${Breakpad_SOURCE_DIR}/src/common/linux/linux_libc_support.cc
			${Breakpad_SOURCE_DIR}/src/common/linux/memory_mapped_file.cc
			${Breakpad_SOURCE_DIR}/src/common/linux/safe_readlink.cc
		)

		target_include_directories(Breakpad PUBLIC
			${Breakpad_SOURCE_DIR}/src/common/android/include/
			${Breakpad_SOURCE_DIR}/src/
			${Breakpad_SOURCE_DIR}/src/common/android/include/

			# add lss headers
			${Breakpad_BINARY_DIR}/src/
		)
	endif()
endif()

add_library(${PROJECT_NAME} SHARED ${SOURCES})

target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_20)

file(MAKE_DIRECTORY "${GEODE_BIN_PATH}/nightly")

# Prevent SDK from activating a loader build
set_target_properties(${PROJECT_NAME} PROPERTIES
	PREFIX ""
	OUTPUT_NAME ${GEODE_OUTPUT_NAME}
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

if (APPLE) 
	# I don't care a single bit where to put this
	file(COPY ${GEODE_LOADER_PATH}/include/link/libfmod.dylib DESTINATION ${GEODE_BIN_PATH}/nightly)
endif()

if (ANDROID)
	# needed to define some opengl functions
	target_link_libraries(${PROJECT_NAME} EGL)

	if (GEODE_USE_BREAKPAD)
		target_compile_definitions(${PROJECT_NAME} PRIVATE -DGEODE_USE_BREAKPAD)
		enable_language(ASM)
		target_link_libraries(${PROJECT_NAME} Breakpad)
	endif()
endif()

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

target_compile_definitions(${PROJECT_NAME} PUBLIC GEODE_EXPORTING MAT_JSON_EXPORTING)

target_compile_definitions(${PROJECT_NAME} PRIVATE
	GEODE_EXPOSE_SECRET_INTERNALS_IN_HEADERS_DO_NOT_DEFINE_PLEASE
	_CRT_SECURE_NO_WARNINGS
)

# These are only needed for building source :-)
if (NOT GEODE_BUILDING_DOCS)
	# Markdown support
	CPMAddPackage("gh:mity/md4c#e9ff661")

	# Zip support (needed for in-memory streams, which zlib's minizip doesn't support)
	set(MZ_LZMA Off CACHE INTERNAL "Enables LZMA & XZ compression")
	set(MZ_ZSTD Off CACHE INTERNAL "")
	set(MZ_BZIP2 Off CACHE INTERNAL "")
	set(MZ_OPENSSL Off CACHE INTERNAL "")
	set(MZ_LIBBSD Off CACHE INTERNAL "")
	set(SKIP_INSTALL_ALL On CACHE INTERNAL "")
	set(CMAKE_WARN_DEPRECATED OFF CACHE BOOL "" FORCE)
	CPMAddPackage(
		GITHUB_REPOSITORY zlib-ng/minizip-ng
		GIT_TAG cee6d8c
		OPTIONS "CMAKE_POLICY_DEFAULT_CMP0048 OLD" "ZLIB_TAG v1.2.13"
	)
	set(CMAKE_WARN_DEPRECATED ON CACHE BOOL "" FORCE)

	# Silence warnings from dependencies
	include(CheckCXXCompilerFlag)
	check_cxx_compiler_flag(-Wno-everything SUPPORTS_W_NO_EVERYTHING)
	if (SUPPORTS_W_NO_EVERYTHING)
		if (TARGET md4c)
			target_compile_options(md4c PRIVATE -Wno-everything)
		endif()
		if (TARGET zlib)
			target_compile_options(zlib PRIVATE -Wno-everything)
		endif()
		if (TARGET zlibstatic)
			target_compile_options(zlibstatic PRIVATE -Wno-everything)
		endif()
	endif()

	# Regex support
	CPMAddPackage("gh:google/re2#954656f")

	target_include_directories(${PROJECT_NAME} PRIVATE ${md4c_SOURCE_DIR}/src)

	target_link_libraries(${PROJECT_NAME} md4c re2 minizip)
endif()

target_link_libraries(${PROJECT_NAME} z TulipHook geode-sdk mat-json-impl)

if (MSVC)
	# Disable outputting .exp file
	include(CheckLinkerFlag)
	check_linker_flag(CXX /NOEXP SUPPORTS_NOEXP)
	if (SUPPORTS_NOEXP)
		target_link_options(${PROJECT_NAME} PRIVATE /NOEXP)
	endif()

	# Disable incremental link in CI builds
	if (DEFINED ENV{GEODE_CI})
		target_link_options(${PROJECT_NAME} PRIVATE /INCREMENTAL:NO)
	endif()

	target_link_options(${PROJECT_NAME} PRIVATE /DELAYLOAD:libcocos2d.dll /DELAYLOAD:libExtensions.dll)
endif()

# Use precompiled headers for faster builds
if (NOT GEODE_DISABLE_PRECOMPILED_HEADERS)
	target_precompile_headers(${PROJECT_NAME} PRIVATE
		"${CMAKE_CURRENT_SOURCE_DIR}/include/Geode/Geode.hpp"
		# "${CMAKE_CURRENT_SOURCE_DIR}/include/Geode/Utils.hpp"
		# "${CMAKE_CURRENT_SOURCE_DIR}/include/Geode/Loader.hpp"
		# "${CMAKE_CURRENT_SOURCE_DIR}/include/Geode/UI.hpp"
		# "${CMAKE_CURRENT_SOURCE_DIR}/include/Geode/Bindings.hpp"
		# "${CMAKE_CURRENT_SOURCE_DIR}/include/Geode/cocos/include/cocos2d.h"
		# "${CMAKE_CURRENT_SOURCE_DIR}/include/Geode/cocos/extensions/cocos-ext.h"
	)
endif()

# Create launcher
if (APPLE)
	set_target_properties(geode-loader PROPERTIES 
		SYSTEM_NAME MacOS
		OSX_DEPLOYMENT_TARGET 10.9
		APPLE_SILICON_PROCESSOR x86_64
	)

	add_subdirectory(launcher/mac)

	if(GEODE_TARGET_PLATFORM STREQUAL "iOS")
		add_custom_command(TARGET geode-loader
			POST_BUILD COMMAND
			${CMAKE_INSTALL_NAME_TOOL} -id \"/Library/MobileSubstrate/DynamicLibraries/Geode.dylib\"
			$<TARGET_FILE:geode-loader>)
	endif()

	set(LAUNCHER_TARGET GeodeBootstrapper)
elseif (WIN32)
	add_subdirectory(launcher/windows)

	target_link_libraries(${PROJECT_NAME} dbghelp)

	if (MSVC)
		# disable warnings about CCNode::setID
		target_link_options(${PROJECT_NAME} PUBLIC /ignore:4217)
	endif()

	set(LAUNCHER_TARGET ProxyLoader Updater)
endif()

add_subdirectory(test)

# Add install target on CLI >= 2.10.0 (which adds `geode profile path`)
if (NOT GEODE_BUILDING_DOCS AND ${GEODE_CLI_VERSION} VERSION_GREATER_EQUAL "2.10.0")
	execute_process(
		COMMAND ${GEODE_CLI} profile path -d
		OUTPUT_VARIABLE GEODE_PROFILE_GD_PATH
		RESULT_VARIABLE GEODE_PROFILE_GD_PATH_RESULT
	)
	if (${GEODE_PROFILE_GD_PATH_RESULT} EQUAL 0)
		string(STRIP ${GEODE_PROFILE_GD_PATH} GEODE_PROFILE_GD_PATH)
		install(
			TARGETS geode-loader ${LAUNCHER_TARGET}
			RUNTIME
			DESTINATION "${GEODE_PROFILE_GD_PATH}"
		)
		file(GLOB RESOURCE_FILES ${GEODE_BIN_PATH}/nightly/resources/*.*)
		install(
			FILES ${RESOURCE_FILES}
			DESTINATION "${GEODE_PROFILE_GD_PATH}/geode/resources/geode.loader"
		)
	endif()
endif()
