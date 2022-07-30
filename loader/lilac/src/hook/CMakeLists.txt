cmake_minimum_required(VERSION 3.8)

if (GEODE_TARGET_PLATFORM STREQUAL "MacOS")
    set(PLATFORM "macos")
elseif (GEODE_TARGET_PLATFORM STREQUAL "Win32")
    set(PLATFORM "windows")
elseif (GEODE_TARGET_PLATFORM STREQUAL "iOS")
    set(PLATFORM "ios")
else()
    message(FATAL_ERROR "Unable to set platform: ${GEODE_TARGET_PLATFORM}")
endif()

add_library(lilac_hook STATIC
    "hook.cpp"
    "impl.cpp" 
    "${PLATFORM}.cpp")
add_library(lilac::hook ALIAS lilac_hook)

if(WIN32)
target_link_libraries(lilac_hook
    lilac::meta
)
endif()

if (GEODE_TARGET_PLATFORM STREQUAL "iOS")
    target_link_libraries(lilac_hook "-F ${CMAKE_CURRENT_SOURCE_DIR}/../ios -framework CydiaSubstrate")
endif()

target_compile_features(lilac_hook PUBLIC cxx_std_17)
target_include_directories(lilac_hook 
    PRIVATE
        ${lilac_SOURCE_DIR}/include/geode/core/hook
    INTERFACE
        $<BUILD_INTERFACE:${lilac_SOURCE_DIR}/include>
        $<BUILD_INTERFACE:${lilac_SOURCE_DIR}/include/geode>
)
