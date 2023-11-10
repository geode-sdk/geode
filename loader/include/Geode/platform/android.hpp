#pragma once

#include <dlfcn.h>
#include "ItaniumCast.hpp"

namespace geode {
    struct PlatformInfo {
        void* m_so;
    };
}

namespace geode::base {
    /*GEODE_NOINLINE inline*/ uintptr_t get();
}

