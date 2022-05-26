cmake_minimum_required(VERSION 3.0 FATAL_ERROR)
project(CodegenRun LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)

if (NOT DEFINED GEODE_HOST_ARM AND APPLE)
	set(CMAKE_OSX_ARCHITECTURES x86_64)
	set(CMAKE_OSX_SYSROOT )
endif()

add_subdirectory(fmt)

file(GLOB SOURCES
	${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp
)

file(GLOB HEADERS
	${CMAKE_CURRENT_SOURCE_DIR}/Broma/src/*.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/fmt/include/*.h
)

add_executable(Codegen ${SOURCES})
target_link_libraries(Codegen PRIVATE fmt)
target_include_directories(Codegen PRIVATE 
	${CMAKE_CURRENT_SOURCE_DIR}/Broma/src
	${CMAKE_CURRENT_SOURCE_DIR}/fmt/include
	${CMAKE_CURRENT_SOURCE_DIR}/
)
target_precompile_headers(Codegen PRIVATE 
	${CMAKE_CURRENT_SOURCE_DIR}/src/Shared.hpp
	${BROMA_HEADERS}
)

# set(GEODE_TARGET_PLATFORM MacOS)

# set(GEODE_BINDING_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../bindings)

# set(GEODE_CODEGEN_DIR ${CMAKE_CURRENT_BINARY_DIR}/codegen)
# message(${GEODE_BINDING_PATH})

if (NOT DEFINED GEODE_TARGET_PLATFORM)
	message(FATAL_ERROR "GEODE_TARGET_PLATFORM is not defined")
endif()
if (NOT DEFINED GEODE_BINDING_PATH)
	message(FATAL_ERROR "GEODE_BINDING_PATH is not defined")
endif()
if (NOT DEFINED GEODE_CODEGEN_DIR)
	set(GEODE_CODEGEN_DIR ${CMAKE_CURRENT_BINARY_DIR}/codegen)
	message("Using current binary dir for GEODE_CODEGEN_DIR, which is ${CMAKE_CURRENT_BINARY_DIR}/codegen")
endif()

add_custom_target(CodegenRun ALL
	COMMAND Codegen ${GEODE_TARGET_PLATFORM} ${GEODE_BINDING_PATH} ${GEODE_CODEGEN_DIR}
	WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
	COMMENT "Run Codegen"
)
add_dependencies(CodegenRun Codegen)
