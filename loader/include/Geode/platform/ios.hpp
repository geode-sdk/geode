#pragma once

#include <cstring>
#include <mach-o/dyld.h>
#include <type_traits>
#include <typeinfo>
#include "ItaniumCast.hpp"

namespace geode {
    using dylib_t = void*;

    struct PlatformInfo {
        dylib_t m_dylib;
    };
}

namespace geode::base {
    GEODE_NOINLINE inline uintptr_t get() {
        for(uint32_t gdii = 0; gdii < _dyld_image_count(); gdii++) {
            if (strcmp(_dyld_get_image_name(gdii), "GeometryJump")) {
                static uintptr_t base = _dyld_get_image_vmaddr_slide(gdii) + 0x100000000;
                return base;
            }
        }

        return 0;
    }
}

extern "C" inline uintptr_t _geode_ios_base() {
    return geode::base::get();
}
