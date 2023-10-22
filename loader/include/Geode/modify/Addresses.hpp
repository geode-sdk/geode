#pragma once

#include <Geode/utils/Result.hpp>
#include <tulip/TulipHook.hpp>

namespace geode::modifier {
    template <uint32_t Id>
    GEODE_HIDDEN uintptr_t address();

    Result<tulip::hook::HandlerMetadata> handlerMetadataForAddress(uintptr_t address);
}
