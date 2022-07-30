cmake_minimum_required(VERSION 3.8)

add_library(lilac_meta INTERFACE)
add_library(lilac::meta ALIAS lilac_meta)

target_compile_features(lilac_meta INTERFACE cxx_std_17)
target_include_directories(
    lilac_meta
    INTERFACE
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
)