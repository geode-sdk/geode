#pragma once

#ifdef _MSC_VER
    #pragma warning(disable : 4099) // type first seen as class
    #pragma warning(default : 4067)
    #pragma warning(disable : 4251) // dll-interface
    #pragma warning(disable : 4244) // narrowing conversion
#endif

// Set dllexport/dllimport to geode classes & functions

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__) || defined(WIN64) || defined(_WIN64) || defined(__WIN64) && !defined(__CYGWIN__)
    #define GEODE_WINDOWS(...) __VA_ARGS__
    #define GEODE_IS_WINDOWS
    #define GEODE_IS_DESKTOP
    #define GEODE_PLATFORM_NAME "Windows"
    #define GEODE_PLATFORM_EXTENSION ".dll"
    #define GEODE_PLATFORM_SHORT_IDENTIFIER "win"
    #define CC_TARGET_OS_WIN32

    #if defined(WIN64) || defined(_WIN64) || defined(__WIN64) && !defined(__CYGWIN__)
        #define GEODE_IS_WINDOWS64
        #define GEODE_WINDOWS64(...) __VA_ARGS__
        #define GEODE_WINDOWS32(...)
        #define GEODE_CALL
    #else
        #define GEODE_IS_WINDOWS32
        #define GEODE_WINDOWS32(...) __VA_ARGS__
        #define GEODE_WINDOWS64(...)
        #define GEODE_CALL __stdcall
    #endif
#else
    #define GEODE_WINDOWS(...)
    #define GEODE_WINDOWS32(...)
    #define GEODE_WINDOWS64(...)
#endif

#if defined(__APPLE__)
    #include <TargetConditionals.h>
    #if TARGET_OS_IPHONE
        #define GEODE_MACOS(...)
        #define GEODE_INTEL_MAC(...)
        #define GEODE_ARM_MAC(...)
        #define GEODE_IOS(...) __VA_ARGS__
        #define GEODE_IS_IOS
        #define GEODE_IS_MOBILE
        #define GEODE_PLATFORM_NAME "iOS"
        #define GEODE_PLATFORM_EXTENSION ".ios.dylib"
        #define GEODE_PLATFORM_SHORT_IDENTIFIER "ios"
        #define CC_TARGET_OS_IPHONE
    #else
        #define GEODE_IOS(...)
        #define GEODE_MACOS(...) __VA_ARGS__
        #define GEODE_IS_MACOS
        #define GEODE_IS_DESKTOP
        #define GEODE_PLATFORM_EXTENSION ".dylib"
        #define CC_TARGET_OS_MAC

        #if TARGET_CPU_ARM64
            #define GEODE_PLATFORM_NAME "MacArm"
            #define GEODE_IS_ARM_MAC
            #define GEODE_PLATFORM_SHORT_IDENTIFIER "mac-arm"
            #define GEODE_ARM_MAC(...) __VA_ARGS__
            #define GEODE_INTEL_MAC(...)
        #else
            #define GEODE_PLATFORM_NAME "MacIntel"
            #define GEODE_IS_INTEL_MAC
            #define GEODE_PLATFORM_SHORT_IDENTIFIER "mac-intel"
            #define GEODE_ARM_MAC(...)
            #define GEODE_INTEL_MAC(...) __VA_ARGS__
        #endif
    #endif
    #define GEODE_CALL
#else
    #define GEODE_MACOS(...)
    #define GEODE_IOS(...)
    #define GEODE_INTEL_MAC(...)
    #define GEODE_ARM_MAC(...)
#endif

// Android
#if defined(__ANDROID__)
    #define GEODE_ANDROID(...) __VA_ARGS__
    #define GEODE_IS_ANDROID
    #define GEODE_IS_MOBILE
    #define GEODE_CALL
    #define CC_TARGET_OS_ANDROID

    #if defined(__arm__)
        #define GEODE_ANDROID32(...) __VA_ARGS__
		#define GEODE_ANDROID64(...) 
        #define GEODE_IS_ANDROID32
		#define GEODE_PLATFORM_NAME "Android32"
        #define GEODE_PLATFORM_EXTENSION ".android32.so"
        #define GEODE_PLATFORM_SHORT_IDENTIFIER "android32"
	#elif defined(__aarch64__)
        #define GEODE_ANDROID32(...)
		#define GEODE_ANDROID64(...) __VA_ARGS__
        #define GEODE_IS_ANDROID64
        #define GEODE_PLATFORM_NAME "Android64"
        #define GEODE_PLATFORM_EXTENSION ".android64.so"
        #define GEODE_PLATFORM_SHORT_IDENTIFIER "android64"
    #else
        #define GEODE_ANDROID32(...)
        #define GEODE_ANDROID64(...)
	#endif
#else
    #define GEODE_ANDROID(...)
    #define GEODE_ANDROID32(...)
    #define GEODE_ANDROID64(...)
#endif

#ifndef GEODE_PLATFORM_NAME
    #error "Unsupported PlatformID!"
#endif
