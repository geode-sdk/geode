if (NOT DEFINED GEODE_TARGET_PLATFORM)
	if(APPLE)
		if(IOS)
			set(GEODE_TARGET_PLATFORM "iOS")
		else()
			set(GEODE_TARGET_PLATFORM "MacOS")
		endif()
	elseif(WIN32)
		set(GEODE_TARGET_PLATFORM "Win32")
	else()
		message(FATAL_ERROR "Unable to detect platform, please set GEODE_TARGET_PLATFORM in the root CMake file.")
	endif()
endif()

if (NOT ${PROJECT_NAME} STREQUAL ${CMAKE_PROJECT_NAME})
	set(GEODE_TARGET_PLATFORM GEODE_TARGET_PLATFORM PARENT_SCOPE)
endif()

if (GEODE_TARGET_PLATFORM STREQUAL "iOS")
	set_target_properties(${PROJECT_NAME} PROPERTIES
		SYSTEM_NAME iOS
		OSX_SYSROOT ${GEODE_IOS_SDK}
		OSX_ARCHITECTURES arm64
	)

	target_compile_definitions(${PROJECT_NAME} -DCC_TARGET_OS_IPHONE)

	set(GEODE_PLATFORM_BINARY "GeodeIOS.dylib")
elseif (GEODE_TARGET_PLATFORM STREQUAL "MacOS")
	set_target_properties(${PROJECT_NAME} PROPERTIES 
		SYSTEM_NAME MacOS
		OSX_DEPLOYMENT_TARGET 10.9
		APPLE_SILICON_PROCESSOR x86_64
	)

	target_include_directories(${PROJECT_NAME} INTERFACE
		${GEODE_LOADER_PATH}/include/Geode/cocos/cocos2dx/platform/mac
		${GEODE_LOADER_PATH}/include/Geode/cocos/cocos2dx/platform/third_party/mac
		${GEODE_LOADER_PATH}/include/Geode/cocos/cocos2dx/platform/third_party/mac/OGLES
	)

	target_link_libraries(${PROJECT_NAME} INTERFACE "-framework Cocoa")
	target_compile_options(${PROJECT_NAME} INTERFACE -fms-extensions -Wno-deprecated -Wno-ignored-attributes -Os -flto -fvisibility=internal)
	target_compile_definitions(${PROJECT_NAME} INTERFACE -DCC_TARGET_OS_MAC)

	set(GEODE_PLATFORM_BINARY "Geode.dylib")

elseif (GEODE_TARGET_PLATFORM STREQUAL "Win32")
	set_target_properties(${PROJECT_NAME} PROPERTIES
		SYSTEM_NAME Win32
		GENERATOR_PLATFORM x86
	)

	target_include_directories(${PROJECT_NAME} INTERFACE
		${GEODE_LOADER_PATH}/include/Geode/cocos/cocos2dx/platform/win32
		${GEODE_LOADER_PATH}/include/Geode/cocos/cocos2dx/platform/third_party/win32
		${GEODE_LOADER_PATH}/include/Geode/cocos/cocos2dx/platform/third_party/win32/zlib
		${GEODE_LOADER_PATH}/include/Geode/cocos/cocos2dx/platform/third_party/win32/OGLES
	)

	target_compile_definitions(${PROJECT_NAME} INTERFACE -DCC_TARGET_OS_WIN32)

	target_link_libraries(${PROJECT_NAME} INTERFACE 
		${GEODE_LOADER_PATH}/include/link/libcocos2d.lib
		${GEODE_LOADER_PATH}/include/link/libExtensions.lib
		${GEODE_LOADER_PATH}/include/link/libcurl.lib
	)

	set(GEODE_PLATFORM_BINARY "Geode.dll")
elseif (GEODE_TARGET_PLATFORM STREQUAL "Android")
	message(FATAL_ERROR "IDK figure it out")
endif()
