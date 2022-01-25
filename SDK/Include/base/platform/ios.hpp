#pragma once

#include <mach-o/dyld.h>

namespace geode::base {
	GEODE_NOINLINE inline uintptr_t get() {
		static uintptr_t base = _dyld_get_image_vmaddr_slide(0)+0x100000000;
		return base;
	}
}
