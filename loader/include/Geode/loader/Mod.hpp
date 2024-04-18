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
#include "Loader.hpp"

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
        [[deprecated("Use Mod::getDevelopers instead")]]
        std::string getDeveloper() const;
        std::vector<std::string> getDevelopers() const;
        std::optional<std::string> getDescription() const;
        std::optional<std::string> getDetails() const;
        ghc::filesystem::path getPackagePath() const;
        VersionInfo getVersion() const;
        bool isEnabled() const;
        bool isInternal() const;
        bool needsEarlyLoad() const;
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

        /**
         * Check if this Mod has updates available on the mods index. If 
         * you're using this for automatic update checking, use 
         * `openInfoPopup` or `openIndexPopup` from the `ui/GeodeUI.hpp` 
         * header to open the Mod's page to let the user install the update
         * @returns The latest available version on the index if there are 
         * updates for this mod
         */
        std::optional<VersionInfo> hasAvailableUpdate() const;

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

        /**
         * Returns a prefixed launch argument name. See `Mod::getLaunchArgument`
         * for details about mod-specific launch arguments.
         */
        std::string getLaunchArgumentName(std::string_view const name) const;
        /**
         * Returns the names of the available mod-specific launch arguments.
         */
        std::vector<std::string> getLaunchArgumentNames() const;
        /**
         * Equivalent to a prefixed `Loader::hasLaunchArgument` call. See `Mod::getLaunchArgument`
         * for details about mod-specific launch arguments.
         * @param name The argument name
         */
        bool hasLaunchArgument(std::string_view const name) const;
        /**
         * Get a mod-specific launch argument. This is equivalent to `Loader::getLaunchArgument`
         * with the argument name prefixed by the mod ID. For example, a launch argument named
         * `mod-arg` for the mod `author.test` would be specified with `--geode:author.test.mod-arg=value`.
         * @param name The argument name
         * @return The value, if present
         */
        std::optional<std::string> getLaunchArgument(std::string_view const name) const;
        /**
         * Equivalent to a prefixed `Loader::getLaunchFlag` call. See `Mod::getLaunchArgument`
         * for details about mod-specific launch arguments.
         * @param name The argument name
         */
        bool getLaunchFlag(std::string_view const name) const;
        /**
         * Equivalent to a prefixed `Loader::parseLaunchArgument` call. See `Mod::getLaunchArgument`
         * for details about mod-specific launch arguments.
         * @param name The argument name
         */
        template <class T>
        std::optional<T> parseLaunchArgument(std::string_view const name) const {
            return Loader::get()->parseLaunchArgument<T>(this->getLaunchArgumentName(name));
        }

        matjson::Value& getSaveContainer();
        matjson::Value& getSavedSettingsData();

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
                if (auto value = saved.try_get<T>(key)) {
                    return *value;
                }
            }
            return T();
        }

        template <class T>
        T getSavedValue(std::string_view const key, T const& defaultValue) {
            auto& saved = this->getSaveContainer();
            if (saved.contains(key)) {
                if (auto value = saved.try_get<T>(key)) {
                    return *value;
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

        /**
         * Claims an existing hook object, marking this mod as its owner.
         * If the hook has "auto enable" set, this will enable the hook.
         * @returns Returns a pointer to the hook, or an error if the
         * hook already has an owner, or was unable to enable the hook.
         */
        Result<Hook*> claimHook(std::shared_ptr<Hook> hook);

        /**
         * Disowns a hook which this mod owns, making this mod no longer its owner.
         * If the hook has "auto enable" set, this will disable the hook.
         * @returns Returns an error if this mod doesn't own the hook, or
         * if disabling the hook failed.
         */
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

        /**
         * Claims an existing patch object, marking this mod as its owner.
         * If the patch has "auto enable" set, this will enable the patch.
         * @returns Returns a pointer to the patch, or an error if the
         * patch already has an owner, or was unable to enable the patch.
         */
        Result<Patch*> claimPatch(std::shared_ptr<Patch> patch);

        /**
         * Disowns a patch which this mod owns, making this mod no longer its owner.
         * If the patch has "auto enable" set, this will disable the patch.
         * @returns Returns an error if this mod doesn't own the patch, or
         * if disabling the patch failed.
         */
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

        bool hasProblems() const;
        bool shouldLoad() const;
        bool isCurrentlyLoading() const;

        friend class ModImpl;
    };
}

GEODE_HIDDEN inline char const* operator"" _spr(char const* str, size_t) {
    return geode::Mod::get()->expandSpriteName(str);
}
