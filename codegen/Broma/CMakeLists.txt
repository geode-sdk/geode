cmake_minimum_required(VERSION 3.0 FATAL_ERROR)
project(Broma LANGUAGES C CXX)

add_library(Broma ${CMAKE_CURRENT_SOURCE_DIR}/src/broma.cpp)

target_compile_features(Broma PRIVATE cxx_std_17)

target_include_directories(Broma PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

add_subdirectory(PEGTL-3.2.7)

target_link_libraries(Broma taocpp::pegtl)