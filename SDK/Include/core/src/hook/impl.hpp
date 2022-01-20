#ifndef GEODE_CORE_HOOK_IMPL_HPP
#define GEODE_CORE_HOOK_IMPL_HPP

#include <hook.hpp>

#include <vector>
#include <map>

#if defined(GEODE_IS_WINDOWS)
    #include "windows.hpp"
#elif defined(GEODE_IS_MACOS)
    #include "macos.hpp"
#endif

namespace geode::core::hook {
    class HookManager {
    private:
        struct HookChain;

        struct CallFrame {
            const void* address = nullptr;
            HookChain* parent = nullptr;
            char original_bytes[TargetPlatform::get_trap_size()] = { 0 };
        };

        struct HookChain {
            const void* address = nullptr;
            std::vector<const void*> detours = {};
            std::vector<CallFrame*> frames = {};
            char original_bytes[TargetPlatform::get_trap_size()] = { 0 };
        };

        struct Handle {
            const void* address;
            const void* detour;
        };

    private:
        static inline std::map<const void*, HookChain> all_hooks = {};
        static inline std::map<const void*, CallFrame> all_frames = {};

    private:
        /* these don't check char buffer bounds. it should have sizeof(trap) size.
        * pass nullptr to add_trap if you don't want to save the old bytes into a buffer.
        */

        static void add_trap(const void* address, char buffer[]);
        static void remove_trap(const void* address, char buffer[]);

        static bool find_in_hooks(Exception& info);
        static bool find_in_frames(Exception& info);

    public:
        // returns true if handled, false if not.
        static bool handler(Exception& info);

        static hook::Handle GEODE_CALL add_hook(const void* address, const void* detour);
        static bool GEODE_CALL remove_hook(hook::Handle handle);
    };
}

#endif /* GEODE_CORE_HOOK_IMPL_HPP */
