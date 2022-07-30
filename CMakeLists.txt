cmake_minimum_required(VERSION 3.21 FATAL_ERROR)

project(geode-sdk VERSION 0.2.0 LANGUAGES CXX C)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(${PROJECT_NAME} INTERFACE)

target_compile_definitions(${PROJECT_NAME} INTERFACE -DPROJECT_NAME=${CMAKE_PROJECT_NAME})

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
	BYPRODUCTS ${GEODE_CODEGEN_PATH}/GeneratedSource.cpp
)
add_dependencies(${PROJECT_NAME} CodegenRun)
add_dependencies(CodegenRun Codegen)

target_sources(${PROJECT_NAME} INTERFACE ${GEODE_CODEGEN_PATH}/GeneratedSource.cpp)
target_include_directories(${PROJECT_NAME} INTERFACE ${GEODE_CODEGEN_PATH}/..)

target_include_directories(${PROJECT_NAME} INTERFACE
	${GEODE_LOADER_PATH}/include
	${GEODE_LOADER_PATH}/include/Geode/cocos/
	${GEODE_LOADER_PATH}/include/Geode/cocos/cocos2dx
	${GEODE_LOADER_PATH}/include/Geode/cocos/cocos2dx/include
	${GEODE_LOADER_PATH}/include/Geode/cocos/cocos2dx/support/zip_support
	${GEODE_LOADER_PATH}/include/Geode/cocos/cocos2dx/kazmath/include
	${GEODE_LOADER_PATH}/include/Geode/cocos/extensions
	${GEODE_LOADER_PATH}/include/Geode/fmod
)
target_link_directories(${PROJECT_NAME} INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/loader/include/link)

add_subdirectory(filesystem)
add_subdirectory(fmt)

target_link_libraries(${PROJECT_NAME} INTERFACE filesystem fmt)

add_subdirectory(loader)

if (NOT EXISTS ${GEODE_BIN_PATH})
	make_directory(${GEODE_BIN_PATH})
endif()

if (GEODE_LINK_SOURCE)
	set(GEODE_PLATFORM_BIN_PATH ${GEODE_BIN_PATH}/nightly/${GEODE_PLATFORM_BINARY})
else()
	set(GEODE_PLATFORM_BIN_PATH ${GEODE_BIN_PATH}/${PROJECT_VERSION}/${GEODE_PLATFORM_BINARY})
endif()


if (PROJECT_IS_TOP_LEVEL)
	target_link_libraries(${PROJECT_NAME} INTERFACE geode-loader)
elseif(EXISTS ${GEODE_PLATFORM_BIN_PATH})
	target_link_libraries(${PROJECT_NAME} INTERFACE "${GEODE_PLATFORM_BIN_PATH}")
	target_precompile_headers(${PROJECT_NAME} INTERFACE "${GEODE_LOADER_PATH}/include/Geode/Geode.hpp")
else()
	message(FATAL_ERROR "No valid loader binary to link to! Install a pre-built binary for version ${PROJECT_VERSION} with Geode CLI or build it from source.")
endif()
