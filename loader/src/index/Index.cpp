#include "Index.hpp"
#include <thread>
#include <Geode/utils/json.hpp>
#include "fetch.hpp"

#define GITHUB_DONT_RATE_LIMIT_ME_PLS 0

static Result<nlohmann::json> readJSON(ghc::filesystem::path const& path) {
    auto indexJsonData = file_utils::readString(path);
    if (!indexJsonData) {
        return Err("Unable to read " + path.string());
    }
    try {
        return Ok(nlohmann::json::parse(indexJsonData.value()));
    } catch(std::exception& e) {
        return Err("Error parsing JSON: " + std::string(e.what()));
    }
}

static Result<> unzipTo(
    ghc::filesystem::path const& from,
    ghc::filesystem::path const& to
) {
    // unzip downloaded
    auto unzip = ZipFile(from.string());
    if (!unzip.isLoaded()) {
        return Err("Unable to unzip index.zip");
    }

    for (auto file : unzip.getAllFiles()) {
        // this is a very bad check for seeing 
        // if file is a directory. it seems to 
        // work on windows at least. idk why 
        // getAllFiles returns the directories 
        // aswell now
        if (
            string_utils::endsWith(file, "\\") ||
            string_utils::endsWith(file, "/")
        ) continue;

        auto zipPath = file;

        // dont include the github repo folder
        file = file.substr(file.find_first_of("/") + 1);

        auto path = ghc::filesystem::path(file);
        if (path.has_parent_path()) {
            if (
                !ghc::filesystem::exists(to / path.parent_path()) &&
                !ghc::filesystem::create_directories(to / path.parent_path())
            ) {
                return Err(
                    "Unable to create directories \"" + 
                    path.parent_path().string() + "\""
                );
            }
        }
        unsigned long size;
        auto data = unzip.getFileData(zipPath, &size);
        if (!data || !size) {
            return Err("Unable to read \"" + std::string(zipPath) + "\"");
        }
        auto wrt = file_utils::writeBinary(
            to / file,
            byte_array(data, data + size)
        );
        if (!wrt) {
            return Err("Unable to write \"" + file + "\": " + wrt.error());
        }
    }

    return Ok();
}


Index* Index::get() {
    static auto ret = new Index();
    return ret;
}

bool Index::isIndexUpdated() const {
    return m_upToDate;
}


void Index::updateIndexThread(bool force) {
    auto indexDir = Loader::get()->getGeodeDirectory() / "index";

    // download index

#if GITHUB_DONT_RATE_LIMIT_ME_PLS == 0

    indexUpdateProgress(
        UpdateStatus::Progress, "Fetching index metadata", 0
    );

    // get all commits in index repo
    auto commit = fetchJSON(
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
        auto data = file_utils::readString(indexDir / "current");
        if (data) {
            currentCommitSHA = data.value();
        }
    }

    // update if forced or latest commit has 
    // different sha
    if (force || currentCommitSHA != upcomingCommitSHA) {
        // save new sha in file
        file_utils::writeString(indexDir / "current", upcomingCommitSHA);

        // download latest commit (by downloading 
        // the repo as a zip)

        indexUpdateProgress(
            UpdateStatus::Progress,
            "Downloading index",
            50
        );
        auto gotZip = fetchFile(
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

        auto unzip = unzipTo(indexDir / "index.zip", indexDir);
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

PlatformID platformFromString(std::string const& str) {
    switch (hash(string_utils::trim(string_utils::toLower(str)).c_str())) {
        default:
        case hash("unknown"): return PlatformID::Unknown;
        case hash("windows"): return PlatformID::Windows;
        case hash("macos"): return PlatformID::MacOS;
        case hash("ios"): return PlatformID::iOS;
        case hash("android"): return PlatformID::Android;
        case hash("linux"): return PlatformID::Linux;
    }
}

void Index::addIndexItemFromFolder(ghc::filesystem::path const& dir) {
    if (ghc::filesystem::exists(dir / "index.json")) {

        auto readJson = readJSON(dir / "index.json");
        if (!readJson) {
            Log::get() << Severity::Warning
                << "Error reading index.json: "
                << readJson.error() << ", skipping";
            return;
        }
        auto json = readJson.value();
        if (!json.is_object()) {
            Log::get() << Severity::Warning
                << "[index.json] is not an object, skipping";
            return;
        }

        auto readModJson = readJSON(dir / "mod.json");
        if (!readModJson) {
            Log::get() << Severity::Warning
                << "Error reading mod.json: "
                << readModJson.error() << ", skipping";
            return;
        }
        auto info = ModInfo::create(readModJson.value());
        if (!info) {
            Log::get() << Severity::Warning
                << info.error() << ", skipping";
            return;
        }

        IndexItem item;

        item.m_path = dir;
        item.m_info = info.value();

        if (json.contains("download") && json["download"].is_object()) {
            auto download = json["download"];
            std::set<PlatformID> unsetPlatforms = {
                PlatformID::Windows,
                PlatformID::MacOS,
                PlatformID::iOS,
                PlatformID::Android,
                PlatformID::Linux,
            };
            for (auto& key : std::initializer_list<std::string> {
                "windows",
                "macos",
                "android",
                "ios",
                "*",
            }) {
                if (download.contains(key)) {
                    auto platformDownload = download[key];
                    if (!platformDownload.is_object()) {
                        Log::get() << Severity::Warning
                            << "[index.json].download."
                            << key
                            << " is not an object, skipping";
                        return;
                    }
                    if (
                        !platformDownload.contains("url") ||
                        !platformDownload["url"].is_string()
                    ) {
                        Log::get() << Severity::Warning
                            << "[index.json].download."
                            << key
                            << ".url is not a string, skipping";
                        return;
                    }
                    if (
                        !platformDownload.contains("name") ||
                        !platformDownload["name"].is_string()
                    ) {
                        Log::get() << Severity::Warning
                            << "[index.json].download."
                            << key
                            << ".name is not a string, skipping";
                        return;
                    }
                    if (
                        !platformDownload.contains("hash") ||
                        !platformDownload["hash"].is_string()
                    ) {
                        Log::get() << Severity::Warning
                            << "[index.json].download."
                            << key
                            << ".hash is not a string, skipping";
                        return;
                    }
                    IndexItem::Download down;
                    down.m_url = platformDownload["url"];
                    down.m_filename = platformDownload["name"];
                    down.m_hash = platformDownload["hash"];
                    if (key == "*") {
                        for (auto& platform : unsetPlatforms) {
                            item.m_download[platform] = down;
                        }
                    } else {
                        auto id = platformFromString(key);
                        item.m_download[id] = down;
                        unsetPlatforms.erase(id);
                    }
                }
            }
        } else {
            Log::get() << Severity::Warning
                << "[index.json] is missing \"download\", adding anyway";
        }

        m_items.push_back(item);

    } else {
        Log::get() << Severity::Warning << "Index directory "
            << dir << " is missing index.json, skipping";
    }
}

void Index::updateIndexFromLocalCache() {
    m_items.clear();
    auto indexDir = Loader::get()->getGeodeDirectory() / "index" / "index";
    for (auto const& dir : ghc::filesystem::directory_iterator(indexDir)) {
        if (ghc::filesystem::is_directory(dir)) {
            this->addIndexItemFromFolder(dir);
        }
    }
}

std::vector<IndexItem> const& Index::getItems() const {
    return m_items;
}

std::vector<IndexItem> Index::getUninstalledItems() const {
    std::vector<IndexItem> items;
    for (auto& item : m_items) {
        if (!Loader::get()->isModInstalled(item.m_info.m_id)) {
            if (item.m_download.count(GEODE_PLATFORM_TARGET)) {
                items.push_back(item);
            }
        }
    }
    return items;
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
        if (!item.m_download.count(GEODE_PLATFORM_TARGET)) {
            return Err(
                "This mod is not available on your "
                "current platform \"" GEODE_PLATFORM_NAME "\" - Sorry! :("
            );
        }
        auto download = item.m_download.at(GEODE_PLATFORM_TARGET);
        if (!download.m_url.size()) {
            return Err(
                "Download URL not set! Report this bug to "
                "the Geode developers - this should not happen, ever."
            );
        }
        if (!download.m_filename.size()) {
            return Err(
                "Download filename not set! Report this bug to "
                "the Geode developers - this should not happen, ever."
            );
        }
        if (!download.m_hash.size()) {
            return Err(
                "Checksum not set! Report this bug to "
                "the Geode developers - this should not happen, ever."
            );
        }
        auto list = checkDependenciesForItem(item);
        if (!list) {
            return Err(list.error());
        }
        vector_utils::push(ids, list.value());
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
