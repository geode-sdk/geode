add_definitions(-DANDROID)
add_definitions(-DCC_TARGET_OS_ANDROID)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fms-extensions -Xlinker --no-demangle -Wno-deprecated -m32 ")

target_link_libraries(${PROJECT_NAME} log android EGL GLESv2)

include_directories(
	${GEODE_SDK_PATH}/include/cocos/cocos2dx/platform/android
)

macro(link_prebuilts project)
	target_link_libraries(${project} 
		${GEODE_SDK_PATH}/link/libcocos2dcpp.so
	)
endmacro()

link_prebuilts(${PROJECT_NAME})
