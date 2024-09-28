#pragma once

#include "Server.hpp"

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

    struct ModDownloadEvent : public Event {
        std::string id;
        ModDownloadEvent(std::string const& id);
    };

    class ModDownloadFilter : public EventFilter<ModDownloadEvent> {
    public:
        using Callback = void(ModDownloadEvent*);

    protected:
        std::string m_id;

    public:
        ListenerResult handle(MiniFunction<Callback> fn, ModDownloadEvent* event);

        ModDownloadFilter();
        ModDownloadFilter(std::string const& id);
    };

    using DependencyFor = std::pair<std::string, ModMetadata::Dependency::Importance>;

    class ModDownload final {
    private:
        class Impl;

        std::shared_ptr<Impl> m_impl;

        ModDownload(
            std::string const& id,
            std::optional<VersionInfo> const& version,
            std::optional<DependencyFor> const& dependencyFor,
            std::optional<std::string> const& replacesMod
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
            std::string const& id,
            std::optional<VersionInfo> const& version,
            std::optional<DependencyFor> const& dependencyFor = std::nullopt,
            std::optional<std::string> const& replacesMod = std::nullopt
        );
        void startUpdateAll();
        void confirmAll();
        void cancelAll();
        void dismissAll();
        bool checkAutoConfirm();

        std::optional<ModDownload> getDownload(std::string const& id) const;
        std::vector<ModDownload> getDownloads() const;
        bool hasActiveDownloads() const;

        bool wantsRestart() const;
    };
}
