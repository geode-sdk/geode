cmake_minimum_required(VERSION 3.0 FATAL_ERROR)
project(Codegen LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)

add_subdirectory(fmt)

macro(add_gen project file)
	add_executable(${project}Gen ${project}Gen.cpp)
	target_link_libraries(${project}Gen PUBLIC fmt)
	target_include_directories(${project}Gen PUBLIC 
		${CMAKE_CURRENT_SOURCE_DIR}/Broma/src
		${CMAKE_CURRENT_SOURCE_DIR}/fmt/include
	)
	add_custom_target(${project}Run
		COMMAND ${project}Gen ${CACAO_TARGET_PLATFORM} "${CMAKE_CURRENT_SOURCE_DIR}/data" "${CACAO_CODEGEN_DIR}/${file}"
		WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
		COMMENT "Run ${project}Gen"
	)
	add_dependencies(${CMAKE_PROJECT_NAME} ${project}Run)
endmacro()


add_gen(Interface Interface.hpp)
add_gen(Source Source.cpp)
add_gen(Header Header.hpp)
