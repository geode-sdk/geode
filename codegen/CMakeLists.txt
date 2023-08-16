cmake_minimum_required(VERSION 3.0 FATAL_ERROR)
project(Codegen LANGUAGES C CXX)

include(../cmake/CPM.cmake)

CPMAddPackage("gh:fmtlib/fmt#9.1.0")
CPMAddPackage("gh:geode-sdk/Broma#460f82d")

file(GLOB SOURCES
	${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp
)

add_executable(${PROJECT_NAME} ${SOURCES})
target_compile_features(Codegen PUBLIC cxx_std_17)

target_link_libraries(Codegen PRIVATE fmt::fmt Broma)
target_include_directories(Codegen PRIVATE 
	${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_precompile_headers(Codegen PRIVATE 
	${CMAKE_CURRENT_SOURCE_DIR}/src/Shared.hpp
)

install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX})
