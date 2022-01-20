#ifndef GEODE_CORE_HOOK_WINDOWS_HPP
#define GEODE_CORE_HOOK_WINDOWS_HPP

#include "platform.hpp"

namespace geode::core::hook {
    class WindowsX86 : public Platform<WindowsX86> {
    public:
    #if defined(NDEBUG)
        static constexpr char trap[] = { '\xCC' };
    #else
        static constexpr char trap[] = { '\x0F', '\x0B' };
    #endif

    public:
        static bool read_memory(void* addr, void* to, size_t size);
        static bool write_memory(void* to, const void* from, size_t size);
        static bool initialize();
    };

    using TargetPlatform = Platform<WindowsX86>;
}

#endif /* GEODE_CORE_HOOK_WINDOWS_HPP */