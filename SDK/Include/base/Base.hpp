// 
// Copyright camila314 & alk1m123 2022. 
//
#pragma once 

#include <cstddef>
#include <stdint.h>
#include <gdstdlib.hpp>

#if defined(CC_TARGET_OS_MAC)
	#include <mach-o/dyld.h>
#elif defined(CC_TARGET_OS_WIN32)
	#include <windows.h>
#elif defined(CC_TARGET_OS_IPHONE)
	#include <mach-o/dyld.h>
#elif defined(CC_TARGET_OS_ANDROID)
	#include <dlfcn.h>
#endif

static uintptr_t base = 0;
inline uintptr_t getBase() {
	if (!base) {
	#if defined(CC_TARGET_OS_MAC)
		base = _dyld_get_image_vmaddr_slide(0)+0x100000000;
	#elif defined(CC_TARGET_OS_WIN32)
		base = reinterpret_cast<uintptr_t>(GetModuleHandle(0));
	#elif defined(CC_TARGET_OS_IPHONE)
		base = _dyld_get_image_vmaddr_slide(0)+0x100000000;
	#elif defined(CC_TARGET_OS_ANDROID)
		base = reinterpret_cast<uintptr_t>(dlopen("libcocos2dcpp.so", RTLD_LAZY));
	#else
		static_assert(false, "Invalid platform");
	#endif
	}
	return base;
}

#define CACAO_VERSION "3.4.0"

