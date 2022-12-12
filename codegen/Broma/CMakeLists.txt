cmake_minimum_required(VERSION 3.0 FATAL_ERROR)
project(Broma LANGUAGES C CXX)

add_library(Broma ${CMAKE_CURRENT_SOURCE_DIR}/src/broma.cpp)

target_compile_features(Broma PRIVATE cxx_std_17)

target_include_directories(Broma PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_compile_definitions(Broma PUBLIC TAO_PEGTL_GHC_FILESYSTEM=1)

target_link_libraries(Broma taocpp::pegtl ghc_filesystem)