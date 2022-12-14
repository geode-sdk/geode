#pragma once

#include "../DefaultInclude.hpp"
#include "../cocos/support/zip_support/ZipUtils.h"
#include "../external/json/json.hpp"
#include "../utils/Result.hpp"
#include "../utils/VersionInfo.hpp"
#include "../utils/general.hpp"
#include "Hook.hpp"
#include "ModInfo.hpp"
#include "Setting.hpp"
#include "Types.hpp"

#include <optional>
#include <string_view>
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

    GEODE_HIDDEN Mod* takeNextLoaderMod();

    class ModImpl;

    /**
     * @class Mod
     * Represents a Mod ingame.
     */
    class GEODE_DLL Mod {
    protected:
        class Impl;
        std::unique_ptr<Impl> m_impl;

        friend class Loader;
        friend struct ModInfo;

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
        Mod(ModInfo const& info);
        ~Mod();
        

        std::string getID() const;
        std::string getName() const;
        std::string getDeveloper() const;
        std::optional<std::string> getDescription() const;
        std::optional<std::string> getDetails() const;
        ghc::filesystem::path getPackagePath() const;
        VersionInfo getVersion() const;
        bool isEnabled() const;
        bool isLoaded() const;
        bool supportsDisabling() const;
        bool supportsUnloading() const;
        bool wasSuccesfullyLoaded() const;
        ModInfo getModInfo() const;
        ghc::filesystem::path getTempDir() const;
        ghc::filesystem::path getBinaryPath() const;

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
        void registerCustomSetting(
            std::string const& key,
            std::unique_ptr<SettingValue> value
        );

        nlohmann::json& getSaveContainer();

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
                auto old = this->getSettingValue<T>(sett);
                SettingValueSetter<T>::set(sett, value);
                return old;
            }
            return T();
        }

        template <class T>
        T getSavedValue(std::string const& key) {
            auto& saved = this->getSaveContainer();
            if (saved.count(key)) {
                try {
                    // json -> T may fail
                    return saved.at(key);
                }
                catch (...) {
                }
            }
            return T();
        }

        template <class T>
        T getSavedValue(std::string const& key, T const& defaultValue) {
            auto& saved = this->getSaveContainer();
            if (saved.count(key)) {
                try {
                    // json -> T may fail
                    return saved.at(key);
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
        template<class T>
        T setSavedValue(std::string const& key, T const& value) {
            auto& saved = this->getSaveContainer();
            auto old = this->getSavedValue<T>(key);
            saved[key] = value;
            return old;
        }

        /**
         * Get the mod container stored in the Interface
         * @returns nullptr if Interface is not initialized,
         * the mod pointer if it is initialized
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
         * @returns Successful result containing the
         * Hook handle, errorful result with info on
         * error
         */
        template <auto Detour, template <class, class...> class Convention>
        Result<Hook*> addHook(void* address) {
            return this->addHook<Detour, Convention>("", address);
        }

        /**
         * Create a hook at an address. Call the original
         * function by calling the original function –
         * no trampoline needed. Also takes a displayName
         * parameter to use for when visualizing the hook.
         * @param address The absolute address of
         * the function to hook, i.e. gd_base + 0xXXXX
         * @param detour Pointer to your detour function
         * @returns Successful result containing the
         * Hook handle, errorful result with info on
         * error
         */
        template <auto Detour, template <class, class...> class Convention>
        Result<Hook*> addHook(std::string const& displayName, void* address) {
            auto hook =
                Hook::create<Detour, Convention>((decltype(Detour))address, displayName, this);
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
        Result<> loadBinary();

        /**
         * Disable & unload this mod
         * @warning May crash if the mod doesn't
         * properly handle unloading!
         * @returns Successful result on success,
         * errorful result with info on error
         */
        Result<> unloadBinary();

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
         * Disable & unload this mod (if supported), then delete the mod's
         * .geode package. If unloading isn't supported, the mod's binary
         * will stay loaded, and in all cases the Mod* instance will still
         * exist and be interactable.
         * @returns Successful result on success,
         * errorful result with info on error
         */
        Result<> uninstall();
        bool isUninstalled() const;

        /**
         * Check whether or not this Mod
         * depends on another mod
         */
        bool depends(std::string const& id) const;

        /**
         * Check whether all the required
         * dependencies for this mod have
         * been loaded or not
         * @returns True if the mod has unresolved
         * dependencies, false if not.
         */
        bool hasUnresolvedDependencies() const;
        /**
         * Update the state of each of the
         * dependencies. Depending on if the
         * mod has unresolved dependencies,
         * it will either be loaded or unloaded
         * @returns True if the mod has unresolved
         * dependencies, false if not.
         */
        Result<> updateDependencies();
        /**
         * Get a list of all the unresolved
         * dependencies this mod has
         * @returns List of all the unresolved
         * dependencies
         */
        std::vector<Dependency> getUnresolvedDependencies();

        char const* expandSpriteName(char const* name);

        /**
         * Get info about the mod as JSON
         * @note For IPC
         */
        ModJson getRuntimeInfo() const;

        friend class ModImpl;
    };

    /**
     * To bypass the need for cyclic dependencies,
     * this function does the exact same as Mod::get()
     * However, it can be externed, unlike Mod::get()
     * @returns Same thing Mod::get() returns
     */
    inline GEODE_HIDDEN Mod* getMod() {
        return Mod::get();
    }
}

inline char const* operator"" _spr(char const* str, size_t) {
    return geode::Mod::get()->expandSpriteName(str);
}

// this header uses Mod
#include "ModEvent.hpp"
