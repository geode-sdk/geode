#pragma once

#include <Geode/DefaultInclude.hpp>
#include <inttypes.h>
#include <Geode/utils/types.hpp>
#include <string_view>

#include "../hook-core/Hook.hpp"

namespace geode {
    class Mod;
    class Loader;

    class GEODE_DLL Hook {
    private:
        Mod*  m_owner;
        std::string m_displayName;
        void* m_address;
        void* m_detour;
        core::HookHandle m_handle;
        bool  m_enabled;
        Result<core::HookHandle>(*m_addFunction)(void*);

        // Only allow friend classes to create
        // hooks. Whatever method created the
        // hook should take care of populating
        // m_owner and m_handle.
        Hook() : m_enabled(false) {}

        template <auto Detour, template <class, class...> class Conv, class Ret, class ...Args>
        static Hook* create(Ret(*address)(Args...), std::string const& displayName, Mod* owner) {
        	auto ret = new Hook;
        	ret->m_address = (void*)address;
        	ret->m_detour = (void*)Detour;
        	ret->m_owner = owner;
        	ret->m_displayName = displayName;
        	ret->m_addFunction = (Result<core::HookHandle>(*)(void*))&core::hook::add<Detour, Conv, Ret, Args...>;
        	return ret;
        }

        // no copying
        Hook(Hook const&) = delete;
        Hook operator=(Hook const&) = delete;

        friend class Mod;
        friend class Loader;

    public:
        /**
         * Get the address of the function hooked.
         * @returns Address
         */
        uintptr_t getAddress() const { return reinterpret_cast<uintptr_t>(m_address); }

        /**
         * Get the display name of the function hooked.
         * @returns Display name
         */
        std::string_view getDisplayName() const { return m_displayName; }

        /**
         * Get whether the hook is enabled or not.
         * @returns True if enabled, false if not.
         */
        bool isEnabled() const { return m_enabled; }

        /**
         * Get the owner of this hook.
         * @returns Pointer to the owner's Mod handle.
         */
        Mod* getOwner() const { return m_owner; }
    };

    class GEODE_DLL Patch {
    protected:
        Mod*  m_owner;
        void* m_address;
        byte_array m_original;
        byte_array m_patch;
        bool  m_applied;
    
        // Only allow friend classes to create
        // patches. Whatever method created the
        // patches should take care of populating
        // m_owner, m_address, m_original and 
        // m_patch.
        Patch() : m_applied(false) {}

        // no copying
        Patch(Patch const&) = delete;
        Patch operator=(Patch const&) = delete;

        friend class Mod;
        friend class Loader;

    public:
        /**
         * Get the address of the patch.
         * @returns Address
         */
        uintptr_t getAddress() const { return reinterpret_cast<uintptr_t>(m_address); }

        /**
         * Get whether the patch is applied or not.
         * @returns True if applied, false if not.
         */
        bool isApplied() const { return m_applied; }

        bool apply();
        bool restore();

        /**
         * Get the owner of this patch.
         * @returns Pointer to the owner's Mod handle.
         */
        Mod* getOwner() const { return m_owner; }
    };
}
