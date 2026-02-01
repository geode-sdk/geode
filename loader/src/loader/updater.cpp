#include "updater.hpp"
#include <Geode/utils/web.hpp>
#include <resources.hpp>
#include <hash.hpp>
#include <utility>
#include "LoaderImpl.hpp"
#include "ModMetadataImpl.hpp"
#include <Geode/utils/string.hpp>
#include <Geode/utils/StringMap.hpp>

#include "../server/Server.hpp"

using namespace geode::prelude;

static StringMap<async::TaskHolder<web::WebResponse>> RUNNING_REQUESTS {};

bool s_isNewUpdateDownloaded = false;

namespace {
    inline std::string formatDownloadUrl(std::string_view tag) {
        return fmt::format("https://github.com/geode-sdk/geode/releases/download/{0}/geode-{0}-{1}.zip", tag, GEODE_PLATFORM_SHORT_IDENTIFIER_NOARCH);
    }

    inline std::string formatResourcesUrl(std::string_view tag) {
        return fmt::format("https://github.com/geode-sdk/geode/releases/download/{}/resources.zip", tag);
    }
}

void updater::downloadLatestLoaderResources() {
    log::debug("Downloading latest resources");

    async::spawn(
        server::getLatestLoaderVersion(),
        [](Result<server::ServerLoaderVersion, server::ServerError> res) {
            if (res.ok()) {
                auto& release = res.unwrap();

                updater::tryDownloadLoaderResources(
                    formatResourcesUrl(release.tag),
                    false
                );
            } else {
                ResourceDownloadEvent().send(
                    UpdateFailed("Unable to download resources: " + res.unwrapErr().details)
                );
            }
        }
    );
}

void updater::tryDownloadLoaderResources(std::string url, bool tryLatestOnError) {
    if (RUNNING_REQUESTS.contains(url)) return;

    // TODO: progress
    // ResourceDownloadEvent().send(
    //     UpdateProgress(
    //         static_cast<uint8_t>(progress->downloadProgress().value_or(0)),
    //         "Downloading resources"
    //     )
    // );

    auto& holder = RUNNING_REQUESTS[url];
    holder.spawn(
        "Geode resources download",
        web::WebRequest{}.get(url),
        [url](auto response) {
            if (response.ok()) {
                auto tempResourcesZip = dirs::getTempDir() / "new.zip";
                auto resourcesDir = dirs::getGeodeResourcesDir() / Mod::get()->getID();

                // unzip resources zip
                auto data = std::move(response).data();
                auto unzip = file::Unzip::create(data);
                if (unzip) {
                    auto ok = unzip.unwrap().extractAllTo(resourcesDir);
                    if (ok) {
                        updater::updateSpecialFiles();
                        ResourceDownloadEvent().send(UpdateFinished());
                    }
                    else {
                        ResourceDownloadEvent().send(UpdateFailed("Unable to unzip new resources: " + ok.unwrapErr()));
                    }
                }
                else {
                    ResourceDownloadEvent().send(UpdateFailed("Unable to unzip new resources: " + unzip.unwrapErr()));
                }
            }
            else {
                auto reason = response.string().unwrapOr("Unknown");
                // if the url was not found, try downloading latest release instead
                // (for development versions)
                if (response.code() == 404) {
                    log::warn("Unable to download resources: {}", reason);
                }
                ResourceDownloadEvent().send(
                    UpdateFailed("Unable to download resources: " + reason)
                );
            }
            RUNNING_REQUESTS.erase(url);
        }
    );
}

void updater::updateSpecialFiles() {
    auto resourcesDir = dirs::getGeodeResourcesDir() / Mod::get()->getID();
    auto res = ModMetadataImpl::getImpl(ModImpl::get()->m_metadata).addSpecialFiles(resourcesDir);
    if (res.isErr()) {
        log::warn("Unable to add special files: {}", res.unwrapErr());
    }
}

void updater::downloadLoaderResources(bool useLatestRelease) {
    static bool DOWNLOADING_LOADER_RESOURCES = false;

    if (DOWNLOADING_LOADER_RESOURCES) return;
    DOWNLOADING_LOADER_RESOURCES = true;

    async::spawn(
        server::getLoaderVersion(Loader::get()->getVersion().toNonVString()),
        [useLatestRelease](Result<server::ServerLoaderVersion, server::ServerError> res) {
            if (res.ok()) {
                auto& release = res.unwrap();

                updater::tryDownloadLoaderResources(
                    formatResourcesUrl(release.tag), false
                );

                DOWNLOADING_LOADER_RESOURCES = false;
                return;
            }
            if (useLatestRelease) {
                log::info("Loader version {} does not exist, trying to download latest resources", Loader::get()->getVersion().toVString());
                downloadLatestLoaderResources();
            }
            else {
                log::warn("Loader version {} does not exist on GitHub, not downloading the resources", Loader::get()->getVersion().toVString());
                ResourceDownloadEvent().send(UpdateFinished());
            }

            DOWNLOADING_LOADER_RESOURCES = false;
        }
    );

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
        std::filesystem::exists(resourcesDir) &&
            std::filesystem::is_directory(resourcesDir)
    )) {
        log::debug("Resources directory does not exist");
        updater::downloadLoaderResources(true);
        return false;
    }

    // TODO: actually have a proper way to disable checking resources
    // for development builds
    if (std::filesystem::exists(resourcesDir / "dont-update.txt")) {
        // this is kind of a hack, but it's the easiest way to prevent
        // auto update while developing
        log::debug("Not updating resources since dont-update.txt exists");
        return true;
    }

    // make sure every file was covered
    size_t coverage = 0;

    // verify hashes
    for (auto& file : std::filesystem::directory_iterator(resourcesDir)) {
        auto name = utils::string::pathToString(file.path().filename());
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

void updater::downloadLoaderUpdate(std::string url) {
    if (RUNNING_REQUESTS.contains("@downloadLoaderUpdate")) return;

    auto req = web::WebRequest();
    req.onProgress([](web::WebProgress const& progress) {
        LoaderUpdateEvent().send(
            UpdateProgress(
                static_cast<uint8_t>(progress.downloadProgress().value_or(0)),
                "Downloading update"
            )
        );
    });

    auto& holder = RUNNING_REQUESTS["@downloadLoaderUpdate"];
    holder.spawn(
        req.get(std::move(url)),
        [](web::WebResponse response) {
            RUNNING_REQUESTS.erase("@downloadLoaderUpdate");

            auto updateZip = dirs::getTempDir() / "loader-update.zip";
            auto targetDir = dirs::getGeodeDir() / "update";

            if (response.ok()) {
                // unzip resources zip
                auto data = std::move(response).data();
                auto unzip = file::Unzip::create(data);
                if (unzip) {
                    auto ok = unzip.unwrap().extractAllTo(targetDir);
                    if (ok) {
                        s_isNewUpdateDownloaded = true;
                        LoaderUpdateEvent().send(UpdateFinished());
                    }
                    else {
                        LoaderUpdateEvent().send(
                            UpdateFailed("Unable to unzip update: " + ok.unwrapErr())
                        );
                        Mod::get()->setSavedValue("last-modified-auto-update-check", std::string());
                    }
                }
                else {
                    LoaderUpdateEvent().send(
                        UpdateFailed("Unable to unzip update: " + unzip.unwrapErr())
                    );
                    Mod::get()->setSavedValue("last-modified-auto-update-check", std::string());
                }
            }
            else {
                auto info = response.string().unwrapOr("Unknown error");
                log::error("Failed to download latest update {}", info);
                LoaderUpdateEvent().send(
                    UpdateFailed("Unable to download update: " + info)
                );

                Mod::get()->setSavedValue("last-modified-auto-update-check", std::string());
            }
        }
    );
}

void updater::checkForLoaderUpdates() {
    // Check for updates in the background
    async::spawn(
        server::getLatestLoaderVersion(),
        [](Result<server::ServerLoaderVersion, server::ServerError> res) {
            if (res.ok()) {
                auto& release = res.unwrap();
                auto ver = VersionInfo::parse(release.tag).unwrapOrDefault();

                log::info("Latest Geode version is {}", ver.toVString());
                Mod::get()->setSavedValue("latest-version-auto-update-check", ver.toVString());

                // make sure release is newer
                if (ver <= Loader::get()->getVersion()) {
                    if(ver <= VersionInfo(2, 0, 0, VersionTag(VersionTag::Beta, 1))) {
                        log::warn("Invalid loader version detected, resetting update check time");

                        Mod::get()->setSavedValue("last-modified-auto-update-check", std::string());
                    }
                    return;
                }

                // find release asset
                updater::downloadLoaderUpdate(formatDownloadUrl(release.tag));
            } else {
                auto info = res.unwrapErr().details;
                log::error("Failed to fetch updates {}", info);
                LoaderUpdateEvent().send(
                    UpdateFailed("Unable to check for updates: " + info)
                );
            }
        }
    );
}

bool updater::isNewUpdateDownloaded() {
    return s_isNewUpdateDownloaded;
}
