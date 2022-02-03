add_definitions(-DCC_TARGET_OS_IPHONE)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fms-extensions -Xlinker --no-demangle -Wno-deprecated -arch arm64 -isysroot ${GEODE_IOS_SDK} ${GEODE_FLAGS}")
set(CMAKE_C_FLAGS "${CMAKE_CXX_FLAGS} -fms-extensions -Xlinker --no-demangle -Wno-deprecated -arch arm64 -isysroot ${GEODE_IOS_SDK} ${GEODE_FLAGS}")
set(CMAKE_SYSTEM_NAME iOS)
set(CMAKE_OSX_SYSROOT ${GEODE_IOS_SDK})

target_include_directories(${PROJECT_NAME} PUBLIC
	Geode/api
	${GEODE_INCLUDE_DIR}/cocos/cocos2dx/platform/ios
	${GEODE_INCLUDE_DIR}/cocos/cocos2dx/platform/third_party/ios
	${GEODE_INCLUDE_DIR}/cocos/cocos2dx/platform/third_party/ios/OGLES
)

set(PACKAGE_DIR ${CMAKE_CURRENT_BINARY_DIR}/ios_package)
set(OUT_FILE "${PROJECT_NAME}_${CMAKE_PROJECT_VERSION}-3+release_iphoneos-arm64.deb")

add_custom_command(
	COMMAND python3 ${CMAKE_SOUCE_DIR}/Geode/kit/pkg/iOS.py "${PACKAGE_DIR}" "${CMAKE_CURRENT_BINARY_DIR}/lib${PROJECT_NAME}.dylib" "${PROJECT_NAME}" "${CMAKE_CURRENT_SOURCE_DIR}/control" "${OUT_FILE}"
	DEPENDS ${GEODE_IOS_CONTROL}
	OUTPUT  ${PACKAGE_DIR}/_
	COMMENT "Packaging ${PROJECT_NAME}"
)

add_custom_target(
	iOSPackage ALL
	DEPENDS ${PACKAGE_DIR}/_
)

add_dependencies(iOSPackage ${PROJECT_NAME})

if (DEFINED GEODE_IOS_REMOTE_HOST)
	install(CODE "execute_process(COMMAND scp \"${PACKAGE_DIR}/${OUT_FILE}\" root@${GEODE_IOS_REMOTE_HOST}:/var/mobile/Documents)")
	install(CODE "execute_process(COMMAND ssh root@${GEODE_IOS_REMOTE_HOST} dpkg -i \"/var/mobile/Documents/${OUT_FILE}\" \"&&\" killall GeometryJump)")
endif()

macro(link_prebuilts project)
endmacro()
