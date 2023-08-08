#pragma once

#include "../utils/Result.hpp"
#include "../utils/VersionInfo.hpp"
#include "ModInfo.hpp"
#include "Setting.hpp"
#include "Types.hpp"

#include <json.hpp>
#include <memory>

namespace geode {
    namespace utils::file {
        class Unzip;
    }

    struct GEODE_DLL [[deprecated("use ModMetadata::Dependency instead")]] Dependency;
    struct [[deprecated("use ModMetadata::IssuesInfo instead")]] IssuesInfo;

    class ModMetadataImpl;

    /**
     * Represents all the data gather-able
     * from mod.json
     */
    class GEODE_DLL ModMetadata {
        class Impl;
        std::unique_ptr<Impl> m_impl;

    public:
        ModMetadata();
        explicit ModMetadata(std::string id);
        ModMetadata(ModMetadata const& other);
        ModMetadata(ModMetadata&& other) noexcept;
        ModMetadata& operator=(ModMetadata const& other);
        ModMetadata& operator=(ModMetadata&& other) noexcept;
        ~ModMetadata();

        struct GEODE_DLL Dependency {
            enum class Importance : uint8_t { Required, Recommended, Suggested };
            std::string id;
            ComparableVersionInfo version;
            Importance importance = Importance::Required;
            Mod* mod = nullptr;
            [[nodiscard]] bool isResolved() const;

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated-declarations"
#pragma ide diagnostic ignored "google-explicit-constructor"
            operator geode::Dependency();
            operator geode::Dependency() const;

            static Dependency fromDeprecated(geode::Dependency const& value);
#pragma clang diagnostic pop
        };

        struct GEODE_DLL Incompatibility {
            std::string id;
            ComparableVersionInfo version;
            Mod* mod = nullptr;
            [[nodiscard]] bool isResolved() const;
        };

        struct IssuesInfo {
            std::string info;
            std::optional<std::string> url;

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated-declarations"
#pragma ide diagnostic ignored "google-explicit-constructor"
            operator geode::IssuesInfo();
            operator geode::IssuesInfo() const;

            static IssuesInfo fromDeprecated(geode::IssuesInfo const& value);
#pragma clang diagnostic pop
        };

        /**
         * Path to the mod file
         */
        [[maybe_unused, nodiscard]] ghc::filesystem::path getPath() const;
        /**
         * Name of the platform binary within
         * the mod zip
         */
        [[maybe_unused, nodiscard]] std::string getBinaryName() const;
        /**
         * Mod Version. Should follow semantic versioning.
         */
        [[maybe_unused, nodiscard]] VersionInfo getVersion() const;
        /**
         * Human-readable ID of the Mod.
         * Recommended to be in the format
         * "developer.mod". Not
         * guaranteed to be either case-
         * nor space-sensitive. Should
         * be restricted to the ASCII
         * character set.
         */
        [[maybe_unused, nodiscard]] std::string getID() const;
        /**
         * Name of the mod. May contain
         * spaces & punctuation, but should
         * be restricted to the ASCII
         * character set.
         */
        [[maybe_unused, nodiscard]] std::string getName() const;
        /**
         * The name of the head developer.
         * Should be a single name, like
         * "HJfod" or "Geode Team".
         * If the mod has multiple
         * developers, this field should
         * be one of their name or a team
         * name, and the rest of the credits
         * should be named in `m_credits`
         * instead.
         */
        [[maybe_unused, nodiscard]] std::string getDeveloper() const;
        /**
         * Short & concise description of the
         * mod.
         */
        [[maybe_unused, nodiscard]] std::optional<std::string> getDescription() const;
        /**
         * Detailed description of the mod, written in Markdown (see
         * <Geode/ui/MDTextArea.hpp>) for more info
         */
        [[maybe_unused, nodiscard]] std::optional<std::string> getDetails() const;
        /**
         * Changelog for the mod, written in Markdown (see
         * <Geode/ui/MDTextArea.hpp>) for more info
         */
        [[maybe_unused, nodiscard]] std::optional<std::string> getChangelog() const;
        /**
         * Support info for the mod; this means anything to show ways to
         * support the mod's development, like donations. Written in Markdown
         * (see MDTextArea for more info)
         */
        [[maybe_unused, nodiscard]] std::optional<std::string> getSupportInfo() const;
        /**
         * Git Repository of the mod
         */
        [[maybe_unused, nodiscard]] std::optional<std::string> getRepository() const;
        /**
         * Info about where users should report issues and request help
         */
        [[maybe_unused, nodiscard]] std::optional<IssuesInfo> getIssues() const;
        /**
         * Dependencies
         */
        [[maybe_unused, nodiscard]] std::vector<Dependency> getDependencies() const;
        /**
         * Incompatibilities
         */
        [[maybe_unused, nodiscard]] std::vector<Incompatibility> getIncompatibilities() const;
        /**
         * Mod spritesheet names
         */
        [[maybe_unused, nodiscard]] std::vector<std::string> getSpritesheets() const;
        /**
         * Mod settings
         * @note Not a map because insertion order must be preserved
         */
        [[maybe_unused, nodiscard]] std::vector<std::pair<std::string, Setting>> getSettings() const;
        /**
         * Whether this mod has to be loaded before the loading screen or not
         */
        [[maybe_unused, nodiscard]] bool needsEarlyLoad() const;
        /**
         * Whether this mod is an API or not
         */
        [[maybe_unused, nodiscard]] bool isAPI() const;
        /**
         * Create ModInfo from an unzipped .geode package
         */
        [[maybe_unused]] static Result<ModMetadata> createFromGeodeZip(utils::file::Unzip& zip);
        /**
         * Create ModInfo from a .geode package
         */
        [[maybe_unused]] static Result<ModMetadata> createFromGeodeFile(ghc::filesystem::path const& path);
        /**
         * Create ModInfo from a mod.json file
         */
        [[maybe_unused]] static Result<ModMetadata> createFromFile(ghc::filesystem::path const& path);
        /**
         * Create ModInfo from a parsed json document
         */
        [[maybe_unused]] static Result<ModMetadata> create(ModJson const& json);

        /**
         * Convert to JSON. Essentially same as getRawJSON except dynamically
         * adds runtime fields like path
         */
        [[maybe_unused, nodiscard]] ModJson toJSON() const;
        /**
         * Get the raw JSON file
         */
        [[maybe_unused, nodiscard]] ModJson getRawJSON() const;

        [[maybe_unused]] bool operator==(ModMetadata const& other) const;

        [[maybe_unused]] static bool validateID(std::string const& id);

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated-declarations"
#pragma ide diagnostic ignored "google-explicit-constructor"
        operator ModInfo();
        operator ModInfo() const;
#pragma clang diagnostic pop

    private:
        /**
         * Version is passed for backwards
         * compatibility if we update the mod.json
         * format
         */
        static Result<ModMetadata> createFromSchemaV010(ModJson const& json);

        Result<> addSpecialFiles(ghc::filesystem::path const& dir);
        Result<> addSpecialFiles(utils::file::Unzip& zip);

        std::vector<std::pair<std::string, std::optional<std::string>*>> getSpecialFiles();

        friend class Loader;

        friend class ModMetadataImpl;

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated-declarations"
        // ModInfo => ModMetadata conversion stuff
        friend class ModInfo::Impl;
#pragma clang diagnostic pop
    };
}

template <>
struct json::Serialize<geode::ModMetadata> {
    static json::Value to_json(geode::ModMetadata const& info) {
        return info.toJSON();
    }
};
