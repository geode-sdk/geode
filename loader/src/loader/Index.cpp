#include <Geode/loader/Index.hpp>
#include <Geode/loader/Loader.hpp>
#include <Geode/loader/Dirs.hpp>
#include <Geode/utils/ranges.hpp>
#include <Geode/utils/web.hpp>
#include <Geode/utils/string.hpp>
#include <Geode/utils/map.hpp>

USE_GEODE_NAMESPACE();

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

    ListenerResult handle(std::function<Callback> fn, SourceUpdateEvent* event) {
        fn(event);
        return ListenerResult::Propagate;
    }
    SourceUpdateFilter() {}
};

// Save data

struct IndexSourceSaveData {
    std::string downloadedCommitSHA;
};
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(IndexSourceSaveData, downloadedCommitSHA);

struct IndexSaveData {
    std::unordered_map<std::string, IndexSourceSaveData> sources;
};
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(IndexSaveData, sources);

// ModInstallEvent

ListenerResult ModInstallFilter::handle(std::function<Callback> fn, ModInstallEvent* event) {
    if (m_id == event->modID) {
        fn(event);
    }
    return ListenerResult::Propagate;
}

ModInstallFilter::ModInstallFilter(
    std::string const& id
) : m_id(id) {}

// IndexUpdateEvent

IndexUpdateEvent::IndexUpdateEvent(
    const UpdateStatus status
) : status(status) {}

ListenerResult IndexUpdateFilter::handle(
    std::function<Callback> fn,
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
        .isFeatured = root.has("is-featured").template get<bool>(),
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
    GEODE_UNWRAP_INTO(auto files, file::listFiles(dir));
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

// Index

Index::Index() {
    new EventListener(
        std::bind(&Index::onSourceUpdate, this, std::placeholders::_1),
        SourceUpdateFilter()
    );
    this->addSource("geode-sdk/index-test");
}

Index* Index::get() {
    static auto inst = new Index();
    return inst;
}

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
        else if (std::holds_alternative<UpdateError>(status)) {
            if (whatToPost != Progress) {
                whatToPost = Failed;
            }
        }
        // otherwise if all are finished, whatToPost is already set to that
    }

    switch (whatToPost) {
        case Finished: {
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
                if (std::holds_alternative<UpdateError>(status)) {
                    info += src + ": " + std::get<UpdateError>(status) + "\n";
                }
            }
            // clear source statuses to allow updating index again
            m_sourceStatuses.clear();
            // post finish event
            IndexUpdateEvent(UpdateError(info)).post();
        } break;
    }
}

void Index::checkSourceUpdates(IndexSourceImpl* src) {
    if (src->isUpToDate) {
        return this->updateSourceFromLocal(src);
    }
    SourceUpdateEvent(src, UpdateProgress(0, "Checking status")).post();
    auto data = Mod::get()->getSavedMutable<IndexSaveData>("index");
    auto oldSHA = data.sources[src->repository].downloadedCommitSHA;
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
                auto data = Mod::get()->getSavedMutable<IndexSaveData>("index");
                data.sources[src->repository].downloadedCommitSHA = newSHA;
                this->downloadSource(src);
            }
        })
        .expect([src](std::string const& err) {
            SourceUpdateEvent(
                src,
                UpdateError(fmt::format("Error checking for updates: {}", err))
            ).post();
        });
}

void Index::downloadSource(IndexSourceImpl* src) {
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
                return SourceUpdateEvent(
                    src, UpdateError("Unable to clear cached index")
                ).post();
            }

            // unzip new index
            auto unzip = file::Unzip::intoDir(targetFile, targetDir, true)
                .expect("Unable to unzip new index");
            if (!unzip) {
                return SourceUpdateEvent(
                    src, UpdateError(unzip.unwrapErr())
                ).post();
            }

            // remove the directory github adds to the root of the zip
            (void)flattenGithubRepo(targetDir);

            // update index
            this->updateSourceFromLocal(src);
        })
        .expect([src](std::string const& err) {
            SourceUpdateEvent(
                src, UpdateError(fmt::format("Error downloading: {}", err))
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
            if (m_items[add->info.m_id].count(add->info.m_version.getMajor())) {
                log::warn(
                    "Item {}@{} has already been added, skipping",
                    add->info.m_id, add->info.m_version
                );
                continue;
            }
            // add new major version of this item
            m_items[add->info.m_id].insert({
                add->info.m_version.getMajor(),
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

    // update all sources in GD thread
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

std::vector<IndexItemHandle> Index::getItems() const {
    std::vector<IndexItemHandle> res;
    for (auto& items : map::values(m_items)) {
        if (items.size()) {
            res.push_back(items.rbegin()->second);
        }
    }
    return res;
}

bool Index::isKnownItem(
    std::string const& id,
    std::optional<size_t> version
) const {
    if (m_items.count(id)) {
        if (version) {
            return m_items.at(id).count(version.value());
        } else {
            return true;
        }
    } else {
        return false;
    }
}

IndexItemHandle Index::getItem(
    std::string const& id,
    std::optional<size_t> version
) const {
    if (m_items.count(id)) {
        auto versions = m_items.at(id);
        if (version) {
            if (versions.count(version.value())) {
                return versions.at(version.value());
            }
        } else {
            if (versions.size()) {
                return m_items.at(id).rbegin()->second;
            }
        }
    }
    return nullptr;
}

IndexItemHandle Index::getItem(ModInfo const& info) const {
    return this->getItem(info.m_id, info.m_version.getMajor());
}

IndexItemHandle Index::getItem(Mod* mod) const {
    return this->getItem(mod->getID(), mod->getVersion().getMajor());
}

bool Index::updateAvailable(IndexItemHandle item) const {
    auto installed = Loader::get()->getInstalledMod(item->info.m_id);
    if (!installed) {
        return false;
    }
    return item->info.m_version > installed->getVersion();
}
