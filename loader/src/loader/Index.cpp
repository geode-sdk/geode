#include <Geode/loader/Index.hpp>
#include <Geode/loader/Loader.hpp>
#include <Geode/loader/Dirs.hpp>
#include <Geode/utils/ranges.hpp>
#include <Geode/utils/web.hpp>
#include <Geode/utils/string.hpp>
#include <Geode/utils/map.hpp>

USE_GEODE_NAMESPACE();

struct IndexSourceSaveData {
    std::string downloadedCommitSHA;
};
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(IndexSourceSaveData, downloadedCommitSHA);

struct IndexSaveData {
    std::unordered_map<std::string, IndexSourceSaveData> sources;
};
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(IndexSaveData, sources);

std::string IndexSource::dirname() const {
    return string::replace(this->repository, "/", "_");
}

// IndexUpdateEvent

IndexUpdateEvent::IndexUpdateEvent(
    std::string const& src,
    UpdateStatus status
) : m_sourceRepository(src), m_status(status) {}

std::string IndexUpdateEvent::getSource() const {
    return m_sourceRepository;
}

UpdateStatus IndexUpdateEvent::getStatus() const {
    return m_status;
}

ListenerResult IndexUpdateFilter::handle(
    std::function<Callback> fn,
    IndexUpdateEvent* event
) {
    fn(event);
    return ListenerResult::Propagate;
}

IndexUpdateFilter::IndexUpdateFilter() {}

// ModInstallEvent

ModInstallEvent::ModInstallEvent(
    std::string const& id,
    UpdateStatus status
) : m_id(id), m_status(status) {}

std::string ModInstallEvent::getModID() const {
    return m_id;
}

UpdateStatus ModInstallEvent::getStatus() const {
    return m_status;
}

ListenerResult ModInstallFilter::handle(std::function<Callback> fn, ModInstallEvent* event) {
    if (m_id == event->getModID()) {
        fn(event);
    }
    return ListenerResult::Propagate;
}

ModInstallFilter::ModInstallFilter(
    std::string const& id
) : m_id(id) {}

// Index

Index::Index() {
    this->addSource("https://github.com/geode-sdk/index-test");
}

Index* Index::get() {
    auto inst = new Index();
    return inst;
}

void Index::addSource(std::string const& repository) {
    m_sources.push_back(IndexSource {
        .repository = repository
    });
}

void Index::removeSource(std::string const& repository) {
    ranges::remove(m_sources, [repository](IndexSource const& src) {
        return src.repository == repository;
    });
}

std::vector<IndexSource> Index::getSources() const {
    return m_sources;
}

bool Index::isUpToDate() const {
    for (auto& source : m_sources) {
        if (!source.isUpToDate) {
            return false;
        }
    }
    return true;
}

void Index::checkSourceUpdates(IndexSource& src) {
    if (src.isUpToDate) {
        return this->updateSourceFromLocal(src);
    }
    IndexUpdateEvent(src.repository, UpdateProgress(0, "Checking status")).post();
    auto data = Mod::get()->getSavedMutable<IndexSaveData>("index");
    auto oldSHA = data.sources[src.repository].downloadedCommitSHA;
    web::AsyncWebRequest()
        .join(fmt::format("index-update-{}", src.repository))
        .header(fmt::format("If-None-Match: \"{}\"", oldSHA))
        .header("Accept: application/vnd.github.sha")
        .fetch(fmt::format("https://api.github.com/repos/{}/commits/main", src.repository))
        .text()
        .then([this, &src, oldSHA](std::string const& newSHA) {
            // if no new hash was given (rate limited) or the new hash is the 
            // same as old, then just update from local cache
            if (newSHA.empty() || oldSHA == newSHA) {
                this->updateSourceFromLocal(src);
            }
            // otherwise save hash and download source
            else {
                auto data = Mod::get()->getSavedMutable<IndexSaveData>("index");
                data.sources[src.repository].downloadedCommitSHA = newSHA;
                this->downloadSource(src);
            }
        })
        .expect([&src](std::string const& err) {
            IndexUpdateEvent(
                src.repository,
                UpdateError(fmt::format("Error checking for updates: {}", err))
            ).post();
        });
}

void Index::downloadSource(IndexSource& src) {
    IndexUpdateEvent(src.repository, UpdateProgress(0, "Beginning download")).post();

    auto targetFile = dirs::getIndexDir() / fmt::format("{}.zip", src.dirname());

    web::AsyncWebRequest()
        .join(fmt::format("index-download-{}", src.repository))
        .fetch(fmt::format("https://github.com/{}/zipball/main", src.repository))
        .into(targetFile)
        .then([this, &src, targetFile](auto) {
            auto targetDir = dirs::getIndexDir() / src.dirname();
            // delete old unzipped index
            try {
                if (ghc::filesystem::exists(targetDir)) {
                    ghc::filesystem::remove_all(targetDir);
                }
            }
            catch(...) {
                return IndexUpdateEvent(
                    src.repository,
                    UpdateError("Unable to clear cached index")
                ).post();
            }

            // unzip new index
            auto unzip = file::Unzip::intoDir(targetFile, targetDir, true);
            if (!unzip) {
                return IndexUpdateEvent(
                    src.repository,
                    UpdateError("Unable to unzip new index")
                ).post();
            }

            // update index
            this->updateSourceFromLocal(src);
        })
        .expect([&src](std::string const& err) {
            IndexUpdateEvent(
                src.repository,
                UpdateError(fmt::format("Error downloading: {}", err))
            ).post();
        })
        .progress([&src](auto&, double now, double total) {
            IndexUpdateEvent(
                src.repository,
                UpdateProgress(static_cast<uint8_t>(now / total * 100.0), "Downloading")
            ).post();
        });
}

void Index::updateSourceFromLocal(IndexSource& src) {
    IndexUpdateEvent(src.repository, UpdateProgress(100, "Updating local cache")).post();
    // delete old items from this url if such exist
    for (auto& [_, versions] : m_items) {
        for (auto it = versions.begin(); it != versions.end(); ) {
            if (it->second.sourceRepository == src.repository) {
                it = versions.erase(it);
            } else {
                ++it;
            }
        }
    }
    this->cleanupItems();

    // todo: add shit

    src.isUpToDate = true;
    IndexUpdateEvent(src.repository, UpdateFinished()).post();
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

void Index::update(bool force) {
    // create index dir if it doesn't exist
    (void)file::createDirectoryAll(dirs::getIndexDir());

    // update all sources in GD thread
    Loader::get()->queueInGDThread([force, this]() {
        for (auto& src : m_sources) {
            if (force) {
                this->downloadSource(src);
            } else {
                this->checkSourceUpdates(src);
            }
        }
    });
}

std::vector<IndexItem> Index::getItems() const {
    std::vector<IndexItem> res;
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

std::optional<IndexItem> Index::getItem(
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
    return std::nullopt;
}
