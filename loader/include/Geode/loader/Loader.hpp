#pragma once

#include <ghc/fs_fwd.hpp>
#include "../utils/Result.hpp"
#include "../utils/MiniFunction.hpp"
#include "Log.hpp"
#include "ModInfo.hpp"
#include "Types.hpp"

#include <atomic>
#include <mutex>

namespace geode {
    using ScheduledFunction = utils::MiniFunction<void()>;

    struct InvalidGeodeFile {
        ghc::filesystem::path path;
        std::string reason;
    };

    class LoaderImpl;

    class GEODE_DLL Loader {
    private:
        class Impl;
        std::unique_ptr<Impl> m_impl;
        Loader();
        ~Loader();

    protected:
        void createDirectories();

        void updateModResources(Mod* mod);
        void addSearchPaths();

        void dispatchScheduledFunctions(Mod* mod);
        friend void GEODE_CALL ::geode_implicit_load(Mod*);

        Result<Mod*> loadModFromInfo(ModInfo const& info);
        
        Mod* takeNextMod();

    public:
        // TODO: do we want to expose all of these functions?
        static Loader* get();

        Result<> saveData();
        Result<> loadData();

        VersionInfo getVersion();
        VersionInfo minModVersion();
        VersionInfo maxModVersion();
        bool isModVersionSupported(VersionInfo const& version);

        Result<Mod*> loadModFromFile(ghc::filesystem::path const& file);
        void loadModsFromDirectory(ghc::filesystem::path const& dir, bool recursive = true);
        void refreshModsList();
        bool isModInstalled(std::string const& id) const;
        Mod* getInstalledMod(std::string const& id) const;
        bool isModLoaded(std::string const& id) const;
        Mod* getLoadedMod(std::string const& id) const;
        std::vector<Mod*> getAllMods();
        Mod* getModImpl();
        void updateAllDependencies();
        std::vector<InvalidGeodeFile> getFailedMods() const;

        void updateResources();
        void updateResources(bool forceReload);

        void queueInGDThread(ScheduledFunction func);
        void waitForModsToBeLoaded();

        /**
         * Open the platform-specific external console (if one exists)
         */
        void openPlatformConsole();
        /**
         * Close the platform-specific external console (if one exists)
         */
        void closePlatformConsole();

        bool didLastLaunchCrash() const;

        bool userTriedToLoadDLLs() const;

        friend class LoaderImpl;

        friend Mod* takeNextLoaderMod();
    };

    /**
     * @brief Take the next mod to load
     * 
     * @return Mod* The next mod to load
    */
    inline GEODE_HIDDEN Mod* takeNextLoaderMod() {
        return Loader::get()->takeNextMod();
    }
}
