#pragma once

#include "FileWatcher.hpp"

#include <matjson.hpp>
#include <Geode/loader/Dirs.hpp>
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
    static constexpr std::string_view LAUNCH_ARG_PREFIX = "--geode:";

    class Loader::Impl {
    public:
        mutable std::mutex m_mutex;

        std::string m_gdVersion;
        std::optional<bool> m_forwardCompatMode;

        std::vector<std::filesystem::path> m_modSearchDirectories;
        std::vector<LoadProblem> m_problems;
        std::unordered_map<std::string, Mod*> m_mods;
        std::deque<Mod*> m_modsToLoad;
        std::vector<std::filesystem::path> m_texturePaths;
        bool m_isSetup = false;

        LoadingState m_loadingState = LoadingState::None;

        std::vector<utils::MiniFunction<void(void)>> m_mainThreadQueue;
        mutable std::mutex m_mainThreadMutex;
        std::vector<std::pair<Hook*, Mod*>> m_uninitializedHooks;
        bool m_readyToHook = false;

        std::mutex m_nextModMutex;
        std::unique_lock<std::mutex> m_nextModLock = std::unique_lock<std::mutex>(m_nextModMutex, std::defer_lock);
        std::condition_variable m_nextModCV;
        std::mutex m_nextModAccessMutex;
        Mod* m_nextMod = nullptr;

        Mod* m_currentlyLoadingMod = nullptr;

        int m_refreshingModCount = 0;
        int m_refreshedModCount = 0;
        int m_lateRefreshedModCount = 0;

        std::unordered_map<std::string, std::string> m_launchArgs;

        std::chrono::time_point<std::chrono::high_resolution_clock> m_timerBegin;

        std::string getGameVersion();
        bool isForwardCompatMode();

        void provideNextMod(Mod* mod);
        Mod* takeNextMod();
        void releaseNextMod();

        std::unordered_map<void*, tulip::hook::HandlerHandle> m_handlerHandles;

        Result<tulip::hook::HandlerHandle> getHandler(void* address);
        Result<tulip::hook::HandlerHandle> getOrCreateHandler(void* address, tulip::hook::HandlerMetadata const& metadata);

        bool loadHooks();

        Impl();
        ~Impl();

        void createDirectories();

        void updateModResources(Mod* mod);
        void addSearchPaths();
        void addNativeBinariesPath(std::filesystem::path const& path);

        Result<> setup();
        void forceReset();

        void saveData();
        void loadData();

        VersionInfo getVersion();
        VersionInfo minModVersion();
        VersionInfo maxModVersion();
        bool isModVersionSupported(VersionInfo const& version);

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
        std::vector<LoadProblem> getProblems() const;

        bool supportsLaunchArguments() const;
        std::string getLaunchCommand() const;
        void initLaunchArguments();
        std::vector<std::string> getLaunchArgumentNames() const;
        bool hasLaunchArgument(std::string_view const name) const;
        std::optional<std::string> getLaunchArgument(std::string_view const name) const;
        bool getLaunchFlag(std::string_view const name) const;

        void updateResources(bool forceReload);

        void queueInMainThread(ScheduledFunction&& func);
        void executeMainThreadQueue();

        bool isReadyToHook() const;
        void addUninitializedHook(Hook* hook, Mod* mod);

        Mod* getInternalMod();
        Result<> setupInternalMod();

        bool userTriedToLoadDLLs() const;

        void addProblem(LoadProblem const& problem);

        bool m_forceSafeMode = false;
        bool isSafeMode() const;
        // enables safe mode, even if the launch arg wasnt provided
        void forceSafeMode();
    };

    class LoaderImpl : public Loader::Impl {
    public:
        static Loader::Impl* get();
    };
}
