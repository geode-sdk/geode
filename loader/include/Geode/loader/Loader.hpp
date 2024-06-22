#pragma once

#include <filesystem>
#include "../utils/Result.hpp"
#include "../utils/MiniFunction.hpp"
#include "Log.hpp"
#include "ModEvent.hpp"
#include "ModMetadata.hpp"
#include "Types.hpp"

#include <atomic>
#include <matjson.hpp>
#include <mutex>
#include <optional>
#include <string_view>

namespace geode {
    using ScheduledFunction = utils::MiniFunction<void()>;

    struct InvalidGeodeFile {
        std::filesystem::path path;
        std::string reason;
    };

    struct LoadProblem {
        enum class Type : uint8_t {
            Unknown,
            Suggestion,
            Recommendation,
            Conflict,
            OutdatedConflict,
            InvalidFile,
            Duplicate,
            SetupFailed,
            LoadFailed,
            EnableFailed,
            MissingDependency,
            PresentIncompatibility,
            UnzipFailed,
            UnsupportedVersion,
            UnsupportedGeodeVersion,
            NeedsNewerGeodeVersion,
            DisabledDependency,
            OutdatedDependency,
            OutdatedIncompatibility,
        };
        Type type;
        std::variant<std::filesystem::path, ModMetadata, Mod*> cause;
        std::string message;
    };

    class LoaderImpl;

    class GEODE_DLL Loader final {
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
        std::vector<LoadProblem> getAllProblems() const;
        std::vector<LoadProblem> getProblems() const;
        std::vector<LoadProblem> getRecommendations() const;

        /**
         * Returns the available launch argument names.
         */
        std::vector<std::string> getLaunchArgumentNames() const;
        /**
         * Returns whether the specified launch argument was passed in via the command line.
         * @param name The argument name
         */
        bool hasLaunchArgument(std::string_view const name) const;
        /**
         * Get a launch argument. These are passed into the game as command-line arguments
         * with the format `--geode:arg-name=value`.
         * @param name The argument name
         * @return The value, if present
         */
        std::optional<std::string> getLaunchArgument(std::string_view const name) const;
        /**
         * Get a launch argument flag. Returns whether the argument is present and its
         * value is exactly `true`.
         * @param name The argument name
         */
        bool getLaunchFlag(std::string_view const name) const;
        /**
         * Get and parse a launch argument value using the setting value system.
         * @param name The argument name
         */
        template <class T>
        std::optional<T> parseLaunchArgument(std::string_view const name) const {
            auto str = this->getLaunchArgument(name);
            if (!str.has_value()) {
                return std::nullopt;
            }
            std::string parseError;
            auto jsonOpt = matjson::parse(str.value(), parseError);
            if (!jsonOpt.has_value()) {
                log::debug("Parsing launch argument '{}' failed: {}", name, parseError);
                return std::nullopt;
            }
            auto value = jsonOpt.value();
            if (!value.is<T>()) {
                return std::nullopt;
            }
            return value.as<T>();
        }

        void queueInMainThread(ScheduledFunction&& func);

        /**
         * Returns the current game version.
         * @return The game version
         */
        std::string getGameVersion();

        friend class LoaderImpl;

        friend Mod* takeNextLoaderMod();
    };

    /**
     * @brief Queues a function to run on the main thread
     * 
     * @param func the function to queue
    */
    inline GEODE_HIDDEN void queueInMainThread(ScheduledFunction&& func) {
        Loader::get()->queueInMainThread(std::forward<ScheduledFunction>(func));
    }

    /**
     * @brief Take the next mod to load
     *
     * @return Mod* The next mod to load
    */
    inline GEODE_HIDDEN Mod* takeNextLoaderMod() {
        return Loader::get()->takeNextMod();
    }
}
