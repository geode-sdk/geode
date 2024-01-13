#pragma once

#include <string>
#include <Geode/utils/MiniFunction.hpp>

namespace geode::updater {
    void updateSpecialFiles();
    void tryDownloadLoaderResources(std::string const& url, bool tryLatestOnError = true);
    void downloadLoaderResources(bool useLatestRelease = false);
    void downloadLoaderUpdate(std::string const& url);
    void fetchLatestGithubRelease(
        const utils::MiniFunction<void(matjson::Value const&)>& then,
        utils::MiniFunction<void(std::string const&)> expect
    );

    bool verifyLoaderResources();
    void checkForLoaderUpdates();
    bool isNewUpdateDownloaded();
}
