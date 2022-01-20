cmake_minimum_required(VERSION 3.8)

project(geode_core LANGUAGES C CXX)

add_subdirectory("src/hook")
if(WIN32) # TODO: replace it with target platform
    add_subdirectory("src/meta")
endif()

if(CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
    add_subdirectory("test")
endif()

target_include_directories(
    geode_core_hook INTERFACE 
    ${geode_core_SOURCE_DIR}/include/geode
    ${geode_core_SOURCE_DIR}/include/geode/core
    ${geode_core_SOURCE_DIR}/include/geode/core/hook
)
