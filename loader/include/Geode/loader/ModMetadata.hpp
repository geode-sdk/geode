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

            [[deprecated]] operator geode::Dependency();
            [[deprecated]] operator geode::Dependency() const;

            [[deprecated]] static Dependency fromDeprecated(geode::Dependency const& value);
        };

        struct GEODE_DLL Incompatibility {
            enum class Importance : uint8_t {
                Breaking,
                Conflicting
            };
            std::string id;
            ComparableVersionInfo version;
            Importance importance = Importance::Breaking;
            Mod* mod = nullptr;
            [[nodiscard]] bool isResolved() const;
        };

        struct IssuesInfo {
            std::string info;
            std::optional<std::string> url;

            [[deprecated]] operator geode::IssuesInfo();
            [[deprecated]] operator geode::IssuesInfo() const;

            [[deprecated]] static IssuesInfo fromDeprecated(geode::IssuesInfo const& value);
        };

        /**
         * Path to the mod file
         */
        [[nodiscard]] ghc::filesystem::path getPath() const;
        /**
         * Name of the platform binary within
         * the mod zip
         */
        [[nodiscard]] std::string getBinaryName() const;
        /**
         * Mod Version. Should follow semantic versioning.
         */
        [[nodiscard]] VersionInfo getVersion() const;
        /**
         * Human-readable ID of the Mod.
         * Recommended to be in the format
         * "developer.mod". Not
         * guaranteed to be either case-
         * nor space-sensitive. Should
         * be restricted to the ASCII
         * character set.
         */
        [[nodiscard]] std::string getID() const;
        /**
         * Name of the mod. May contain
         * spaces & punctuation, but should
         * be restricted to the ASCII
         * character set.
         */
        [[nodiscard]] std::string getName() const;
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
        [[nodiscard]] std::string getDeveloper() const;
        /**
         * Short & concise description of the
         * mod.
         */
        [[nodiscard]] std::optional<std::string> getDescription() const;
        /**
         * Detailed description of the mod, written in Markdown (see
         * <Geode/ui/MDTextArea.hpp>) for more info
         */
        [[nodiscard]] std::optional<std::string> getDetails() const;
        /**
         * Changelog for the mod, written in Markdown (see
         * <Geode/ui/MDTextArea.hpp>) for more info
         */
        [[nodiscard]] std::optional<std::string> getChangelog() const;
        /**
         * Support info for the mod; this means anything to show ways to
         * support the mod's development, like donations. Written in Markdown
         * (see MDTextArea for more info)
         */
        [[nodiscard]] std::optional<std::string> getSupportInfo() const;
        /**
         * Git Repository of the mod
         */
        [[nodiscard]] std::optional<std::string> getRepository() const;
        /**
         * Info about where users should report issues and request help
         */
        [[nodiscard]] std::optional<IssuesInfo> getIssues() const;
        /**
         * Dependencies
         */
        [[nodiscard]] std::vector<Dependency> getDependencies() const;
        /**
         * Incompatibilities
         */
        [[nodiscard]] std::vector<Incompatibility> getIncompatibilities() const;
        /**
         * Mod spritesheet names
         */
        [[nodiscard]] std::vector<std::string> getSpritesheets() const;
        /**
         * Mod settings
         * @note Not a map because insertion order must be preserved
         */
        [[nodiscard]] std::vector<std::pair<std::string, Setting>> getSettings() const;
        /**
         * Whether this mod has to be loaded before the loading screen or not
         */
        [[nodiscard]] bool needsEarlyLoad() const;
        /**
         * Whether this mod is an API or not
         */
        [[nodiscard]] bool isAPI() const;

#if defined(GEODE_EXPOSE_SECRET_INTERNALS_IN_HEADERS_DO_NOT_DEFINE_PLEASE)
        void setPath(ghc::filesystem::path const& value);
        void setBinaryName(std::string const& value);
        void setVersion(VersionInfo const& value);
        void setID(std::string const& value);
        void setName(std::string const& value);
        void setDeveloper(std::string const& value);
        void setDescription(std::optional<std::string> const& value);
        void setDetails(std::optional<std::string> const& value);
        void setChangelog(std::optional<std::string> const& value);
        void setSupportInfo(std::optional<std::string> const& value);
        void setRepository(std::optional<std::string> const& value);
        void setIssues(std::optional<IssuesInfo> const& value);
        void setDependencies(std::vector<Dependency> const& value);
        void setIncompatibilities(std::vector<Incompatibility> const& value);
        void setSpritesheets(std::vector<std::string> const& value);
        void setSettings(std::vector<std::pair<std::string, Setting>> const& value);
        void setNeedsEarlyLoad(bool const& value);
        void setIsAPI(bool const& value);
#endif

        /**
         * Create ModInfo from an unzipped .geode package
         */
        static Result<ModMetadata> createFromGeodeZip(utils::file::Unzip& zip);
        /**
         * Create ModInfo from a .geode package
         */
        static Result<ModMetadata> createFromGeodeFile(ghc::filesystem::path const& path);
        /**
         * Create ModInfo from a mod.json file
         */
        static Result<ModMetadata> createFromFile(ghc::filesystem::path const& path);
        /**
         * Create ModInfo from a parsed json document
         */
        static Result<ModMetadata> create(ModJson const& json);

        /**
         * Convert to JSON. Essentially same as getRawJSON except dynamically
         * adds runtime fields like path
         */
        [[nodiscard]] ModJson toJSON() const;
        /**
         * Get the raw JSON file
         */
        [[nodiscard]] ModJson getRawJSON() const;

        bool operator==(ModMetadata const& other) const;

        static bool validateID(std::string const& id);

        [[deprecated]] operator ModInfo();
        [[deprecated]] operator ModInfo() const;

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
        friend class IndexItem;
    };
}

template <>
struct json::Serialize<geode::ModMetadata> {
    static json::Value to_json(geode::ModMetadata const& info) {
        return info.toJSON();
    }
};
