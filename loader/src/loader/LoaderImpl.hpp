#include "FileWatcher.hpp"

#include <Geode/external/json/json.hpp>
#include <Geode/loader/Dirs.hpp>
#include <Geode/loader/Index.hpp>
#include <Geode/loader/Loader.hpp>
#include <Geode/loader/Log.hpp>
#include <Geode/loader/Mod.hpp>
#include <Geode/utils/Result.hpp>
#include <Geode/utils/map.hpp>
#include <Geode/utils/ranges.hpp>
#include "ModImpl.hpp"
#include <about.hpp>
#include <crashlog.hpp>
#include <mutex>
#include <optional>
#include <thread>
#include <unordered_map>
#include <unordered_set>
#include <vector>

// TODO: Find a file convention for impl headers
namespace geode {
    struct ResourceDownloadEvent : public Event {
        const UpdateStatus status;
        ResourceDownloadEvent(UpdateStatus const& status);
    };

    class GEODE_DLL ResourceDownloadFilter : public EventFilter<ResourceDownloadEvent> {
    public:
        using Callback = void(ResourceDownloadEvent*);

        ListenerResult handle(std::function<Callback> fn, ResourceDownloadEvent* event);
        ResourceDownloadFilter();
    };

    class Loader::Impl {
    public:
        mutable std::mutex m_mutex;

        std::vector<ghc::filesystem::path> m_modSearchDirectories;
        std::vector<ModInfo> m_modsToLoad;
        std::vector<InvalidGeodeFile> m_invalidMods;
        std::unordered_map<std::string, Mod*> m_mods;
        std::vector<ghc::filesystem::path> m_texturePaths;
        bool m_isSetup = false;

        std::condition_variable m_earlyLoadFinishedCV;
        std::mutex m_earlyLoadFinishedMutex;
        std::atomic_bool m_earlyLoadFinished = false;
        std::vector<std::function<void(void)>> m_gdThreadQueue;
        mutable std::mutex m_gdThreadMutex;
        bool m_platformConsoleOpen = false;
        std::vector<std::pair<Hook*, Mod*>> m_internalHooks;
        bool m_readyToHook = false;

        std::mutex m_nextModMutex;
        std::unique_lock<std::mutex> m_nextModLock = std::unique_lock<std::mutex>(m_nextModMutex, std::defer_lock);
        std::condition_variable m_nextModCV;
        std::mutex m_nextModAccessMutex;
        Mod* m_nextMod = nullptr;

        void provideNextMod(Mod* mod);
        Mod* takeNextMod();
        void releaseNextMod();

        void downloadLoaderResources();

        bool loadHooks();
        void setupIPC();

        Impl();
        ~Impl();

        void createDirectories();

        void updateModResources(Mod* mod);
        void addSearchPaths();

        friend void GEODE_CALL ::geode_implicit_load(Mod*);

        Result<Mod*> loadModFromInfo(ModInfo const& info);

        Result<> setup();
        void reset();

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

        void waitForModsToBeLoaded();

        bool didLastLaunchCrash() const;

        nlohmann::json processRawIPC(void* rawHandle, std::string const& buffer);

        void queueInGDThread(ScheduledFunction func);
        void executeGDThreadQueue();

        void logConsoleMessage(std::string const& msg);
        bool platformConsoleOpen() const;
        void openPlatformConsole();
        void closePlatformConsole();
        void platformMessageBox(char const* title, std::string const& info);

        bool verifyLoaderResources();

        bool isReadyToHook() const;
        void addInternalHook(Hook* hook, Mod* mod);

        void setupInternalMod();
    };

    class LoaderImpl {
    public:
        static Loader::Impl* get();
    };
}
