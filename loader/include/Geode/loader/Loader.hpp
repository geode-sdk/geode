#pragma once

#include <ghc/fs_fwd.hpp>
#include "../utils/Result.hpp"
#include "../utils/MiniFunction.hpp"
#include "Log.hpp"
#include "ModEvent.hpp"
#include "ModInfo.hpp"
#include "ModMetadata.hpp"
#include "Types.hpp"

#include <atomic>
#include <mutex>

namespace geode {
    using ScheduledFunction = utils::MiniFunction<void()>;

    struct InvalidGeodeFile {
        ghc::filesystem::path path;
        std::string reason;
    };

    struct LoadProblem {
        enum class Type : uint8_t {
            Unknown,
            Suggestion,
            Recommendation,
            Conflict,
            InvalidFile,
            Duplicate,
            SetupFailed,
            LoadFailed,
            EnableFailed,
            MissingDependency,
            PresentIncompatibility
        };
        Type type;
        std::variant<ghc::filesystem::path, ModMetadata, Mod*> cause;
        std::string message;
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

        [[deprecated]] Result<Mod*> loadModFromInfo(ModInfo const& info);

        Mod* takeNextMod();

    public:
        // TODO: do we want to expose all of these functions?
        static Loader* get();

        enum class LoadingState : uint8_t {
            None,
            Queue,
            List,
            Graph,
            EarlyMods,
            Mods,
            Problems,
            Done
        };

        Result<> saveData();
        Result<> loadData();

        VersionInfo getVersion();
        VersionInfo minModVersion();
        VersionInfo maxModVersion();
        bool isModVersionSupported(VersionInfo const& version);

        [[deprecated]] Result<Mod*> loadModFromFile(ghc::filesystem::path const& file);
        [[deprecated]] void loadModsFromDirectory(ghc::filesystem::path const& dir, bool recursive = true);
        [[deprecated]] void refreshModsList();
        LoadingState getLoadingState();
        bool isModInstalled(std::string const& id) const;
        Mod* getInstalledMod(std::string const& id) const;
        bool isModLoaded(std::string const& id) const;
        Mod* getLoadedMod(std::string const& id) const;
        std::vector<Mod*> getAllMods();
        [[deprecated("use Mod::get instead")]] Mod* getModImpl();
        [[deprecated]] void updateAllDependencies();
        [[deprecated("use getProblems instead")]] std::vector<InvalidGeodeFile> getFailedMods() const;
        std::vector<LoadProblem> getProblems() const;

        void updateResources();
        void updateResources(bool forceReload);

        [[deprecated("use queueInMainThread instead")]] void queueInGDThread(ScheduledFunction func);
        void queueInMainThread(ScheduledFunction func);
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
