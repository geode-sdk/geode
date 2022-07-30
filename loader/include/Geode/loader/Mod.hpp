#pragma once

#include <Geode/DefaultInclude.hpp>
#include "Types.hpp"
#include "Hook.hpp"
#include "../utils/types.hpp"
#include "../utils/Result.hpp"
#include "../utils/VersionInfo.hpp"
#include <Geode/utils/json.hpp>
#include <string_view>
#include <vector>
#include <unordered_map>
#include <type_traits>
#include <cocos2d.h>


class InternalLoader;
class InternalMod;

namespace geode {                  
    struct PlatformInfo;

    class Hook;
    class Patch;
    class Loader;
    class Log;
    class Mod;

    class Unknown;
	using unknownmemfn_t = void(Unknown::*)();
	using unknownfn_t = void(*)();

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
    struct GEODE_DLL ModInfo {
        /**
         * Path to the mod file
         */
        ghc::filesystem::path m_path;
        /**
         * Name of the platform binary within 
         * the mod zip
         */
        std::string m_binaryName = GEODE_WINDOWS("mod.dll")
                                   GEODE_MACOS("mod.dylib")
                                   GEODE_IOS("mod.dylib")
                                   GEODE_ANDROID("mod.so");
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
         * "HJfod" or "The Geode Team".
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
         * Detailed description of the mod, writtenin Markdown (see
         * https://github.com/geode-sdk/api/blob/main/include/nodes/MDTextArea.hpp) 
         * for more info
         */
        std::string m_details = "";
        /**
         * Changelog for the mod, written in Markdown (see
         * https://github.com/geode-sdk/api/blob/main/include/nodes/MDTextArea.hpp) 
         * for more info
         */
        std::string m_changelog = "";
        /**
         * Git Repository of the mod.
         */
        std::string m_repository = "";
        /**
         * Dependencies
         */
        std::vector<Dependency> m_dependencies;
        /**
         * Mod spritesheet names
         */
        std::vector<std::string> m_spritesheets;
        /**
         * Default data store values
         */
        nlohmann::json m_defaultDataStore;

        /**
         * Whether the mod can be disabled or not
         */
        bool m_supportsDisabling = true;
        /**
         * Whether the mod can be unloaded or not
         */
        bool m_supportsUnloading = false;
        /**
         * Create ModInfo from a .geode package
         */
        static Result<ModInfo> createFromGeodeFile(ghc::filesystem::path const& path);
        /**
         * Create ModInfo from a mod.json file
         */
        static Result<ModInfo> createFromFile(ghc::filesystem::path const& path);
        /**
         * Create ModInfo from a parsed json document
         */
        static Result<ModInfo> create(nlohmann::json const& json);

    private:
        /**
         * Version is passed for backwards 
         * compatibility if we update the mod.json 
         * format
         */
        static Result<ModInfo> createFromSchemaV010(nlohmann::json const& json);
    };

    /**
     * @class DataStore
     * Internal class for notifying Mod
     * when the datastore changes
     */
    class GEODE_DLL DataStore {
        nlohmann::json m_store;
        Mod* m_mod;

        DataStore(Mod* m, nlohmann::json& j) : m_mod(m), m_store(j) {}
        
        friend class Mod;

    public:
        ~DataStore();

        nlohmann::json& getJson() const;
        nlohmann::json& operator[](std::string const&);
        DataStore& operator=(nlohmann::json&);
        bool contains(std::string const&) const;
        operator nlohmann::json();

    };

    /**
     * @class Mod
     * Represents a Mod ingame. Inherit
     * from this class to create your own
     * mod interfaces.
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
        ghc::filesystem::path m_saveDirPath;
        /**
         * Pointers to mods that depend on 
         * this Mod. Makes it possible to 
         * enable / disable them automatically, 
         * when their dependency is disabled.
         */
        std::vector<Mod*> m_parentDependencies;
        /**
         * Pointer to the Mod's implicit load function
         */
        geode_load m_implicitLoadFunc = nullptr;
        /**
         * Pointer to the Mod's load function
         */
        geode_load m_loadFunc = nullptr;
        /**
         * Pointer to the Mod's unload function
         */
        geode_unload m_unloadFunc = nullptr;
        /**
         * Pointer to the Mod's enable function
         */
        geode_enable m_enableFunc = nullptr;
        /**
         * Pointer to the Mod's enable function
         */
        geode_disable m_disableFunc = nullptr;
        geode_load_data m_loadDataFunc = nullptr;
        geode_save_data m_saveDataFunc = nullptr;
        geode_setting_updated m_settingUpdatedFunc = nullptr;
        /**
         * Whether temp/<mod id>/resources should be 
         * added to CCFileUtils search paths
         */
        bool m_addResourcesToSearchPath = false;
        /**
         * Error info in case loading failed
         */
        std::string m_loadErrorInfo = "";

        /**
         * Data Store object
         */
        nlohmann::json m_dataStore;

        /**
         * Load the platform binary
         */
        Result<> loadPlatformBinary();
        Result<> unloadPlatformBinary();

        Result<> saveDataStore();
        Result<> loadDataStore();

        void postDSUpdate();

        Result<> createTempDir();

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

        friend class ::InternalMod;
        friend class Loader;
        friend class ::InternalLoader;
        friend struct ModInfo;
        friend class DataStore;

    public:
        std::string getID() const;
        std::string getName() const;
        std::string getDeveloper() const;
        std::string getDescription() const;
        std::string getDetails() const;
        std::string getPath() const;
        VersionInfo getVersion() const;
        bool        isEnabled() const;
        bool        isLoaded() const;
        bool        supportsDisabling() const;
        bool        supportsUnloading() const;
        bool        wasSuccesfullyLoaded() const;
        std::string getLoadErrorInfo() const;
        ModInfo     getModInfo() const;
        ghc::filesystem::path getTempDir() const;
        ghc::filesystem::path getBinaryPath() const;

        /**
         * Get the mod container stored in the Interface
         * @returns nullptr if Interface is not initialized,
         * the mod pointer if it is initialized
         */
        template<class = void>
        static inline Mod* get();

        /**
         * Log to geode's integrated console / 
         * the platform debug console.
         * @returns Reference to log stream. Make sure 
         * to end your logging with geode::endl.
         */
        Log log();

        /**
         * Log an information. Equivalent to 
         * ```
         * Mod::log() << Severity::severity << info.
         * ```
         * @param info Log infomration
         * @param severity Log severity
         */
        void logInfo(
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
        template<auto Detour, template <class, class...> class Convention>
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
		template<auto Detour, template <class, class...> class Convention>
        Result<Hook*> addHook(std::string const& displayName, void* address) {
		    auto hook = Hook::create<Detour, Convention>((decltype(Detour))address, displayName, this);
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
         * Enable this mod
         * @returns Successful result on success, 
         * errorful result with info on error
         */
        Result<> enable();
        
        /**
         * Disable this mod if it supports doing so
         * @returns Successful result on success, 
         * errorful result with info on error
         */
        Result<> disable();

        /**
         * Disable & unload this mod (if supported), 
         * then delete the mod's .geode file.
         * @returns Successful result on success, 
         * errorful result with info on error
         */
        Result<> uninstall();
        bool isUninstalled() const;
        
        /**
         * Get the mod's save directory 
         * path
         */
        ghc::filesystem::path getSaveDir() const;

        /**
         * Return the data store object
         * @returns DataStore object
         * store
         */
        DataStore getDataStore();

        /**
         * Reset the data store to the
         * default value
         */
        Result<> resetDataStore();


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

        template<class T>
        T* with() {
            return reinterpret_cast<T*>(this);
        }

		const char* expandSpriteName(const char* name);
    };
}
