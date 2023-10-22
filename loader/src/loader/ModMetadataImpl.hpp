#pragma once

#include <Geode/loader/Loader.hpp>
#include <Geode/loader/Mod.hpp>
#include <Geode/utils/JsonValidation.hpp>
#include <Geode/utils/VersionInfo.hpp>

using namespace geode::prelude;

namespace geode {
    class ModMetadata::Impl {
    public:
        ghc::filesystem::path m_path;
        std::string m_binaryName;
        VersionInfo m_version{1, 0, 0};
        std::string m_id;
        std::string m_name;
        std::string m_developer;
        std::optional<std::string> m_description;
        std::optional<std::string> m_details;
        std::optional<std::string> m_changelog;
        std::optional<std::string> m_supportInfo;
        std::optional<std::string> m_repository;
        std::optional<IssuesInfo> m_issues;
        std::vector<Dependency> m_dependencies;
        std::vector<Incompatibility> m_incompatibilities;
        std::vector<std::string> m_spritesheets;
        std::vector<std::pair<std::string, Setting>> m_settings;
        bool m_needsEarlyLoad = false;
        bool m_isAPI = false;

        ModJson m_rawJSON;

        static Result<ModMetadata> createFromGeodeZip(utils::file::Unzip& zip);
        static Result<ModMetadata> createFromGeodeFile(ghc::filesystem::path const& path);
        static Result<ModMetadata> createFromFile(ghc::filesystem::path const& path);
        static Result<ModMetadata> create(ModJson const& json);

        ModJson toJSON() const;
        ModJson getRawJSON() const;

        bool operator==(ModMetadata::Impl const& other) const;

        static bool validateID(std::string const& id);

        static Result<ModMetadata> createFromSchemaV010(ModJson const& rawJson);

        Result<> addSpecialFiles(ghc::filesystem::path const& dir);
        Result<> addSpecialFiles(utils::file::Unzip& zip);

        std::vector<std::pair<std::string, std::optional<std::string>*>> getSpecialFiles();
    };

    class ModMetadataImpl : public ModMetadata::Impl {
    public:
        static ModMetadata::Impl& getImpl(ModMetadata& info);
    };
}
