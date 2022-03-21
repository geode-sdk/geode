#pragma once

#include "Macros.hpp"
#include "Types.hpp"
#include <string_view>
#include <vector>
#include <string>
#include <unordered_map>
#include <utils/Result.hpp>
#include <functional>
#include <unordered_set>
#include <fs/filesystem.hpp>
#include <Log.hpp>

class Geode;

namespace geode {
    #pragma warning(disable: 4251)

    static constexpr const std::string_view geodeDirectory          = "geode";
    static constexpr const std::string_view geodeModDirectory      = "mods";
    static constexpr const std::string_view geodeLogDirectory      = "log";
    static constexpr const std::string_view geodeResourceDirectory = "resources";
    static constexpr const std::string_view geodeTempDirectory     = "temp";
    static constexpr const std::string_view geodeModExtension      = ".geode";

    class Mod;
    class Hook;
    struct ModInfo;
    class Log;
    class LogPtr;

    class GEODE_DLL Loader {
    public:
        struct UnloadedModInfo {
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
        
        std::unordered_map<std::string, Mod*> m_mods;
        std::vector<LogPtr*> m_logs;
        std::ofstream m_logStream;
        std::vector<ghc::filesystem::path> m_modDirectories;
        std::vector<UnloadedModInfo> m_erroredMods;
        LoaderSettings m_loadedSettings;
        bool m_isSetup = false;
        static bool s_unloading;

        /**
         * Lowest supported mod version.
         * Any mod targeting a geode version 
         * lower than this will not be loaded, 
         * as they will be considered out-of-date.
         */
        static constexpr const int s_supportedSchemaMin = 1;
        /**
         * Highest support mod version.
         * Any mod targeting a geode version 
         * higher than this will not be loaded, 
         * as a higher version means that 
         * the user's geode is out-of-date, 
         * or that the user is a time traveller 
         * and has downloaded a mod from the 
         * future.
         */
        static constexpr const int s_supportedSchemaMax = 1;

        /**
         * This function is to avoid ridiculous 
         * indentation in `checkMetaInformation`
         * 
         * The json parameter is void* because 
         * I don't want to force-include json 
         * for every geode user and forward-
         * declaring the template-full basic_json 
         * class looks horrifying
         * 
         * This function is only used in one place 
         * anyway, only by me who knows how to 
         * use it, so who cares
         */
        template<int Schema>
        Result<Mod*> checkBySchema(std::string const& path, void* json);

        Result<std::string> createTempDirectoryForMod(ModInfo const& info);
        Result<Mod*> loadModFromFile(std::string const& file);
        void createDirectories();

        void updateAllDependencies();

        friend class Mod;
        friend class CustomLoader;
        friend class Geode;
        friend struct ModInfo;
        
    public:
        Loader();
        virtual ~Loader();

        Result<> saveSettings();
        Result<> loadSettings();

        bool shouldLoadMod(std::string const& id) const;
        std::vector<UnloadedModInfo> const& getFailedMods() const;

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
         * Get the shared Loader instance
         * @returns Shared loader instance
         */
        static Loader* get();

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

        void pushLog(LogPtr* log);
        void popLog(LogPtr* log);
        std::vector<LogPtr*> const& getLogs() const;
        std::vector<LogPtr*> getLogs(
            std::initializer_list<Severity> severityFilter
        );
        
        /**
         * You shouldn't be calling this manually, 
         * but if you are, make sure to set 
         * Mod::m_addResourcesToSearchPath to true 
         * first
         */
        void addModResourcesPath(Mod* mod);
        void updateResourcePaths();
        void updateModResources(Mod* mod);
        void updateResources();

        /**
         * Check if a mod with an ID has been loaded
         * @param id The ID of the mod
         * @param resolved Whether the mod's dependencies have to 
         * be resolved to be returned. If set to true and the mod 
         * is unresolved, this function returns false
         * @returns True if the mod was found, or false if 
         * the mod is not loaded or it was unresolved and the `resolved` 
         * parameter was set to true
         */
        bool isModLoaded(std::string const& id, bool resolved = true) const;
        /**
         * Get a loaded mod by its ID
         * @param id The ID of the mod
         * @param resolved Whether the mod's dependencies have to 
         * be resolved to be returned. If set to true and the mod 
         * is unresolved, this function returns nullptr
         * @returns Pointer to Mod if it was found, or nullptr if 
         * the mod is not loaded or it was unresolved and the `resolved` 
         * parameter was set to true
         */
        Mod* getLoadedMod(std::string const& id, bool resolved = true) const;
        /**
         * Get a list of all loaded mods
         * @returns List of all loaded mods
         */
        std::vector<Mod*> getLoadedMods(bool resolved = false) const;
        /**
         * Get the count of loaded mods
         * @returns Tuple where the first element is the count of 
         * all loaded mods and second element is the count of 
         * unresolved mods
         */
        std::tuple<size_t, size_t> getLoadedModCount() const;
        /**
         * Unload a mod fully. This will remove it 
         * from the mods list and delete the Mod. Use 
         * with caution!
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
        void queueInGDThread(std::function<void GEODE_CALL(void)> func);
    };

}
