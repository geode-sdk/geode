#ifndef LILAC_CORE_META_PREPROC_HPP
#define LILAC_CORE_META_PREPROC_HPP

#include <platform/platform.hpp>

namespace lilac::core::meta {
    #if defined(NDEBUG)
        static constexpr bool debug = false;
    #else
        static constexpr bool debug = true;
    #endif
}

#endif /* LILAC_CORE_META_PREPROC_HPP */
