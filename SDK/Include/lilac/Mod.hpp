#pragma once

#include "Macros.hpp"
#include "Types.hpp"
#include <helpers/Result.hpp>
#include <helpers/VersionInfo.hpp>
#include <string_view>
#include <vector>
#include <unordered_map>
#include "Setting.hpp"
#include <helpers/types.hpp>

class Lilac;
class InternalMod;

namespace lilac {                  
    struct PlatformInfo;

    class Hook;
    class Patch;
    class Loader;
    class LogStream;
    class Mod;

    struct Dependency {
        std::string m_id;
        // todo: Dynamic versions (1.*.*)
        VersionInfo m_version { 1, 0, 0 };
        ModResolveState m_state = ModResolveState::Unloaded;
        bool m_required = false;
        Mod* m_mod = nullptr;
        bool isUnresolved() const;
    };

    /**
     * Represents all the data gatherable 
     * from mod.json
     */
    struct LILAC_DLL ModInfo {
        /**
         * Path to the mod file
         */
        std::string m_path;
        /**
         * Name of the platform binary within 
         * the mod zip
         */
        std::string m_binaryName = LILAC_WINDOWS("mod.dll")
                                   LILAC_MACOS("mod.dylib")
                                   LILAC_ANDROID("mod.so");
        /**
         * Mod Version. Should follow semver.
         */
        VersionInfo m_version { 1, 0, 0 };
        /**
         * Human-readable ID of the Mod.
         * Recommended to be in the format
         * "com.developer.mod". Not
         * guaranteed to be either case-
         * nor space-sensitive. Should
         * be restricted to the ASCII
         * character set.
         */
        std::string m_id;
        /**
         * Name of the mod. May contain
         * spaces & punctuation, but should
         * be restricted to the ASCII
         * character set.
         */
        std::string m_name;
        /**
         * The name of the head developer.
         * Should be a single name, like
         * "HJfod" or "The Lilac Team".
         * If the mod has multiple
         * developers, this field should
         * be one of their name or a team
         * name, and the rest of the credits
         * should be named in `m_credits`
         * instead.
         */
        std::string m_developer;
        /**
         * Short & concise description of the 
         * mod.
         */
        std::string m_description = "";
        /**
         * Free-form detailed description
         * of the mod. Do not write credits
         * here; use `m_credits` instead.
         */
        std::string m_details = "";
        /**
         * Free-form list of credits.
         */
        std::string m_credits = "";
        /**
         * Dependencies
         */
        std::vector<Dependency> m_dependencies;
        /**
         * Settings
         */
        std::unordered_map<std::string, Setting*> m_settings;
    };

    /**
     * @class Mod
     * Represents a Mod ingame. Inherit
     * from this class to create your own
     * mod interfaces.
     * @abstract
     */
    class LILAC_DLL Mod {
    protected:
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
        bool m_loaded = false;
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
        ghc::filesystem::path m_saveDirName;
        /**
         * Pointers to mods that depend on 
         * this Mod. Makes it possible to 
         * enable / disable them automatically, 
         * when their dependency is disabled.
         */
        std::vector<Mod*> m_parentDependencies;
        /**
         * Pointer to the Mod's load function
         */
        lilac_load   m_loadFunc   = nullptr;
        /**
         * Pointer to the Mod's unload function
         */
        lilac_unload m_unloadFunc = nullptr;

        /**
         * Load the platform binary
         */
        Result<> loadPlatformBinary();
        Result<> unloadPlatformBinary();

 public:    // Quick fix, fod pls make it work
        /**
         * Low-level add hook
         */
        Result<Hook*> addHookBase(
            void* addr,
            void* detour,
            Hook* hook = nullptr
        );
        Result<Hook*> addHookBase(Hook* hook);

        Result<> createTempDir();

        /**
         * Mod info
         */
        ModInfo m_info; // pain
 protected:

        static bool validateID(std::string const& id);
        // no copying
        Mod(Mod const&)           = delete;
        Mod operator=(Mod const&) = delete;
        
        /**
         * Protected constructor/destructor
         */
        Mod() = delete;
        Mod(ModInfo const& info);
        virtual ~Mod();

        friend class InternalMod;
        friend class Loader;
        friend class Lilac;
        friend struct ModInfo;

    public:
        std::string getID()         const;
        std::string getName()       const;
        std::string getDeveloper()  const;
        std::string getDescription()const;
        std::string getDetails()    const;
        std::string getCredits()    const;
        std::string getPath()       const;
        VersionInfo getVersion()    const;
        bool        isEnabled()     const;
        bool        isLoaded()      const;

        /**
         * Log to lilac's integrated console / 
         * the platform debug console.
         * @returns Reference to log stream. Make sure 
         * to end your logging with lilac::endl.
         */
        LogStream& log();

        /**
         * Throw an error. Equivalent to 
         * ```
         * Mod::log() << Severity::severity << info << lilac::endl.
         * ```
         * @param info Error infomration
         * @param severity Error severity
         */
        void throwError(
            std::string const& info,
            Severity severity
        );

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
        Result<Hook*> addHook(void* address, void* detour);

        /**
         * Create a hook at an address with a detour
         * and trampoline
         * @param address The absolute address of 
         * the function to hook, i.e. gd_base + 0xXXXX
         * @param detour Pointer to your detour function
         * @param trampoline Pointer to a function pointer 
         * used to call the original
         * @returns Successful result containing the 
         * Hook handle, errorful result with info on 
         * error
         */
        Result<Hook*> addHook(void* address, void* detour, void** trampoline);

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
         * Load this mod
         * @returns Successful result on success, 
         * errorful result with info on error
         */
        Result<> load();

        /**
         * Unload this mod
         * @warning May crash if the mod doesn't 
         * properly handle unloading!
         * @returns Successful result on success, 
         * errorful result with info on error
         */
        Result<> unload();

        /**
         * Enable & load this mod
         * @returns Successful result on success, 
         * errorful result with info on error
         */
        Result<> enable();
        
        /**
         * Disable & unload this mod
         * @warning May crash if the mod doesn't 
         * properly handle unloading!
         * @returns Successful result on success, 
         * errorful result with info on error
         */
        Result<> disable();

        /**
         * Get the value of a setting
         * @param key The setting's key
         * @returns Successful result with the 
         * setting's value on success, errorful 
         * result with info on error. Note that 
         * if the setting is managed by a custom 
         * control and/or has a custom type, this 
         * function will return an errorful result; 
         * you will need to use that custom control's 
         * getter, provided it has one implemented.
         */
        template<typename T>
        Result<T> getSettingValue(std::string const& key) const {
            if (!this->m_info.m_settings.count(key)) {
                return Err<>("No setting \"" + key + "\" exists");
            }
            return this->m_info.m_settings.at(key)->getValueAs<T>();
        }

        /**
         * Register a custom setting control 
         * for a setting marked as "custom". 
         * @param key Setting key
         * @param control Setting control
         * @param override Override the existing 
         * control, even if one has been set 
         * already. Note: will override the 
         * setting even if it's not marked as 
         * custom.
         * @returns Successful result on success, 
         * errorful result with info on error
         */
        Result<> setCustomSetting(
            std::string const& key,
            Setting* control,
            bool override = false
        );

        std::vector<Setting*> getSettings() const;
        
        /**
         * Get the mod's save directory 
         * path
         */
        ghc::filesystem::path getSaveDir() const;

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
    };
}
