#pragma once

#define GEODE_C_DLL
#define GEODE_C_API


// Set dllexport/dllimport to geode classes & functions

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
	#define GEODE_WINDOWS(...) __VA_ARGS__
	#define GEODE_IS_WINDOWS
	#define GEODE_IS_DESKTOP
	#define GEODE_PLATFORM_NAME "Windows"
	#define GEODE_CALL __stdcall
	#define GEODE_PLATFORM_EXTENSION ".dll"
	
	#ifdef GEODE_EXPORTING
	    #define GEODE_C_DLL  __declspec(dllexport)
	#else
	    #define GEODE_C_DLL  __declspec(dllimport)
	#endif
	#define GEODE_C_API __declspec(dllexport) __stdcall
#else
	#define GEODE_WINDOWS(...)
#endif

#if defined(__APPLE__)
	#include <TargetConditionals.h>
	#if TARGET_OS_MAC
		#define GEODE_IOS(...)
		#define GEODE_MACOS(...) __VA_ARGS__
		#define GEODE_IS_MACOS
		#define GEODE_IS_DESKTOP
		#define GEODE_PLATFORM_NAME "MacOS"
		#define GEODE_PLATFORM_EXTENSION ".dylib"
	#elif TARGET_OS_IPHONE
		#define GEODE_MACOS(...)
		#define GEODE_IOS(...) __VA_ARGS__
		#define GEODE_IS_IOS
		#define GEODE_IS_MOBILE
		#define GEODE_PLATFORM_NAME "iOS"
		#define GEODE_PLATFORM_EXTENSION ".dylib"
	#endif
	#define GEODE_CALL
#else
	#define GEODE_MACOS(...)
	#define GEODE_IOS(...)
#endif

// Android
#if defined(__ANDROID__)
	#define GEODE_ANDROID(...) __VA_ARGS__
	#define GEODE_IS_ANDROID
	#define GEODE_IS_MOBILE
	#define GEODE_PLATFORM_NAME "Android"
	#define GEODE_CALL
	#define GEODE_PLATFORM_EXTENSION ".so"
#else
	#define GEODE_ANDROID(...)
#endif

#ifndef GEODE_PLATFORM_NAME
	#error "Unsupported PlatformID!"
#endif
