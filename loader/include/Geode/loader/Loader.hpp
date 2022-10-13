#pragma once

#include <Geode/DefaultInclude.hpp>
#include "Types.hpp"
#include <string_view>
#include <vector>
#include <string>
#include <unordered_map>
#include <Geode/utils/Result.hpp>
#include <functional>
#include <unordered_set>
#include <fs/filesystem.hpp>
#include "Log.hpp"

namespace geode {
    #pragma warning(disable: 4251)

    static constexpr std::string_view GEODE_DIRECTORY          = "geode";
    static constexpr std::string_view GEODE_MOD_DIRECTORY      = "mods";
    static constexpr std::string_view GEODE_LOG_DIRECTORY      = "log";
    static constexpr std::string_view GEODE_RESOURCE_DIRECTORY = "resources";
    static constexpr std::string_view GEODE_TEMP_DIRECTORY     = "temp";
    static constexpr std::string_view GEODE_MOD_EXTENSION      = ".geode";

    class Mod;
    class Hook;
    struct ModInfo;
    class VersionInfo;

    namespace modifier {
    	template<class, class, class>
		class FieldIntermediate;
    }
}

/**
 * The predeclaration of the implicit entry
 */
GEODE_API bool GEODE_CALL geode_implicit_load(geode::Mod*);

namespace geode {

    class GEODE_DLL Loader {
    public:
        struct FailedModInfo {
            std::string m_file;
            std::string m_reason;
        };

    protected:
        struct LoaderSettings {
            struct ModSettings {
                bool m_enabled = true;
            };
            std::unordered_map<std::string, ModSettings> m_mods;
        };

        using ScheduledFunction = std::function<void GEODE_CALL(void)>;
        
        std::vector<ScheduledFunction> m_scheduledFunctions;
        std::unordered_map<std::string, Mod*> m_mods;
        std::vector<log::Log> m_logs;
        std::ofstream m_logStream;
        std::vector<ghc::filesystem::path> m_modDirectories;
        std::vector<FailedModInfo> m_erroredMods;
        LoaderSettings m_loadedSettings;

        bool m_isSetup = false;
        static bool s_unloading;

        Result<std::string> createTempDirectoryForMod(ModInfo const& info);
        Result<Mod*> loadModFromFile(std::string const& file);
        size_t loadModsFromDirectory(
            ghc::filesystem::path const& path, bool recursive
        );
        void createDirectories();

        void updateAllDependencies();

        void releaseScheduledFunctions(Mod* mod);

        friend class Mod;
        friend class CustomLoader;
        friend struct ModInfo;

    	size_t getFieldIndexForClass(size_t hash);

    	template <class, class, class>
        friend class modifier::FieldIntermediate;

        friend bool GEODE_CALL ::geode_implicit_load(Mod*);
        
    public:
        ~Loader();

        /**
         * Get the shared Loader instance
         * @returns Shared loader instance
         */
        static Loader* get();
        
        static VersionInfo getVersion();
        static std::string getVersionType();

        Result<> saveSettings();
        Result<> loadSettings();
        
        bool didLastLaunchCrash() const;
        ghc::filesystem::path getCrashLogDirectory() const;

        /**
         * Directory where Geometry Dash is
         */
        ghc::filesystem::path getGameDirectory() const;
        /**
         * Directory where GD saves its files
         */
        ghc::filesystem::path getSaveDirectory() const;
        /**
         * Directory where Geode is
         */
        ghc::filesystem::path getGeodeDirectory() const;
        /**
         * Directory where Geode saves its files
         */
        ghc::filesystem::path getGeodeSaveDirectory() const;

        /**
         * Minimum supported mod version
         */
        static VersionInfo minModVersion();
        /**
         * Maximum supported mod version
         */
        static VersionInfo maxModVersion();
        /**
         * Check if a mod's version is within the supported range
         */
        static bool supportedModVersion(VersionInfo const& version);

        /**
         * Whether mod specified with ID is enabled 
         * @param id The ID of the mod
         */
        bool shouldLoadMod(std::string const& id) const;

        /**
         * Set up the Loader.
         * @returns True if setup was succesful or 
         * has been done before, false if an error 
         * occurred
         */
        bool setup();

        /**
         * Refresh the mods list. Scans all search 
         * directories again for unloaded mods
         * @returns Amount of new mods loaded
         */
        size_t refreshMods();

        /**
         * Returns true if the Loader is unloading / 
         * currently unloaded. Used for threading; 
         * should be thread-safe
         * @returns True if the loader is unloading / 
         * unloaded, false otherwise
         */
        static bool isUnloading();

        void pushLog(log::Log&& log);
        void popLog(log::Log* log);
        std::vector<log::Log*> getLogs(
            std::initializer_list<Severity> severityFilter = {}
        );

        void clearLogs();
        
        /**
         * You shouldn't be calling this manually, 
         * but if you are, make sure to set 
         * Mod::m_addResourcesToSearchPath to true 
         * first
         */
        void updateResourcePaths();
        void updateModResources(Mod* mod);
        void updateResources();

        /**
         * Check if a mod with an ID is installed. Any 
         * valid .geode file in the mods directory will 
         * be listed as installed
         * @param id The ID of the mod
         * @returns True if the mod is installed
         */
        bool isModInstalled(std::string const& id) const;
        /**
         * Get an installed mod by its ID
         * @param id The ID of the mod
         * @returns Pointer to Mod if it was found, or nullptr if 
         * the mod is not installed
         */
        Mod* getInstalledMod(std::string const& id) const;
        /**
         * Check if a mod with an ID is loaded
         * @param id The ID of the mod
         * @returns True if the mod was found, or false if 
         * the mod is not loaded nor installed
         */
        bool isModLoaded(std::string const& id) const;
        /**
         * Get a loaded mod by its ID
         * @param id The ID of the mod
         * @returns Pointer to Mod if it was found, or nullptr if 
         * the mod is not loaded nor installed
         */
        Mod* getLoadedMod(std::string const& id) const;
        /**
         * Get a list of all installed mods
         * @returns List of all installed mods
         */
        std::vector<Mod*> getAllMods() const;
        /**
         * Get all mods that are a serious 
         * disappointment to their parents
         */
        std::vector<FailedModInfo> getFailedMods() const;
        /**
         * Unload a mod fully. This will remove it 
         * from the mods list and delete the Mod. If 
         * the mod does not properly handle unloading, 
         * this function may cause a crash; Use with 
         * caution!
         */
        void unloadMod(Mod* mod);

        /**
         * Get Geode's internal representation. Use with 
         * caution!
         * @returns Pointer to InternalMod
         */
        static Mod* getInternalMod();

        /**
         * Run a function in the GD thread. Useful if you're 
         * doing logic in another thread and need to interact 
         * with GUI. The function will be run the next time 
         * `CCScheduler::update` is called
         * @param func Function to run
         */
        void queueInGDThread(ScheduledFunction func);

        /**
         * Run a function when the Mod is loaded. Useful if for 
         * some reason you need to run some function in 
         * static initialization.
         * @param func Function to run
         */
        void scheduleOnModLoad(Mod* m, ScheduledFunction func);

        /**
         * Open the platform-specific external console (if one exists)
         */
        static void openPlatformConsole();
        /**
         * Close the platform-specific external console (if one exists)
         */
        static void closePlatfromConsole();
    };
}
