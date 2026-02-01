#pragma once

#include <Geode/Result.hpp>
#include <Geode/utils/ZStringView.hpp>
#include "../utils/VersionInfo.hpp"
#include "Types.hpp"
#include <filesystem>
#include <matjson.hpp>
#include <memory>
#include <string>

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

        class GEODE_DLL Dependency final {
            class Impl;
            std::unique_ptr<Impl> m_impl;

        public:
            enum class Importance : uint8_t { Required, Recommended, Suggested };
            Dependency();
            Dependency(Dependency const& other);
            Dependency(Dependency&& other) noexcept;
            Dependency& operator=(Dependency const& other);
            Dependency& operator=(Dependency&& other) noexcept;
            ~Dependency();

            std::string const& getID() const;
            void setID(std::string value);
            ComparableVersionInfo const& getVersion() const;
            void setVersion(ComparableVersionInfo value);
            Importance getImportance() const;
            void setImportance(Importance value);
            Mod* getMod() const;
            void setMod(Mod* mod);
            matjson::Value const& getSettings() const;
            void setSettings(matjson::Value value);
            [[nodiscard]] bool isResolved() const;
        };

        class GEODE_DLL Incompatibility final {
            class Impl;
            std::unique_ptr<Impl> m_impl;

        public:
            enum class Importance : uint8_t {
                Breaking,
                Conflicting,
                Superseded,
            };
            Incompatibility();
            Incompatibility(Incompatibility const& other);
            Incompatibility(Incompatibility&& other) noexcept;
            Incompatibility& operator=(Incompatibility const& other);
            Incompatibility& operator=(Incompatibility&& other) noexcept;
            ~Incompatibility();

            std::string const& getID() const;
            void setID(std::string value);
            ComparableVersionInfo const& getVersion() const;
            void setVersion(ComparableVersionInfo value);
            Importance getImportance() const;
            void setImportance(Importance value);
            Mod* getMod() const;
            void setMod(Mod* mod);
            [[nodiscard]] bool isResolved() const;
        };

        class GEODE_DLL IssuesInfo final {
            class Impl;
            std::unique_ptr<Impl> m_impl;

        public:
            IssuesInfo();
            IssuesInfo(IssuesInfo const& other);
            IssuesInfo(IssuesInfo&& other) noexcept;
            IssuesInfo& operator=(IssuesInfo const& other);
            IssuesInfo& operator=(IssuesInfo&& other) noexcept;
            ~IssuesInfo();

            std::string const& getInfo() const;
            void setInfo(std::string value);
            std::optional<std::string> const& getURL() const;
            void setURL(std::optional<std::string> value);
        };

        /**
         * Path to the mod file
         */
        [[nodiscard]] std::filesystem::path const& getPath() const;
        /**
         * Name of the platform binary within
         * the mod zip
         */
        [[nodiscard]] ZStringView getBinaryName() const;
        /**
         * Mod Version. Should follow semantic versioning.
         */
        [[nodiscard]] VersionInfo getVersion() const;
        /**
         * Human-readable ID of the Mod. Should be in the format
         * "developer.mod". May only contain lowercase ASCII characters,
         * numbers, dashes, underscores, and a single separating dot
         */
        [[nodiscard]] ZStringView getID() const;
        /**
         * Name of the mod. May contain
         * spaces & punctuation, but should
         * be restricted to the ASCII
         * character set.
         */
        [[nodiscard]] ZStringView getName() const;
        /**
         * The developers of this mod
         */
        [[nodiscard]] std::vector<std::string> const& getDevelopers() const;
        /**
         * Short & concise description of the
         * mod.
         */
        [[nodiscard]] std::optional<std::string> const& getDescription() const;
        /**
         * Detailed description of the mod, written in Markdown (see
         * <Geode/ui/MDTextArea.hpp>) for more info
         */
        [[nodiscard]] std::optional<std::string> const& getDetails() const;
        /**
         * Changelog for the mod, written in Markdown (see
         * <Geode/ui/MDTextArea.hpp>) for more info
         */
        [[nodiscard]] std::optional<std::string> const& getChangelog() const;
        /**
         * Support info for the mod; this means anything to show ways to
         * support the mod's development, like donations. Written in Markdown
         * (see MDTextArea for more info)
         */
        [[nodiscard]] std::optional<std::string> const& getSupportInfo() const;
        /**
         * Get the links (related websites / servers / etc.) for this mod
         */
        ModMetadataLinks const& getLinks() const;
        /**
         * Info about where users should report issues and request help
         */
        [[nodiscard]] std::optional<IssuesInfo> const& getIssues() const;
        /**
         * Dependencies
         */
        [[nodiscard]] std::vector<Dependency> const& getDependencies() const;
        /**
         * Incompatibilities
         */
        [[nodiscard]] std::vector<Incompatibility> const& getIncompatibilities() const;
        /**
         * Mod spritesheet names
         */
        [[nodiscard]] std::vector<std::string> const& getSpritesheets() const;
        /**
         * Mod settings
         * @note Not a map because insertion order must be preserved
         */
        [[nodiscard]] std::vector<std::pair<std::string, matjson::Value>> const& getSettings() const;
        /**
         * Get the tags for this mod
         */
        [[nodiscard]] std::unordered_set<std::string> const& getTags() const;
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
         * Gets the load priority of this mod.
         */
        [[nodiscard]] int getLoadPriority() const;

        /**
         * Checks if mod can be installed on the current GD version.
         * Returns Ok() if it can, Err explaining why not otherwise.
        */
        Result<> checkGameVersion() const;
        /**
         * Checks if mod can be installed on the current Geode version.
         * Returns Ok() if it can, Err explaining why not otherwise.
        */
        Result<> checkGeodeVersion() const;
        /**
         * Checks if mod can be installed on the current GD & Geode version.
         * Returns Ok() if it can, Err explaining why not otherwise.
        */
        Result<> checkTargetVersions() const;

#if defined(GEODE_EXPOSE_SECRET_INTERNALS_IN_HEADERS_DO_NOT_DEFINE_PLEASE)
        void setPath(std::filesystem::path value);
        void setBinaryName(std::string value);
        void setVersion(VersionInfo value);
        void setID(std::string value);
        void setName(std::string value);
        void setDeveloper(std::string value);
        void setDevelopers(std::vector<std::string> value);
        void setDescription(std::optional<std::string> value);
        void setDetails(std::optional<std::string> value);
        void setChangelog(std::optional<std::string> value);
        void setSupportInfo(std::optional<std::string> value);
        void setRepository(std::optional<std::string> value);
        void setIssues(std::optional<IssuesInfo> value);
        void setDependencies(std::vector<Dependency> value);
        void setIncompatibilities(std::vector<Incompatibility> value);
        void setSpritesheets(std::vector<std::string> value);
        void setSettings(std::vector<std::pair<std::string, matjson::Value>> value);
        void setTags(std::unordered_set<std::string> value);
        void setNeedsEarlyLoad(bool value);
        void setIsAPI(bool value);
        void setGameVersion(std::string value);
        void setGeodeVersion(VersionInfo value);
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

        static bool validateID(std::string_view id);

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
    static Value toJson(geode::ModMetadata const& value)
    {
        return Value(value.toJSON());
    }
};
