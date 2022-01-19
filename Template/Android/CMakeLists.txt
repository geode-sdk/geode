cmake_minimum_required(VERSION 3.0 FATAL_ERROR)
set(GEODE_ANDROID_NDK _ndk)
include(Geode/cmake/AndroidToolchain.cmake)

project(CacTemplate VERSION 0.1.0 LANGUAGES C CXX)

set(GEODE_TARGET_PLATFORM Android)
set(GEODE_GD_APK_FILE _apk)

set(SOURCE_FILES main.cpp)
include(Geode/cmake/GeodeRoot.cmake)
