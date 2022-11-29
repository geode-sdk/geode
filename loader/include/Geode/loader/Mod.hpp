#pragma once

#include "Types.hpp"
#include "Hook.hpp"
#include "Setting.hpp"
#include "ModInfo.hpp"

#include "../DefaultInclude.hpp"
#include "../utils/Result.hpp"
#include "../utils/VersionInfo.hpp"
#include "../external/json/json.hpp"
#include "../utils/general.hpp"
#include "../cocos/support/zip_support/ZipUtils.h"
#include <optional>
#include <string_view>
#include <type_traits>
#include <unordered_map>
#include <vector>

namespace geode {
    template<class T>
    struct HandleToSaved : public T {
        Mod* m_mod;
        std::string m_key;

        HandleToSaved(std::string const& key, Mod* mod, T const& value)
          : T(value),
            m_key(key),
            m_mod(mod) {}
        HandleToSaved(HandleToSaved const&) = delete;
        HandleToSaved(HandleToSaved&&) = delete;
        ~HandleToSaved();
    };

    /**
     * @class Mod
     * Represents a Mod ingame.
     * @abstract
     */
    class GEODE_DLL Mod {
    protected:
        /**
         * Mod info
         */
        ModInfo m_info;
        /**
         * Platform-specific info
         */
        PlatformInfo* m_platformInfo = nullptr;
        /**
         * Hooks owned by this mod
         */
        std::vector<Hook*> m_hooks;
        /**
         * Patches owned by this mod
         */
        std::vector<Patch*> m_patches;
        /**
         * Whether the mod is enabled or not
         */
        bool m_enabled = false;
        /**
         * Whether the mod binary is loaded or not
         */
        bool m_binaryLoaded = false;
        /**
         * Whether the mod is loadable or not
         */
        bool m_resolved = false;
        /**
         * Mod temp directory name
         */
        ghc::filesystem::path m_tempDirName;
        /**
         * Mod save directory name
         */
        ghc::filesystem::path m_saveDirPath;
        /**
         * Pointers to mods that depend on
         * this Mod. Makes it possible to
         * enable / disable them automatically,
         * when their dependency is disabled.
         */
        std::vector<Mod*> m_parentDependencies;
        decltype(geode_implicit_load)* m_implicitLoadFunc;
        /**
         * Saved values
         */
        nlohmann::json m_saved;

        /**
         * Load the platform binary
         */
        Result<> loadPlatformBinary();
        Result<> unloadPlatformBinary();
        Result<> createTempDir();

        // no copying
        Mod(Mod const&) = delete;
        Mod operator=(Mod const&) = delete;

        /**
         * Protected constructor/destructor
         */
        Mod() = delete;
        Mod(ModInfo const& info);
        virtual ~Mod();

        friend class ::InternalMod;
        friend class Loader;
        friend class ::InternalLoader;
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
        decltype(ModInfo::m_settings) getSettings() const;
        bool hasSetting(std::string const& key) const;
        std::shared_ptr<Setting> getSetting(std::string const& key) const;

        template <class T>
        T getSettingValue(std::string const& key) const {
            if (this->hasSetting(key)) {
                return geode::getBuiltInSettingValue<T>(this->getSetting(key));
            }
            return T();
        }

        template <class T>
        bool setSettingValue(std::string const& key, T const& value) {
            if (this->hasSetting(key)) {
                geode::setBuiltInSettingValue<T>(this->getSetting(key), value);
                return true;
            }
            return false;
        }

        template<class T>
        T getSavedValue(std::string const& key) {
            if (m_saved.count(key)) {
                try {
                    // json -> T may fail
                    return m_saved.at(key);
                } catch(...) {}
            }
            return T();
        }

        template<class T>
        T getSavedValue(std::string const& key, T const& defaultValue) {
            if (m_saved.count(key)) {
                try {
                    // json -> T may fail
                    return m_saved.at(key);
                } catch(...) {}
            }
            m_saved[key] = defaultValue;
            return defaultValue;
        }

        template<class T>
        HandleToSaved<T> getSavedMutable(std::string const& key) {
            return HandleToSaved(key, this, this->getSavedValue<T>(key));
        }

        template<class T>
        HandleToSaved<T> getSavedMutable(std::string const& key, T const& defaultValue) {
            return HandleToSaved(key, this, this->getSavedValue<T>(key, defaultValue));
        }

        /**
         * Set the value of an automatically saved variable. When the game is 
         * closed, the value is automatically saved under the key
         * @param key Key of the saved value
         * @param value Value
         */
        template<class T>
        void setSavedValue(std::string const& key, T const& value) {
            m_saved[key] = value;
        }

        /**
         * Get the mod container stored in the Interface
         * @returns nullptr if Interface is not initialized,
         * the mod pointer if it is initialized
         */
        template <class = void>
        static inline GEODE_HIDDEN Mod* get() {
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
        Result<Patch*> patch(void* address, byte_array data);

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
        bool updateDependencyStates();
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
    };

    template<class T>
    HandleToSaved<T>::~HandleToSaved() {
        m_mod->setSavedValue(m_key, static_cast<T>(*this));
    }

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
