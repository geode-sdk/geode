#pragma once

#include "../DefaultInclude.hpp"
#include "../cocos/support/zip_support/ZipUtils.h"
#include "../utils/Result.hpp"
#include "../utils/VersionInfo.hpp"
#include "../utils/general.hpp"
#include "Hook.hpp"
#include "ModMetadata.hpp"
#include "Setting.hpp"
#include "Types.hpp"

#include <matjson.hpp>
#include <optional>
#include <string_view>
#include <tulip/TulipHook.hpp>
#include <type_traits>
#include <unordered_map>
#include <vector>

namespace geode {
    template <class T>
    struct HandleToSaved : public T {
        Mod* m_mod;
        std::string m_key;

        HandleToSaved(std::string const& key, Mod* mod, T const& value) :
            T(value), m_key(key), m_mod(mod) {}

        HandleToSaved(HandleToSaved const&) = delete;
        HandleToSaved(HandleToSaved&&) = delete;
        ~HandleToSaved();
    };

    enum class ModRequestedAction {
        None,
        Enable,
        Disable,
        Uninstall,
        UninstallWithSaveData
    };

    GEODE_HIDDEN Mod* takeNextLoaderMod();

    class ModImpl;

    /**
     * Represents a Mod ingame.
     * @class Mod
     */
    class GEODE_DLL Mod {
    protected:
        class Impl;
        std::unique_ptr<Impl> m_impl;

        friend class Loader;

        template <class = void>
        static inline GEODE_HIDDEN Mod* sharedMod = nullptr;

        // used internally in geode_implicit_load
        template <class = void>
        static inline GEODE_HIDDEN void setSharedMod(Mod* mod) {
            sharedMod<> = mod;
        }

        friend void GEODE_CALL ::geode_implicit_load(Mod*);

    public:
        // no copying
        Mod(Mod const&) = delete;
        Mod operator=(Mod const&) = delete;

        // Protected constructor/destructor
        Mod() = delete;
        Mod(ModMetadata const& metadata);
        ~Mod();

        std::string getID() const;
        std::string getName() const;
        std::string getDeveloper() const;
        std::optional<std::string> getDescription() const;
        std::optional<std::string> getDetails() const;
        ghc::filesystem::path getPackagePath() const;
        VersionInfo getVersion() const;
        bool isEnabled() const;
        bool supportsDisabling() const;
        bool needsEarlyLoad() const;
        bool wasSuccessfullyLoaded() const;
        ModMetadata getMetadata() const;
        ghc::filesystem::path getTempDir() const;
        /**
         * Get the path to the mod's platform binary (.dll on Windows, .dylib
         * on Mac & iOS, .so on Android)
         */
        ghc::filesystem::path getBinaryPath() const;
        /**
         * Get the path to the mod's runtime resources directory (contains all
         * of its resources)
         */
        ghc::filesystem::path getResourcesDir() const;

#if defined(GEODE_EXPOSE_SECRET_INTERNALS_IN_HEADERS_DO_NOT_DEFINE_PLEASE)
        void setMetadata(ModMetadata const& metadata);
        std::vector<Mod*> getDependants() const;
#endif

        Result<> saveData();
        Result<> loadData();

        /**
         * Get the mod's save directory path
         */
        ghc::filesystem::path getSaveDir() const;
        /**
         * Get the mod's config directory path
         */
        ghc::filesystem::path getConfigDir(bool create = true) const;

        bool hasSettings() const;
        std::vector<std::string> getSettingKeys() const;
        bool hasSetting(std::string_view const key) const;
        std::optional<Setting> getSettingDefinition(std::string_view const key) const;
        SettingValue* getSetting(std::string_view const key) const;

        /**
         * Register a custom setting's value class. See Mod::addCustomSetting
         * for a convenience wrapper that creates the value in-place to avoid
         * code duplication. Also see
         * [the tutorial page](https://docs.geode-sdk.org/mods/settings) for
         * more information about custom settings
         * @param key The setting's key
         * @param value The SettingValue class that shall handle this setting
         * @see addCustomSetting
         */
        void registerCustomSetting(std::string_view const key, std::unique_ptr<SettingValue> value);
        /**
         * Register a custom setting's value class. The new SettingValue class
         * will be created in-place using `std::make_unique`. See
         * [the tutorial page](https://docs.geode-sdk.org/mods/settings) for
         * more information about custom settings
         * @param key The setting's key
         * @param value The value of the custom setting
         * @example
         * $on_mod(Loaded) {
         *     Mod::get()->addCustomSetting<MySettingValue>("setting-key", DEFAULT_VALUE);
         * }
         */
        template <class T, class V>
        void addCustomSetting(std::string_view const key, V const& value) {
            this->registerCustomSetting(key, std::make_unique<T>(std::string(key), this->getID(), value));
        }

        matjson::Value& getSaveContainer();

        template <class T>
        T getSettingValue(std::string_view const key) const {
            if (auto sett = this->getSetting(key)) {
                return SettingValueSetter<T>::get(sett);
            }
            return T();
        }

        template <class T>
        T setSettingValue(std::string_view const key, T const& value) {
            if (auto sett = this->getSetting(key)) {
                auto old = this->getSettingValue<T>(key);
                SettingValueSetter<T>::set(sett, value);
                return old;
            }
            return T();
        }

        bool hasSavedValue(std::string_view const key);

        template <class T>
        T getSavedValue(std::string_view const key) {
            auto& saved = this->getSaveContainer();
            if (saved.contains(key)) {
                try {
                    // json -> T may fail
                    return saved.get<T>(key);
                }
                catch (...) {
                }
            }
            return T();
        }

        template <class T>
        T getSavedValue(std::string_view const key, T const& defaultValue) {
            auto& saved = this->getSaveContainer();
            if (saved.contains(key)) {
                try {
                    // json -> T may fail
                    return saved.get<T>(key);
                }
                catch (...) {
                }
            }
            saved[key] = defaultValue;
            return defaultValue;
        }

        /**
         * Set the value of an automatically saved variable. When the game is
         * closed, the value is automatically saved under the key
         * @param key Key of the saved value
         * @param value Value
         * @returns The old value
         */
        template <class T>
        T setSavedValue(std::string_view const key, T const& value) {
            auto& saved = this->getSaveContainer();
            auto old = this->getSavedValue<T>(key);
            saved[key] = value;
            return old;
        }

        /**
         * Get the Mod of the current mod being developed
         * @returns The current mod
         */
        template <class = void>
        static inline GEODE_HIDDEN Mod* get() {
            if (!sharedMod<>) {
                sharedMod<> = takeNextLoaderMod();
            }
            return sharedMod<>;
        }

        /**
         * Create a hook at an address. Call the original
         * function by calling the original function –
         * no trampoline needed
         * @param address The absolute address of
         * the function to hook, i.e. gd_base + 0xXXXX
         * @param detour Pointer to your detour function
         * @param displayName Name of the hook that will be
         * displayed in the hook list
         * @param convention Calling convention of the hook
         * @param hookMetadata Metadata of the hook
         * @returns Successful result containing the
         * Hook pointer, errorful result with info on
         * error
         */
        template<class DetourType>
        Result<Hook*> hook(
            void* address, DetourType detour, std::string const& displayName = "",
            tulip::hook::TulipConvention convention = tulip::hook::TulipConvention::Default,
            tulip::hook::HookMetadata const& hookMetadata = tulip::hook::HookMetadata()
        ) {
            auto hook = Hook::create(address, detour, displayName, convention, hookMetadata);
            GEODE_UNWRAP_INTO(auto ptr, this->claimHook(std::move(hook)));
            return Ok(ptr);
        }

        Result<Hook*> hook(
            void* address, void* detour, std::string const& displayName,
            tulip::hook::HandlerMetadata const& handlerMetadata,
            tulip::hook::HookMetadata const& hookMetadata
        ) {
            auto hook = Hook::create(address, detour, displayName, handlerMetadata, hookMetadata);
            GEODE_UNWRAP_INTO(auto ptr, this->claimHook(std::move(hook)));
            return Ok(ptr);
        }

        Result<Hook*> claimHook(std::shared_ptr<Hook>&& hook);

        Result<> disownHook(Hook* hook);

        /**
         * Get all hooks owned by this Mod
         * @returns Vector of hooks
         */
        [[nodiscard]] std::vector<Hook*> getHooks() const;

        /**
         * Write a patch at an address
         * @param address The address to write into
         * @param data The data to write there
         * @returns Successful result on success,
         * errorful result with info on error
         */
        Result<Patch*> patch(void* address, ByteVector const& data) {
            auto patch = Patch::create(address, data);
            GEODE_UNWRAP_INTO(auto ptr, this->claimPatch(std::move(patch)));
            return Ok(ptr);
        }

        Result<Patch*> claimPatch(std::shared_ptr<Patch>&& patch);

        Result<> disownPatch(Patch* patch);

        /**
         * Get all patches owned by this Mod
         * @returns Vector of patches
         */
        [[nodiscard]] std::vector<Patch*> getPatches() const;

        /**
         * Enable this mod
         * @returns Successful result on success,
         * errorful result with info on error
         */
        Result<> enable();

        /**
         * Disable this mod
         * @returns Successful result on success,
         * errorful result with info on error
         */
        Result<> disable();

        /**
         * Delete the mod's .geode package.
         * @param deleteSaveData Whether should also delete the mod's save data
         * @returns Successful result on success,
         * errorful result with info on error
         */
        Result<> uninstall(bool deleteSaveData = false);
        bool isUninstalled() const;

        ModRequestedAction getRequestedAction() const;

        /**
         * Check whether or not this Mod
         * depends on another mod
         */
        bool depends(std::string_view const id) const;

        /**
         * Check whether all the required
         * dependencies for this mod have
         * been loaded or not
         * @returns True if the mod has unresolved
         * dependencies, false if not.
         */
        bool hasUnresolvedDependencies() const;
        /**
         * Check whether none of the
         * incompatibilities with this mod are loaded
         * @returns True if the mod has unresolved
         * incompatibilities, false if not.
         */
        bool hasUnresolvedIncompatibilities() const;

        char const* expandSpriteName(char const* name);

        /**
         * Get info about the mod as JSON
         * @note For IPC
         */
        ModJson getRuntimeInfo() const;

        bool isLoggingEnabled() const;
        void setLoggingEnabled(bool enabled);

        bool shouldLoad() const;

        friend class ModImpl;
    };
}

GEODE_HIDDEN inline char const* operator"" _spr(char const* str, size_t) {
    return geode::Mod::get()->expandSpriteName(str);
}
