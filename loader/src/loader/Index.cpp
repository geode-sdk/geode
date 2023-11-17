#include <Geode/loader/Index.hpp>
#include <Geode/loader/Loader.hpp>
#include <Geode/loader/Dirs.hpp>
#include <Geode/utils/ranges.hpp>
#include <Geode/utils/web.hpp>
#include <Geode/utils/string.hpp>
#include <Geode/utils/map.hpp>
#include <hash/hash.hpp>
#include <Geode/utils/JsonValidation.hpp>

#include <thread>

#ifdef GEODE_IS_WINDOWS
#include <filesystem>
#endif

using namespace geode::prelude;

// ModInstallEvent

ModInstallEvent::ModInstallEvent(
    std::string const& id, const UpdateStatus status
) : modID(id), status(status) {}

ListenerResult ModInstallFilter::handle(utils::MiniFunction<Callback> fn, ModInstallEvent* event) {
    if (m_id == event->modID) {
        fn(event);
    }
    return ListenerResult::Propagate;
}

ModInstallFilter::ModInstallFilter(std::string const& id) : m_id(id) {}

// IndexUpdateEvent

IndexUpdateEvent::IndexUpdateEvent(const UpdateStatus status) : status(status) {}

ListenerResult IndexUpdateFilter::handle(
    utils::MiniFunction<Callback> fn,
    IndexUpdateEvent* event
) {
    fn(event);
    return ListenerResult::Propagate;
}

IndexUpdateFilter::IndexUpdateFilter() {}

// IndexItem

class IndexItem::Impl final {
private:
    ghc::filesystem::path m_rootPath;
    ghc::filesystem::path m_path;
    ModMetadata m_metadata;
    std::string m_downloadURL;
    std::string m_downloadHash;
    std::unordered_set<PlatformID> m_platforms;
    bool m_isFeatured = false;
    bool m_isInstalled = false;
    std::unordered_set<std::string> m_tags;

    friend class IndexItem;

public:
    /**
     * Create IndexItem from a directory
     */
    static Result<std::shared_ptr<IndexItem>> create(
        ghc::filesystem::path const& rootDir,
        ghc::filesystem::path const& dir
    );

    bool isInstalled() const;
};

IndexItem::IndexItem() : m_impl(std::make_unique<Impl>()) {}
IndexItem::~IndexItem() = default;

ghc::filesystem::path IndexItem::getRootPath() const {
    return m_impl->m_rootPath;
}

ghc::filesystem::path IndexItem::getPath() const {
    return m_impl->m_path;
}

ModInfo IndexItem::getModInfo() const {
    return this->getMetadata();
}

ModMetadata IndexItem::getMetadata() const {
    return m_impl->m_metadata;
}

std::string IndexItem::getDownloadURL() const {
    return m_impl->m_downloadURL;
}

std::string IndexItem::getPackageHash() const {
    return m_impl->m_downloadHash;
}

std::unordered_set<PlatformID> IndexItem::getAvailablePlatforms() const {
    return m_impl->m_platforms;
}

bool IndexItem::isFeatured() const {
    return m_impl->m_isFeatured;
}

std::unordered_set<std::string> IndexItem::getTags() const {
    return m_impl->m_tags;
}

bool IndexItem::isInstalled() const {
    return m_impl->isInstalled();
}

#if defined(GEODE_EXPOSE_SECRET_INTERNALS_IN_HEADERS_DO_NOT_DEFINE_PLEASE)
void IndexItem::setMetadata(ModMetadata const& value) {
    m_impl->m_metadata = value;
}

void IndexItem::setDownloadURL(std::string const& value) {
    m_impl->m_downloadURL = value;
}

void IndexItem::setPackageHash(std::string const& value) {
    m_impl->m_downloadHash = value;
}

void IndexItem::setAvailablePlatforms(std::unordered_set<PlatformID> const& value) {
    m_impl->m_platforms = value;
}

void IndexItem::setIsFeatured(bool const& value) {
    m_impl->m_isFeatured = value;
}

void IndexItem::setTags(std::unordered_set<std::string> const& value) {
    m_impl->m_tags = value;
}

void IndexItem::setIsInstalled(bool const& value) {
    m_impl->m_isInstalled = value;
}
#endif

Result<IndexItemHandle> IndexItem::Impl::create(ghc::filesystem::path const& rootDir, ghc::filesystem::path const& dir) {
    GEODE_UNWRAP_INTO(
        auto entry, file::readJson(dir / "entry.json")
            .expect("Unable to read entry.json")
    );
    GEODE_UNWRAP_INTO(
        auto metadata, ModMetadata::createFromFile(dir / "mod.json")
            .expect("Unable to read mod.json: {error}")
    );
    auto metadataRes = metadata.addSpecialFiles(rootDir);
    if (!metadataRes) {
        log::warn("Unable to add special files from {}: {}", rootDir, metadataRes.unwrapErr());
    }

    JsonChecker checker(entry);
    auto root = checker.root("[entry.json]").obj();

    std::unordered_set<PlatformID> platforms;
    for (auto& plat : root.has("platforms").iterate()) {
        platforms.insert(PlatformID::from(plat.get<std::string>()));
    }

    std::unordered_set<std::string> tags;
    for (auto& tag : root.has("tags").iterate()) {
        tags.insert(tag.get<std::string>());
    }

    auto item = std::make_shared<IndexItem>();
    item->m_impl->m_rootPath = rootDir;
    item->m_impl->m_path = dir;
    item->m_impl->m_metadata = metadata;
    item->m_impl->m_platforms = platforms;
    item->m_impl->m_tags = tags;
    root.has("mod").obj().has("download").into(item->m_impl->m_downloadURL);
    root.has("mod").obj().has("hash").into(item->m_impl->m_downloadHash);
    root.has("featured").into(item->m_impl->m_isFeatured);

    if (checker.isError()) {
        return Err(checker.getError());
    }
    return Ok(item);
}

bool IndexItem::Impl::isInstalled() const {
    if (m_isInstalled) {
        return true;
    }
    if (!Loader::get()->isModInstalled(m_metadata.getID())) {
        return false;
    }
    auto installed = Loader::get()->getInstalledMod(m_metadata.getID());
    if (installed->getVersion() != m_metadata.getVersion()) {
        return false;
    }
    return true;
}

// Helpers

static Result<> flattenGithubRepo(ghc::filesystem::path const& dir) {
    // github zipballs have a folder at root, but we already have our 
    // own folder for that so let's just bring everything from that 
    // folder to ours
    GEODE_UNWRAP_INTO(auto files, file::readDirectory(dir));
    try {
        // only flatten if there is only one file and it's a directory
        if (files.size() == 1 && ghc::filesystem::is_directory(files[0])) {
            for (auto& file : ghc::filesystem::directory_iterator(files[0])) {
                #ifdef GEODE_IS_WINDOWS
                ghc::filesystem::path const relative = std::filesystem::relative(file.path().wstring(), files[0].wstring()).wstring();
                #else
                auto const relative = ghc::filesystem::relative(file, files[0]);
                #endif
                ghc::filesystem::rename(
                    file, dir / relative
                );
            }
            ghc::filesystem::remove(files[0]);
        }
    } catch(std::exception& e) {
        return Err(e.what());
    }
    return Ok();
}

// Index impl

class Index::Impl final {
public:
    // for once, the fact that std::map is ordered is useful (this makes 
    // getting the latest version of a mod as easy as items.rbegin())
    using ItemVersions = std::map<VersionInfo, IndexItemHandle>;

private:
    std::unordered_map<
        IndexItemHandle,
        utils::web::SentAsyncWebRequestHandle
    > m_runningInstallations;
    std::atomic<bool> m_isUpToDate = false;
    std::atomic<bool> m_updating = false;
    std::atomic<bool> m_triedToUpdate = false;
    std::unordered_map<std::string, ItemVersions> m_items;

    friend class Index;

    void cleanupItems();
    void downloadIndex(std::string commitHash = "");
    void checkForUpdates();
    void updateFromLocalTree();
    void installNext(size_t index, IndexInstallList const& list);

public:
    Impl() {
        new EventListener<IndexUpdateFilter>([this](IndexUpdateEvent* ev) {
            m_updating = std::holds_alternative<UpdateProgress>(ev->status);
        });
    }
};

// Index globals

Index::Index() : m_impl(std::make_unique<Impl>()) {}
Index::~Index() = default;

Index* Index::get() {
    static auto inst = new Index();
    return inst;
}

// Updating

void Index::Impl::cleanupItems() {
    // delete mods with no versions
    for (auto it = m_items.begin(); it != m_items.end(); ) {
        if (!it->second.size()) {
            it = m_items.erase(it);
        } else {
            ++it;
        }
    }
}

bool Index::isUpToDate() const {
    return m_impl->m_isUpToDate;
}

bool Index::hasTriedToUpdate() const {
    return m_impl->m_triedToUpdate;
}

void Index::Impl::downloadIndex(std::string commitHash) {
    log::debug("Downloading index");

    IndexUpdateEvent(UpdateProgress(0, "Beginning download")).post();

    auto targetFile = dirs::getTempDir() / "updated-index.zip";

    web::AsyncWebRequest()
        .join("index-download")
        .fetch("https://github.com/geode-sdk/mods/zipball/main")
        .into(targetFile)
        .then([this, targetFile, commitHash](auto) {
            auto targetDir = dirs::getIndexDir() / "v0";
            // delete old unzipped index
            try {
                if (ghc::filesystem::exists(targetDir)) {
                    ghc::filesystem::remove_all(targetDir);
                }
            }
            catch(...) {
                IndexUpdateEvent(UpdateFailed("Unable to clear cached index")).post();
                return;
            }

            std::thread([=, this]() {
                // unzip new index
                auto unzip = file::Unzip::intoDir(targetFile, targetDir, true)
                    .expect("Unable to unzip new index");
                if (!unzip) {
                    Loader::get()->queueInMainThread([unzip] {
                        IndexUpdateEvent(UpdateFailed(unzip.unwrapErr())).post();
                    });
                    return;
                }

                // remove the directory github adds to the root of the zip
                (void)flattenGithubRepo(targetDir);
                if (!commitHash.empty()) {
                    auto const checksumPath = dirs::getIndexDir() / ".checksum";
                    (void)file::writeString(checksumPath, commitHash);
                }

                Loader::get()->queueInMainThread([this] {
                    // update index
                    this->updateFromLocalTree();
                });
            }).detach();
        })
        .expect([](std::string const& err) {
            IndexUpdateEvent(UpdateFailed(fmt::format("Error downloading: {}", err))).post();
        })
        .progress([](auto&, double now, double total) {
            IndexUpdateEvent(
                UpdateProgress(
                    static_cast<uint8_t>(now / total * 100.0),
                    "Downloading"
                )
            ).post();
        });
}

void Index::Impl::checkForUpdates() {
    if (m_isUpToDate) {
        return this->updateFromLocalTree();
    }

    log::debug("Checking updates for index");
    IndexUpdateEvent(UpdateProgress(0, "Checking status")).post();

    auto checksum = dirs::getIndexDir() / ".checksum";

    // read old commit SHA
    // not using saved values for this one as we don't want to refetch 
    // index even if the game crashes
    auto oldSHA = file::readString(checksum).unwrapOr("");
    web::AsyncWebRequest()
        .join("index-update")
        .userAgent("github_api/1.0")
        .header(fmt::format("If-None-Match: \"{}\"", oldSHA))
        .header("Accept: application/vnd.github.sha")
        .fetch("https://api.github.com/repos/geode-sdk/mods/commits/main")
        .text()
        .then([this, checksum, oldSHA](std::string const& newSHA) {
            // check if should just be updated from local cache
            if (
                // if no new hash was given (rate limited) or the new hash is the 
                // same as old
                (newSHA.empty() || oldSHA == newSHA) &&
                // make sure the downloaded local copy actually exists
                ghc::filesystem::exists(dirs::getIndexDir() / "v0" / "config.json")
            ) {
                this->updateFromLocalTree();
            }
            // otherwise save hash and download source
            else {
                this->downloadIndex(newSHA);
            }
        })
        .expect([](std::string const& err) {
            IndexUpdateEvent(
                UpdateFailed(fmt::format("Error checking for updates: {}", err))
            ).post();
        });
}

void Index::Impl::updateFromLocalTree() {
    log::debug("Updating local index cache");
    log::pushNest();

    IndexUpdateEvent(UpdateProgress(100, "Updating local cache")).post();
    // delete old items
    m_items.clear();

    auto indexRoot = dirs::getIndexDir() / "v0";
    auto entriesRoot = indexRoot / "mods-v2";

    auto configRes = file::readJson(indexRoot / "config.json");
    if (!configRes) {
        IndexUpdateEvent("Unable to read index config").post();
        return;
    }
    auto config = configRes.unwrap();

    JsonChecker checker(config);
    auto root = checker.root("[config.json]").obj();

    for (auto& [modID, entry] : root.has("entries").items()) {
        for (auto& version : entry.obj().has("versions").iterate()) {
            auto rootDir = entriesRoot / modID;
            auto dir = rootDir / version.get<std::string>();

            auto addRes = IndexItem::Impl::create(rootDir, dir);
            if (!addRes) {
                log::warn("Unable to add index item from {}: {}", dir, addRes.unwrapErr());
                continue;
            }
            auto add = addRes.unwrap();
            auto metadata = add->getMetadata();

            m_items[modID].insert({metadata.getVersion(),
                add
            });
        }
    }

    // mark source as finished
    m_isUpToDate = true;
    IndexUpdateEvent(UpdateFinished()).post();

    log::popNest();
}

void Index::update(bool force) {
    // create index dir if it doesn't exist
    (void) file::createDirectoryAll(dirs::getIndexDir());

    m_impl->m_triedToUpdate = true;

    // check if update is already happening
    if (m_impl->m_updating) {
        return;
    }
    m_impl->m_updating = true;

    // update sources
    if (force) {
        m_impl->downloadIndex();
    }
    else {
        m_impl->checkForUpdates();
    }
}

// Items

std::vector<IndexItemHandle> Index::getItems() const {
    std::vector<IndexItemHandle> res;
    for (auto& items : map::values(m_impl->m_items)) {
        for (auto& item : items) {
            res.push_back(item.second);
        }
    }
    return res;
}

std::vector<IndexItemHandle> Index::getLatestItems() const {
    std::vector<IndexItemHandle> res;
    for (auto& [modID, versions] : m_impl->m_items) {
        res.push_back(this->getMajorItem(modID));
    }
    return res;
}

std::vector<IndexItemHandle> Index::getFeaturedItems() const {
    std::vector<IndexItemHandle> res;
    for (auto& items : map::values(m_impl->m_items)) {
        for (auto& item : items) {
            if (item.second->isFeatured()) {
                res.push_back(item.second);
            }
        }
    }
    return res;
}

std::vector<IndexItemHandle> Index::getItemsByDeveloper(
    std::string const& name
) const {
    std::vector<IndexItemHandle> res;
    for (auto& items : map::values(m_impl->m_items)) {
        for (auto& item : items) {
            if (item.second->getMetadata().getDeveloper() == name) {
                res.push_back(item.second);
            }
        }
    }
    return res;
}

std::vector<IndexItemHandle> Index::getItemsByModID(
    std::string const& modID
) const {
    std::vector<IndexItemHandle> res;
    if (m_impl->m_items.count(modID)) {
        for (auto& [versionStr, item] : m_impl->m_items[modID]) {
            res.push_back(item);
        }
    }
    return res;
}

bool Index::isKnownItem(
    std::string const& id,
    std::optional<VersionInfo> version
) const {
    return this->getItem(id, version).get();
}

IndexItemHandle Index::getMajorItem(
    std::string const& id
) const {
    if (m_impl->m_items.count(id)) {
        return m_impl->m_items.at(id).rbegin()->second;
    }
    return nullptr;
}

IndexItemHandle Index::getItem(
    std::string const& id,
    std::optional<VersionInfo> version
) const {
    if (m_impl->m_items.count(id)) {
        auto versions = m_impl->m_items.at(id);
        if (version) {
            for (auto& [_, item] : ranges::reverse(m_impl->m_items.at(id))) {
                if (version.value() == item->getMetadata().getVersion()) {
                    return item;
                }
            }
        }
    }
    return this->getMajorItem(id);
}

IndexItemHandle Index::getItem(
    std::string const& id,
    ComparableVersionInfo version
) const {
    if (m_impl->m_items.count(id)) {
        // prefer most major version
        for (auto& [_, item] : ranges::reverse(m_impl->m_items.at(id))) {
            if (version.compare(item->getMetadata().getVersion())) {
                return item;
            }
        }
    }
    return nullptr;
}

IndexItemHandle Index::getItem(ModInfo const& info) const {
    return this->getItem(info.id(), info.version());
}

IndexItemHandle Index::getItem(ModMetadata const& metadata) const {
    return this->getItem(metadata.getID(), metadata.getVersion());
}

IndexItemHandle Index::getItem(Mod* mod) const {
    return this->getItem(mod->getID(), mod->getVersion());
}

bool Index::isUpdateAvailable(IndexItemHandle item) const {
    auto installed = Loader::get()->getInstalledMod(item->getMetadata().getID());
    if (!installed) {
        return false;
    }
    return item->getMetadata().getVersion() > installed->getVersion();
}

bool Index::areUpdatesAvailable() const {
    for (auto& mod : Loader::get()->getAllMods()) {
        auto item = this->getMajorItem(mod->getID());
        if (item && item->getMetadata().getVersion() > mod->getVersion() && mod->isEnabled()) {
            return true;
        }
    }
    return false;
}

// Item installation

Result<> Index::canInstall(IndexItemHandle item) const {
    if (!item->getAvailablePlatforms().count(GEODE_PLATFORM_TARGET)) {
        return Err("Mod is not available on {}", GEODE_PLATFORM_NAME);
    }

    for (auto& dep : item->getMetadata().getDependencies()) {
        // if the dep is resolved, then all its dependencies must be installed
        // already in order for that to have happened
        if (dep.isResolved()) continue;

        if (dep.importance != ModMetadata::Dependency::Importance::Required) continue;

        // check if this dep is available in the index
        if (auto depItem = this->getItem(dep.id, dep.version)) {
            if (!depItem->getAvailablePlatforms().count(GEODE_PLATFORM_TARGET)) {
                return Err(
                    "Dependency {} is not available on {}",
                    dep.id, GEODE_PLATFORM_NAME
                );
            }
            // recursively add dependencies
            GEODE_UNWRAP_INTO(auto deps, this->canInstall(depItem));
        }
        // otherwise user must get this dependency manually from somewhere
        else {
            return Err(
                "Dependency {} version {} not found in the index! Likely "
                "reason is that the version of the dependency this mod "
                "depends on is not available. Please let the developer "
                "of the mod ({}) know!",
                dep.id, dep.version.toString(), item->getMetadata().getDeveloper()
            );
        }
    }

    return Ok();
}

Result<IndexInstallList> Index::getInstallList(IndexItemHandle item) const {
    if (!item->getAvailablePlatforms().count(GEODE_PLATFORM_TARGET)) {
        return Err("Mod is not available on {}", GEODE_PLATFORM_NAME);
    }

    IndexInstallList list;
    list.target = item;
    for (auto& dep : item->getMetadata().getDependencies()) {
        // if the dep is resolved, then all its dependencies must be installed
        // already in order for that to have happened
        if (dep.isResolved()) continue;

        if (dep.importance == ModMetadata::Dependency::Importance::Suggested) continue;

        if (Loader::get()->isModInstalled(dep.id)) continue;

        // check if this dep is available in the index
        if (auto depItem = this->getItem(dep.id, dep.version)) {
            if (!depItem->getAvailablePlatforms().count(GEODE_PLATFORM_TARGET)) {
                // it's fine to not install optional dependencies
                if (dep.importance != ModMetadata::Dependency::Importance::Required) continue;
                return Err(
                    "Dependency {} is not available on {}",
                    dep.id, GEODE_PLATFORM_NAME
                );
            }
            // recursively add dependencies
            GEODE_UNWRAP_INTO(auto deps, this->getInstallList(depItem));
            ranges::push(list.list, deps.list);
        }
        // otherwise user must get this dependency manually from somewhere
        else {
            // it's fine to not install optional dependencies
            if (dep.importance != ModMetadata::Dependency::Importance::Required) continue;
            return Err(
                "Dependency {} version {} not found in the index! Likely "
                "reason is that the version of the dependency this mod "
                "depends on is not available. Please let the developer "
                "of the mod ({}) know!",
                dep.id, dep.version.toString(), item->getMetadata().getDeveloper()
            );
        }
    }
    // add this item to the end of the list
    list.list.push_back(item);
    return Ok(list);
}

void Index::Impl::installNext(size_t index, IndexInstallList const& list) {
    auto postError = [this, list](std::string const& error) {
        m_runningInstallations.erase(list.target);
        ModInstallEvent(list.target->getMetadata().getID(), error).post();
    };

    // If we're at the end of the list, move the downloaded items to mods
    if (index >= list.list.size()) {
        m_runningInstallations.erase(list.target);
        // Move all downloaded files
        for (auto& item : list.list) {
            // If the mod is already installed, delete the old .geode file
            if (auto mod = Loader::get()->getInstalledMod(item->getMetadata().getID())) {
                auto res = mod->uninstall();
                if (!res) {
                    return postError(fmt::format(
                        "Unable to uninstall old version of {}: {}",
                        item->getMetadata().getID(), res.unwrapErr()
                    ));
                }
            }

            // Move the temp file
            try {
                ghc::filesystem::rename(
                    dirs::getTempDir() / (item->getMetadata().getID() + ".index"),
                    dirs::getModsDir() / (item->getMetadata().getID() + ".geode")
                );
            } catch(std::exception& e) {
                return postError(fmt::format(
                    "Unable to install {}: {}",
                    item->getMetadata().getID(), e.what()
                ));
            }
        }

        auto const& eventModID = list.target->getMetadata().getID();
        Loader::get()->queueInMainThread([eventModID]() {
            ModInstallEvent(eventModID, UpdateFinished()).post();
        });

        return;
    }

    auto scaledProgress = [index, list](double progress) -> uint8_t {
        return static_cast<uint8_t>(
            progress * (static_cast<double>(index + 1) / list.list.size())
        );
    };

    auto item = list.list.at(index);
    auto tempFile = dirs::getTempDir() / (item->getMetadata().getID() + ".index");
    m_runningInstallations[list.target] = web::AsyncWebRequest()
        .join("install_item_" + item->getMetadata().getID())
        .fetch(item->getDownloadURL())
        .into(tempFile)
        .then([=](auto) {
            // Check for 404
            auto notFound = utils::file::readString(tempFile);
            if (notFound && notFound.unwrap() == "Not Found") {
                return postError(fmt::format(
                    "Binary file download for {} returned \"404 Not found\". "
                    "Report this to the Geode development team.",
                    item->getMetadata().getID()
                ));
            }

            // Verify checksum
            ModInstallEvent(
                list.target->getMetadata().getID(),
                UpdateProgress(
                    scaledProgress(100),
                    fmt::format("Verifying {}", item->getMetadata().getID())
                )
            ).post();

            if (::calculateHash(tempFile) != item->getPackageHash()) {
                return postError(fmt::format(
                    "Checksum mismatch with {}! (Downloaded file did not match what "
                    "was expected. Try again, and if the download fails another time, "
                    "report this to the Geode development team.)",
                    item->getMetadata().getID()
                ));
            }

            item->setIsInstalled(true);

            // Install next item in queue
            this->installNext(index + 1, list);
        })
        .expect([postError, list, item](std::string const& err) {
            postError(fmt::format(
                "Unable to download {}: {}",
                item->getMetadata().getID(), err
            ));
        })
        .progress([this, item, list, scaledProgress](auto&, double now, double total) {
            ModInstallEvent(
                list.target->getMetadata().getID(),
                UpdateProgress(
                    scaledProgress(now / total * 100.0),
                    fmt::format("Downloading {}", item->getMetadata().getID())
                )
            ).post();
        })
        .cancelled([postError](auto&) {
            postError("Download cancelled");
        })
        .send();
}

void Index::cancelInstall(IndexItemHandle item) {
    Loader::get()->queueInMainThread([this, item]() {
        if (m_impl->m_runningInstallations.count(item)) {
            m_impl->m_runningInstallations.at(item)->cancel();
            m_impl->m_runningInstallations.erase(item);
        }
    });
}

void Index::install(IndexInstallList const& list) {
    if (list.list.empty()) {
        ModInstallEvent(list.target->getMetadata().getID(), UpdateFinished()).post();
        return;
    }
    Loader::get()->queueInMainThread([this, list]() {
        m_impl->installNext(0, list);
    });
}

void Index::install(IndexItemHandle item) {
    Loader::get()->queueInMainThread([this, item]() {
        if (m_impl->m_runningInstallations.count(item)) {
            return;
        }
        auto list = this->getInstallList(item);
        if (list) {
            this->install(list.unwrap());
        } else {
            ModInstallEvent(
                item->getMetadata().getID(),
                UpdateFailed(list.unwrapErr())
            ).post();
        }
    });
}

// Item properites

std::unordered_set<std::string> Index::getTags() const {
    std::unordered_set<std::string> tags;
    for (auto& [_, versions] : m_impl->m_items) {
        for (auto& [_, item] : versions) {
            for (auto& tag : item->getTags()) {
                tags.insert(tag);
            }
        }
    }
    return tags;
}
