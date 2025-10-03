#pragma once

#include <filesystem>
#include <Geode/Result.hpp>
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
    using ScheduledFunction = std::function<void()>;

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

        bool isSuggestion() const {
            return
                type == LoadProblem::Type::Recommendation ||
                type == LoadProblem::Type::Suggestion;
        }
        bool isOutdated() const {
            return
                type == LoadProblem::Type::UnsupportedVersion ||
                type == LoadProblem::Type::NeedsNewerGeodeVersion ||
                type == LoadProblem::Type::UnsupportedGeodeVersion;
        }
        bool isProblem() const {
            return !isSuggestion() && !isOutdated();
        }
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
        std::vector<LoadProblem> getLoadProblems() const;
        std::vector<LoadProblem> getOutdated() const;
        std::vector<LoadProblem> getRecommendations() const;

        /**
         * Returns the available launch argument names.
         */
        std::vector<std::string> getLaunchArgumentNames() const;
        /**
         * Returns whether the specified launch argument was passed in via the command line.
         * @param name The argument name
         */
        bool hasLaunchArgument(std::string_view name) const;
        /**
         * Get a launch argument. These are passed into the game as command-line arguments
         * with the format `--geode:arg-name=value`.
         * @param name The argument name
         * @return The value, if present
         */
        std::optional<std::string> getLaunchArgument(std::string_view name) const;
        /**
         * Get a launch argument flag. Returns whether the argument is present and its
         * value is exactly `true`.
         * @param name The argument name
         */
        bool getLaunchFlag(std::string_view name) const;
        /**
         * Get and parse a launch argument value using the setting value system.
         * @param name The argument name
         */
        template <class T>
        Result<T> parseLaunchArgument(std::string_view name) const {
            auto str = this->getLaunchArgument(name);
            if (!str.has_value()) {
                return Err(fmt::format("Launch argument '{}' not found", name));
            }
            auto jsonOpt = matjson::Value::parse(str.value());
            if (jsonOpt.isErr()) {
                return Err(fmt::format("Parsing launch argument '{}' failed: {}", name, jsonOpt.unwrapErr()));
            }
            auto value = jsonOpt.unwrap();
            return value.template as<T>();
        }

        void queueInMainThread(ScheduledFunction&& func);

        /**
         * Returns the current game version.
         * @return The game version
         */
        std::string getGameVersion();

        /**
         * Returns whether the loader does not use dynamic patching or hooking.
         * You should use GEODE_MOD_STATIC_PATCH macro instead of Mod::patch and
         * GEODE_MOD_STATIC_HOOK macro instead of Mod::hook if that is the case.
         * Modify classes are handled automatically, and enabling/disabling hooks
         * works fine too.
         * @return True if the loader does not use dynamic patching or hooking,
         * false if it does.
         */
        bool isPatchless() const;

        friend class LoaderImpl;

        friend Mod* takeNextLoaderMod();
    };

    /**
     * @brief Queues a function to run on the main thread
     *
     * @param func the function to queue
    */
    inline void queueInMainThread(ScheduledFunction&& func) {
        Loader::get()->queueInMainThread(std::forward<ScheduledFunction>(func));
    }

    /**
     * @brief Take the next mod to load
     *
     * @return Mod* The next mod to load
    */
    inline Mod* takeNextLoaderMod() {
        return Loader::get()->takeNextMod();
    }
}
