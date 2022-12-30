cmake_minimum_required(VERSION 3.3.0)

set(PROJECT_NAME TestMembers)

project(${PROJECT_NAME} VERSION 1.0.0)

add_library(${PROJECT_NAME} SHARED MacOS.cpp Windows.cpp)
target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_20)

target_link_libraries(
    ${PROJECT_NAME}
    geode-sdk
)