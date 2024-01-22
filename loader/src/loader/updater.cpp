#include "updater.hpp"
#include <Geode/utils/web.hpp>
#include <Geode/loader/Index.hpp>
#include <resources.hpp>
#include <hash.hpp>
#include <utility>
#include "LoaderImpl.hpp"
#include "ModMetadataImpl.hpp"

using namespace geode::prelude;

updater::ResourceDownloadEvent::ResourceDownloadEvent(
    UpdateStatus status
) : status(std::move(status)) {}

ListenerResult updater::ResourceDownloadFilter::handle(
    const utils::MiniFunction<Callback>& fn,
    ResourceDownloadEvent* event
) {
    fn(event);
    return ListenerResult::Propagate;
}

updater::ResourceDownloadFilter::ResourceDownloadFilter() = default;

updater::LoaderUpdateEvent::LoaderUpdateEvent(
    UpdateStatus status
) : status(std::move(status)) {}

ListenerResult updater::LoaderUpdateFilter::handle(
    const utils::MiniFunction<Callback>& fn,
    LoaderUpdateEvent* event
) {
    fn(event);
    return ListenerResult::Propagate;
}

updater::LoaderUpdateFilter::LoaderUpdateFilter() = default;

// cache for the json of the latest github release to avoid hitting
// the github api too much
std::optional<matjson::Value> s_latestGithubRelease;
bool s_isNewUpdateDownloaded = false;

void updater::fetchLatestGithubRelease(
    const utils::MiniFunction<void(matjson::Value const&)>& then,
    utils::MiniFunction<void(std::string const&)> expect
) {
    if (s_latestGithubRelease) {
        return then(s_latestGithubRelease.value());
    }
    // TODO: add header to not get rate limited
    web::AsyncWebRequest()
        .join("loader-auto-update-check")
        .userAgent("github_api/1.0")
        .fetch("https://api.github.com/repos/geode-sdk/geode/releases/latest")
        .json()
        .then([then](matjson::Value const& json) {
            s_latestGithubRelease = json;
            then(json);
        })
        .expect(std::move(expect));
}

void updater::tryDownloadLoaderResources(
    std::string const& url,
    bool tryLatestOnError
) {
    auto tempResourcesZip = dirs::getTempDir() / "new.zip";
    auto resourcesDir = dirs::getGeodeResourcesDir() / Mod::get()->getID();

    web::AsyncWebRequest()
        // use the url as a join handle
        .join(url)
        .fetch(url)
        .into(tempResourcesZip)
        .then([tempResourcesZip, resourcesDir](auto) {
            // unzip resources zip
            auto unzip = file::Unzip::intoDir(tempResourcesZip, resourcesDir, true);
            if (!unzip) {
                ResourceDownloadEvent(
                    UpdateFailed("Unable to unzip new resources: " + unzip.unwrapErr())
                ).post();
                return;
            }
            updater::updateSpecialFiles();

            ResourceDownloadEvent(UpdateFinished()).post();
        })
        .expect([tryLatestOnError](std::string const& info, int code) {
            // if the url was not found, try downloading latest release instead
            // (for development versions)
            if (code == 404) {
                log::warn("Unable to download resources: {}", info);
            }
            ResourceDownloadEvent(
                UpdateFailed("Unable to download resources: " + info)
            ).post();
        })
        .progress([](auto&, double now, double total) {
            ResourceDownloadEvent(
                UpdateProgress(
                    static_cast<uint8_t>(now / total * 100.0),
                    "Downloading resources"
                )
            ).post();
        });
}

void updater::updateSpecialFiles() {
    auto resourcesDir = dirs::getGeodeResourcesDir() / Mod::get()->getID();
    auto res = ModMetadataImpl::getImpl(ModImpl::get()->m_metadata).addSpecialFiles(resourcesDir);
    if (res.isErr()) {
        log::warn("Unable to add special files: {}", res.unwrapErr());
    }
}

void updater::downloadLoaderResources(bool useLatestRelease) {
    web::AsyncWebRequest()
        .join("loader-tag-exists-check")
        .userAgent("github_api/1.0")
        .fetch(fmt::format(
            "https://api.github.com/repos/geode-sdk/geode/git/ref/tags/{}",
            Loader::get()->getVersion().toString()
        ))
        .json()
        .then([](matjson::Value const& json) {
            updater::tryDownloadLoaderResources(fmt::format(
                "https://github.com/geode-sdk/geode/releases/download/{}/resources.zip",
                Loader::get()->getVersion().toString()
            ), true);
        })
        .expect([=](std::string const& info, int code) {
            if (code == 404) {
                if (useLatestRelease) {
                    log::debug("Loader version {} does not exist on Github, downloading latest resources", Loader::get()->getVersion().toString());
                    fetchLatestGithubRelease(
                        [](matjson::Value const& raw) {
                            auto json = raw;
                            JsonChecker checker(json);
                            auto root = checker.root("[]").obj();

                            // find release asset
                            for (auto asset : root.needs("assets").iterate()) {
                                auto obj = asset.obj();
                                if (obj.needs("name").template get<std::string>() == "resources.zip") {
                                    updater::tryDownloadLoaderResources(
                                        obj.needs("browser_download_url").template get<std::string>(),
                                        false
                                    );
                                    return;
                                }
                            }

                            ResourceDownloadEvent(
                                UpdateFailed("Unable to find resources in latest GitHub release")
                            ).post();
                        },
                        [](std::string const& info) {
                            ResourceDownloadEvent(
                                UpdateFailed("Unable to download resources: " + info)
                            ).post();
                        }
                    );
                    return;
                }
                else {
                    log::debug("Loader version {} does not exist on GitHub, not downloading the resources", Loader::get()->getVersion().toString());
                }
                ResourceDownloadEvent(
                    UpdateFinished()
                ).post();
            }
            else {
                ResourceDownloadEvent(
                    UpdateFailed("Unable to check if tag exists: " + info)
                ).post();
            }
        });
}

bool updater::verifyLoaderResources() {
    static std::optional<bool> CACHED = std::nullopt;
    if (CACHED.has_value()) {
        return CACHED.value();
    }

    // geode/resources/geode.loader
    auto resourcesDir = dirs::getGeodeResourcesDir() / Mod::get()->getID();

    // if the resources dir doesn't exist, then it's probably incorrect
    if (!(
        ghc::filesystem::exists(resourcesDir) &&
            ghc::filesystem::is_directory(resourcesDir)
    )) {
        log::debug("Resources directory does not exist");
        updater::downloadLoaderResources(true);
        return false;
    }

    // TODO: actually have a proper way to disable checking resources
    // for development builds
    if (ghc::filesystem::exists(resourcesDir / "dont-update.txt")) {
        // this is kind of a hack, but it's the easiest way to prevent
        // auto update while developing
        log::debug("Not updating resources since dont-update.txt exists");
        return true;
    }

    // make sure every file was covered
    size_t coverage = 0;

    // verify hashes
    for (auto& file : ghc::filesystem::directory_iterator(resourcesDir)) {
        auto name = file.path().filename().string();
        // skip unknown files
        if (!LOADER_RESOURCE_HASHES.count(name)) {
            continue;
        }
        // verify hash
        // if we hash anything other than text, change this
        auto hash = calculateSHA256Text(file.path());
        const auto& expected = LOADER_RESOURCE_HASHES.at(name);
        if (hash != expected) {
            log::debug("Resource hash mismatch: {} ({}, {})", name, hash.substr(0, 7), expected.substr(0, 7));
            updater::downloadLoaderResources();
            return false;
        }
        coverage += 1;
    }

    // make sure every file was found
    if (coverage != LOADER_RESOURCE_HASHES.size()) {
        log::debug("Resource coverage mismatch");
        updater::downloadLoaderResources();
        return false;
    }

    return true;
}

void updater::downloadLoaderUpdate(std::string const& url) {
    auto updateZip = dirs::getTempDir() / "loader-update.zip";
    auto targetDir = dirs::getGeodeDir() / "update";

    web::AsyncWebRequest()
        .join("loader-update-download")
        .fetch(url)
        .into(updateZip)
        .then([updateZip, targetDir](auto) {
            // unzip resources zip
            auto unzip = file::Unzip::intoDir(updateZip, targetDir, true);
            if (!unzip) {
                LoaderUpdateEvent(
                    UpdateFailed("Unable to unzip update: " + unzip.unwrapErr())
                ).post();
                return;
            }
            s_isNewUpdateDownloaded = true;
            LoaderUpdateEvent(UpdateFinished()).post();
        })
        .expect([](std::string const& info) {
            LoaderUpdateEvent(
                UpdateFailed("Unable to download update: " + info)
            ).post();
        })
        .progress([](auto&, double now, double total) {
            LoaderUpdateEvent(
                UpdateProgress(
                    static_cast<uint8_t>(now / total * 100.0),
                    "Downloading update"
                )
            ).post();
        });
}

void updater::checkForLoaderUpdates() {
    // Check for updates in the background
    fetchLatestGithubRelease(
        [](matjson::Value const& raw) {
            auto json = raw;
            JsonChecker checker(json);
            auto root = checker.root("[]").obj();

            VersionInfo ver { 0, 0, 0 };
            root.needs("tag_name").into(ver);

            // make sure release is newer
            if (ver <= Loader::get()->getVersion()) {
                return;
            }

            // don't auto-update major versions
            if (ver.getMajor() > Loader::get()->getVersion().getMajor()) {
                return;
            }

            // find release asset
            for (auto asset : root.needs("assets").iterate()) {
                auto obj = asset.obj();
                if (string::endsWith(
                    obj.needs("name").template get<std::string>(),
                    GEODE_PLATFORM_SHORT_IDENTIFIER ".zip"
                )) {
                    updater::downloadLoaderUpdate(
                        obj.needs("browser_download_url").template get<std::string>()
                    );
                    return;
                }
            }

            LoaderUpdateEvent(
                UpdateFailed("Unable to find release asset for " GEODE_PLATFORM_NAME)
            ).post();
        },
        [](std::string const& info) {
            LoaderUpdateEvent(
                UpdateFailed("Unable to check for updates: " + info)
            ).post();
        }
    );
}

bool updater::isNewUpdateDownloaded() {
    return s_isNewUpdateDownloaded;
}
