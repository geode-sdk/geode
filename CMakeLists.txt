cmake_minimum_required(VERSION 3.21 FATAL_ERROR)

set(BUILD_SHARED_LIBS OFF CACHE BOOL "Build libraries static" FORCE)

# Read version
file(READ VERSION GEODE_VERSION)
string(STRIP "${GEODE_VERSION}" GEODE_VERSION)

# Check if version has a tag like v1.0.0-alpha
string(FIND ${GEODE_VERSION} "-" GEODE_VERSION_HAS_TAG)
if (GEODE_VERSION_HAS_TAG)
	string(REGEX MATCH "[a-z]+[0-9]?$" GEODE_VERSION_TAG ${GEODE_VERSION})
	string(SUBSTRING "${GEODE_VERSION}" 0 ${GEODE_VERSION_HAS_TAG} GEODE_VERSION)
else()
	set(GEODE_VERSION_TAG "")
endif()

message(STATUS "Version: ${GEODE_VERSION}, tag: ${GEODE_VERSION_TAG}")

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

include(cmake/GeodeFile.cmake)
include(cmake/Platform.cmake)
include(cmake/CPM.cmake)

target_sources(${PROJECT_NAME} INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/entry.cpp)

include(ExternalProject)
set(GEODE_CODEGEN_BINARY_OUT ${CMAKE_CURRENT_BINARY_DIR}/codegen)
ExternalProject_Add(CodegenProject
	BUILD_ALWAYS ON
	SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/codegen
	CMAKE_CACHE_ARGS "-DCMAKE_INSTALL_PREFIX:STRING=${GEODE_CODEGEN_BINARY_OUT}"
	CMAKE_ARGS ${GEODE_CODEGEN_CMAKE_ARGS}
)


file(GLOB CODEGEN_DEPENDS CONFIGURE_DEPENDS 
	${CMAKE_CURRENT_SOURCE_DIR}/bindings/*.bro
	${CMAKE_CURRENT_SOURCE_DIR}/codegen/src/*.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/codegen/src/*.hpp
)

add_custom_command(
	DEPENDS ${CODEGEN_DEPENDS}
	DEPENDS CodegenProject
	COMMAND ${GEODE_CODEGEN_BINARY_OUT}/Codegen ${GEODE_TARGET_PLATFORM} bindings ${GEODE_CODEGEN_PATH}
	COMMAND echo codegen > ${GEODE_CODEGEN_PATH}/.stamp
	WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
	COMMENT "Run Codegen"
	OUTPUT ${GEODE_CODEGEN_PATH}/Geode/GeneratedSource.cpp ${GEODE_CODEGEN_PATH}/Geode/GeneratedAddress.cpp ${GEODE_CODEGEN_PATH}/.stamp
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
target_link_libraries(GeodeCodegenSources PRIVATE ghc_filesystem fmt TulipHookInclude)
target_include_directories(GeodeCodegenSources PRIVATE
	${GEODE_CODEGEN_PATH}
	${GEODE_LOADER_PATH}/include
	${GEODE_LOADER_PATH}/include/Geode/cocos/include
	${GEODE_LOADER_PATH}/include/Geode/cocos/extensions
	${GEODE_LOADER_PATH}/include/Geode/fmod
)
target_compile_features(GeodeCodegenSources PUBLIC cxx_std_20)
target_precompile_headers(GeodeCodegenSources INTERFACE
	"${GEODE_LOADER_PATH}/include/Geode/Bindings.hpp"
)

target_include_directories(${PROJECT_NAME} INTERFACE
	${GEODE_CODEGEN_PATH}
	${GEODE_LOADER_PATH}/include
	${GEODE_LOADER_PATH}/include/Geode/cocos/include
	${GEODE_LOADER_PATH}/include/Geode/cocos/extensions
	${GEODE_LOADER_PATH}/include/Geode/fmod
)
target_link_directories(${PROJECT_NAME} INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/loader/include/link)


CPMAddPackage("gh:fmtlib/fmt#9.1.0")
CPMAddPackage("gh:gulrak/filesystem#3e5b930")

# Tulip hook (hooking)
CPMAddPackage("gh:geode-sdk/TulipHook#52052be")

target_link_libraries(${PROJECT_NAME} INTERFACE ghc_filesystem fmt TulipHookInclude GeodeCodegenSources)


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
		"No valid loader binary to link to! Install prebuilts with `geode sdk install-prebuilts`, "
		"or build Geode from source and add `set(GEODE_LINK_NIGHTLY On)` to your CMakeLists.txt "
		"in the line before calling add_subdirectory for Geode."
	)
endif()
