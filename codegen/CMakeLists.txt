cmake_minimum_required(VERSION 3.0 FATAL_ERROR)
project(Codegen LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)

add_subdirectory(Broma)
add_subdirectory(../fmt ${CMAKE_CURRENT_BINARY_DIR}/fmt)
add_subdirectory(../filesystem ${CMAKE_CURRENT_BINARY_DIR}/fs)

file(GLOB SOURCES
	${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp
)

add_executable(${PROJECT_NAME} ${SOURCES})
target_link_libraries(Codegen PRIVATE fmt::fmt Broma)
target_include_directories(Codegen PRIVATE 
	${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_precompile_headers(Codegen PRIVATE 
	${CMAKE_CURRENT_SOURCE_DIR}/src/Shared.hpp
)

install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX})