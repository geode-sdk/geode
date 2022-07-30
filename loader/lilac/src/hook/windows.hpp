#ifndef GEODE_CORE_HOOK_WINDOWS_HPP
#define GEODE_CORE_HOOK_WINDOWS_HPP

#include "platform.hpp"

namespace lilac::hook {
    class WindowsX86 : public Platform<WindowsX86> {
    public:
#if defined(NDEBUG)
        // int3
        static constexpr char trap[] = { '\xCC' };
#else
        // ud2
        static constexpr char trap[] = { '\x0F', '\x0B' };
#endif

    public:
        static bool write_memory(void* to, const void* from, size_t size);
        static bool initialize();
    };

    using TargetPlatform = Platform<WindowsX86>;
}

#endif /* GEODE_CORE_HOOK_WINDOWS_HPP */
