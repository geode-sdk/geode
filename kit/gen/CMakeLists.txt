cmake_minimum_required(VERSION 3.0 FATAL_ERROR)
project(Codegen LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)

if (NOT DEFINED GEODE_HOST_ARM AND APPLE)
	set(CMAKE_OSX_ARCHITECTURES x86_64)
	set(CMAKE_OSX_SYSROOT )
endif()

add_subdirectory(../../include/fmt ${CMAKE_BINARY_DIR}/fmt)

macro(add_gen project file)
	add_executable(${project}Gen ${project}Gen.cpp)
	target_link_libraries(${project}Gen PUBLIC fmt)
	target_include_directories(${project}Gen PUBLIC 
		${CMAKE_CURRENT_SOURCE_DIR}/Broma/src
		${CMAKE_CURRENT_SOURCE_DIR}/fmt/include
	)
	add_custom_target(${project}Run
		COMMAND ${project}Gen ${GEODE_TARGET_PLATFORM} "${CMAKE_CURRENT_SOURCE_DIR}/data" "${GEODE_CODEGEN_DIR}/${file}"
		WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
		COMMENT "Run ${project}Gen"
	)
	add_dependencies(${CMAKE_PROJECT_NAME} ${project}Run)
endmacro()


add_gen(Source GeneratedSource.cpp)
add_gen(Header GeneratedHeader.hpp)
# add_gen(Tidy Tidy.bro) # run manually
add_gen(Wrapper GeneratedWrapper.hpp)
add_gen(Modify GeneratedModify.hpp)
add_gen(Type GeneratedType.hpp)
add_gen(Address GeneratedAddress.hpp)
