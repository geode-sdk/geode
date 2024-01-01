if (NOT DEFINED GEODE_TARGET_PLATFORM)
	if(APPLE)
		if(IOS)
			set(GEODE_TARGET_PLATFORM "iOS")
		else()
			set(GEODE_TARGET_PLATFORM "MacOS")
		endif()
	elseif(WIN32)
		set(GEODE_TARGET_PLATFORM "Win32")
	elseif(ANDROID)
		if (ANDROID_ABI STREQUAL "arm64-v8a")
			set(GEODE_TARGET_PLATFORM "Android64")
		elseif(ANDROID_ABI STREQUAL "armeabi-v7a")
			set(GEODE_TARGET_PLATFORM "Android32")
		endif()
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

	set(GEODE_OUTPUT_NAME "GeodeIOS")
	set(GEODE_PLATFORM_BINARY "GeodeIOS.dylib")
	set(GEODE_MOD_BINARY_SUFFIX ".dylib" CACHE STRING "" FORCE)
elseif (GEODE_TARGET_PLATFORM STREQUAL "MacOS")
	set_target_properties(${PROJECT_NAME} PROPERTIES 
		SYSTEM_NAME MacOS
		APPLE_SILICON_PROCESSOR x86_64
	)

	# this should be set globally
	set(CMAKE_OSX_ARCHITECTURES "x86_64")

	# only exists as a global property
	set(CMAKE_OSX_DEPLOYMENT_TARGET 10.13)

	find_package(CURL REQUIRED) 

	target_include_directories(${PROJECT_NAME} INTERFACE
		${CURL_INCLUDE_DIR}
	)

	target_link_libraries(${PROJECT_NAME} INTERFACE
		"-framework Cocoa"
		"-framework OpenGL"
		${CURL_LIBRARIES}
		${GEODE_LOADER_PATH}/include/link/libfmod.dylib
	)

	target_compile_definitions(${PROJECT_NAME} INTERFACE
		-DCommentType=CommentTypeDummy
	)

	set(GEODE_OUTPUT_NAME "Geode")
	set(GEODE_PLATFORM_BINARY "Geode.dylib")
	set(GEODE_MOD_BINARY_SUFFIX ".ios.dylib" CACHE STRING "" FORCE)
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
		${GEODE_LOADER_PATH}/include/link/glew32.lib
		${GEODE_LOADER_PATH}/include/link/gdstring.lib
		${GEODE_LOADER_PATH}/include/link/fmod.lib
		opengl32
	)

	# Windows links against .lib and not .dll
	set(GEODE_OUTPUT_NAME "Geode")
	set(GEODE_PLATFORM_BINARY "Geode.lib")
	set(GEODE_MOD_BINARY_SUFFIX ".dll" CACHE STRING "" FORCE)
elseif (GEODE_TARGET_PLATFORM STREQUAL "Android32")
	set_target_properties(${PROJECT_NAME} PROPERTIES
		SYSTEM_NAME Android
	)

	target_link_libraries(${PROJECT_NAME} INTERFACE
		${GEODE_LOADER_PATH}/include/link/android32/libcurl.a
		${GEODE_LOADER_PATH}/include/link/android32/libssl.a
		${GEODE_LOADER_PATH}/include/link/android32/libcrypto.a
		${GEODE_LOADER_PATH}/include/link/android32/libcocos2dcpp.so
		GLESv2
		log
	)

	set(GEODE_OUTPUT_NAME "Geode.armeabi-v7a")
	set(GEODE_PLATFORM_BINARY "Geode.armeabi-v7a.so")
	set(GEODE_MOD_BINARY_SUFFIX ".v7.so" CACHE STRING "" FORCE)
elseif (GEODE_TARGET_PLATFORM STREQUAL "Android64")
	set_target_properties(${PROJECT_NAME} PROPERTIES
		SYSTEM_NAME Android
	)

	target_link_libraries(${PROJECT_NAME} INTERFACE
		${GEODE_LOADER_PATH}/include/link/android64/libcurl.a
		${GEODE_LOADER_PATH}/include/link/android64/libssl.a
		${GEODE_LOADER_PATH}/include/link/android64/libcrypto.a
		${GEODE_LOADER_PATH}/include/link/android64/libcocos2dcpp.so
		GLESv2
		log
	)

	set(GEODE_OUTPUT_NAME "Geode.arm64-v8a")
	set(GEODE_PLATFORM_BINARY "Geode.arm64-v8a.so")
	set(GEODE_MOD_BINARY_SUFFIX ".v8.so" CACHE STRING "" FORCE)
else()
	message(FATAL_ERROR "Unknown platform ${GEODE_TARGET_PLATFORM}")
endif()
