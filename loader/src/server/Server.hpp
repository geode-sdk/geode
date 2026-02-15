#pragma once
#include "Geode/utils/VersionInfo.hpp"

#include <Geode/DefaultInclude.hpp>
#include <Geode/utils/web.hpp>
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

    struct ServerTag final {
        size_t id;
        std::string name;
        std::string displayName;
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
    };

    struct ServerModUpdate final {
        std::string id;
        VersionInfo version;

        Mod* hasUpdateForInstalledMod() const;
    };

    struct ServerModDeprecation final {
        std::string id;
        std::vector<std::string> by;
        std::string reason;

        Mod* hasDeprecationForInstalledMod() const;
    };

    struct ServerModUpdateAllCheck final {
        std::vector<ServerModUpdate> updates;
        std::vector<ServerModDeprecation> deprecations;
    };

    struct ServerModUpdateOneCheck final {
        std::optional<ServerModUpdate> update;
        std::optional<ServerModDeprecation> deprecation;
    };

    struct ServerModLinks final {
        std::optional<std::string> community;
        std::optional<std::string> homepage;
        std::optional<std::string> source;
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

        ModMetadata latestVersion() const;
        std::string formatDevelopersToString() const;
        Mod* hasUpdateForInstalledMod() const;
    };

    struct ServerModsList final {
        std::vector<ServerModMetadata> mods;
        size_t totalModCount = 0;
    };

    struct ServerLoaderVersion final {
        std::string version;
        std::string tag;
        std::string commitHash;
        std::string gameVersion;
    };

    enum class ModsSort {
        Downloads,
        RecentlyUpdated,
        RecentlyPublished,
    };

    static char const* sortToString(ModsSort sorting);

    struct ModsQuery final {
        std::optional<std::string> query;
        std::unordered_set<PlatformID> platforms = {GEODE_PLATFORM_TARGET};
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
        ServerError(int code, fmt::string_view format, Args&&... args) :
            code(code), details(fmt::vformat(format, fmt::make_format_args(args...))) {}
    };

    struct ServerProgress {
        std::string message;
        std::optional<uint8_t> percentage;

        ServerProgress() = default;

        ServerProgress(std::string msg) : message(std::move(msg)) {}

        ServerProgress(std::string msg, std::optional<uint8_t> percentage) :
            message(std::move(msg)), percentage(percentage) {}
    };

    template <class T>
    using ServerResult = Result<T, ServerError>;

    template <class T>
    using ServerFuture = arc::Future<ServerResult<T>>;

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

    ServerFuture<ServerModsList> getMods(ModsQuery query, bool useCache = true);
    ServerFuture<ServerModMetadata> getMod(std::string id, bool useCache = true);
    ServerFuture<ServerModVersion> getModVersion(
        std::string id, ModVersion version = ModVersionLatest(), bool useCache = true
    );
    ServerFuture<ByteVector> getModLogo(std::string id, bool useCache = true);
    ServerFuture<std::vector<ServerTag>> getTags(bool useCache = true);

    ServerFuture<ServerModUpdateOneCheck> checkUpdates(Mod const* mod);
    ServerFuture<ServerModUpdateAllCheck> batchedCheckUpdates(std::vector<std::string> const& batch);
    ServerFuture<ServerModUpdateAllCheck> checkAllUpdates(bool useCache = true);

    ServerFuture<ServerLoaderVersion> getLoaderVersion(std::string tag, bool useCache = true);
    ServerFuture<ServerLoaderVersion> getLatestLoaderVersion(bool useCache = true);

    void clearServerCaches(bool clearGlobalCaches = false);
}

template <>
struct matjson::Serialize<server::ServerTag> {
    static geode::Result<server::ServerTag> fromJson(matjson::Value const& value);
};

template <>
struct matjson::Serialize<server::ServerDeveloper> {
    static geode::Result<server::ServerDeveloper> fromJson(matjson::Value const& value);
};

template <>
struct matjson::Serialize<server::ServerModVersion> {
    static geode::Result<server::ServerModVersion> fromJson(matjson::Value const& value);
};

template <>
struct matjson::Serialize<server::ServerModUpdate> {
    static geode::Result<server::ServerModUpdate> fromJson(matjson::Value const& value);
};

template <>
struct matjson::Serialize<server::ServerModDeprecation> {
    static geode::Result<server::ServerModDeprecation> fromJson(matjson::Value const& value);
};

template <>
struct matjson::Serialize<server::ServerModUpdateAllCheck> {
    static geode::Result<server::ServerModUpdateAllCheck> fromJson(matjson::Value const& value);
};

template <>
struct matjson::Serialize<server::ServerModLinks> {
    static geode::Result<server::ServerModLinks> fromJson(matjson::Value const& value);
};

template <>
struct matjson::Serialize<server::ServerModMetadata> {
    static geode::Result<server::ServerModMetadata> fromJson(matjson::Value const& value);
};

template <>
struct matjson::Serialize<server::ServerModsList> {
    static geode::Result<server::ServerModsList> fromJson(matjson::Value const& value);
};

template <>
struct matjson::Serialize<server::ServerLoaderVersion> {
    static geode::Result<server::ServerLoaderVersion> fromJson(matjson::Value const& value);
};
