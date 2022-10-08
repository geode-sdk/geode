#include "Index.hpp"
#include <thread>
#include <Geode/utils/json.hpp>
#include <Geode/utils/JsonValidation.hpp>
#include <Geode/utils/fetch.hpp>

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

void Index::updateIndexThread(bool force) {
    auto indexDir = Loader::get()->getGeodeDirectory() / "index";

    // download index

#if GITHUB_DONT_RATE_LIMIT_ME_PLS == 0

    indexUpdateProgress(
        UpdateStatus::Progress, "Fetching index metadata", 0
    );

    // get all commits in index repo
    auto commit = web::fetchJSON(
        "https://api.github.com/repos/geode-sdk/mods/commits"
    );
    if (!commit) {
        return indexUpdateProgress(UpdateStatus::Failed, commit.error());
    }
    auto json = commit.value();
    if (
        json.is_object() &&
        json.contains("documentation_url") &&
        json.contains("message")
    ) {
        // whoops! got rate limited
        return indexUpdateProgress(
            UpdateStatus::Failed,
            json["message"].get<std::string>()
        );
    }

    indexUpdateProgress(
        UpdateStatus::Progress, "Checking index status", 25
    );

    // read sha of latest commit

    if (!json.is_array()) {
        return indexUpdateProgress(
            UpdateStatus::Failed,
            "Fetched commits, expected 'array', got '" +
                std::string(json.type_name()) + "'. "
            "Report this bug to the Geode developers!"
        );
    }

    if (!json.at(0).is_object()) {
        return indexUpdateProgress(
            UpdateStatus::Failed,
            "Fetched commits, expected 'array.object', got 'array." +
                std::string(json.type_name()) + "'. "
            "Report this bug to the Geode developers!"
        );
    }

    if (!json.at(0).contains("sha")) {
        return indexUpdateProgress(
            UpdateStatus::Failed,
            "Fetched commits, missing '0.sha'. "
            "Report this bug to the Geode developers!"
        );
    }

    auto upcomingCommitSHA = json.at(0)["sha"];

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

        // download latest commit (by downloading 
        // the repo as a zip)

        indexUpdateProgress(
            UpdateStatus::Progress,
            "Downloading index",
            50
        );
        auto gotZip = web::fetchFile(
            "https://github.com/geode-sdk/mods/zipball/main",
            indexDir / "index.zip"
        );
        if (!gotZip) {
            return indexUpdateProgress(
                UpdateStatus::Failed,
                gotZip.error()
            );
        }

        // delete old index
        if (ghc::filesystem::exists(indexDir / "index")) {
            ghc::filesystem::remove_all(indexDir / "index");
        }

        auto unzip = file::unzipTo(indexDir / "index.zip", indexDir);
        if (!unzip) {
            return indexUpdateProgress(
                UpdateStatus::Failed, unzip.error()
            );
        }
    }
#endif

    // update index

    indexUpdateProgress(
        UpdateStatus::Progress,
        "Updating index",
        75
    );
    this->updateIndexFromLocalCache();

    m_upToDate = true;
    m_updating = false;

    indexUpdateProgress(
        UpdateStatus::Finished,
        "",
        100
    );
}

void Index::indexUpdateProgress(
    UpdateStatus status,
    std::string const& info,
    uint8_t percentage
) {
    Loader::get()->queueInGDThread([this, status, info, percentage]() -> void {
        log::debug(info);
        m_callbacksMutex.lock();
        for (auto& d : m_callbacks) {
            d(status, info, percentage);
        }
        if (
            status == UpdateStatus::Finished ||
            status == UpdateStatus::Failed
        ) {
            m_callbacks.clear();
        }
        m_callbacksMutex.unlock();
    });
}

void Index::updateIndex(IndexUpdateCallback callback, bool force) {
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

    // add delegate thread-safely if it's not 
    // added already
    if (callback) {
        m_callbacksMutex.lock();
        m_callbacks.push_back(callback);
        m_callbacksMutex.unlock();
    }

    // if already updating, let delegate know 
    // and return
    if (m_updating) {
        if (callback) {
            callback(
                UpdateStatus::Progress,
                "Waiting for update info",
                0
            );
        }
        return;
    }
    m_updating = true;

    // create directory for the local clone of 
    // the index
    auto indexDir = Loader::get()->getGeodeDirectory() / "index";
    ghc::filesystem::create_directories(indexDir);

    // update index in another thread to avoid 
    // pausing UI
    std::thread(&Index::updateIndexThread, this, force).detach();
}

void Index::addIndexItemFromFolder(ghc::filesystem::path const& dir) {
    if (ghc::filesystem::exists(dir / "index.json")) {

        auto readJson = readJSON(dir / "index.json");
        if (!readJson) {
            log::warn("Error reading index.json: ", readJson.error(), ", skipping");
            return;
        }
        auto json = readJson.value();
        if (!json.is_object()) {
            log::warn("[index.json] is not an object, skipping");
            return;
        }

        auto readModJson = readJSON<ModJson>(dir / "mod.json");
        if (!readModJson) {
            log::warn("Error reading mod.json: ", readModJson.error(), ", skipping");
            return;
        }
        auto info = ModInfo::create(readModJson.value());
        if (!info) {
            log::warn(dir, ": ", info.error(), ", skipping");
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
            log::warn("[index.json] parsing error: ", e.what(), ", skipping");
            return;
        }

        m_items.push_back(item);

    } else {
        log::warn("Index directory ", dir, " is missing index.json, skipping");
    }
}

void Index::updateIndexFromLocalCache() {
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
    for (auto const& dir : ghc::filesystem::directory_iterator(modsDir)) {
        if (ghc::filesystem::is_directory(dir)) {
            this->addIndexItemFromFolder(dir);
        }
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

Result<InstallTicket*> Index::installItems(
    std::vector<IndexItem> const& items,
    ItemInstallCallback progress
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
    return Ok(new InstallTicket(this, ids, progress));
}

Result<InstallTicket*> Index::installItem(
    IndexItem const& item,
    ItemInstallCallback progress
) {
    return this->installItems({ item }, progress);
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

Result<InstallTicket*> Index::installUpdates(
    IndexUpdateCallback callback, bool force
) {
    // find items that need updating
    std::vector<IndexItem> itemsToUpdate {};
    for (auto& item : m_items) {
        if (this->isUpdateAvailableForItem(item)) {
            itemsToUpdate.push_back(item);
        }
    }

    // generate ticket
    auto ticket = this->installItems(
        itemsToUpdate,
        [itemsToUpdate, callback](
            InstallTicket*,
            UpdateStatus status,
            std::string const& info,
            uint8_t progress
        ) -> void {
            switch (status) {
                case UpdateStatus::Failed: {
                    callback(
                        UpdateStatus::Failed,
                        "Updating failed: " + info,
                        0
                    );
                } break;

                case UpdateStatus::Finished: {
                    std::string updatedStr = "";
                    for (auto& item : itemsToUpdate) {
                        updatedStr += item.m_info.m_name + " (" + 
                            item.m_info.m_id + ")\n";
                    }
                    callback(
                        UpdateStatus::Finished,
                        "Updated the following mods: " +
                        updatedStr +
                        "Please restart to apply changes.",
                        100
                    );
                } break;

                case UpdateStatus::Progress: {
                    callback(UpdateStatus::Progress, info, progress);
                } break;
            }
        }
    );
    if (!ticket) {
        return Err(ticket.error());
    }

    // install updates concurrently
    ticket.value()->start(InstallMode::Concurrent);

    return ticket;
}
