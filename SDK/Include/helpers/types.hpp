#pragma once

#include <vector>

namespace geode {
    using byte_array = std::vector<uint8_t>;

    #ifndef GEODE_NO_CRINGE

    using unknown_t = uintptr_t;
    using edx_t = uintptr_t;

    #endif
}
