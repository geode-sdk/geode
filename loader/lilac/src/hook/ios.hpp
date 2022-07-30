#ifndef GEODE_CORE_HOOK_IOS_HPP
#define GEODE_CORE_HOOK_IOS_HPP

#include "platform.hpp"

namespace lilac::hook {
    class iOS : public Platform<iOS> {
    public:
        static constexpr char trap[] = { '\x00', '\x00', '\x20', '\xD4' };

    public:
        static bool write_memory(void* to, const void* from, size_t size);
        static bool initialize();
        static const void* align_address(const void* address);
    };

    using TargetPlatform = Platform<iOS>;
}

#endif /* GEODE_CORE_HOOK_IOS_HPP */
