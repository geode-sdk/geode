#ifndef GEODE_CORE_HOOK_MACOS_HPP
#define GEODE_CORE_HOOK_MACOS_HPP

#include "platform.hpp"

namespace lilac::hook {
    class MacOSX : public Platform<MacOSX> {
    public:
        static constexpr char trap[] = { '\x0F', '\x0B' };

    public:
        static bool write_memory(void* to, const void* from, size_t size);
        static bool initialize();
    };

    using TargetPlatform = Platform<MacOSX>;
}

#endif /* GEODE_CORE_HOOK_MACOS_HPP */
