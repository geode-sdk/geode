#pragma once

#include "Types.hpp"
#include "Log.hpp"
#include "../external/filesystem/fs/filesystem.hpp"
#include <mutex>
#include <atomic>
#include "../utils/Result.hpp"
#include "ModInfo.hpp"

namespace geode {
    using ScheduledFunction = std::function<void GEODE_CALL(void)>;

    struct InvalidGeodeFile {
        ghc::filesystem::path m_path;
        std::string m_reason;
    };

    class GEODE_DLL Loader {
    protected:
        std::vector<ghc::filesystem::path> m_modSearchDirectories;
        std::vector<ModInfo> m_modsToLoad;
        std::vector<InvalidGeodeFile> m_invalidMods;
        std::unordered_map<std::string, Mod*> m_mods;
        std::vector<ghc::filesystem::path> m_texturePaths;
        std::vector<ScheduledFunction> m_scheduledFunctions;
        mutable std::mutex m_scheduledFunctionsMutex;
        bool m_isSetup = false;
        std::atomic_bool m_earlyLoadFinished = false;

        void createDirectories();

        void updateModResources(Mod* mod);
        void addSearchPaths();

        void dispatchScheduledFunctions(Mod* mod);
        friend void GEODE_CALL ::geode_implicit_load(Mod*);

        Result<Mod*> loadModFromInfo(ModInfo const& info);

    public:
        ~Loader();
        static Loader* get();

        Result<> setup();

        Result<> saveData();
        Result<> loadData();

        static VersionInfo getVersion();
        static VersionInfo minModVersion();
        static VersionInfo maxModVersion();
        static bool isModVersionSupported(VersionInfo const& version);

        Result<Mod*> loadModFromFile(ghc::filesystem::path const& file);
        Result<> loadModsFromDirectory(
            ghc::filesystem::path const& dir,
            bool recursive = true
        );
        Result<> refreshModsList();
        bool isModInstalled(std::string const& id) const;
        Mod* getInstalledMod(std::string const& id) const;
        bool isModLoaded(std::string const& id) const;
        Mod* getLoadedMod(std::string const& id) const;
        std::vector<Mod*> getAllMods();
        static Mod* getInternalMod();
        void updateAllDependencies();
        std::vector<InvalidGeodeFile> getFailedMods() const;

        void updateResources();

        void queueInGDThread(ScheduledFunction func);
        void scheduleOnModLoad(Mod* mod, ScheduledFunction func);
        void waitForModsToBeLoaded();
        
        /**
         * Open the platform-specific external console (if one exists)
         */
        static void openPlatformConsole();
        /**
         * Close the platform-specific external console (if one exists)
         */
        static void closePlatfromConsole();

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
    };
}
