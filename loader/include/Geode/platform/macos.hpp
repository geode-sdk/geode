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
        // on arm macos, launching through steam seems to set image 0 to some steam helper library
        // i don't know why... :(

        static uintptr_t base = []() {
            // dyld man page says this is unsafe but idc
            auto image_count = _dyld_image_count();

            for (auto i = 0u; i < image_count; i++) {
                std::string_view image_name = _dyld_get_image_name(i);
                if (!image_name.ends_with(".dylib")) {
                    return _dyld_get_image_vmaddr_slide(i) + 0x100000000;
                }
            }

            // we couldn't find the base, so just assume it's 0
            // should probably have an error for this, but geode::log isn't available yet
            return _dyld_get_image_vmaddr_slide(0) + 0x100000000;
        }();
        return base;
    }
}
