#pragma once

#include "FileWatcher.hpp"

#include <json.hpp>
#include <Geode/loader/Dirs.hpp>
#include <Geode/loader/Index.hpp>
#include <Geode/loader/Loader.hpp>
#include <Geode/loader/Log.hpp>
#include <Geode/loader/Mod.hpp>
#include <Geode/utils/Result.hpp>
#include <Geode/utils/map.hpp>
#include <Geode/utils/ranges.hpp>
#include <Geode/utils/MiniFunction.hpp>
#include "ModImpl.hpp"
#include <crashlog.hpp>
#include <mutex>
#include <optional>
#include <thread>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <queue>
#include <tulip/TulipHook.hpp>

// TODO: Find a file convention for impl headers
namespace geode {
    struct ResourceDownloadEvent : public Event {
        const UpdateStatus status;
        ResourceDownloadEvent(UpdateStatus const& status);
    };

    class ResourceDownloadFilter : public EventFilter<ResourceDownloadEvent> {
    public:
        using Callback = void(ResourceDownloadEvent*);

        ListenerResult handle(utils::MiniFunction<Callback> fn, ResourceDownloadEvent* event);
        ResourceDownloadFilter();
    };

    struct LoaderUpdateEvent : public Event {
        const UpdateStatus status;
        LoaderUpdateEvent(UpdateStatus const& status);
    };

    class LoaderUpdateFilter : public EventFilter<LoaderUpdateEvent> {
    public:
        using Callback = void(LoaderUpdateEvent*);

        ListenerResult handle(utils::MiniFunction<Callback> fn, LoaderUpdateEvent* event);
        LoaderUpdateFilter();
    };

    class Loader::Impl {
    public:
        mutable std::mutex m_mutex;

        std::vector<ghc::filesystem::path> m_modSearchDirectories;
        std::vector<LoadProblem> m_problems;
        std::unordered_map<std::string, Mod*> m_mods;
        std::queue<Mod*> m_modsToLoad;
        std::vector<ghc::filesystem::path> m_texturePaths;
        bool m_isSetup = false;

        // cache for the json of the latest github release to avoid hitting 
        // the github api too much
        std::optional<json::Value> m_latestGithubRelease;
        bool m_isNewUpdateDownloaded = false;

        LoadingState m_loadingState;

        std::vector<utils::MiniFunction<void(void)>> m_gdThreadQueue;
        mutable std::mutex m_gdThreadMutex;
        std::vector<std::pair<Hook*, Mod*>> m_internalHooks;
        bool m_readyToHook = false;

        bool m_platformConsoleOpen = false;
        void* m_platformData = nullptr;

        std::mutex m_nextModMutex;
        std::unique_lock<std::mutex> m_nextModLock = std::unique_lock<std::mutex>(m_nextModMutex, std::defer_lock);
        std::condition_variable m_nextModCV;
        std::mutex m_nextModAccessMutex;
        Mod* m_nextMod = nullptr;

        void provideNextMod(Mod* mod);
        Mod* takeNextMod();
        void releaseNextMod();

        std::unordered_map<void*, tulip::hook::HandlerHandle> m_handlerHandles;

        Result<> createHandler(void* address, tulip::hook::HandlerMetadata const& metadata);
        bool hasHandler(void* address);
        Result<tulip::hook::HandlerHandle> getHandler(void* address);
        Result<> removeHandler(void* address);

        void updateSpecialFiles();
        void tryDownloadLoaderResources(std::string const& url, bool tryLatestOnError = true);
        void downloadLoaderResources(bool useLatestRelease = false);
        void downloadLoaderUpdate(std::string const& url);
        void fetchLatestGithubRelease(
            utils::MiniFunction<void(json::Value const&)> then,
            utils::MiniFunction<void(std::string const&)> expect
        );

        bool loadHooks();
        void setupIPC();

        Impl();
        ~Impl();

        void createDirectories();

        void updateModResources(Mod* mod);
        void addSearchPaths();

        friend void GEODE_CALL ::geode_implicit_load(Mod*);

        [[deprecated]] Result<Mod*> loadModFromInfo(ModInfo const& info);

        Result<> setup();
        void forceReset();

        Result<> saveData();
        Result<> loadData();

        VersionInfo getVersion();
        VersionInfo minModVersion();
        VersionInfo maxModVersion();
        bool isModVersionSupported(VersionInfo const& version);

        [[deprecated]] Result<Mod*> loadModFromFile(ghc::filesystem::path const& file);
        [[deprecated]] void loadModsFromDirectory(ghc::filesystem::path const& dir, bool recursive = true);
        [[deprecated]] void refreshModsList();
        void queueMods(std::vector<ModMetadata>& modQueue);
        void populateModList(std::vector<ModMetadata>& modQueue);
        void buildModGraph();
        void loadModGraph(Mod* node, bool early);
        void findProblems();
        void refreshModGraph();
        void continueRefreshModGraph();

        bool isModInstalled(std::string const& id) const;
        Mod* getInstalledMod(std::string const& id) const;
        bool isModLoaded(std::string const& id) const;
        Mod* getLoadedMod(std::string const& id) const;
        std::vector<Mod*> getAllMods();
        [[deprecated]] Mod* getModImpl();
        [[deprecated]] void updateAllDependencies();
        [[deprecated]] std::vector<InvalidGeodeFile> getFailedMods() const;
        std::vector<LoadProblem> getProblems() const;

        void updateResources();
        void updateResources(bool forceReload);

        void waitForModsToBeLoaded();

        bool didLastLaunchCrash() const;

        json::Value processRawIPC(void* rawHandle, std::string const& buffer);

        void queueInMainThread(ScheduledFunction func);
        void executeGDThreadQueue();

        void logConsoleMessage(std::string const& msg);
        void logConsoleMessageWithSeverity(std::string const& msg, Severity severity);

        bool platformConsoleOpen() const;
        void openPlatformConsole();
        void closePlatformConsole();
        void platformMessageBox(char const* title, std::string const& info);

        bool verifyLoaderResources();
        void checkForLoaderUpdates();
        bool isNewUpdateDownloaded() const;

        bool isReadyToHook() const;
        void addInternalHook(Hook* hook, Mod* mod);

        Mod* createInternalMod();
        Result<> setupInternalMod();

        bool userTriedToLoadDLLs() const;
    };

    class LoaderImpl : public Loader::Impl {
    public:
        static Loader::Impl* get();
    };
}
