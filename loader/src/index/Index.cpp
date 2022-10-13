#include "Index.hpp"
#include <thread>
#include <Geode/utils/json.hpp>
#include <Geode/utils/JsonValidation.hpp>
#include <Geode/utils/fetch.hpp>
#include <hash.hpp>
#include <Geode/utils/file.hpp>
#include <Geode/utils/string.hpp>
#include <Geode/utils/vector.hpp>
#include <Geode/utils/map.hpp>

#define GITHUB_DONT_RATE_LIMIT_ME_PLS 0

template<class Json = nlohmann::json>
static Result<Json> readJSON(ghc::filesystem::path const& path) {
    auto indexJsonData = utils::file::readString(path);
    if (!indexJsonData) {
        return Err("Unable to read " + path.string());
    }
    try {
        return Ok(Json::parse(indexJsonData.value()));
    } catch(std::exception& e) {
        return Err("Error parsing JSON: " + std::string(e.what()));
    }
}

static PlatformID platformFromString(std::string const& str) {
    switch (hash(utils::string::trim(utils::string::toLower(str)).c_str())) {
        default:
        case hash("unknown"): return PlatformID::Unknown;
        case hash("windows"): return PlatformID::Windows;
        case hash("macos"): return PlatformID::MacOS;
        case hash("ios"): return PlatformID::iOS;
        case hash("android"): return PlatformID::Android;
        case hash("linux"): return PlatformID::Linux;
    }
}


Index* Index::get() {
    static auto ret = new Index();
    return ret;
}

bool Index::isIndexUpdated() const {
    return m_upToDate;
}

std::vector<IndexItem> Index::getFeaturedItems() const {
    std::vector<IndexItem> items;
    items.reserve(m_featured.size());
    std::transform(
        m_featured.begin(),
        m_featured.end(),
        std::back_inserter(items),
        [this](auto const& item) {
            return this->getKnownItem(item);
        }
    );
    return items;
}

bool Index::isFeaturedItem(std::string const& item) const {
    return m_featured.count(item);
}

void Index::updateIndex(IndexUpdateCallback callback, bool force) {
    #define RETURN_ERROR(str)           \
        std::string err__ = (str);      \
        if (callback) callback(         \
            UpdateStatus::Failed,       \
            err__,                      \
            0                           \
        );                              \
        log::info("Index update failed: {}", err__);\
        return;

    // if already updated and no force, let 
    // delegate know
    if (!force && m_upToDate) {
        if (callback) {
            callback(
                UpdateStatus::Finished,
                "Index already updated",
                100
            );
        }
        return;
    }

    // create directory for the local clone of 
    // the index
    auto indexDir = Loader::get()->getGeodeDirectory() / "index";
    ghc::filesystem::create_directories(indexDir);

#if GITHUB_DONT_RATE_LIMIT_ME_PLS == 1

    auto err = this->updateIndexFromLocalCache();
    if (!err) {
        RETURN_ERROR(err);
    }
    
    m_upToDate = true;
    m_updating = false;

    if (callback) callback(UpdateStatus::Finished, "", 100);
    return;

#endif

    web::AsyncWebRequest()
        .join("index-update")
        .fetch("https://api.github.com/repos/geode-sdk/mods/commits")
        .json()
        .then([this, force, callback](nlohmann::json const& json) {
            auto indexDir = Loader::get()->getGeodeDirectory() / "index";
            
            // check if rate-limited (returns object)
            JsonChecker checkerObj(json);
            auto obj = checkerObj.root("[geode-sdk/mods/commits]").obj();
            if (obj.has("documentation_url") && obj.has("message")) {
                RETURN_ERROR(obj.has("message").get<std::string>());
            }

            // get sha of latest commit
            JsonChecker checker(json);
            auto root = checker.root("[geode-sdk/mods/commits]").array();

            std::string upcomingCommitSHA;
            if (auto first = root.at(0).obj().needs("sha")) {
                upcomingCommitSHA = first.get<std::string>();
            } else {
                RETURN_ERROR("Unable to get hash from latest commit: " + checker.getError());
            }

            // read sha of currently installed commit
            std::string currentCommitSHA = "";
            if (ghc::filesystem::exists(indexDir / "current")) {
                auto data = utils::file::readString(indexDir / "current");
                if (data) {
                    currentCommitSHA = data.value();
                }
            }

            // update if forced or latest commit has 
            // different sha
            if (force || currentCommitSHA != upcomingCommitSHA) {
                // save new sha in file
                utils::file::writeString(indexDir / "current", upcomingCommitSHA);

                web::AsyncWebRequest()
                    .join("index-download")
                    .fetch("https://github.com/geode-sdk/mods/zipball/main")
                    .into(indexDir / "index.zip")
                    .then([this, indexDir, callback](auto) {
                        // delete old index
                        try {
                            if (ghc::filesystem::exists(indexDir / "index")) {
                                ghc::filesystem::remove_all(indexDir / "index");
                            }
                        } catch(std::exception& e) {
                            RETURN_ERROR("Unable to delete old index " + std::string(e.what()));
                        }

                        // unzip new index
                        auto unzip = file::unzipTo(indexDir / "index.zip", indexDir);
                        if (!unzip) {
                            RETURN_ERROR(unzip.error());
                        }

                        // update index
                        auto err = this->updateIndexFromLocalCache();
                        if (!err) {
                            RETURN_ERROR(err.error());
                        }

                        m_upToDate = true;
                        m_updating = false;

                        if (callback) callback(
                            UpdateStatus::Finished, "", 100
                        );
                    })
                    .expect([callback](std::string const& err) {
                        RETURN_ERROR(err);
                    })
                    .progress([callback](web::SentAsyncWebRequest& req, double now, double total) {
                        if (callback) callback(
                            UpdateStatus::Progress,
                            "Downloading",
                            static_cast<int>(now / total * 100.0)
                        );
                    });
            } else {
                auto err = this->updateIndexFromLocalCache();
                if (!err) {
                    RETURN_ERROR(err.error());
                }

                m_upToDate = true;
                m_updating = false;

                if (callback) callback(
                    UpdateStatus::Finished,
                    "", 100
                );
            }
        })
        .expect([callback](std::string const& err) {
            RETURN_ERROR(err);
        })
        .progress([callback](web::SentAsyncWebRequest& req, double now, double total) {
            if (callback) callback(
                UpdateStatus::Progress,
                "Downloading",
                static_cast<int>(now / total * 100.0)
            );
        });
}

void Index::addIndexItemFromFolder(ghc::filesystem::path const& dir) {
    if (ghc::filesystem::exists(dir / "index.json")) {

        auto readJson = readJSON(dir / "index.json");
        if (!readJson) {
            log::warn("Error reading index.json: {}, skipping", readJson.error());
            return;
        }
        auto json = readJson.value();
        if (!json.is_object()) {
            log::warn("[index.json] is not an object, skipping");
            return;
        }

        auto readModJson = readJSON<ModJson>(dir / "mod.json");
        if (!readModJson) {
            log::warn("Error reading mod.json: {}, skipping", readModJson.error());
            return;
        }
        auto info = ModInfo::create(readModJson.value());
        if (!info) {
            log::warn("{}: {}, skipping", dir, info.error());
            return;
        }

        IndexItem item;

        item.m_path = dir;
        item.m_info = info.value();

        if (
            !json.contains("download") ||
            !json["download"].is_object()
        ) {
            log::warn("[index.json].download is not an object, skipping");
            return;
        }

        #define REQUIRE_DOWNLOAD_KEY(key, type) \
            if (!download.contains(key) || !download[key].is_##type()) {\
                log::warn("[index.json].download." key " is not a " #type ", skipping");\
                return;\
            }

        try {

            auto download = json["download"];

            REQUIRE_DOWNLOAD_KEY("url", string);
            REQUIRE_DOWNLOAD_KEY("name", string);
            REQUIRE_DOWNLOAD_KEY("hash", string);
            REQUIRE_DOWNLOAD_KEY("platforms", array);

            item.m_download.m_url = download["url"];
            item.m_download.m_filename = download["name"];
            item.m_download.m_hash = download["hash"];
            for (auto& platform : download["platforms"]) {
                item.m_download.m_platforms.insert(platformFromString(platform));
            }

            if (json.contains("categories")) {
                if (!json["categories"].is_array()) {
                    log::warn("[index.json].categories is not an array, skipping");
                    return;
                }
                item.m_categories = json["categories"].get<std::unordered_set<std::string>>();
                m_categories.insert(item.m_categories.begin(), item.m_categories.end());
            }

        } catch(std::exception& e) {
            log::warn("[index.json] parsing error: {}, skipping", e.what());
            return;
        }

        m_items.push_back(item);

    } else {
        log::warn("Index directory {} is missing index.json, skipping", dir);
    }
}

Result<> Index::updateIndexFromLocalCache() {
    m_items.clear();
    auto baseIndexDir = Loader::get()->getGeodeDirectory() / "index";

    // load geode.json (index settings)
    if (auto baseIndexJson = readJSON(baseIndexDir / "geode.json")) {
        auto json = baseIndexJson.value();
        auto checker = JsonChecker(json);
        checker.root("[index/geode.json]").obj()
            .has("featured").into(m_featured);
    }

    // load index mods
    auto modsDir = baseIndexDir / "index";
    if (ghc::filesystem::exists(modsDir)) {
        for (auto const& dir : ghc::filesystem::directory_iterator(modsDir)) {
            if (ghc::filesystem::is_directory(dir)) {
                this->addIndexItemFromFolder(dir);
            }
        }
        log::info("Index updated");
        return Ok();
    } else {
        return Err(
            "Index appears not to have been "
            "downloaded, or is fully empty"
        );
    }
}

std::vector<IndexItem> Index::getItems() const {
    return m_items;
}

std::unordered_set<std::string> Index::getCategories() const {
    return m_categories;
}

bool Index::isKnownItem(std::string const& id) const {
    for (auto& item : m_items) {
        if (item.m_info.m_id == id) return true;
    }
    return false;
}

IndexItem Index::getKnownItem(std::string const& id) const {
    for (auto& item : m_items) {
        if (item.m_info.m_id == id) {
            return item;
        }
    }
    return IndexItem();
}

struct UninstalledDependency {
    std::string m_id;
    bool m_isInIndex;
};

static void getUninstalledDependenciesRecursive(
    ModInfo const& info,
    std::vector<UninstalledDependency>& deps
) {
    for (auto& dep : info.m_dependencies) {
        UninstalledDependency d;
        d.m_isInIndex = Index::get()->isKnownItem(dep.m_id);
        if (!Loader::get()->isModInstalled(dep.m_id)) {
            d.m_id = dep.m_id;
            deps.push_back(d);
        }
        if (d.m_isInIndex) {
            getUninstalledDependenciesRecursive(
                Index::get()->getKnownItem(dep.m_id).m_info,
                deps
            );
        }
    }
}

Result<std::vector<std::string>> Index::checkDependenciesForItem(
    IndexItem const& item
) {
    // todo: check versions
    std::vector<UninstalledDependency> deps;
    getUninstalledDependenciesRecursive(item.m_info, deps);
    if (deps.size()) {
        std::vector<std::string> unknownDeps;
        for (auto& dep : deps) {
            if (!dep.m_isInIndex) {
                unknownDeps.push_back(dep.m_id);
            }
        }
        if (unknownDeps.size()) {
            std::string list = "";
            for (auto& ud : unknownDeps) {
                list += "<cp>" + ud + "</c>, ";
            }
            list.pop_back();
            list.pop_back();
            return Err(
                "This mod or its dependencies <cb>depends</c> on the "
                "following unknown mods: " + list + ". You will have "
                "to manually install these mods before you can install "
                "this one."
            );
        }
        std::vector<std::string> list = {};
        for (auto& d : deps) {
            list.push_back(d.m_id);
        }
        list.push_back(item.m_info.m_id);
        return Ok(list);
    } else {
        return Ok<std::vector<std::string>>({ item.m_info.m_id });
    }
}

Result<InstallItems> Index::installItems(
    std::vector<IndexItem> const& items
) {
    std::vector<std::string> ids {};
    for (auto& item : items) {
        if (!item.m_download.m_platforms.count(GEODE_PLATFORM_TARGET)) {
            return Err(
                "This mod is not available on your "
                "current platform \"" GEODE_PLATFORM_NAME "\" - Sorry! :("
            );
        }
        if (!item.m_download.m_url.size()) {
            return Err(
                "Download URL not set! Report this bug to "
                "the Geode developers - this should not happen, ever."
            );
        }
        if (!item.m_download.m_filename.size()) {
            return Err(
                "Download filename not set! Report this bug to "
                "the Geode developers - this should not happen, ever."
            );
        }
        if (!item.m_download.m_hash.size()) {
            return Err(
                "Checksum not set! Report this bug to "
                "the Geode developers - this should not happen, ever."
            );
        }
        auto list = checkDependenciesForItem(item);
        if (!list) {
            return Err(list.error());
        }
        utils::vector::push(ids, list.value());
    }
    return Ok(InstallItems(ids));
}

Result<InstallItems> Index::installItem(
    IndexItem const& item
) {
    return this->installItems({ item });
}

bool Index::isUpdateAvailableForItem(std::string const& id) const {
    if (!Loader::get()->isModInstalled(id)) {
        return false;
    }
    if (!this->isKnownItem(id)) {
        return false;
    }
    return
        this->getKnownItem(id).m_info.m_version > 
        Loader::get()->getInstalledMod(id)->getVersion();
}

bool Index::isUpdateAvailableForItem(IndexItem const& item) const {
    if (!Loader::get()->isModInstalled(item.m_info.m_id)) {
        return false;
    }
    return
        item.m_info.m_version > 
        Loader::get()->getInstalledMod(item.m_info.m_id)->getVersion();
}

bool Index::areUpdatesAvailable() const {
    for (auto& item : m_items) {
        if (this->isUpdateAvailableForItem(item.m_info.m_id)) {
            return true;
        }
    }
    return false;
}

Result<InstallItems> Index::installAllUpdates() {
    // find items that need updating
    std::vector<IndexItem> itemsToUpdate {};
    for (auto& item : m_items) {
        if (this->isUpdateAvailableForItem(item)) {
            itemsToUpdate.push_back(item);
        }
    }
    return this->installItems(itemsToUpdate);
}

InstallHandles Index::getRunningInstallations() const {
    return map::getValues(m_installations);
}

InstallHandle Index::isInstallingItem(std::string const& id) {
    if (m_installations.count(id)) {
        return m_installations.at(id);
    }
    return nullptr;
}

std::vector<std::string> InstallItems::toInstall() const {
    return m_toInstall;
}

InstallHandles InstallItems::begin(ItemInstallCallback callback) const {
    InstallHandles res {};

    for (auto& inst : m_toInstall) {
        // by virtue of running this function we know item must be valid
        auto item = Index::get()->getKnownItem(inst);

        auto indexDir = Loader::get()->getGeodeDirectory() / "index";
        auto tempFile = indexDir / item.m_download.m_filename;

        auto handle = web::AsyncWebRequest()
            .join("install_mod_" + inst)
            .fetch(item.m_download.m_url)
            .into(tempFile)
            .then([callback, item, inst, indexDir, tempFile](auto) {
                // check for 404
                auto notFound = utils::file::readString(tempFile);
                if (notFound && notFound.value() == "Not Found") {
                    try { ghc::filesystem::remove(tempFile); } catch(...) {}
                    if (callback) callback(
                        inst, UpdateStatus::Failed,
                        "Binary file download returned \"Not found\". Report "
                        "this to the Geode development team.",
                        0
                    );
                }

                // verify checksum
                if (callback) callback(inst, UpdateStatus::Progress, "Verifying", 100);
                if (::calculateHash(tempFile.string()) != item.m_download.m_hash) {
                    try { ghc::filesystem::remove(tempFile); } catch(...) {}
                    if (callback) callback(
                        inst, UpdateStatus::Failed,
                        "Checksum mismatch! (Downloaded file did not match what "
                        "was expected. Try again, and if the download fails another time, "
                        "report this to the Geode development team.",
                        0
                    );
                }

                // move temp file to geode directory
                try {
                    auto modDir = Loader::get()->getGeodeDirectory() / "mods";
                    auto targetFile = modDir / item.m_download.m_filename;

                    // find valid filename that doesn't exist yet
                    auto filename = ghc::filesystem::path(
                        item.m_download.m_filename
                    ).replace_extension("").string();

                    size_t number = 0;
                    while (ghc::filesystem::exists(targetFile)) {
                        targetFile = modDir /
                            (filename + std::to_string(number) + ".geode");
                        number++;
                    }

                    // move file
                    ghc::filesystem::rename(tempFile, targetFile);
                } catch(std::exception& e) {
                    try { ghc::filesystem::remove(tempFile); } catch(...) {}
                    if (callback) callback(
                        inst, UpdateStatus::Failed,
                        "Unable to move downloaded file to mods directory: \"" + 
                        std::string(e.what()) + " \" "
                        "(This might be due to insufficient permissions to "
                        "write files under SteamLibrary, try running GD as "
                        "administrator)",
                        0
                    );
                }

                // finished
                if (callback) callback(inst, UpdateStatus::Finished, "", 100);
            })
            .expect([inst, callback](std::string const& error) {
                if (callback) callback(inst, UpdateStatus::Failed, error, 0);
                Index::get()->m_installations.erase(inst);
            })
            .cancelled([inst](auto&) {
                Index::get()->m_installations.erase(inst);
            })
            .progress([inst, callback](web::SentAsyncWebRequest&, double now, double total) {
                if (callback) callback(
                    inst, UpdateStatus::Progress,
                    "Downloading binary",
                    static_cast<uint8_t>(now / total * 100.0)
                );
            })
            .send();
        
        res.push_back(handle);
        Index::get()->m_installations.insert({ inst, handle });
    }
    
    return res;
}
