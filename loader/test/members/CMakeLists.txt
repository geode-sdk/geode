cmake_minimum_required(VERSION 3.3.0)

set(PROJECT_NAME TestMembers)

project(${PROJECT_NAME} VERSION 1.0.0)

add_library(${PROJECT_NAME} SHARED MacOS.cpp Windows.cpp)
target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_20)

target_include_directories(${PROJECT_NAME} PRIVATE
	${GEODE_CODEGEN_PATH}
	${GEODE_LOADER_PATH}/include
	${GEODE_LOADER_PATH}/include/Geode/cocos/include
	${GEODE_LOADER_PATH}/include/Geode/cocos/extensions
	${GEODE_LOADER_PATH}/include/Geode/fmod
)

target_compile_definitions(${PROJECT_NAME} PRIVATE -DGEODE_DONT_WARN_INCORRECT_MEMBERS)

target_link_libraries(${PROJECT_NAME} PRIVATE ghc_filesystem mat-json)

add_dependencies(${PROJECT_NAME} CodegenRun)