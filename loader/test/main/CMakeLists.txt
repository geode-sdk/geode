cmake_minimum_required(VERSION 3.3.0)

set(PROJECT_NAME TestMod)

project(${PROJECT_NAME} VERSION 1.0.0)

add_library(${PROJECT_NAME} SHARED main.cpp)
target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_20)

set(GEODE_LINK_SOURCE ON)
set_target_properties(${PROJECT_NAME} PROPERTIES PREFIX "")

setup_geode_mod(${PROJECT_NAME} DONT_INSTALL)
