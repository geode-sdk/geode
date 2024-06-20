#pragma once

#include "../utils/Result.hpp"
#include "../utils/VersionInfo.hpp"
#include "Setting.hpp"
#include "Types.hpp"

#include <matjson.hpp>
#include <memory>

namespace geode {
    namespace utils::file {
        class Unzip;
    }

    class ModMetadataImpl;

    class GEODE_DLL ModMetadataLinks final {
        class Impl;
        std::unique_ptr<Impl> m_impl;

        friend class ModMetadataImpl;
    
    public:
        ModMetadataLinks();
        ModMetadataLinks(ModMetadataLinks const& other);
        ModMetadataLinks(ModMetadataLinks&& other) noexcept;
        ModMetadataLinks& operator=(ModMetadataLinks const& other);
        ModMetadataLinks& operator=(ModMetadataLinks&& other) noexcept;
        ~ModMetadataLinks();

        /**
         * Get the URL for the home website for this mod
         */
        std::optional<std::string> getHomepageURL() const;
        /**
         * Get the URL for the source code repository for this mod
         */
        std::optional<std::string> getSourceURL() const;
        /**
         * Get the URL for the community page (Discord server etc.) for this mod
         */
        std::optional<std::string> getCommunityURL() const;

#if defined(GEODE_EXPOSE_SECRET_INTERNALS_IN_HEADERS_DO_NOT_DEFINE_PLEASE)
        Impl* getImpl();
#endif
    };

    /**
     * Represents all the data gather-able
     * from mod.json
     */
    class GEODE_DLL ModMetadata final {
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
        };

        struct GEODE_DLL Incompatibility {
            enum class Importance : uint8_t {
                Breaking,
                Conflicting,
                Superseded,
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
        };

        /**
         * Path to the mod file
         */
        [[nodiscard]] std::filesystem::path getPath() const;
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
         * Human-readable ID of the Mod. Should be in the format 
         * "developer.mod". May only contain lowercase ASCII characters, 
         * numbers, dashes, underscores, and a single separating dot
         */
        [[nodiscard]] std::string getID() const;
        /**
         * True if the mod has a mod ID that will be rejected in the future, 
         * such as using uppercase letters or having multiple dots. Mods like 
         * this should release new versions that supersede the old ones
         */
        [[nodiscard]] bool usesDeprecatedIDForm() const;
        /**
         * Name of the mod. May contain
         * spaces & punctuation, but should
         * be restricted to the ASCII
         * character set.
         */
        [[nodiscard]] std::string getName() const;
        /**
         * The name of the head developer.
         * If the mod has multiple * developers, this will return the first 
         * developer in the list.
         */
        [[nodiscard, deprecated("Use ModMetadata::getDevelopers() instead")]]
        std::string getDeveloper() const;
        /**
         * The developers of this mod
         */
        [[nodiscard]] std::vector<std::string> getDevelopers() const;
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
        [[nodiscard, deprecated("Use ModMetadata::getLinks instead")]]
        std::optional<std::string> getRepository() const;
        /**
         * Get the links (related websites / servers / etc.) for this mod
         */
        ModMetadataLinks getLinks() const;
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
         * Get the tags for this mod
         */
        [[nodiscard]] std::unordered_set<std::string> getTags() const;
        /**
         * Whether this mod has to be loaded before the loading screen or not
         */
        [[nodiscard]] bool needsEarlyLoad() const;
        /**
         * Whether this mod is an API or not
         */
        [[nodiscard]] bool isAPI() const;

        /**
         * Gets the target GD version for the current platform.
         * Returns nullopt if mod targets any GD version.
        */
        [[nodiscard]] std::optional<std::string> getGameVersion() const;

        /**
         * Gets the target Geode version for the current platform.
        */
        [[nodiscard]] VersionInfo getGeodeVersion() const;

        /**
         * Checks if mod can be installed on the current GD version.
         * Returns Ok() if it can, Err otherwise.
        */
        Result<> checkGameVersion() const;

#if defined(GEODE_EXPOSE_SECRET_INTERNALS_IN_HEADERS_DO_NOT_DEFINE_PLEASE)
        void setPath(std::filesystem::path const& value);
        void setBinaryName(std::string const& value);
        void setVersion(VersionInfo const& value);
        void setID(std::string const& value);
        void setName(std::string const& value);
        void setDeveloper(std::string const& value);
        void setDevelopers(std::vector<std::string> const& value);
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
        void setTags(std::unordered_set<std::string> const& value);
        void setNeedsEarlyLoad(bool const& value);
        void setIsAPI(bool const& value);
        ModMetadataLinks& getLinksMut();
#endif

        /**
         * Create ModInfo from an unzipped .geode package
         */
        static Result<ModMetadata> createFromGeodeZip(utils::file::Unzip& zip);
        /**
         * Create ModInfo from a .geode package
         */
        static Result<ModMetadata> createFromGeodeFile(std::filesystem::path const& path);
        /**
         * Create ModInfo from a mod.json file
         */
        static Result<ModMetadata> createFromFile(std::filesystem::path const& path);
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

        /**
         * Format a list of mod developers, truncated if there are multiple 
         * developers in the same way as in the mods list
         * @note Static because this is used by InstallListCell
         */
        [[nodiscard]] static std::string formatDeveloperDisplayString(
            std::vector<std::string> const& developers
        );

    private:
        /**
         * Version is passed for backwards
         * compatibility if we update the mod.json
         * format
         */
        static Result<ModMetadata> createFromSchemaV010(ModJson const& json);

        Result<> addSpecialFiles(std::filesystem::path const& dir);
        Result<> addSpecialFiles(utils::file::Unzip& zip);

        std::vector<std::pair<std::string, std::optional<std::string>*>> getSpecialFiles();

        friend class Loader;

        friend class ModMetadataImpl;
        friend class IndexItem;
    };
}

template <>
struct matjson::Serialize<geode::ModMetadata> {
    static matjson::Value to_json(geode::ModMetadata const& info) {
        return info.toJSON();
    }
};
