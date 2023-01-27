#pragma once

#include "Types.hpp"
#include <json.hpp>
#include "../utils/VersionInfo.hpp"
#include "../utils/Result.hpp"
#include "Setting.hpp"

namespace geode {
    namespace utils::file {
        class Unzip;
    }

    struct GEODE_DLL Dependency {
        std::string id;
        ComparableVersionInfo version;
        bool required = false;
        Mod* mod = nullptr;
        bool isResolved() const;
    };

    struct IssuesInfo {
        std::string info;
        std::optional<std::string> url;
    };

    /**
     * Represents all the data gatherable
     * from mod.json
     */
    struct GEODE_DLL ModInfo {
        /**
         * Path to the mod file
         */
        ghc::filesystem::path path;
        /**
         * Name of the platform binary within
         * the mod zip
         */
        std::string binaryName;
        /**
         * Mod Version. Should follow semver.
         */
        VersionInfo version { 1, 0, 0 };
        /**
         * Human-readable ID of the Mod.
         * Recommended to be in the format
         * "com.developer.mod". Not
         * guaranteed to be either case-
         * nor space-sensitive. Should
         * be restricted to the ASCII
         * character set.
         */
        std::string id;
        /**
         * Name of the mod. May contain
         * spaces & punctuation, but should
         * be restricted to the ASCII
         * character set.
         */
        std::string name;
        /**
         * The name of the head developer.
         * Should be a single name, like
         * "HJfod" or "The Geode Team".
         * If the mod has multiple
         * developers, this field should
         * be one of their name or a team
         * name, and the rest of the credits
         * should be named in `m_credits`
         * instead.
         */
        std::string developer;
        /**
         * Short & concise description of the
         * mod.
         */
        std::optional<std::string> description;
        /**
         * Detailed description of the mod, writtenin Markdown (see
         * <Geode/ui/MDTextArea.hpp>) for more info
         */
        std::optional<std::string> details;
        /**
         * Changelog for the mod, written in Markdown (see
         * <Geode/ui/MDTextArea.hpp>) for more info
         */
        std::optional<std::string> changelog;
        /**
         * Support info for the mod; this means anything to show ways to
         * support the mod's development, like donations. Written in Markdown
         * (see MDTextArea for more info)
         */
        std::optional<std::string> supportInfo;
        /**
         * Git Repository of the mod
         */
        std::optional<std::string> repository;
        /**
         * Info about where users should report issues and request help
         */
        std::optional<IssuesInfo> issues;
        /**
         * Dependencies
         */
        std::vector<Dependency> dependencies;
        /**
         * Mod spritesheet names
         */
        std::vector<std::string> spritesheets;
        /**
         * Mod settings
         * @note Not a map because insertion order must be preserved
         */
        std::vector<std::pair<std::string, Setting>> settings;
        /**
         * Whether the mod can be disabled or not
         */
        bool supportsDisabling = true;
        /**
         * Whether the mod can be unloaded or not
         */
        bool supportsUnloading = false;
        /**
         * Whether this mod has to be loaded before the loading screen or not
         */
        bool needsEarlyLoad = false;
        /**
         * Create ModInfo from an unzipped .geode package
         */
        static Result<ModInfo> createFromGeodeZip(utils::file::Unzip& zip);
        /**
         * Create ModInfo from a .geode package
         */
        static Result<ModInfo> createFromGeodeFile(ghc::filesystem::path const& path);
        /**
         * Create ModInfo from a mod.json file
         */
        static Result<ModInfo> createFromFile(ghc::filesystem::path const& path);
        /**
         * Create ModInfo from a parsed json document
         */
        static Result<ModInfo> create(ModJson const& json);

        /**
         * Convert to JSON. Essentially same as getRawJSON except dynamically 
         * adds runtime fields like path
         */
        ModJson toJSON() const;
        /**
         * Get the raw JSON file
         */
        ModJson getRawJSON() const;

        bool operator==(ModInfo const& other) const;

        static bool validateID(std::string const& id);

    private:
        std::shared_ptr<ModJson> m_rawJSON;

        /**
         * Version is passed for backwards
         * compatibility if we update the mod.json
         * format
         */
        static Result<ModInfo> createFromSchemaV010(ModJson const& json);

        Result<> addSpecialFiles(ghc::filesystem::path const& dir);
        Result<> addSpecialFiles(utils::file::Unzip& zip);

        std::vector<std::pair<std::string, std::optional<std::string>*>> getSpecialFiles();
    };
}

template <>
struct json::Serialize<geode::ModInfo> {
    static json::Value to_json(geode::ModInfo const& info) {
        return info.toJSON();
    }
};
