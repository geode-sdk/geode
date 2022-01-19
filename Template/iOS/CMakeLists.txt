cmake_minimum_required(VERSION 3.0 FATAL_ERROR)

project(CacTemplate VERSION 0.1.0 LANGUAGES C CXX)

set(CACAO_TARGET_PLATFORM iOS)
set(CACAO_IOS_SDK _sdk)

set(SOURCE_FILES main.cpp)
include(Cacao/cmake/CacaoRoot.cmake)
