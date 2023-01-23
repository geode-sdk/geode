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

	set(GEODE_PLATFORM_BINARY "GeodeIOS.dylib")
elseif (GEODE_TARGET_PLATFORM STREQUAL "MacOS")
	set_target_properties(${PROJECT_NAME} PROPERTIES 
		SYSTEM_NAME MacOS
		OSX_DEPLOYMENT_TARGET 10.9
		APPLE_SILICON_PROCESSOR x86_64
	)

	target_link_libraries(${PROJECT_NAME} INTERFACE curl "-framework Cocoa")
	target_compile_options(${PROJECT_NAME} INTERFACE -fms-extensions #[[-Wno-deprecated]] -Wno-ignored-attributes -Os #[[-flto]] #[[-fvisibility=internal]])

	set(GEODE_PLATFORM_BINARY "Geode.dylib")

elseif (GEODE_TARGET_PLATFORM STREQUAL "Win32")
	set_target_properties(${PROJECT_NAME} PROPERTIES
		SYSTEM_NAME Win32
		GENERATOR_PLATFORM x86
	)

	target_compile_definitions(${PROJECT_NAME} INTERFACE NOMINMAX)

	target_link_libraries(${PROJECT_NAME} INTERFACE 
		${GEODE_LOADER_PATH}/include/link/libcocos2d.lib
		${GEODE_LOADER_PATH}/include/link/libExtensions.lib
		${GEODE_LOADER_PATH}/include/link/libcurl.lib
		${GEODE_LOADER_PATH}/include/link/gdstring.lib
		${GEODE_LOADER_PATH}/include/link/glew32.lib
		${GEODE_LOADER_PATH}/include/link/fmod.lib
	)

	# Windows links against .lib and not .dll
	set(GEODE_PLATFORM_BINARY "Geode.lib")
elseif (GEODE_TARGET_PLATFORM STREQUAL "Android")
	message(FATAL_ERROR "IDK figure it out")
endif()
