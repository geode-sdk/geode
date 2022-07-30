#pragma once

#define GEODE_C_DLL
#define GEODE_C_API

#ifdef _MSC_VER
#pragma warning(disable: 4099) // type first seen as class
#pragma warning(default: 4067)
#pragma warning(disable: 4251) // dll-interface
#pragma warning(disable: 4244) // narrowing conversion
#endif


// Set dllexport/dllimport to geode classes & functions

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
	#define GEODE_WINDOWS(...) __VA_ARGS__
	#define GEODE_IS_WINDOWS
	#define GEODE_IS_DESKTOP
	#define GEODE_PLATFORM_NAME "Windows"
	#define GEODE_CALL __stdcall
	#define GEODE_PLATFORM_EXTENSION ".dll"
	
	#ifdef GEODE_EXPORTING
		#undef GEODE_C_DLL
	    #define GEODE_C_DLL  __declspec(dllexport)
	#else
		#undef GEODE_C_DLL
	    #define GEODE_C_DLL  __declspec(dllimport)
	#endif
	#undef GEODE_C_API
	#define GEODE_C_API __declspec(dllexport) __stdcall
#else
	#define GEODE_WINDOWS(...)
#endif

#if defined(__APPLE__)
	#include <TargetConditionals.h>
	#if TARGET_OS_IPHONE
		#define GEODE_MACOS(...)
		#define GEODE_IOS(...) __VA_ARGS__
		#define GEODE_IS_IOS
		#define GEODE_IS_MOBILE
		#define GEODE_PLATFORM_NAME "iOS"
		#define GEODE_PLATFORM_EXTENSION ".dylib"
	#else
		#define GEODE_IOS(...)
		#define GEODE_MACOS(...) __VA_ARGS__
		#define GEODE_IS_MACOS
		#define GEODE_IS_DESKTOP
		#define GEODE_PLATFORM_NAME "MacOS"
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
