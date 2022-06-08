add_definitions(-DCC_TARGET_OS_IPHONE)

if (NOT DEFINED GEODE_IOS_SDK AND APPLE)
	execute_process(COMMAND xcrun --show-sdk-path --sdk iphoneos
		OUTPUT_VARIABLE GEODE_IOS_SDK
		ERROR_QUIET
		OUTPUT_STRIP_TRAILING_WHITESPACE
	)
endif()

if (NOT DEFINED GEODE_IOS_SDK)
	message(FATAL_ERROR "iOS Target requires GEODE_IOS_SDK defined")
endif()

set(CMAKE_SYSTEM_NAME iOS)
target_compile_options(${PROJECT_NAME} PUBLIC -fms-extensions -Wno-deprecated -Wno-ignored-attributes -Os -flto -fvisibility=internal)
target_link_options(${PROJECT_NAME} PUBLIC -arch arm64)

set(CMAKE_OSX_ARCHITECTURES arm64)
set(CMAKE_OSX_SYSROOT ${GEODE_IOS_SDK})
set_target_properties(${PROEJCT_NAME} PROPERTIES OSX_ARCHITECTURES arm64)
set_target_properties(${PROJECT_NAME} PROPERTIES OSX_SYSROOT ${GEODE_IOS_SDK})

if (NOT (CMAKE_CXX_COMPILER_ID STREQUAL "Clang" OR CMAKE_CXX_COMPILER_ID STREQUAL "AppleClang"))
	message(FATAL_ERROR "iOS Targets must use clang" ${CMAKE_CXX_COMPILER_ID})
endif()

include_directories(
	${GEODE_SDK_PATH}/include/cocos/cocos2dx/platform/ios
	${GEODE_SDK_PATH}/include/cocos/cocos2dx/platform/third_party/ios
	${GEODE_SDK_PATH}/include/cocos/cocos2dx/platform/third_party/ios/OGLES
)