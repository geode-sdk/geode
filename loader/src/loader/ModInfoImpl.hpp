#pragma once

#include <Geode/loader/Loader.hpp>
#include <Geode/loader/Mod.hpp>
#include <Geode/utils/JsonValidation.hpp>
#include <Geode/utils/VersionInfo.hpp>

using namespace geode::prelude;

namespace geode {
    class ModInfo::Impl {
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
        std::vector<std::string> m_spritesheets;
        std::vector<std::pair<std::string, Setting>> m_settings;
        bool m_supportsDisabling = true;
        bool m_supportsUnloading = false;
        bool m_needsEarlyLoad = false;
        bool m_isAPI = false;

        ModJson m_rawJSON;

        static Result<ModInfo> createFromGeodeZip(utils::file::Unzip& zip);
        static Result<ModInfo> createFromGeodeFile(ghc::filesystem::path const& path);
        static Result<ModInfo> createFromFile(ghc::filesystem::path const& path);
        static Result<ModInfo> create(ModJson const& json);

        ModJson toJSON() const;
        ModJson getRawJSON() const;

        bool operator==(ModInfo::Impl const& other) const;

        static bool validateID(std::string const& id);

        static Result<ModInfo> createFromSchemaV010(ModJson const& json);

        Result<> addSpecialFiles(ghc::filesystem::path const& dir);
        Result<> addSpecialFiles(utils::file::Unzip& zip);

        std::vector<std::pair<std::string, std::optional<std::string>*>> getSpecialFiles();
    };

    class ModInfoImpl {
    public:
        static ModInfo::Impl& getImpl(ModInfo& info);
    };
}