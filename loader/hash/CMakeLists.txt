cmake_minimum_required(VERSION 3.0 FATAL_ERROR)

project(GeodeChecksum VERSION 1.0)

add_executable(${PROJECT_NAME} hash.cpp)
target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_20)

target_link_libraries(${PROJECT_NAME} PUBLIC ghc_filesystem)

message(STATUS "Building Checksum Exe")
