#pragma once

#include <vector>

namespace lilac {
    using byte_array = std::vector<uint8_t>;

    #ifndef LILAC_NO_CRINGE

    using unknown_t = uintptr_t;
    using edx_t = uintptr_t;

    #endif
}
