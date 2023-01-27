#pragma once

#include "../DefaultInclude.hpp"
#include "../cocos/support/zip_support/ZipUtils.h"
#include <json.hpp>
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
#include <tulip/TulipHook.hpp>

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
     * Represents a Mod ingame.
     * @class Mod
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

        json::Value& getSaveContainer();

        template <class T>
        T getSettingValue(std::string const& key) const;

        template <class T>
        T setSettingValue(std::string const& key, T const& value);

        template <class T>
        T getSavedValue(std::string const& key);

        template <class T>
        T getSavedValue(std::string const& key, T const& defaultValue);

        /**
         * Set the value of an automatically saved variable. When the game is
         * closed, the value is automatically saved under the key
         * @param key Key of the saved value
         * @param value Value
         * @returns The old value
         */
        template<class T>
        T setSavedValue(std::string const& key, T const& value);

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
         * @param hookMetadata Metadata of the hook       
         * @returns Successful result containing the
         * Hook pointer, errorful result with info on
         * error
         */
        template <class Convention, class DetourType>
        Result<Hook*> addHook(
            void* address, DetourType detour, 
            std::string const& displayName = "", 
            tulip::hook::HookMetadata const& hookMetadata = tulip::hook::HookMetadata()
        ) {
            auto hook = Hook::create<Convention>(this, address, detour, displayName, hookMetadata);
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
}

inline char const* operator"" _spr(char const* str, size_t) {
    return geode::Mod::get()->expandSpriteName(str);
}
