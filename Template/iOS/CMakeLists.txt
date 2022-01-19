cmake_minimum_required(VERSION 3.0 FATAL_ERROR)

project(CacTemplate VERSION 0.1.0 LANGUAGES C CXX)

set(GEODE_TARGET_PLATFORM iOS)
set(GEODE_IOS_SDK _sdk)

set(SOURCE_FILES main.cpp)
include(Geode/cmake/GeodeRoot.cmake)
