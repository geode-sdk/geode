cmake_minimum_required(VERSION 3.0 FATAL_ERROR)
set(CACAO_ANDROID_NDK _ndk)
include(Cacao/cmake/AndroidToolchain.cmake)

project(CacTemplate VERSION 0.1.0 LANGUAGES C CXX)

set(CACAO_TARGET_PLATFORM Android)
set(CACAO_GD_APK_FILE _apk)

set(SOURCE_FILES main.cpp)
include(Cacao/cmake/CacaoRoot.cmake)
