#pragma once

#include "Server.hpp"
#include <asp/time/SystemTime.hpp>

namespace server {
    struct DownloadStatusFetching {
        uint8_t percentage;
        bool operator==(DownloadStatusFetching const&) const = default;
    };
    struct DownloadStatusConfirm {
        ServerModVersion version;
        bool operator==(DownloadStatusConfirm const&) const = default;
    };
    struct DownloadStatusDownloading {
        uint8_t percentage;
        bool operator==(DownloadStatusDownloading const&) const = default;
    };
    struct DownloadStatusDone {
        ServerModVersion version;
        bool operator==(DownloadStatusDone const&) const = default;
    };
    struct DownloadStatusError {
        std::string details;
        bool operator==(DownloadStatusError const&) const = default;
    };
    struct DownloadStatusCancelled {
        bool operator==(DownloadStatusCancelled const&) const = default;
    };

    using DownloadStatus = std::variant<
        DownloadStatusFetching,
        DownloadStatusConfirm,
        DownloadStatusDownloading,
        DownloadStatusDone,
        DownloadStatusError,
        DownloadStatusCancelled
    >;

    class ModDownloadEvent : public GlobalEvent<ModDownloadEvent, bool(std::string_view), bool(), std::string> {
    public:
        // filter params id
        using GlobalEvent::GlobalEvent;
    };

    using DependencyFor = std::pair<std::string, bool>;

    class ModDownload final {
    private:
        class Impl;

        std::shared_ptr<Impl> m_impl;

        ModDownload(
            std::string id,
            std::optional<VersionInfo> version,
            std::optional<DependencyFor> dependencyFor,
            std::optional<std::string> replacesMod
        );

        friend class ModDownloadManager;

    public:
        void confirm();
        void cancel();

        bool isDone() const;
        bool isActive() const;
        bool canRetry() const;
        std::optional<std::string> getReplacesMod() const;
        std::optional<DependencyFor> getDependencyFor() const;
        std::string getID() const;
        DownloadStatus getStatus() const;
        std::optional<VersionInfo> getVersion() const;
    };

    struct RecentlyUpdatedMod final {
        std::string modID;
        asp::SystemTime updateTime;
    };

    class ModDownloadManager final {
    private:
        class Impl;

        std::unique_ptr<Impl> m_impl;

        ModDownloadManager();

        friend class ModDownload;

    public:
        static ModDownloadManager* get();
        ~ModDownloadManager();

        std::optional<ModDownload> startDownload(
            std::string id,
            std::optional<VersionInfo> version,
            std::optional<DependencyFor> dependencyFor = std::nullopt,
            std::optional<std::string> replacesMod = std::nullopt
        );
        void startUpdateAll();
        void confirmAll();
        void cancelAll();
        void dismissAll();
        bool checkAutoConfirm();

        std::optional<ModDownload> getDownload(std::string_view id) const;
        std::vector<ModDownload> getDownloads() const;
        bool hasActiveDownloads() const;

        bool wantsRestart() const;

        void markRecentlyUpdated(std::string_view id);
        std::vector<RecentlyUpdatedMod> const& getRecentlyUpdatedMods();
    };
}

template <>
struct matjson::Serialize<server::RecentlyUpdatedMod> {
    static Value toJson(server::RecentlyUpdatedMod const& value) {
        return matjson::makeObject({
            { "id", value.modID },
            { "time", value.updateTime.timeSinceEpoch().seconds()},
        });
    }
    static geode::Result<server::RecentlyUpdatedMod> fromJson(Value const& value) {
        return geode::Ok(server::RecentlyUpdatedMod {
            .modID = GEODE_UNWRAP(value["id"].asString()),
            .updateTime = asp::SystemTime::fromUnix(GEODE_UNWRAP(value["time"].asUInt())),
        });
    }
};
