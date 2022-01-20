cmake_minimum_required(VERSION 3.8)

add_library(geode_core_meta INTERFACE)
add_library(geode::core::meta ALIAS geode_core_meta)

target_compile_features(geode_core_meta INTERFACE cxx_std_17)
target_include_directories(
    geode_core_meta
    INTERFACE
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
)