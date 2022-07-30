cmake_minimum_required(VERSION 3.8)

add_executable(geode_hook_test "hook_test.cpp")
target_link_libraries(geode_hook_test 
    lilac::hook)

target_compile_features(geode_hook_test PRIVATE cxx_std_17)

if (WIN32)
add_executable(geode_meta_test "meta_test.cpp")
target_link_libraries(geode_meta_test 
    lilac::meta
    lilac::hook)

target_compile_features(geode_meta_test PRIVATE cxx_std_17)
endif()
