#pragma once

#define LILAC_C_DLL
#define LILAC_C_API


// Set dllexport/dllimport to lilac classes & functions

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
	#define LILAC_WINDOWS(...) __VA_ARGS__
	#define LILAC_IS_WINDOWS
	#define LILAC_IS_DESKTOP
	#define LILAC_PLATFORM_NAME "Windows"
	#define LILAC_CALL __stdcall
	#define LILAC_PLATFORM_EXTENSION ".dll"
	
	#ifdef LILAC_EXPORTING
	    #define LILAC_C_DLL  __declspec(dllexport)
	#else
	    #define LILAC_C_DLL  __declspec(dllimport)
	#endif
	#define LILAC_C_API __declspec(dllexport) __stdcall
#else
	#define LILAC_WINDOWS(...)
#endif

#if defined(__APPLE__)
	#include <TargetConditionals.h>
	#if TARGET_OS_MAC
		#define LILAC_IOS(...)
		#define LILAC_MACOS(...) __VA_ARGS__
		#define LILAC_IS_MACOS
		#define LILAC_IS_DESKTOP
		#define LILAC_PLATFORM_NAME "MacOS"
		#define LILAC_PLATFORM_EXTENSION ".dylib"
	#elif TARGET_OS_IPHONE
		#define LILAC_MACOS(...)
		#define LILAC_IOS(...) __VA_ARGS__
		#define LILAC_IS_IOS
		#define LILAC_IS_MOBILE
		#define LILAC_PLATFORM_NAME "iOS"
		#define LILAC_PLATFORM_EXTENSION ".dylib"
	#endif
	#define LILAC_CALL
#else
	#define LILAC_MACOS(...)
	#define LILAC_IOS(...)
#endif

// Android
#if defined(__ANDROID__)
	#define LILAC_ANDROID(...) __VA_ARGS__
	#define LILAC_IS_ANDROID
	#define LILAC_IS_MOBILE
	#define LILAC_PLATFORM_NAME "Android"
	#define LILAC_CALL
	#define LILAC_PLATFORM_EXTENSION ".so"
#else
	#define LILAC_ANDROID(...)
#endif

#ifndef LILAC_PLATFORM_NAME
	#error "Unsupported PlatformID!"
#endif
