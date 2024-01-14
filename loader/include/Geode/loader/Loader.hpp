#pragma once

#include <ghc/fs_fwd.hpp>
#include "../utils/Result.hpp"
#include "../utils/MiniFunction.hpp"
#include "Log.hpp"
#include "ModEvent.hpp"
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
            PresentIncompatibility,
            UnzipFailed
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
        Mod* takeNextMod();

    public:
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

        bool isForwardCompatMode();

        void saveData();
        void loadData();

        VersionInfo getVersion();
        VersionInfo minModVersion();
        VersionInfo maxModVersion();
        bool isModVersionSupported(VersionInfo const& version);

        LoadingState getLoadingState();
        bool isModInstalled(std::string const& id) const;
        Mod* getInstalledMod(std::string const& id) const;
        bool isModLoaded(std::string const& id) const;
        Mod* getLoadedMod(std::string const& id) const;
        std::vector<Mod*> getAllMods();
        std::vector<LoadProblem> getProblems() const;

        void queueInMainThread(ScheduledFunction func);

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
