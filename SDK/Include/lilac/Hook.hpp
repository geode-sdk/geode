#pragma once

#include "Macros.hpp"
#include <inttypes.h>
#include <helpers/types.hpp>

namespace lilac {
    class Mod;
    class Loader;

    class LILAC_DLL Hook {
    protected:
        Mod*  m_owner;
        void* m_address;
        void* m_detour;
        void* m_handle = nullptr;
        bool  m_enabled;

        // Only allow friend classes to create
        // hooks. Whatever method created the
        // hook should take care of populating
        // m_owner, m_address, m_detour and 
        // m_handle.
        Hook() : m_enabled(false) {}

        // no copying
        Hook(Hook const&) = delete;
        Hook operator=(Hook const&) = delete;

        friend class Mod;
        friend class Loader;

    public:
        /**
         * Get the address of the function hooked.
         * @returns Address
         * @author HJfod
         */
        uintptr_t getAddress() const { return reinterpret_cast<uintptr_t>(m_address); }

        /**
         * Get whether the hook is enabled or not.
         * @returns True if enabled, false if not.
         * @author HJfod
         */
        bool isEnabled() const { return m_enabled; }

        /**
         * Get the owner of this hook.
         * @returns Pointer to the owner's Mod handle.
         * @author HJfod
         */
        Mod* getOwner() const { return m_owner; }
    };

    class LILAC_DLL Patch {
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
