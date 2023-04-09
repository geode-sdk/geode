#include <Geode/loader/Index.hpp>
#include <Geode/loader/Loader.hpp>
#include <Geode/loader/Dirs.hpp>
#include <Geode/utils/ranges.hpp>
#include <Geode/utils/web.hpp>
#include <Geode/utils/string.hpp>
#include <Geode/utils/map.hpp>
#include <hash/hash.hpp>
#include <Geode/utils/JsonValidation.hpp>


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

// IndexUpdateEvent implementation

// The reason sources have private implementation events that are 
// turned into the global IndexUpdateEvent is because it makes it much 
// simpler to keep track of progress, what errors were received, etc. 
// without having to store a ton of members

struct geode::IndexSourceImpl final {
    std::string repository;
    bool isUpToDate = false;

    std::string dirname() const {
        return string::replace(this->repository, "/", "_");
    }

    ghc::filesystem::path path() const {
        return dirs::getIndexDir() / this->dirname();
    }

    ghc::filesystem::path checksum() const {
        // not storing this in the source's directory as that gets replaced by 
        // the newly fetched index
        return dirs::getIndexDir() / (this->dirname() + ".checksum");
    }
};

void IndexSourceImplDeleter::operator()(IndexSourceImpl* src) {
    delete src;
}

struct geode::SourceUpdateEvent : public Event {
    IndexSourceImpl* source;
    const UpdateStatus status;
    SourceUpdateEvent(IndexSourceImpl* src, const UpdateStatus status)
      : source(src), status(status) {}
};

class SourceUpdateFilter : public EventFilter<SourceUpdateEvent> {
public:
    using Callback = void(SourceUpdateEvent*);

    ListenerResult handle(utils::MiniFunction<Callback> fn, SourceUpdateEvent* event) {
        fn(event);
        return ListenerResult::Propagate;
    }
    SourceUpdateFilter() {}
};

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

Result<IndexItemHandle> IndexItem::createFromDir(
    std::string const& sourceRepository,
    ghc::filesystem::path const& dir
) {
    GEODE_UNWRAP_INTO(
        auto entry, file::readJson(dir / "entry.json")
            .expect("Unable to read entry.json")
    );
    GEODE_UNWRAP_INTO(
        auto info, ModInfo::createFromFile(dir / "mod.json")
            .expect("Unable to read mod.json: {error}")
    );

    JsonChecker checker(entry);
    auto root = checker.root("[entry.json]").obj();

    std::unordered_set<PlatformID> platforms;
    for (auto& plat : root.has("platforms").iterate()) {
        platforms.insert(PlatformID::from(plat.template get<std::string>()));
    }

    auto item = std::make_shared<IndexItem>(IndexItem {
        .sourceRepository = sourceRepository,
        .path = dir,
        .info = info,
        .download = {
            .url = root.has("mod").obj().has("download").template get<std::string>(),
            .hash = root.has("mod").obj().has("hash").template get<std::string>(),
            .platforms = platforms,
        },
        .isFeatured = root.has("featured").template get<bool>(),
        .tags = root.has("tags").template get<std::unordered_set<std::string>>()
    });
    if (checker.isError()) {
        return Err(checker.getError());
    }
    return Ok(item);
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
                ghc::filesystem::rename(
                    file, dir / ghc::filesystem::relative(file, files[0])
                );
            }
            ghc::filesystem::remove(files[0]);
        }
    } catch(std::exception& e) {
        return Err(e.what());
    }
    return Ok();
}

// Index globals

Index::Index() {
    new EventListener(
        std::bind(&Index::onSourceUpdate, this, std::placeholders::_1),
        SourceUpdateFilter()
    );
    this->addSource("geode-sdk/mods");
}

Index* Index::get() {
    static auto inst = new Index();
    return inst;
}

// Sources

void Index::addSource(std::string const& repository) {
    m_sources.emplace_back(new IndexSourceImpl {
        .repository = repository
    });
}

void Index::removeSource(std::string const& repository) {
    ranges::remove(m_sources, [repository](IndexSourcePtr const& src) {
        return src->repository == repository;
    });
}

std::vector<std::string> Index::getSources() const {
    std::vector<std::string> res;
    for (auto& src : m_sources) {
        res.push_back(src->repository);
    }
    return res;
}

// Updating

void Index::onSourceUpdate(SourceUpdateEvent* event) {
    // save status for aggregating SourceUpdateEvents to a single global 
    // IndexUpdateEvent
    m_sourceStatuses[event->source->repository] = event->status;

    // figure out aggregate event
    enum { Finished, Progress, Failed, } whatToPost = Finished;
    for (auto& [src, status] : m_sourceStatuses) {
        // if some source is still updating, post progress
        if (std::holds_alternative<UpdateProgress>(status)) {
            whatToPost = Progress;
            break;
        }
        // otherwise, if some source failed, then post failed
        else if (std::holds_alternative<UpdateFailed>(status)) {
            if (whatToPost != Progress) {
                whatToPost = Failed;
            }
        }
        // otherwise if all are finished, whatToPost is already set to that
    }

    switch (whatToPost) {
        case Finished: {
            log::debug("Index up-to-date");
            // clear source statuses to allow updating index again
            m_sourceStatuses.clear();
            // post finish event
            IndexUpdateEvent(UpdateFinished()).post();
        } break;

        case Progress: {
            // get total progress
            size_t total = 0;
            for (auto& [src, status] : m_sourceStatuses) {
                if (std::holds_alternative<UpdateProgress>(status)) {
                    total += std::get<UpdateProgress>(status).first;
                } else {
                    total += 100;
                }
            }
            IndexUpdateEvent(
                UpdateProgress(
                    static_cast<uint8_t>(total / m_sourceStatuses.size()),
                    "Downloading"
                )
            ).post();
        } break;

        case Failed: {
            std::string info = "";
            for (auto& [src, status] : m_sourceStatuses) {
                if (std::holds_alternative<UpdateFailed>(status)) {
                    info += src + ": " + std::get<UpdateFailed>(status) + "\n";
                }
            }
            log::debug("Index update failed: {}", info);
            // clear source statuses to allow updating index again
            m_sourceStatuses.clear();
            // post finish event
            IndexUpdateEvent(UpdateFailed(info)).post();
        } break;
    }
}

void Index::checkSourceUpdates(IndexSourceImpl* src) {
    if (src->isUpToDate) {
        return this->updateSourceFromLocal(src);
    }

    log::debug("Checking updates for source {}", src->repository);
    SourceUpdateEvent(src, UpdateProgress(0, "Checking status")).post();

    // read old commit SHA
    // not using saved values for this one as we don't want to refetch 
    // index even if the game crashes
    auto oldSHA = file::readString(src->checksum()).unwrapOr("");
    web::AsyncWebRequest()
        .join(fmt::format("index-update-{}", src->repository))
        .header(fmt::format("If-None-Match: \"{}\"", oldSHA))
        .header("Accept: application/vnd.github.sha")
        .fetch(fmt::format("https://api.github.com/repos/{}/commits/main", src->repository))
        .text()
        .then([this, src, oldSHA](std::string const& newSHA) {
            // check if should just be updated from local cache
            if (
                // if no new hash was given (rate limited) or the new hash is the 
                // same as old
                (newSHA.empty() || oldSHA == newSHA) &&
                // make sure the downloaded local copy actually exists
                ghc::filesystem::exists(src->path()) &&
                ghc::filesystem::exists(src->path() / "config.json")
            ) {
                this->updateSourceFromLocal(src);
            }
            // otherwise save hash and download source
            else {
                (void)file::writeString(src->checksum(), newSHA);
                this->downloadSource(src);
            }
        })
        .expect([src](std::string const& err) {
            SourceUpdateEvent(
                src,
                UpdateFailed(fmt::format("Error checking for updates: {}", err))
            ).post();
        });
}

void Index::downloadSource(IndexSourceImpl* src) {
    log::debug("Downloading source {}", src->repository);

    SourceUpdateEvent(src, UpdateProgress(0, "Beginning download")).post();

    auto targetFile = dirs::getIndexDir() / fmt::format("{}.zip", src->dirname());

    web::AsyncWebRequest()
        .join(fmt::format("index-download-{}", src->repository))
        .fetch(fmt::format("https://github.com/{}/zipball/main", src->repository))
        .into(targetFile)
        .then([this, src, targetFile](auto) {
            auto targetDir = src->path();
            // delete old unzipped index
            try {
                if (ghc::filesystem::exists(targetDir)) {
                    ghc::filesystem::remove_all(targetDir);
                }
            }
            catch(...) {
                SourceUpdateEvent(
                    src, UpdateFailed("Unable to clear cached index")
                ).post();
                return;
            }

            // unzip new index
            auto unzip = file::Unzip::intoDir(targetFile, targetDir, true)
                .expect("Unable to unzip new index");
            if (!unzip) {
                SourceUpdateEvent(
                    src, UpdateFailed(unzip.unwrapErr())
                ).post();
                return;
            }

            // remove the directory github adds to the root of the zip
            (void)flattenGithubRepo(targetDir);

            // update index
            this->updateSourceFromLocal(src);
        })
        .expect([src](std::string const& err) {
            SourceUpdateEvent(
                src, UpdateFailed(fmt::format("Error downloading: {}", err))
            ).post();
        })
        .progress([src](auto&, double now, double total) {
            SourceUpdateEvent(
                src,
                UpdateProgress(
                    static_cast<uint8_t>(now / total * 100.0),
                    "Downloading"
                )
            ).post();
        });
}

void Index::updateSourceFromLocal(IndexSourceImpl* src) {
    log::debug("Updating local cache for source {}", src->repository);
    SourceUpdateEvent(src, UpdateProgress(100, "Updating local cache")).post();
    // delete old items from this url if such exist
    for (auto& [_, versions] : m_items) {
        for (auto it = versions.begin(); it != versions.end(); ) {
            if (it->second->sourceRepository == src->repository) {
                it = versions.erase(it);
            } else {
                ++it;
            }
        }
    }
    this->cleanupItems();

    // read directory and add new items
    try {
        for (auto& dir : ghc::filesystem::directory_iterator(src->path() / "mods")) {
            auto addRes = IndexItem::createFromDir(src->repository, dir);
            if (!addRes) {
                log::warn("Unable to add index item from {}: {}", dir, addRes.unwrapErr());
                continue;
            }
            auto add = addRes.unwrap();
            // check if this major version of this item has already been added 
            if (m_items[add->info.id()].count(add->info.version().getMajor())) {
                log::warn(
                    "Item {}@{} has already been added, skipping",
                    add->info.id(), add->info.version()
                );
                continue;
            }
            // add new major version of this item
            m_items[add->info.id()].insert({
                add->info.version().getMajor(),
                add
            });
        }
    } catch(std::exception& e) {
        SourceUpdateEvent(src, fmt::format(
            "Unable to read source {}", src->repository
        )).post();
        return;
    }

    // mark source as finished
    src->isUpToDate = true;
    SourceUpdateEvent(src, UpdateFinished()).post();
}

void Index::cleanupItems() {
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
    for (auto& source : m_sources) {
        if (!source->isUpToDate) {
            return false;
        }
    }
    return true;
}

bool Index::hasTriedToUpdate() const {
    return m_triedToUpdate;
}

void Index::update(bool force) {
    // create index dir if it doesn't exist
    (void)file::createDirectoryAll(dirs::getIndexDir());

    m_triedToUpdate = true;

    // update all sources in GD thread for synchronization (m_sourceStatuses 
    // and every other member access happens in AsyncWebRequest callbacks 
    // which are always run in the GD thread aswell)
    Loader::get()->queueInGDThread([force, this]() {
        // check if some sources are already being updated
        if (m_sourceStatuses.size()) {
            return;
        }

        // update sources
        for (auto& src : m_sources) {
            if (force) {
                this->downloadSource(src.get());
            } else {
                this->checkSourceUpdates(src.get());
            }
        }
    });
}

// Items

std::vector<IndexItemHandle> Index::getItems() const {
    std::vector<IndexItemHandle> res;
    for (auto& items : map::values(m_items)) {
        if (items.size()) {
            res.push_back(items.rbegin()->second);
        }
    }
    return res;
}

std::vector<IndexItemHandle> Index::getFeaturedItems() const {
    std::vector<IndexItemHandle> res;
    for (auto& items : map::values(m_items)) {
        if (items.size()) {
            if (items.rbegin()->second->isFeatured) {
                res.push_back(items.rbegin()->second);
            }
        }
    }
    return res;
}

std::vector<IndexItemHandle> Index::getItemsByDeveloper(
    std::string const& name
) const {
    std::vector<IndexItemHandle> res;
    for (auto& items : map::values(m_items)) {
        if (items.size()) {
            if (items.rbegin()->second->info.developer() == name) {
                res.push_back(items.rbegin()->second);
            }
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

IndexItemHandle Index::getItem(
    std::string const& id,
    std::optional<VersionInfo> version
) const {
    if (m_items.count(id)) {
        auto versions = m_items.at(id);
        if (version) {
            // prefer most major version
            for (auto& [_, item] : ranges::reverse(m_items.at(id))) {
                if (version.value() == item->info.version()) {
                    return item;
                }
            }
        } else {
            if (versions.size()) {
                return m_items.at(id).rbegin()->second;
            }
        }
    }
    return nullptr;
}

IndexItemHandle Index::getItem(
    std::string const& id,
    ComparableVersionInfo version
) const {
    if (m_items.count(id)) {
        // prefer most major version
        for (auto& [_, item] : ranges::reverse(m_items.at(id))) {
            if (version.compare(item->info.version())) {
                return item;
            }
        }
    }
    return nullptr;
}

IndexItemHandle Index::getItem(ModInfo const& info) const {
    return this->getItem(info.id(), info.version());
}

IndexItemHandle Index::getItem(Mod* mod) const {
    return this->getItem(mod->getID(), mod->getVersion());
}

bool Index::isUpdateAvailable(IndexItemHandle item) const {
    auto installed = Loader::get()->getInstalledMod(item->info.id());
    if (!installed) {
        return false;
    }
    return item->info.version() > installed->getVersion();
}

bool Index::areUpdatesAvailable() const {
    for (auto& mod : Loader::get()->getAllMods()) {
        auto item = this->getItem(mod);
        if (item && item->info.version() > mod->getVersion()) {
            return true;
        }
    }
    return false;
}

// Item installation

Result<IndexInstallList> Index::getInstallList(IndexItemHandle item) const {
    if (!item->download.platforms.count(GEODE_PLATFORM_TARGET)) {
        return Err("Mod is not available on {}", GEODE_PLATFORM_NAME);
    }
    
    IndexInstallList list;
    list.target = item;
    for (auto& dep : item->info.dependencies()) {
        if (!dep.isResolved()) {
            // check if this dep is available in the index
            if (auto depItem = this->getItem(dep.id, dep.version)) {
                if (!depItem->download.platforms.count(GEODE_PLATFORM_TARGET)) {
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
            // else
            else {
                return Err(
                    "Dependency {} version {} not found in the index! Likely "
                    "reason is that the version of the dependency this mod "
                    "depends on is not available. Please let the the developer "
                    "({}) of the mod know!",
                    dep.id, dep.version.toString(), item->info.developer()
                );
            }
        }
        // if the dep is resolved, then all its dependencies must be installed 
        // already in order for that to have happened
    }
    // add this item to the end of the list
    list.list.push_back(item);
    return Ok(list);
}

void Index::installNext(size_t index, IndexInstallList const& list) {
    auto postError = [this, list](std::string const& error) {
        m_runningInstallations.erase(list.target);
        ModInstallEvent(list.target->info.id(), error).post();
    };

    // If we're at the end of the list, move the downloaded items to mods
    if (index >= list.list.size()) {
        m_runningInstallations.erase(list.target);
        // Move all downloaded files
        for (auto& item : list.list) {
            // If the mod is already installed, delete the old .geode file
            if (auto mod = Loader::get()->getInstalledMod(item->info.id())) {
                auto res = mod->uninstall();
                if (!res) {
                    return postError(fmt::format(
                        "Unable to uninstall old version of {}: {}",
                        item->info.id(), res.unwrapErr()
                    ));
                }
            }

            // Move the temp file
            try {
                ghc::filesystem::rename(
                    dirs::getTempDir() / (item->info.id() + ".index"),
                    dirs::getModsDir() / (item->info.id() + ".geode")
                );
            } catch(std::exception& e) {
                return postError(fmt::format(
                    "Unable to install {}: {}",
                    item->info.id(), e.what()
                ));
            }
        }
        
        // load mods
        Loader::get()->refreshModsList();

        ModInstallEvent(list.target->info.id(), UpdateFinished()).post();
        return;
    }

    auto scaledProgress = [index, list](double progress) -> uint8_t {
        return static_cast<uint8_t>(
            progress * (static_cast<double>(index + 1) / list.list.size())
        );
    };

    auto item = list.list.at(index);
    auto tempFile = dirs::getTempDir() / (item->info.id() + ".index");
    m_runningInstallations[list.target] = web::AsyncWebRequest()
        .join("install_item_" + item->info.id())
        .fetch(item->download.url)
        .into(tempFile)
        .then([=](auto) {
            // Check for 404
            auto notFound = utils::file::readString(tempFile);
            if (notFound && notFound.unwrap() == "Not Found") {
                return postError(fmt::format(
                    "Binary file download for {} returned \"404 Not found\". "
                    "Report this to the Geode development team.",
                    item->info.id()
                ));
            }

            // Verify checksum
            ModInstallEvent(
                list.target->info.id(),
                UpdateProgress(
                    scaledProgress(100),
                    fmt::format("Verifying {}", item->info.id())
                )
            ).post();
            
            if (::calculateHash(tempFile) != item->download.hash) {
                return postError(fmt::format(
                    "Checksum mismatch with {}! (Downloaded file did not match what "
                    "was expected. Try again, and if the download fails another time, "
                    "report this to the Geode development team.)",
                    item->info.id()
                ));
            }

            // Install next item in queue
            this->installNext(index + 1, list);
        })
        .expect([postError, list, item](std::string const& err) {
            postError(fmt::format(
                "Unable to download {}: {}",
                item->info.id(), err
            ));
        })
        .progress([this, item, list, scaledProgress](auto&, double now, double total) {
            ModInstallEvent(
                list.target->info.id(),
                UpdateProgress(
                    scaledProgress(now / total * 100.0),
                    fmt::format("Downloading {}", item->info.id())
                )
            ).post();
        })
        .cancelled([postError](auto&) {
            postError("Download cancelled");
        })
        .send();
}

void Index::cancelInstall(IndexItemHandle item) {
    Loader::get()->queueInGDThread([this, item]() {
        if (m_runningInstallations.count(item)) {
            m_runningInstallations.at(item)->cancel();
            m_runningInstallations.erase(item);
        }
    });
}

void Index::install(IndexInstallList const& list) {
    Loader::get()->queueInGDThread([this, list]() {
        this->installNext(0, list);
    });
}

void Index::install(IndexItemHandle item) {
    Loader::get()->queueInGDThread([this, item]() {
        if (m_runningInstallations.count(item)) {
            return;
        }
        auto list = this->getInstallList(item);
        if (list) {
            this->install(list.unwrap());
        } else {
            ModInstallEvent(
                item->info.id(),
                UpdateFailed(list.unwrapErr())
            ).post();
        }
    });
}

// Item properites

std::unordered_set<std::string> Index::getTags() const {
    std::unordered_set<std::string> tags;
    for (auto& [_, versions] : m_items) {
        for (auto& [_, item] : versions) {
            for (auto& tag : item->tags) {
                tags.insert(tag);
            }
        }
    }
    return tags;
}
