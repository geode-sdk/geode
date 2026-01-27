#pragma once

#include "../DefaultInclude.hpp"
#include "../utils/general.hpp"
#include <matjson.hpp>
#include "Tulip.hpp"
#include <cinttypes>
#include <string_view>
#include <tulip/TulipHook.hpp>

namespace geode {
    class Mod;
    class Loader;

    class GEODE_DLL Hook final {
    private:
        class Impl;
        std::shared_ptr<Impl> m_impl;
        explicit Hook(std::shared_ptr<Impl>&& impl);
        ~Hook();

        friend class Mod;
        friend class Loader;

    public:

        /**
         * Create a hook at an address. By default, the hook is disabled and
         * placed at the end of the detour list; however, this can be
         * controlled using metadata settings.
         * After creating the hook object, we recommend you set its owner
         * by calling `Mod::claimHook`, see its docs for more info.
         * @param address The address to hook
         * @param detour The detour to run when the hook is hit. The detour's
         * calling convention should be cdecl
         * @param displayName A human-readable name describing the hook,
         * usually the fully qualified name of the function being hooked
         * @param handlerMetadata Metadata for the hook handler
         * @param hookMetadata Metadata for the hook itself
         * @returns The created hook, or an error. Make sure to add the created
         * hook to the mod that owns it using mod->claimHook(hook)!
         */
        static std::shared_ptr<Hook> create(
            void* address,
            void* detour,
            std::string displayName,
            tulip::hook::HandlerMetadata handlerMetadata,
            tulip::hook::HookMetadata hookMetadata
        );

        template<class DetourType>
        static std::shared_ptr<Hook> create(
            void* address,
            DetourType detour,
            std::string displayName,
            tulip::hook::TulipConvention convention,
            tulip::hook::HookMetadata hookMetadata = tulip::hook::HookMetadata()
        ) {
            auto handlerMetadata = tulip::hook::HandlerMetadata{
                .m_convention = geode::hook::createConvention(convention),
                .m_abstract = tulip::hook::AbstractFunction::from(detour)
            };
            return Hook::create(
                address,
                reinterpret_cast<void*>(detour),
                std::move(displayName),
                std::move(handlerMetadata),
                std::move(hookMetadata)
            );
        }

        Hook(Hook const&) = delete;
        Hook operator=(Hook const&) = delete;

        /**
         * Get the owner of this hook.
         * @returns Pointer to the owner's Mod handle.
         */
        [[nodiscard]] Mod* getOwner() const;

        /**
         * Get whether the hook is enabled or not.
         * @returns True if enabled, false if not.
         */
        [[nodiscard]] bool isEnabled() const;

        Result<> enable();

        Result<> disable();

        Result<> toggle();
        Result<> toggle(bool enable);

        /**
        * Get whether the hook should be auto enabled or not.
        * @returns Auto enable
        */
        [[nodiscard]] bool getAutoEnable() const;

        /**
         * Set whether the hook should be auto enabled or not.
         * @param autoEnable Auto enable
         */
        void setAutoEnable(bool autoEnable);

        /**
         * Get the address of the function hooked.
         * @returns Address
         */
        [[nodiscard]] uintptr_t getAddress() const;

        /**
         * Get the display name of the function hooked.
         * @returns Display name
         */
        [[nodiscard]] std::string_view getDisplayName() const;

        /**
         * Get info about the hook as JSON
         * @note For IPC
         */
        [[nodiscard]] matjson::Value getRuntimeInfo() const;

        /**
         * Get the metadata of the hook.
         * @returns Hook metadata
         */
        [[nodiscard]] tulip::hook::HookMetadata getHookMetadata() const;

        /**
         * Set the metadata of the hook.
         * @param metadata Hook metadata
         */
        void setHookMetadata(tulip::hook::HookMetadata const& metadata);

        /**
         * Get the priority of the hook.
         * @returns Priority
         */
        [[nodiscard]] int32_t getPriority() const;

        /**
         * Set the priority of the hook.
         * @param priority Priority
         */
        void setPriority(int32_t priority);
    };

    class GEODE_DLL Patch final {
    private:
        class Impl;
        std::shared_ptr<Impl> m_impl;
        explicit Patch(std::shared_ptr<Impl>&& impl);
        ~Patch();

        friend class Mod;
        friend class Loader;

    public:

        static std::shared_ptr<Patch> create(void* address, ByteSpan patch);

        Patch(Patch const&) = delete;
        Patch operator=(Patch const&) = delete;

        /**
         * Get the owner of this patch.
         * @returns Pointer to the owner's Mod handle.
         */
        [[nodiscard]] Mod* getOwner() const;

        /**
         * Get whether the patch is enabled or not.
         * @returns True if enabled, false if not.
         */
        [[nodiscard]] bool isEnabled() const;

        Result<> enable();

        Result<> disable();

        Result<> toggle();
        Result<> toggle(bool enable);

        /**
        * Get whether the patch should be auto enabled or not.
        * @returns Auto enable
        */
        [[nodiscard]] bool getAutoEnable() const;

        /**
         * Set whether the patch should be auto enabled or not.
         * @param autoEnable Auto enable
         */
        void setAutoEnable(bool autoEnable);

        /**
         * Get the bytes of the patch.
         * @returns Bytes used to patch
         */
        ByteVector const& getBytes() const;

        /**
         * Updates the bytes of the patch, disabling and then re-enabling if needed.
         * @param bytes Bytes used to patch
         */
        Result<> updateBytes(ByteSpan bytes);

        /**
         * Get the address of the patch.
         * @returns Address
         */
        [[nodiscard]] uintptr_t getAddress() const;

        /**
         * Get info about the patch as JSON
         * @note For IPC
         */
        [[nodiscard]] matjson::Value getRuntimeInfo() const;
    };
}
