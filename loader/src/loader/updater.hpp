#pragma once

#include <string>
#include <matjson.hpp>
#include <Geode/loader/Event.hpp>

namespace geode::updater {
    using UpdateFinished = std::monostate;
    using UpdateProgress = std::pair<uint8_t, std::string>;
    using UpdateFailed = std::string;
    using UpdateStatus = std::variant<UpdateFinished, UpdateProgress, UpdateFailed>;

    struct ResourceDownloadEvent : public Event {
        const UpdateStatus status;
        explicit ResourceDownloadEvent(UpdateStatus status);
    };

    class ResourceDownloadFilter : public EventFilter<ResourceDownloadEvent> {
    public:
        using Callback = void(ResourceDownloadEvent*);

        static ListenerResult handle(const std::function<Callback>& fn, ResourceDownloadEvent* event);
        ResourceDownloadFilter();
    };

    struct LoaderUpdateEvent : public Event {
        const UpdateStatus status;
        explicit LoaderUpdateEvent(UpdateStatus status);
    };

    class LoaderUpdateFilter : public EventFilter<LoaderUpdateEvent> {
    public:
        using Callback = void(LoaderUpdateEvent*);

        static ListenerResult handle(const std::function<Callback>& fn, LoaderUpdateEvent* event);
        LoaderUpdateFilter();
    };

    void updateSpecialFiles();
    void tryDownloadLoaderResources(std::string const& url, bool tryLatestOnError = true);
    void downloadLoaderResources(bool useLatestRelease = false);
    void downloadLatestLoaderResources();
    void downloadLoaderUpdate(std::string const& url);
    void fetchLatestGithubRelease(
        const std::function<void(matjson::Value const&)>& then,
        std::function<void(std::string const&)> expect,
        bool force = false
    );

    bool verifyLoaderResources();
    void checkForLoaderUpdates();
    bool isNewUpdateDownloaded();
}
