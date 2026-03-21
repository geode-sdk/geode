#pragma once

#include <dlfcn.h>
#include <type_traits>
#include "ItaniumCast.hpp"
#include "clang.hpp"

namespace geode {
    struct PlatformInfo {
        void* m_so;
    };

    struct PlatformDetails {
        std::string releaseVersion;
        std::string arch;
        std::vector<std::string> availableArchs;
        uint32_t sdkVersion;
    };
}

namespace geode::base {
    /*GEODE_NOINLINE inline*/ uintptr_t get();
}

