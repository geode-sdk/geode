#pragma once

#include <Geode/loader/Loader.hpp>
#include <Geode/loader/Mod.hpp>
#include <Geode/utils/JsonValidation.hpp>
#include <Geode/utils/VersionInfo.hpp>
#include <Geode/utils/StringMap.hpp>
#include <Geode/loader/Setting.hpp>
#include <compare>

using namespace geode::prelude;

struct LoadPriority {
    int priority;

    LoadPriority() = default;
    LoadPriority(LoadPriority const&) = default;
    LoadPriority(LoadPriority&&) noexcept = default;
    LoadPriority& operator=(LoadPriority const&) = default;
    LoadPriority& operator=(LoadPriority&&) noexcept = default;

    LoadPriority(int p) : priority(p) {}

    std::strong_ordering operator<=>(LoadPriority const& other) const {
        return this->priority <=> other.priority;
    }
    bool operator==(LoadPriority const& other) const {
        return this->priority == other.priority;
    }

    operator int() const { return priority; }
};

template <>
struct matjson::Serialize<LoadPriority> {
    static Result<LoadPriority, std::string> fromJson(Value const& value) {
        if (value.isNumber()) {
            int p = GEODE_UNWRAP(value.asInt());
            if (p > 4000 || p < -4000) {
                return Err("load-priority must be between -4000 and 4000");
            }
            return Ok(LoadPriority{p});
        } else if (value.isString()) {
            auto str = GEODE_UNWRAP(value.asString());
            utils::string::toLowerIP(str);

            if (str == "first") return Ok(LoadPriority{-3000});
            if (str == "very-early") return Ok(LoadPriority{-2000});
            if (str == "early") return Ok(LoadPriority{-1000});
            if (str == "normal") return Ok(LoadPriority{0});
            if (str == "late") return Ok(LoadPriority{1000});
            if (str == "very-late") return Ok(LoadPriority{2000});
            if (str == "last") return Ok(LoadPriority{3000});
            return Err("Invalid load-priority string");
        }
        return Err("load-priority must be a number or string");
    }

    static Value toJson(LoadPriority const& value) {
        switch (value) {
            case -3000: return "first";
            case -2000: return "very-early";
            case -1000: return "early";
            case 0: return "normal";
            case 1000: return "late";
            case 2000: return "very-late";
            case 3000: return "last";
            default: return value.priority;
        }
    }
};

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
        VersionInfo m_version { 1, 0, 0 };
        std::string m_id;
        std::string m_name;
        std::vector<std::string> m_developers;
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
        LoadPriority m_loadPriority = 0;
        std::vector<std::string> m_errors;
        bool m_completelyUnparseable = false;
        ModJson m_rawJSON;

        ModJson toJSON() const;
        ModJson getRawJSON() const;

        bool operator==(ModMetadata::Impl const& other) const;

        static bool validateID(std::string_view id);

        static ModMetadata parse(ModJson const& rawJson, std::optional<std::string_view> guessedID);
        static ModMetadata createInvalidMetadata(
            std::filesystem::path const& path,
            std::string_view error,
            std::optional<std::string_view> guessedID
        );

        Result<> addSpecialFiles(std::filesystem::path const& dir);
        Result<> addSpecialFiles(utils::file::Unzip& zip);

        std::vector<std::pair<std::string, std::optional<std::string>*>> getSpecialFiles();
    };

    class ModMetadataImpl : public ModMetadata::Impl {
    public:
        static ModMetadata::Impl& getImpl(ModMetadata& info);
        static ModMetadata::Impl const& getImpl(ModMetadata const& info);
    };
}
