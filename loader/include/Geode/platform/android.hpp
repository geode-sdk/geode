#pragma once

#include <dlfcn.h>

namespace geode {
    struct PlatformInfo {
        void* m_so;
    };
}

namespace geode::base {
    /*GEODE_NOINLINE inline*/ uintptr_t get();
}

namespace geode::cast {
    template <class After, class Before>
    After typeinfo_cast(Before ptr) {
        // yall have symbols smh
        return dynamic_cast<After>(ptr);
    }
}
