#pragma once

#include <string>
#include <matjson.hpp>
#include <Geode/loader/Event.hpp>
#include <Geode/utils/function.hpp>
#include <Geode/utils/general.hpp>
#include <Geode/utils/web.hpp>

namespace geode::updater {
    using UpdateFinished = std::monostate;
    using UpdateProgress = std::pair<uint8_t, std::string>;
    using UpdateFailed = std::string;
    using UpdateStatus = std::variant<UpdateFinished, UpdateProgress, UpdateFailed>;

    class ResourceDownloadEvent : public ThreadSafeEvent<ResourceDownloadEvent, bool(UpdateStatus const&)> {
    public:
        // listener params status
        using ThreadSafeEvent::ThreadSafeEvent;
    };

    class LoaderUpdateEvent : public ThreadSafeEvent<LoaderUpdateEvent, bool(UpdateStatus const&)> {
    public:
        // listener params status
        using ThreadSafeEvent::ThreadSafeEvent;
    };

    void updateSpecialFiles();
    Result<> extractLoaderResources(ByteSpan data, std::string_view hash);
    void tryDownloadLoaderResources(std::string url, std::string hash, bool tryLatestOnError = true);
    void downloadLoaderResources(bool useLatestRelease = false);
    void downloadLatestLoaderResources();
    void downloadLoaderUpdate(std::string url, std::string hash);
    Result<> installLoaderUpdate(utils::web::WebResponse response, std::string_view expectedHash);

    bool verifyLoaderResources();
    void checkForLoaderUpdates();
    bool isNewUpdateDownloaded();
}
