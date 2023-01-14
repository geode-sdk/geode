#pragma once
#include "Types.hpp"

#include <Geode/utils/addresser.hpp>

namespace geode::modifier {
    template <uint32_t Id>
    uintptr_t address();
}
