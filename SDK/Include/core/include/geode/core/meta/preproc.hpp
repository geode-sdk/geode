#ifndef GEODE_CORE_META_PREPROC_HPP
#define GEODE_CORE_META_PREPROC_HPP

#include "../macros/platform.hpp"

namespace geode::core::meta {
    #if defined(NDEBUG)
        static constexpr bool debug = false;
    #else
        static constexpr bool debug = true;
    #endif
}

#endif /* GEODE_CORE_META_PREPROC_HPP */
