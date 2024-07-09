#pragma once

#include "Geode/utils/VersionInfo.hpp"
#include <Geode/DefaultInclude.hpp>
#include <Geode/utils/web.hpp>
#include <Geode/loader/SettingEvent.hpp>
#include <chrono>
#include <matjson.hpp>
#include <vector>

using namespace geode::prelude;

namespace server {
    struct ServerDateTime final {
        using Clock = std::chrono::system_clock;
        using Value = std::chrono::time_point<Clock>;

        Value value;

        std::string toAgoString() const;

        static Result<ServerDateTime> parse(std::string const& str);
    };

    struct ServerDeveloper final {
        std::string username;
        std::string displayName;
        bool isOwner;
    };

    struct ServerModVersion final {
        ModMetadata metadata;
        std::string downloadURL;
        std::string hash;
        size_t downloadCount;

        bool operator==(ServerModVersion const&) const = default;

        static Result<ServerModVersion> parse(matjson::Value const& json);
    };
    
    struct ServerModReplacement final {
        std::string id;
        VersionInfo version;
        std::string download_link;

        static Result<ServerModReplacement> parse(matjson::Value const& json);
    };

    struct ServerModUpdate final {
        std::string id;
        VersionInfo version;
        std::optional<ServerModReplacement> replacement;

        static Result<ServerModUpdate> parse(matjson::Value const& json);
        static Result<std::vector<ServerModUpdate>> parseList(matjson::Value const& json);
        
        bool hasUpdateForInstalledMod() const;
    };

    struct ServerModMetadata final {
        std::string id;
        bool featured;
        size_t downloadCount;
        std::vector<ServerDeveloper> developers;
        std::vector<ServerModVersion> versions;
        std::unordered_set<std::string> tags;
        std::optional<std::string> about;
        std::optional<std::string> changelog;
        std::optional<std::string> repository;
        std::optional<ServerDateTime> createdAt;
        std::optional<ServerDateTime> updatedAt;

        static Result<ServerModMetadata> parse(matjson::Value const& json);

        ModMetadata latestVersion() const;
        std::string formatDevelopersToString() const;
        bool hasUpdateForInstalledMod() const;
    };

    struct ServerModsList final {
        std::vector<ServerModMetadata> mods;
        size_t totalModCount = 0;

        static Result<ServerModsList> parse(matjson::Value const& json);
    };

    enum class ModsSort {
        Downloads,
        RecentlyUpdated,
        RecentlyPublished,
    };

    static const char* sortToString(ModsSort sorting);

    struct ModsQuery final {
        std::optional<std::string> query;
        std::unordered_set<PlatformID> platforms = { GEODE_PLATFORM_TARGET };
        std::unordered_set<std::string> tags;
        std::optional<bool> featured;
        ModsSort sorting = ModsSort::Downloads;
        std::optional<std::string> developer;
        size_t page = 0;
        size_t pageSize = 10;
        
        bool operator==(ModsQuery const& other) const = default;
    };

    struct ServerError final {
        int code;
        std::string details;

        ServerError() = default;

        template <class... Args>
        ServerError(
            int code,
            fmt::string_view format,
            Args&&... args
        ) : code(code), details(fmt::vformat(format, fmt::make_format_args(args...))) {}
    };
    struct ServerProgress {
        std::string message;
        std::optional<uint8_t> percentage;

        ServerProgress() = default;
        ServerProgress(std::string const& msg) : message(msg) {}
        ServerProgress(auto msg, uint8_t percentage) : message(msg), percentage(percentage) {}
    };

    template <class T>
    using ServerRequest = Task<Result<T, ServerError>, ServerProgress>;

    struct ModVersionLatest final {
        bool operator==(ModVersionLatest const&) const = default;
    };
    struct ModVersionMajor final {
        size_t major;
        bool operator==(ModVersionMajor const&) const = default;
    };
    using ModVersionSpecific = VersionInfo;
    using ModVersion = std::variant<ModVersionLatest, ModVersionMajor, ModVersionSpecific>;

    std::string getServerAPIBaseURL();
    std::string getServerUserAgent();

    ServerRequest<ServerModsList> getMods(ModsQuery const& query, bool useCache = true);
    ServerRequest<ServerModMetadata> getMod(std::string const& id, bool useCache = true);
    ServerRequest<ServerModVersion> getModVersion(std::string const& id, ModVersion const& version = ModVersionLatest(), bool useCache = true);
    ServerRequest<ByteVector> getModLogo(std::string const& id, bool useCache = true);
    ServerRequest<std::unordered_set<std::string>> getTags(bool useCache = true);

    ServerRequest<std::optional<ServerModUpdate>> checkUpdates(Mod const* mod);
    ServerRequest<std::vector<ServerModUpdate>> checkAllUpdates(bool useCache = true);
    
    void clearServerCaches(bool clearGlobalCaches = false);
}
