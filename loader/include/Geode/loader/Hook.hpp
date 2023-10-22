#pragma once

#include "../DefaultInclude.hpp"
#include "../utils/general.hpp"
#include <json.hpp>
#include "Tulip.hpp"
#include <inttypes.h>
#include <string_view>
#include <tulip/TulipHook.hpp>

namespace geode {
    class Mod;
    class Loader;

    class GEODE_DLL Hook final {
    private:
        class Impl;
        std::shared_ptr<Impl> m_impl;
        Hook(std::shared_ptr<Impl>&& impl);
        ~Hook();

        friend class Mod;
        friend class Loader;

        Result<> enable();
        Result<> disable();

    public:
        /**
         * Create a hook at an address. The hook is enabled immediately. By 
         * default, the hook is placed at the end of the detour list; however, 
         * this can be controlled using metadata settings.
         * @param owner The mod that owns this hook; must be provided
         * @param address The address to hook
         * @param detour The detour to run when the hook is hit. The detour's 
         * calling convention should be cdecl
         * @param displayName A human-readable name describing the hook, 
         * usually the fully qualified name of the function being hooked
         * @param handlerMetadata Metadata for the hook handler
         * @param hookMetadata Metadata for the hook itself
         * @returns The created hook, or an error. Make sure to add the created 
         * hook to the mod that owns it using mod->addHook!
         */
        static Hook* create(
            Mod* owner,
            void* address,
            void* detour,
            std::string const& displayName,
            tulip::hook::HandlerMetadata const& handlerMetadata,
            tulip::hook::HookMetadata const& hookMetadata
        );

        template <class DetourType>
        static Hook* create(
            Mod* owner,
            void* address,
            DetourType detour,
            std::string const& displayName,
            tulip::hook::TulipConvention convention,
            tulip::hook::HookMetadata const& hookMetadata = tulip::hook::HookMetadata()
        ) {
            auto handlerMetadata = tulip::hook::HandlerMetadata{
                .m_convention = geode::hook::createConvention(convention),
                .m_abstract = tulip::hook::AbstractFunction::from(detour)
            };
            return Hook::create(
                owner,
                address,
                reinterpret_cast<void*>(detour),
                displayName,
                handlerMetadata,
                hookMetadata
            );
        }

        Hook(Hook const&) = delete;
        Hook operator=(Hook const&) = delete;

        /**
         * Get the address of the function hooked.
         * @returns Address
         */
        uintptr_t getAddress() const;

        /**
         * Get the display name of the function hooked.
         * @returns Display name
         */
        std::string_view getDisplayName() const;

        /**
         * Get whether the hook is enabled or not.
         * @returns True if enabled, false if not.
         */
        bool isEnabled() const;

        /**
         * Get the owner of this hook.
         * @returns Pointer to the owner's Mod handle.
         */
        Mod* getOwner() const;

        /**
         * Get info about the hook as JSON
         * @note For IPC
         */
        json::Value getRuntimeInfo() const;

        /**
         * Get the metadata of the hook.
         * @returns Hook metadata
         */
        tulip::hook::HookMetadata getHookMetadata() const;

        /**
         * Set the metadata of the hook.
         * @param metadata Hook metadata
         */
        void setHookMetadata(tulip::hook::HookMetadata const& metadata);
        
        /**
         * Get the priority of the hook.
         * @returns Priority
         */
        int32_t getPriority() const;

        /**
         * Set the priority of the hook.
         * @param priority Priority
         */
        void setPriority(int32_t priority);

        /**
        * Get whether the hook should be auto enabled or not.
        * @returns Auto enable
        */
        bool getAutoEnable() const;

        /**
         * Set whether the hook should be auto enabled or not.
         * @param autoEnable Auto enable
         */
        void setAutoEnable(bool autoEnable);
    };

    class GEODE_DLL Patch final {
    // Change to private in 2.0.0
    protected:
        Mod* m_owner;
        void* m_address;
        ByteVector m_original;
        ByteVector m_patch;
        bool m_applied;
        bool m_autoEnable;

        // Only allow friend classes to create
        // patches. Whatever method created the
        // patches should take care of populating
        // m_owner, m_address, m_original and
        // m_patch.
        Patch();
        ~Patch();

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
        uintptr_t getAddress() const;

        /**
         * Get whether the patch is applied or not.
         * @returns True if applied, false if not.
         */
        bool isApplied() const;

        bool apply();
        bool restore();

        /**
         * Get the owner of this patch.
         * @returns Pointer to the owner's Mod handle.
         */
        Mod* getOwner() const;

        /**
         * Get info about the patch as JSON
         * @note For IPC
         */
        json::Value getRuntimeInfo() const;

        /**
        * Get whether the patch should be auto enabled or not.
        * @returns Auto enable
        */
        bool getAutoEnable() const;

        /**
         * Set whether the patch should be auto enabled or not.
         * @param autoEnable Auto enable
         */
        void setAutoEnable(bool autoEnable);
    };
}
