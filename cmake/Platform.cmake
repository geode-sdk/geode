include(cmake/PlatformDetect.cmake)

if (NOT ${PROJECT_NAME} STREQUAL ${CMAKE_PROJECT_NAME})
	set(GEODE_TARGET_PLATFORM ${GEODE_TARGET_PLATFORM} PARENT_SCOPE)
endif()

if (GEODE_TARGET_PLATFORM STREQUAL "iOS")
	# make sure that we get the ios sdk
	execute_process(COMMAND xcrun --show-sdk-path --sdk iphoneos
	OUTPUT_VARIABLE GEODE_IOS_SDK
		OUTPUT_STRIP_TRAILING_WHITESPACE
	)

	message(STATUS "iOS c++ compiler: ${CMAKE_CXX_COMPILER}")
	set(CMAKE_OSX_ARCHITECTURES arm64)
	set(CMAKE_OSX_SYSROOT ${GEODE_IOS_SDK})
	set(CMAKE_OSX_DEPLOYMENT_TARGET "14.0")
	set(CMAKE_SYSTEM_NAME "iOS")

	# this fails on ios builds
	set(BUILD_MD2HTML_EXECUTABLE "OFF")

	set_target_properties(${PROJECT_NAME} PROPERTIES
		SYSTEM_NAME iOS
		OSX_SYSROOT ${GEODE_IOS_SDK}
		OSX_ARCHITECTURES arm64
	)

	target_link_libraries(${PROJECT_NAME} INTERFACE
		"-framework OpenGLES"     # needed for CCClippingNode reimpl and ScrollLayer
		"-framework UIKit"        # needed for file picking (UIApplication)
		"-framework Foundation"   # needed for many things
		"-framework AVFoundation" # needed for microphone access
		${GEODE_LOADER_PATH}/include/link/ios/libssl.a
		${GEODE_LOADER_PATH}/include/link/ios/libcrypto.a
		${GEODE_LOADER_PATH}/include/link/ios/libnghttp2.a
		${GEODE_LOADER_PATH}/include/link/ios/libcurl.a
	)

	target_compile_definitions(${PROJECT_NAME} INTERFACE
		-DGLES_SILENCE_DEPRECATION
	)

	set(GEODE_OUTPUT_NAME "Geode.ios")
	set(GEODE_PLATFORM_BINARY "Geode.ios.dylib")
	set(GEODE_MOD_BINARY_SUFFIX ".ios.dylib" CACHE STRING "" FORCE)

	if (NOT ${PROJECT_NAME} STREQUAL ${CMAKE_PROJECT_NAME})
		set(GEODE_TARGET_PLATFORM_SHORT "ios" PARENT_SCOPE)
		# this is needed because else loading mods will fail below ios 14.5
		set(CMAKE_OSX_DEPLOYMENT_TARGET "14.0" PARENT_SCOPE)
	else()
		set(GEODE_TARGET_PLATFORM_SHORT "ios")
	endif()
elseif (GEODE_TARGET_PLATFORM STREQUAL "MacOS")
	set_target_properties(${PROJECT_NAME} PROPERTIES 
		SYSTEM_NAME MacOS
	)

	# for some reason, this value is initialized as an empty string by default. idk why
	if (NOT DEFINED CMAKE_OSX_ARCHITECTURES OR CMAKE_OSX_ARCHITECTURES STREQUAL "")
		set(CMAKE_OSX_ARCHITECTURES "arm64;x86_64")
	endif()

	# only exists as a global property
	set(CMAKE_OSX_DEPLOYMENT_TARGET 10.15)

	target_link_libraries(${PROJECT_NAME} INTERFACE
		"-framework Cocoa"
		"-framework OpenGL"
		"-framework SystemConfiguration"
		${GEODE_LOADER_PATH}/include/link/macos/libfmod.dylib
		${GEODE_LOADER_PATH}/include/link/macos/libssl.a
		${GEODE_LOADER_PATH}/include/link/macos/libcrypto.a
		${GEODE_LOADER_PATH}/include/link/macos/libnghttp2.a
		${GEODE_LOADER_PATH}/include/link/macos/libngtcp2.a
		${GEODE_LOADER_PATH}/include/link/macos/libnghttp3.a
		${GEODE_LOADER_PATH}/include/link/macos/libngtcp2_crypto_boringssl.a
		${GEODE_LOADER_PATH}/include/link/macos/libcurl.a
	)

	target_compile_definitions(${PROJECT_NAME} INTERFACE
		-DCommentType=CommentTypeDummy
		-DGL_SILENCE_DEPRECATION
	)

	set(GEODE_OUTPUT_NAME "Geode")
	set(GEODE_PLATFORM_BINARY "Geode.dylib")
	set(GEODE_MOD_BINARY_SUFFIX ".dylib" CACHE STRING "" FORCE)

	if (NOT ${PROJECT_NAME} STREQUAL ${CMAKE_PROJECT_NAME})
		set(GEODE_TARGET_PLATFORM_SHORT "mac" PARENT_SCOPE)
	else()
		set(GEODE_TARGET_PLATFORM_SHORT "mac")
	endif()
elseif (GEODE_TARGET_PLATFORM STREQUAL "Win64")
	set_target_properties(${PROJECT_NAME} PROPERTIES
		SYSTEM_NAME Win64
		GENERATOR_PLATFORM x64
	)

	target_compile_definitions(${PROJECT_NAME} INTERFACE NOMINMAX)

	target_link_libraries(${PROJECT_NAME} INTERFACE 
		${GEODE_LOADER_PATH}/include/link/win64/libcocos2d.lib
		${GEODE_LOADER_PATH}/include/link/win64/libExtensions.lib
		${GEODE_LOADER_PATH}/include/link/win64/glew32.lib
		${GEODE_LOADER_PATH}/include/link/win64/fmod.lib
		opengl32
	)

	if (PROJECT_IS_TOP_LEVEL AND CMAKE_BUILD_TYPE STREQUAL "Debug")
		target_link_libraries(${PROJECT_NAME} INTERFACE
			${GEODE_LOADER_PATH}/include/link/win64/gd-libcurl.lib
		)
	else()
		target_link_libraries(${PROJECT_NAME} INTERFACE
			${GEODE_LOADER_PATH}/include/link/win64/nghttp2.lib
			${GEODE_LOADER_PATH}/include/link/win64/libcurl.lib
		)
	endif()

	# Windows links against .lib and not .dll
	set(GEODE_OUTPUT_NAME "Geode")
	set(GEODE_PLATFORM_BINARY "Geode.lib")
	set(GEODE_MOD_BINARY_SUFFIX ".dll" CACHE STRING "" FORCE)

	if (NOT ${PROJECT_NAME} STREQUAL ${CMAKE_PROJECT_NAME})
		set(GEODE_TARGET_PLATFORM_SHORT "win" PARENT_SCOPE)
	else()
		set(GEODE_TARGET_PLATFORM_SHORT "win")
	endif()
elseif (GEODE_TARGET_PLATFORM STREQUAL "Android32")
	set_target_properties(${PROJECT_NAME} PROPERTIES
		SYSTEM_NAME Android
	)

	target_link_libraries(${PROJECT_NAME} INTERFACE
		c
		unwind
		${GEODE_LOADER_PATH}/include/link/android32/libssl.a
		${GEODE_LOADER_PATH}/include/link/android32/libcrypto.a
		${GEODE_LOADER_PATH}/include/link/android32/libnghttp2.a
		${GEODE_LOADER_PATH}/include/link/android32/libngtcp2.a
		${GEODE_LOADER_PATH}/include/link/android32/libnghttp3.a
		${GEODE_LOADER_PATH}/include/link/android32/libngtcp2_crypto_boringssl.a
		${GEODE_LOADER_PATH}/include/link/android32/libcurl.a
		${GEODE_LOADER_PATH}/include/link/android32/libcocos2dcpp.so
		${GEODE_LOADER_PATH}/include/link/android32/libfmod.so
		GLESv2
		log
	)

	set(GEODE_OUTPUT_NAME "Geode.android32")
	set(GEODE_PLATFORM_BINARY "Geode.android32.so")
	set(GEODE_MOD_BINARY_SUFFIX ".android32.so" CACHE STRING "" FORCE)

	if (NOT ${PROJECT_NAME} STREQUAL ${CMAKE_PROJECT_NAME})
		set(GEODE_TARGET_PLATFORM_SHORT "android32" PARENT_SCOPE)
	else()
		set(GEODE_TARGET_PLATFORM_SHORT "android32")
	endif()
elseif (GEODE_TARGET_PLATFORM STREQUAL "Android64")
	set_target_properties(${PROJECT_NAME} PROPERTIES
		SYSTEM_NAME Android
	)

	target_link_libraries(${PROJECT_NAME} INTERFACE
		c
		unwind
		${GEODE_LOADER_PATH}/include/link/android64/libssl.a
		${GEODE_LOADER_PATH}/include/link/android64/libcrypto.a
		${GEODE_LOADER_PATH}/include/link/android64/libnghttp2.a
		${GEODE_LOADER_PATH}/include/link/android64/libngtcp2.a
		${GEODE_LOADER_PATH}/include/link/android64/libnghttp3.a
		${GEODE_LOADER_PATH}/include/link/android64/libngtcp2_crypto_boringssl.a
		${GEODE_LOADER_PATH}/include/link/android64/libcurl.a
		${GEODE_LOADER_PATH}/include/link/android64/libcocos2dcpp.so
		${GEODE_LOADER_PATH}/include/link/android64/libfmod.so
		GLESv2
		log
	)

	# this should help with fixing exceptions
	set(ANDROID_STL c++_shared)
  # a little desperate
	add_definitions(-DANDROID_STL=c++_shared)

	set(GEODE_OUTPUT_NAME "Geode.android64")
	set(GEODE_PLATFORM_BINARY "Geode.android64.so")
	set(GEODE_MOD_BINARY_SUFFIX ".android64.so" CACHE STRING "" FORCE)

	if (NOT ${PROJECT_NAME} STREQUAL ${CMAKE_PROJECT_NAME})
		set(GEODE_TARGET_PLATFORM_SHORT "android64" PARENT_SCOPE)
	else()
		set(GEODE_TARGET_PLATFORM_SHORT "android64")
	endif()
else()
	message(FATAL_ERROR "Unknown platform ${GEODE_TARGET_PLATFORM}")
endif()
