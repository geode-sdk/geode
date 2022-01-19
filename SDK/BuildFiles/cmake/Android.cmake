add_definitions(-DANDROID)
add_definitions(-DCC_TARGET_OS_ANDROID)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fms-extensions -Xlinker --no-demangle -Wno-deprecated -m32 ")

target_link_libraries(${PROJECT_NAME} log android EGL GLESv2)

include_directories(
	${GEODE_INCLUDE_DIR}/cocos/cocos2dx/platform/android
)

macro(link_prebuilts project)
	target_link_libraries(${project} 
		${CMAKE_SOURCE_DIR}/Extern/Android/libcocos2dcpp.so
	)
endmacro()

link_prebuilts(${PROJECT_NAME})

include_directories(
	"${GEODE_ANDROID_NDK}/toolchains/llvm/prebuilt/darwin-x86_64/sysroot/usr/include"
	"${GEODE_ANDROID_NDK}/toolchains/llvm/prebuilt/darwin-x86_64/sysroot/usr/include/arm-linux-androideabi"
	"${GEODE_ANDROID_NDK}/toolchains/llvm/prebuilt/darwin-x86_64/sysroot/usr/include/c++/v1"
)

set(PACKAGE_DIR ${CMAKE_CURRENT_BINARY_DIR}/AndroidBuild)
set(OUT_FILE "${PROJECT_NAME}_${CMAKE_PROJECT_VERSION}-3.apk")

add_custom_command(
	COMMAND python3 ${CMAKE_SOURCE_DIR}/BuildFiles/pkg/Android.py "${PACKAGE_DIR}" "${CMAKE_CURRENT_BINARY_DIR}"  "${PROJECT_NAME}" "${GEODE_APK_FILE}" "${OUT_FILE}"
	DEPENDS ${GEODE_ANDROID_CONTROL}
	OUTPUT  ${PACKAGE_DIR}/_
	COMMENT "Packaging ${PROJECT_NAME}"
)

add_custom_target(
	AndroidPackage ALL
	DEPENDS ${PACKAGE_DIR}/_
)

add_dependencies(AndroidPackage ${PROJECT_NAME})

add_custom_command(
	COMMAND adb install "${PACKAGE_DIR}/${PROJECT_NAME}_${CMAKE_PROJECT_VERSION}-3-aligned-debugSigned.apk"
	WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
	COMMENT "Install target ${PROJECT_NAME}"
	OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/run0
)

add_custom_target(
	Install ALL
	DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/run0
)

add_dependencies(Install AndroidPackage)
