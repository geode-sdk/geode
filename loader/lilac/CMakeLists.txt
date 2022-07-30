cmake_minimum_required(VERSION 3.8)

project(lilac LANGUAGES C CXX)

add_subdirectory("src/hook")
if(WIN32) # TODO: replace it with target platform
    add_subdirectory("src/meta")
endif()

if(CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
    add_subdirectory("test")
endif()

target_include_directories(
    lilac_hook INTERFACE 
    ${lilac_SOURCE_DIR}/include/geode
    ${lilac_SOURCE_DIR}/include/geode/core
    ${lilac_SOURCE_DIR}/include/geode/core/hook
)
