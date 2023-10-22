#pragma once

#include "../DefaultInclude.hpp"
#include "../cocos/support/zip_support/ZipUtils.h"
#include "../utils/Result.hpp"
#include "../utils/VersionInfo.hpp"
#include "../utils/general.hpp"
#include "Hook.hpp"
#include "ModInfo.hpp"
#include "ModMetadata.hpp"
#include "Setting.hpp"
#include "Types.hpp"

#include <json.hpp>
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
        [[deprecated]] Mod(ModInfo const& info);
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
        [[deprecated("use isEnabled instead")]] bool isLoaded() const;
        bool supportsDisabling() const;
        [[deprecated("always true")]] bool canDisable() const;
        [[deprecated("always true")]] bool canEnable() const;
        bool needsEarlyLoad() const;
        [[deprecated("always false")]] bool supportsUnloading() const;
        [[deprecated("use isEnabled instead")]] bool wasSuccesfullyLoaded() const;
        [[deprecated("use isEnabled instead")]] bool wasSuccessfullyLoaded() const;
        [[deprecated("use getMetadata instead")]] ModInfo getModInfo() const;
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
        bool hasSetting(std::string const& key) const;
        std::optional<Setting> getSettingDefinition(std::string const& key) const;
        SettingValue* getSetting(std::string const& key) const;

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
        void registerCustomSetting(std::string const& key, std::unique_ptr<SettingValue> value);
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
        void addCustomSetting(std::string const& key, V const& value) {
            this->registerCustomSetting(key, std::make_unique<T>(key, this->getID(), value));
        }

        json::Value& getSaveContainer();

        template <class T>
        T getSettingValue(std::string const& key) const {
            if (auto sett = this->getSetting(key)) {
                return SettingValueSetter<T>::get(sett);
            }
            return T();
        }

        template <class T>
        T setSettingValue(std::string const& key, T const& value) {
            if (auto sett = this->getSetting(key)) {
                auto old = this->getSettingValue<T>(key);
                SettingValueSetter<T>::set(sett, value);
                return old;
            }
            return T();
        }

        bool hasSavedValue(std::string const& key);

        template <class T>
        T getSavedValue(std::string const& key) {
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
        T getSavedValue(std::string const& key, T const& defaultValue) {
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
        T setSavedValue(std::string const& key, T const& value) {
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
         * Get all hooks owned by this Mod
         * @returns Vector of hooks
         */
        std::vector<Hook*> getHooks() const;

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
        template <class DetourType>
        Result<Hook*> addHook(
            void* address, DetourType detour, std::string const& displayName = "",
            tulip::hook::TulipConvention convention = tulip::hook::TulipConvention::Default,
            tulip::hook::HookMetadata const& hookMetadata = tulip::hook::HookMetadata()
        ) {
            auto hook = Hook::create(this, address, detour, displayName, convention, hookMetadata);
            return this->addHook(hook);
        }

        Result<Hook*> addHook(Hook* hook);

        /**
         * Enable a hook owned by this Mod
         * @returns Successful result on success,
         * errorful result with info on error
         */
        Result<> enableHook(Hook* hook);

        /**
         * Disable a hook owned by this Mod
         * @returns Successful result on success,
         * errorful result with info on error
         */
        Result<> disableHook(Hook* hook);

        /**
         * Remove a hook owned by this Mod
         * @returns Successful result on success,
         * errorful result with info on error
         */
        Result<> removeHook(Hook* hook);

        /**
         * Write a patch at an address
         * @param address The address to write into
         * @param data The data to write there
         * @returns Successful result on success,
         * errorful result with info on error
         */
        Result<Patch*> patch(void* address, ByteVector const& data);

        /**
         * Remove a patch owned by this Mod
         * @returns Successful result on success,
         * errorful result with info on error
         */
        Result<> unpatch(Patch* patch);

        /**
         * Load & enable this mod
         * @returns Successful result on success,
         * errorful result with info on error
         */
        [[deprecated]] Result<> loadBinary();

        /**
         * Disable & unload this mod
         * @warning May crash if the mod doesn't
         * properly handle unloading!
         * @returns Successful result on success,
         * errorful result with info on error
         */
        [[deprecated]] Result<> unloadBinary();

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
         * Disable this mod (if supported), then delete the mod's .geode package.
         * @returns Successful result on success,
         * errorful result with info on error
         */
        Result<> uninstall();
        bool isUninstalled() const;

        ModRequestedAction getRequestedAction() const;

        /**
         * Check whether or not this Mod
         * depends on another mod
         */
        bool depends(std::string const& id) const;

        /**
         * Update the state of each of the
         * dependencies. Depending on if the
         * mod has unresolved dependencies,
         * it will either be loaded or unloaded
         * @returns Error.
         * @deprecated No longer needed.
         */
        [[deprecated("no longer needed")]] Result<> updateDependencies();

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
        /**
         * Get a list of all the unresolved
         * dependencies this mod has
         * @returns List of all the unresolved
         * dependencies
         * @deprecated Use Loader::getProblems instead.
         */
        [[deprecated("use Loader::getProblems instead")]] std::vector<Dependency> getUnresolvedDependencies();

        char const* expandSpriteName(char const* name);

        /**
         * Get info about the mod as JSON
         * @note For IPC
         */
        ModJson getRuntimeInfo() const;

        friend class ModImpl;
    };
}

GEODE_HIDDEN inline char const* operator"" _spr(char const* str, size_t) {
    return geode::Mod::get()->expandSpriteName(str);
}
