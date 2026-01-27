#include "updater.hpp"
#include <Geode/utils/web.hpp>
#include <resources.hpp>
#include <hash.hpp>
#include <utility>
#include "LoaderImpl.hpp"
#include "ModMetadataImpl.hpp"
#include <Geode/utils/string.hpp>
#include <Geode/utils/StringMap.hpp>

using namespace geode::prelude;

static StringMap<web::WebTask> RUNNING_REQUESTS {};

updater::ResourceDownloadEvent::ResourceDownloadEvent(
    UpdateStatus status
) : status(std::move(status)) {}

updater::ResourceDownloadFilter::ResourceDownloadFilter() = default;

updater::LoaderUpdateEvent::LoaderUpdateEvent(
    UpdateStatus status
) : status(std::move(status)) {}

updater::LoaderUpdateFilter::LoaderUpdateFilter() = default;

// cache for the json of the latest github release to avoid hitting
// the github api too much
std::optional<matjson::Value> s_latestGithubRelease;
bool s_isNewUpdateDownloaded = false;

void updater::fetchLatestGithubRelease(
    geode::Function<void(matjson::Value const&)> then,
    geode::Function<void(std::string)> expect, bool force
) {
    if (s_latestGithubRelease) {
        return then(*s_latestGithubRelease);
    }

    //quick hack to make sure it always attempts an update check in forward compat
    if(Loader::get()->isForwardCompatMode()) {
        force = true;
    }

    auto version = VersionInfo::parse(
        Mod::get()->getSavedValue("latest-version-auto-update-check", std::string("0.0.0"))
    );

    log::debug("Last update check result: {}", version.unwrap().toVString());

    std::string modifiedSince;
    if (!force && version && version.unwrap() <= Mod::get()->getVersion() && version.unwrap() != VersionInfo(0, 0, 0)) {
        modifiedSince = Mod::get()->getSavedValue("last-modified-auto-update-check", std::string());
    }

    if (RUNNING_REQUESTS.contains("@loaderAutoUpdateCheck")) return;

    auto req = web::WebRequest();
    req.header("If-Modified-Since", modifiedSince);
    req.userAgent("github_api/1.0");
    RUNNING_REQUESTS.emplace(
        "@loaderAutoUpdateCheck",
        req.get("https://api.github.com/repos/geode-sdk/geode/releases/latest").map(
            [expect = std::move(expect), then = std::move(then)](web::WebResponse* response) mutable {
                if (response->ok()) {
                    if (response->data().empty()) {
                        expect("Empty response");
                    }
                    else {
                        auto json = response->json();
                        if (!json) {
                            expect("Not a JSON response");
                        }
                        else {
                            Mod::get()->setSavedValue("last-modified-auto-update-check", response->header("Last-Modified").value_or(""));
                            s_latestGithubRelease = std::move(json).unwrap();
                            then(*s_latestGithubRelease);
                        }
                    }
                } 
                else if (response->code() == 304) {
                    log::debug("No Geode update available");
                    // don't invoke either of the callbacks, since nothing changed
                }
                else {
                    log::debug("Code: {}", response->code());
                    expect(response->string().unwrapOr("Unknown error"));
                }
                RUNNING_REQUESTS.erase("@loaderAutoUpdateCheck");
                return *response;
            }
        )
    );
}

void updater::downloadLatestLoaderResources() {
    log::debug("Downloading latest resources", Loader::get()->getVersion().toVString());
    fetchLatestGithubRelease(
        [](matjson::Value const& raw) {
            auto root = checkJson(raw, "[]");

            // find release asset
            for (auto& obj : root.needs("assets").items()) {
                if (obj.needs("name").get<std::string>() == "resources.zip") {
                    updater::tryDownloadLoaderResources(
                        obj.needs("browser_download_url").get<std::string>(),
                        false
                    );
                    return;
                }
            }

            ResourceDownloadEvent(
                UpdateFailed("Unable to find resources in latest GitHub release")
            ).post();
        },
        [](std::string info) {
            ResourceDownloadEvent(
                UpdateFailed("Unable to download resources: " + info)
            ).post();
        },
        true
    );
}

void updater::tryDownloadLoaderResources(std::string url, bool tryLatestOnError) {
    auto tempResourcesZip = dirs::getTempDir() / "new.zip";
    auto resourcesDir = dirs::getGeodeResourcesDir() / Mod::get()->getID();

    if (RUNNING_REQUESTS.contains(url)) return;

    auto req = web::WebRequest{}.get(url).map(
        [url, resourcesDir = std::move(resourcesDir)](web::WebResponse* response) {
            if (response->ok()) {
                // unzip resources zip
                auto data = response->data();
                auto unzip = file::Unzip::create(data);
                if (unzip) {
                    auto ok = unzip.unwrap().extractAllTo(resourcesDir);
                    if (ok) {
                        updater::updateSpecialFiles();
                        ResourceDownloadEvent(UpdateFinished()).post();
                    }
                    else {
                        ResourceDownloadEvent(
                            UpdateFailed("Unable to unzip new resources: " + ok.unwrapErr())
                        ).post();
                    }
                }
                else {
                    ResourceDownloadEvent(UpdateFailed("Unable to unzip new resources: " + unzip.unwrapErr())).post();
                }
            }
            else {
                auto reason = response->string().unwrapOr("Unknown");
                // if the url was not found, try downloading latest release instead
                // (for development versions)
                if (response->code() == 404) {
                    log::warn("Unable to download resources: {}", reason);
                }
                ResourceDownloadEvent(
                    UpdateFailed("Unable to download resources: " + reason)
                ).post();
            }
            RUNNING_REQUESTS.erase(url);
            return *response;
        },
        [](web::WebProgress* progress) {
            ResourceDownloadEvent(
                UpdateProgress(
                    static_cast<uint8_t>(progress->downloadProgress().value_or(0)),
                    "Downloading resources"
                )
            ).post();
            return *progress;
        }
    );

    RUNNING_REQUESTS.emplace(std::move(url), std::move(req));
}

void updater::updateSpecialFiles() {
    auto resourcesDir = dirs::getGeodeResourcesDir() / Mod::get()->getID();
    auto res = ModMetadataImpl::getImpl(ModImpl::get()->m_metadata).addSpecialFiles(resourcesDir);
    if (res.isErr()) {
        log::warn("Unable to add special files: {}", res.unwrapErr());
    }
}

void updater::downloadLoaderResources(bool useLatestRelease) {
    if (RUNNING_REQUESTS.contains("@downloadLoaderResources")) return;

    auto req = web::WebRequest();
    req.header("If-Modified-Since", Mod::get()->getSavedValue("last-modified-tag-exists-check", std::string()));
    req.userAgent("github_api/1.0");
    RUNNING_REQUESTS.emplace(
        "@downloadLoaderResources",
        req.get("https://api.github.com/repos/geode-sdk/geode/releases/tags/" + Loader::get()->getVersion().toVString()).map(
        [useLatestRelease](web::WebResponse* response) {
            // PLEASE make sure the erase happens at the end of this function
            // i have spent too much time debugging this crash
            auto doErase = [&] {
                auto retval = *response;
                RUNNING_REQUESTS.erase("@downloadLoaderResources");
                return retval;
            };

            if (response->ok()) {
                if (auto ok = response->json()) {
                    auto root = checkJson(ok.unwrap(), "[]");

                    // find release asset
                    for (auto& obj : root.needs("assets").items()) {
                        if (obj.needs("name").get<std::string>() == "resources.zip") {
                            updater::tryDownloadLoaderResources(
                                obj.needs("browser_download_url").get<std::string>(),
                                false
                            );
                            return doErase();
                        }
                    }

                    ResourceDownloadEvent(UpdateFailed("Unable to find resources in release")).post();
                    return doErase();
                }
            }
            if (useLatestRelease) {
                log::info("Loader version {} does not exist, trying to download latest resources", Loader::get()->getVersion().toVString());
                downloadLatestLoaderResources();
            }
            else {
                log::warn("Loader version {} does not exist on GitHub, not downloading the resources", Loader::get()->getVersion().toVString());
                ResourceDownloadEvent(UpdateFinished()).post();
            }

            return doErase();
        }
    ));
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
    auto updateZip = dirs::getTempDir() / "loader-update.zip";
    auto targetDir = dirs::getGeodeDir() / "update";

    if (RUNNING_REQUESTS.contains("@downloadLoaderUpdate")) return;

    auto req = web::WebRequest();
    RUNNING_REQUESTS.emplace(
        "@downloadLoaderUpdate",
        req.get(std::move(url)).map(
            [targetDir](web::WebResponse* response) {
                if (response->ok()) {
                    // unzip resources zip
                    auto data = response->data();
                    auto unzip = file::Unzip::create(data);
                    if (unzip) {
                        auto ok = unzip.unwrap().extractAllTo(targetDir);
                        if (ok) {
                            s_isNewUpdateDownloaded = true;
                            LoaderUpdateEvent(UpdateFinished()).post();
                        }
                        else {
                            LoaderUpdateEvent(
                                UpdateFailed("Unable to unzip update: " + ok.unwrapErr())
                            ).post();
                            Mod::get()->setSavedValue("last-modified-auto-update-check", std::string());
                        }
                    }
                    else {
                        LoaderUpdateEvent(
                            UpdateFailed("Unable to unzip update: " + unzip.unwrapErr())
                        ).post();
                        Mod::get()->setSavedValue("last-modified-auto-update-check", std::string());
                    }
                }
                else {
                    auto info = response->string().unwrapOr("Unknown error");
                    log::error("Failed to download latest update {}", info);
                    LoaderUpdateEvent(
                        UpdateFailed("Unable to download update: " + info)
                    ).post();

                    Mod::get()->setSavedValue("last-modified-auto-update-check", std::string());
                }
                RUNNING_REQUESTS.erase("@downloadLoaderUpdate");
                return *response;
            },
            [](web::WebProgress* progress) {
                LoaderUpdateEvent(
                    UpdateProgress(
                        static_cast<uint8_t>(progress->downloadProgress().value_or(0)),
                        "Downloading update"
                    )
                ).post();
                return *progress;
            }
        )
    );
}

void updater::checkForLoaderUpdates() {
    // Check for updates in the background
    fetchLatestGithubRelease(
        [](matjson::Value const& raw) {
            auto root = checkJson(raw, "[]");

            VersionInfo ver { 0, 0, 0 };
            root.needs("tag_name").into(ver);

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

            // don't auto-update major versions when not on forward compat
            if (!Loader::get()->isForwardCompatMode() && ver.getMajor() > Loader::get()->getVersion().getMajor()) {
                return;
            }

            // find release asset
            for (auto& obj : root.needs("assets").items()) {
                if (string::endsWith(
                    obj.needs("name").get<std::string>(),
                    fmt::format("{}.zip", PlatformID::toShortString(GEODE_PLATFORM_TARGET, true))
                )) {
                    updater::downloadLoaderUpdate(
                        obj.needs("browser_download_url").get<std::string>()
                    );
                    return;
                }
            }

            log::error("Failed to find release asset for " GEODE_PLATFORM_NAME);
            LoaderUpdateEvent(
                UpdateFailed("Unable to find release asset for " GEODE_PLATFORM_NAME)
            ).post();

            Mod::get()->setSavedValue("last-modified-auto-update-check", std::string());
        },
        [](std::string info) {
            log::error("Failed to fetch updates {}", info);
            LoaderUpdateEvent(
                UpdateFailed("Unable to check for updates: " + info)
            ).post();
        }
    );
}

bool updater::isNewUpdateDownloaded() {
    return s_isNewUpdateDownloaded;
}
