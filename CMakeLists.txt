cmake_minimum_required(VERSION 3.21 FATAL_ERROR)

set(BUILD_SHARED_LIBS OFF CACHE BOOL "Build libraries static" FORCE)
file(READ VERSION GEODE_VERSION)
string(STRIP "${GEODE_VERSION}" GEODE_VERSION)

project(geode-sdk VERSION ${GEODE_VERSION} LANGUAGES CXX C)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
if (PROJECT_IS_TOP_LEVEL AND APPLE)
	set(CMAKE_BUILD_TYPE Debug)
endif()

add_library(${PROJECT_NAME} INTERFACE)

if (CMAKE_BUILD_TYPE STREQUAL Debug OR CMAKE_BUILD_TYPE STREQUAL RelWithDebInfo) 
	target_compile_definitions(${PROJECT_NAME} INTERFACE -DGEODE_DEBUG)
endif()

# Rerun CMake on VERSION file change
set_target_properties(${PROJECT_NAME} PROPERTIES CMAKE_CONFIGURE_DEPENDS VERSION)

target_compile_definitions(${PROJECT_NAME} INTERFACE -DPROJECT_NAME=${CMAKE_PROJECT_NAME})

if (GEODE_DEBUG)
	target_compile_definitions(${PROJECT_NAME} INTERFACE GEODE_DEBUG)
endif()

set(GEODE_CODEGEN_PATH ${CMAKE_CURRENT_BINARY_DIR}/codegenned)
set(GEODE_BIN_PATH ${CMAKE_CURRENT_SOURCE_DIR}/bin)
set(GEODE_LOADER_PATH ${CMAKE_CURRENT_SOURCE_DIR}/loader)

include(cmake/GeodeFile.cmake)
include(cmake/Platform.cmake)

target_sources(${PROJECT_NAME} INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/entry.cpp)


add_subdirectory(codegen)

add_custom_target(CodegenRun ALL
	COMMAND Codegen ${GEODE_TARGET_PLATFORM} bindings ${GEODE_CODEGEN_PATH}
	WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
	COMMENT "Run Codegen"
	BYPRODUCTS ${GEODE_CODEGEN_PATH}/Geode/GeneratedSource.cpp
)
add_dependencies(${PROJECT_NAME} CodegenRun)
add_dependencies(CodegenRun Codegen)

# Hacky way to supress the not generated error
if (NOT EXISTS ${GEODE_CODEGEN_PATH}/Geode/GeneratedSource.cpp)
	make_directory(${GEODE_CODEGEN_PATH})
	make_directory(${GEODE_CODEGEN_PATH}/Geode)
	file(TOUCH ${GEODE_CODEGEN_PATH}/Geode/GeneratedSource.cpp)
endif()

target_sources(${PROJECT_NAME} INTERFACE ${GEODE_CODEGEN_PATH}/Geode/GeneratedSource.cpp)

target_include_directories(${PROJECT_NAME} INTERFACE
	${GEODE_CODEGEN_PATH}
	${GEODE_LOADER_PATH}/include
	${GEODE_LOADER_PATH}/include/Geode/cocos/include
	${GEODE_LOADER_PATH}/include/Geode/cocos/extensions
	${GEODE_LOADER_PATH}/include/Geode/fmod
)
target_link_directories(${PROJECT_NAME} INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/loader/include/link)

add_subdirectory(filesystem)
add_subdirectory(fmt)

target_link_libraries(${PROJECT_NAME} INTERFACE filesystem fmt)



if (NOT EXISTS ${GEODE_BIN_PATH})
	make_directory(${GEODE_BIN_PATH})
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
		"${GEODE_LOADER_PATH}/include/Geode/Loader.hpp"
		"${GEODE_LOADER_PATH}/include/Geode/UI.hpp"
		"${GEODE_LOADER_PATH}/include/Geode/cocos/include/cocos2d.h"
		"${GEODE_LOADER_PATH}/include/Geode/cocos/extensions/cocos-ext.h"
	)
else()
	message(FATAL_ERROR
		"No valid loader binary to link to! Install prebuilts with `geode sdk install-prebuilts` "
		"or build Geode from source."
	)
endif()
