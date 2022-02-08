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
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fms-extensions -Wno-deprecated -fdata-sections -ffunction-sections")
set(CMAKE_C_FLAGS "${CMAKE_CXX_FLAGS} -fms-extensions -Wno-deprecated -fdata-sections -ffunction-sections")
set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -dead_strip")

target_link_options(${PROJECT_NAME} PRIVATE "-arch arm64 -isysroot ${GEODE_IOS_SDK}")

if (NOT (CMAKE_CXX_COMPILER_ID STREQUAL "Clang" OR CMAKE_CXX_COMPILER_ID STREQUAL "AppleClang"))
	message(FATAL_ERROR "iOS Targets must use clang" ${CMAKE_CXX_COMPILER_ID})
endif()

include_directories(
	${GEODE_INCLUDE_DIR}/cocos/cocos2dx/platform/ios
	${GEODE_INCLUDE_DIR}/cocos/cocos2dx/platform/third_party/ios
	${GEODE_INCLUDE_DIR}/cocos/cocos2dx/platform/third_party/ios/OGLES
)