#pragma once

#include <dlfcn.h>

namespace geode::base {
	GEODE_NOINLINE inline uintptr_t get() {
		static uintptr_t base = reinterpret_cast<uintptr_t>(dlopen("libcocos2dcpp.so", RTLD_LAZY));
		return base;
	}
}
