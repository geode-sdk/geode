#pragma once 

#if defined(_MSC_VER)
	/**
	 * MSVC exports with hidden by default so there is no need for hidden
	 */
	#define hidden
	#define dllexport __declspec(dllexport)
	#define dupable __forceinline

#else
	/**
	 * We need a hidden attribute for the hook classes because
	 * since they have the same name in different executables their
	 * global offset tables can override
	 */
	#define hidden __attribute__((visibility("hidden")))
	#define dllexport 
	#define dupable __attribute__((always_inline))
#endif


#if defined(CC_TARGET_OS_ANDROID)

	#include <dlfcn.h>
	#include <android/log.h>

	#define GeodeLog(format, ...) __android_log_print(ANDROID_LOG_DEBUG, "Geode", "%s:%d:\n" format"\n", __FILE__, __LINE__, ##__VA_ARGS__);

#elif defined(CC_TARGET_OS_WIN32)

	#include <meta/function.hpp>
	#include <meta/optcall.hpp>
	#include <meta/membercall.hpp>
	#include <meta/thiscall.hpp>
	#include <stdio.h>

	#define GeodeLog(format, ...) printf("Geode %s:%d:\n" format"\n", __FILE__, __LINE__, ##__VA_ARGS__)

#elif defined(CC_TARGET_OS_MAC)

	#include <stdio.h>

	#define GeodeLog(format, ...) printf("Geode %s:%d:\n" format"\n", __FILE__, __LINE__, ##__VA_ARGS__)

#elif defined(CC_TARGET_OS_IPHONE)
	
	#define GeodeLog(format, ...)

#endif
