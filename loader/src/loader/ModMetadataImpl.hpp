#pragma once

#include <Geode/loader/Loader.hpp>
#include <Geode/loader/Mod.hpp>
#include <Geode/utils/JsonValidation.hpp>
#include <Geode/utils/VersionInfo.hpp>
#include <Geode/loader/SettingV3.hpp>

using namespace geode::prelude;

namespace geode {
    class ModMetadataLinks::Impl final {
    public:
        std::optional<std::string> m_homepage;
        std::optional<std::string> m_source;
        std::optional<std::string> m_community;
    };

    class ModMetadata::Impl {
    public:
        std::filesystem::path m_path;
        std::string m_binaryName;
        VersionInfo m_version{1, 0, 0};
        std::string m_id;
        std::string m_name;
        std::vector<std::string> m_developers;
        // TODO: remove once #895 is fixed
        std::optional<std::string> m_softInvalidReason;
        std::string m_gdVersion;
        VersionInfo m_geodeVersion;
        std::optional<std::string> m_description;
        std::optional<std::string> m_details;
        std::optional<std::string> m_changelog;
        std::optional<std::string> m_supportInfo;
        ModMetadataLinks m_links;
        std::optional<IssuesInfo> m_issues;
        std::vector<Dependency> m_dependencies;
        std::vector<Incompatibility> m_incompatibilities;
        std::vector<std::string> m_spritesheets;
        std::vector<std::pair<std::string, matjson::Value>> m_settings;
        std::unordered_set<std::string> m_tags;
        bool m_needsEarlyLoad = false;
        bool m_isAPI = false;

        ModJson m_rawJSON;

        static Result<ModMetadata> createFromGeodeZip(utils::file::Unzip& zip);
        static Result<ModMetadata> createFromGeodeFile(std::filesystem::path const& path);
        static Result<ModMetadata> createFromFile(std::filesystem::path const& path);
        static Result<ModMetadata> create(ModJson const& json);

        ModJson toJSON() const;
        ModJson getRawJSON() const;

        bool operator==(ModMetadata::Impl const& other) const;

        static bool validateID(std::string const& id);
        static bool validateOldID(std::string const& id);
        static bool isDeprecatedIDForm(std::string const& id);

        static Result<ModMetadata> createFromSchemaV010(ModJson const& rawJson);

        Result<> addSpecialFiles(std::filesystem::path const& dir);
        Result<> addSpecialFiles(utils::file::Unzip& zip);

        std::vector<std::pair<std::string, std::optional<std::string>*>> getSpecialFiles();
    };

    class ModMetadataImpl : public ModMetadata::Impl {
    public:
        static ModMetadata::Impl& getImpl(ModMetadata& info);
    };
}

template <>
struct matjson::Serialize<geode::ModMetadata::Dependency::Importance> {
    static matjson::Value GEODE_DLL to_json(geode::ModMetadata::Dependency::Importance const& importance) {
        switch (importance) {
            case geode::ModMetadata::Dependency::Importance::Required: return {"required"};
            case geode::ModMetadata::Dependency::Importance::Recommended: return {"recommended"};
            case geode::ModMetadata::Dependency::Importance::Suggested: return {"suggested"};
            default: return {"unknown"};
        }
    }
    static geode::ModMetadata::Dependency::Importance GEODE_DLL from_json(matjson::Value const& importance) {
        auto impStr = importance.as_string();
        if (impStr == "required")
            return geode::ModMetadata::Dependency::Importance::Required;
        if (impStr == "recommended")
            return geode::ModMetadata::Dependency::Importance::Recommended;
        if (impStr == "suggested")
            return geode::ModMetadata::Dependency::Importance::Suggested;
        throw matjson::JsonException(R"(Expected importance to be "required", "recommended" or "suggested")");
    }

    static bool is_json(matjson::Value const& value) {
        return value.is_string();
    }
};

template <>
struct matjson::Serialize<geode::ModMetadata::Incompatibility::Importance> {
    static matjson::Value GEODE_DLL to_json(geode::ModMetadata::Incompatibility::Importance const& importance) {
        switch (importance) {
            case geode::ModMetadata::Incompatibility::Importance::Breaking: return {"breaking"};
            case geode::ModMetadata::Incompatibility::Importance::Conflicting: return {"conflicting"};
            case geode::ModMetadata::Incompatibility::Importance::Superseded: return {"superseded"};
            default: return {"unknown"};
        }
    }
    static geode::ModMetadata::Incompatibility::Importance GEODE_DLL from_json(matjson::Value const& importance) {
        auto impStr = importance.as_string();
        if (impStr == "breaking")
            return geode::ModMetadata::Incompatibility::Importance::Breaking;
        if (impStr == "conflicting")
            return geode::ModMetadata::Incompatibility::Importance::Conflicting;
        if (impStr == "superseded")
            return geode::ModMetadata::Incompatibility::Importance::Superseded;
        throw matjson::JsonException(R"(Expected importance to be "breaking", "conflicting", or "superseded")");
    }

    static bool is_json(matjson::Value const& value) {
        return value.is_string();
    }
};
