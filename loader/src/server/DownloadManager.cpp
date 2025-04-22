#include "DownloadManager.hpp"
#include "Geode/loader/Mod.hpp"
#include <Geode/loader/Dirs.hpp>
#include <Geode/utils/map.hpp>
#include <optional>
#include <hash/hash.hpp>
#include <loader/ModImpl.hpp>

using namespace server;

ModDownloadEvent::ModDownloadEvent(std::string const& id) : id(id) {}

ListenerResult ModDownloadFilter::handle(std::function<Callback> fn, ModDownloadEvent* event) {
    if (m_id.empty() || m_id == event->id) {
        fn(event);
    }
    return ListenerResult::Propagate;
}
ModDownloadFilter::ModDownloadFilter() {}
ModDownloadFilter::ModDownloadFilter(std::string const& id) : m_id(id) {}

class ModDownload::Impl final {
public:
    std::string m_id;
    std::optional<VersionInfo> m_version;
    std::optional<DependencyFor> m_dependencyFor;
    std::optional<std::string> m_replacesMod;
    DownloadStatus m_status;
    EventListener<ServerRequest<ServerModVersion>> m_infoListener;
    EventListener<web::WebTask> m_downloadListener;
    unsigned int m_scheduledEventForFrame = 0;

    Impl(
        std::string const& id,
        std::optional<VersionInfo> const& version,
        std::optional<DependencyFor> const& dependencyFor,
        std::optional<std::string> const& replacesMod
    )
      : m_id(id),
        m_version(version),
        m_dependencyFor(dependencyFor),
        m_replacesMod(replacesMod),
        m_status(DownloadStatusFetching {
            .percentage = 0,
        })
    {
        m_infoListener.bind([this](ServerRequest<ServerModVersion>::Event* event) {
            if (auto result = event->getValue()) {
                if (result->isOk()) {
                    auto data = result->unwrap();
                    m_version = data.metadata.getVersion();

                    // Start downloads for any missing required dependencies
                    for (auto dep : data.metadata.getDependencies()) {
                        if (!dep.mod && dep.importance != ModMetadata::Dependency::Importance::Suggested) {
                            ModDownloadManager::get()->startDownload(
                                dep.id, dep.version.getUnderlyingVersion(),
                                std::make_pair(m_id, dep.importance)
                            );
                        }
                    }

                    m_status = DownloadStatusConfirm {
                        .version = data,
                    };
                }
                else {
                    m_status = DownloadStatusError {
                        .details = result->unwrapErr().details,
                    };
                }

                // Clear the listener to free up the memory
                m_infoListener.setFilter(ServerRequest<ServerModVersion>());
            }
            else if (auto progress = event->getProgress()) {
                m_status = DownloadStatusFetching {
                    .percentage = progress->percentage.value_or(0),
                };
            }
            else if (event->isCancelled()) {
                m_status = DownloadStatusCancelled();
                m_infoListener.setFilter(ServerRequest<ServerModVersion>());
            }

            if (!ModDownloadManager::get()->checkAutoConfirm()) {
                // Throttle events to only once per frame to not cause a 
                // billion UI updates at once 
                if (m_scheduledEventForFrame != CCDirector::get()->getTotalFrames()) {
                    m_scheduledEventForFrame = CCDirector::get()->getTotalFrames();
                    Loader::get()->queueInMainThread([id = m_id]() {
                        ModDownloadEvent(id).post();
                    });
                }
            }
        });
        auto fetchVersion = version.has_value() ? ModVersion(*version) : ModVersion(ModVersionLatest());
        m_infoListener.setFilter(getModVersion(id, fetchVersion));
        Loader::get()->queueInMainThread([id = m_id] {
            ModDownloadEvent(id).post();
        });
    }

    void confirm() {
        auto confirm = std::get_if<DownloadStatusConfirm>(&m_status);
        if (!confirm) return;

        auto version = confirm->version;
        m_status = DownloadStatusDownloading {
            .percentage = 0,
        };

        m_downloadListener.bind([this, hash = version.hash, version = version](web::WebTask::Event* event) {
            if (auto value = event->getValue()) {
                if (value->ok()) {
                    if (auto actualHash = ::calculateHash(value->data()); actualHash != hash) {
                        log::error("Failed to download {}, hash mismatch ({} != {})", m_id, actualHash, hash);
                        m_status = DownloadStatusError {
                            .details = "Hash mismatch, downloaded file did not match what was expected",
                        };
                        ModDownloadEvent(m_id).post();
                        return;
                    }

                    bool removingInstalledWasError = false;
                    std::string id = m_replacesMod.has_value() ? m_replacesMod.value() : m_id;
                    if (auto mod = Loader::get()->getInstalledMod(id)) {
                        std::error_code ec;
                        std::filesystem::remove(mod->getPackagePath(), ec);
                        if (ec) {
                            removingInstalledWasError = true;
                            m_status = DownloadStatusError {
                                .details = fmt::format("Unable to delete existing .geode package (code {})", ec),
                            };
                        }
                        // Mark mod as updated
                        ModImpl::getImpl(mod)->m_requestedAction = ModRequestedAction::Update;
                    }
                    // If this was an update, delete the old file first
                    if (!removingInstalledWasError) {
                        auto ok = file::writeBinary(dirs::getModsDir() / (m_id + ".geode"), value->data());
                        if (!ok) {
                            m_status = DownloadStatusError {
                                .details = ok.unwrapErr(),
                            };
                        }
                        else {
                            m_status = DownloadStatusDone {
                                .version = version
                            };
                        }
                    }
                }
                else {
                    auto resp = event->getValue();

                    m_status = DownloadStatusError {
                        .details = fmt::format("Server returned error {}", resp->code()),
                    };
                    log::error("Failed to download {}, server returned error {}", m_id, resp->code());
                    log::error("{}", resp->string().unwrapOr("No response"));

                    const auto& extErr = resp->errorMessage();
                    if (!extErr.empty()) {
                        log::error("Extended error info: {}", extErr);
                    }
                }
            }
            else if (auto progress = event->getProgress()) {
                m_status = DownloadStatusDownloading {
                    .percentage = static_cast<uint8_t>(progress->downloadProgress().value_or(0)),
                };
            }
            else if (event->isCancelled()) {
                m_status = DownloadStatusCancelled();
            }
            // Throttle events to only once per frame to not cause a 
            // billion UI updates at once 
            if (m_scheduledEventForFrame != CCDirector::get()->getTotalFrames()) {
                m_scheduledEventForFrame = CCDirector::get()->getTotalFrames();
                Loader::get()->queueInMainThread([id = m_id]() {
                    ModDownloadEvent(id).post();
                });
            }
        });

        auto req = web::WebRequest();
        req.userAgent(getServerUserAgent());
        m_downloadListener.setFilter(req.get(version.downloadURL));
        ModDownloadEvent(m_id).post();
    }
};

ModDownload::ModDownload(
    std::string const& id,
    std::optional<VersionInfo> const& version,
    std::optional<DependencyFor> const& dependencyFor,
    std::optional<std::string> const& replacesMod
) : m_impl(std::make_shared<Impl>(id, version, dependencyFor, replacesMod)) {}

void ModDownload::confirm() {
    m_impl->confirm();
}

std::optional<DependencyFor> ModDownload::getDependencyFor() const {
    return m_impl->m_dependencyFor;
}
std::optional<std::string> ModDownload::getReplacesMod() const {
    return m_impl->m_replacesMod;
}
bool ModDownload::isDone() const {
    return std::holds_alternative<DownloadStatusDone>(m_impl->m_status);
}
bool ModDownload::isActive() const {
    return !(
        std::holds_alternative<DownloadStatusDone>(m_impl->m_status) ||
        std::holds_alternative<DownloadStatusError>(m_impl->m_status) ||
        std::holds_alternative<DownloadStatusCancelled>(m_impl->m_status)
    );
}
bool ModDownload::canRetry() const {
    return
        std::holds_alternative<DownloadStatusError>(m_impl->m_status) ||
        std::holds_alternative<DownloadStatusCancelled>(m_impl->m_status);
}
std::string ModDownload::getID() const {
    return m_impl->m_id;
}
DownloadStatus ModDownload::getStatus() const {
    return m_impl->m_status;
}
std::optional<VersionInfo> ModDownload::getVersion() const {
    return m_impl->m_version;
}

class ModDownloadManager::Impl {
public:
    std::unordered_map<std::string, ModDownload> m_downloads;
    Task<std::monostate> m_updateAllTask;

    void cancelOrphanedDependencies() {
        // "This doesn't handle circular dependencies!!!!"
        // Well OK and the human skull doesn't handle the 5000 newtons
        // of force from this anvil I'm about to drop on your head

        for (auto& [_, d] : m_downloads) {
            if (auto depFor = d.m_impl->m_dependencyFor) {
                if (
                    !m_downloads.contains(depFor->first) ||
                    std::holds_alternative<DownloadStatusError>(m_downloads.at(depFor->first).getStatus())
                ) {
                    // d.cancel() will cause cancelOrphanedDependencies() to be called again
                    // We want that anyway because cancelling one dependency might cause
                    // dependencies down the chain to become orphaned
                    return d.cancel();
                }
            }
        }
    }
};

void ModDownload::cancel() {
    if (!std::holds_alternative<DownloadStatusDone>(m_impl->m_status)) {
        m_impl->m_status = DownloadStatusCancelled();
        m_impl->m_infoListener.getFilter().cancel();
        m_impl->m_infoListener.setFilter(ServerRequest<ServerModVersion>());
        m_impl->m_downloadListener.getFilter().cancel();
        m_impl->m_downloadListener.setFilter({});

        // Cancel any dependencies of this mod left over (unless some other
        // installation depends on them still)
        ModDownloadManager::get()->m_impl->cancelOrphanedDependencies();
        ModDownloadEvent(m_impl->m_id).post();
    }
}

std::optional<ModDownload> ModDownloadManager::startDownload(
    std::string const& id,
    std::optional<VersionInfo> const& version,
    std::optional<DependencyFor> const& dependencyFor,
    std::optional<std::string> const& replacesMod
) {
    // If this mod has already been successfully downloaded or is currently
    // being downloaded, return as you can't download multiple versions of the
    // same mod simultaniously, since that wouldn't make sense. I mean the new
    // version would just immediately override to the other one
    if (m_impl->m_downloads.contains(id)) {
        // If the download errored last time, then we can try again
        if (m_impl->m_downloads.at(id).canRetry()) {
            m_impl->m_downloads.erase(id);
        }
        // Otherwise return
        else return std::nullopt;
    }

    // Start a new download by constructing a ModDownload (which starts the
    // download)
    m_impl->m_downloads.emplace(id, ModDownload(id, version, dependencyFor, replacesMod));
    return m_impl->m_downloads.at(id);
}
void ModDownloadManager::cancelAll() {
    for (auto& [_, d] : m_impl->m_downloads) {
        d.cancel();
    }
}
void ModDownloadManager::confirmAll() {
    for (auto& [_, d] : m_impl->m_downloads) {
        d.confirm();
    }
}
void ModDownloadManager::startUpdateAll() {
    m_impl->m_updateAllTask = checkAllUpdates().map(
        [this](Result<std::vector<ServerModUpdate>, ServerError>* result) {
            if (result->isOk()) {
                for (auto& mod : result->unwrap()) {
                    if (mod.hasUpdateForInstalledMod()) {
                        if (mod.replacement.has_value()) {
                            this->startDownload(
                                mod.replacement.value().id,
                                mod.replacement.value().version,
                                std::nullopt,
                                mod.id
                            );
                        } else {
                            this->startDownload(mod.id, mod.version);
                        }
                    }
                }
            }
            return std::monostate();
        },
        [](auto) { return std::monostate(); }
    );
}
void ModDownloadManager::dismissAll() {
    std::erase_if(m_impl->m_downloads, [](auto const& d) {
        return d.second.canRetry();
    });
    ModDownloadEvent("").post();
}
bool ModDownloadManager::checkAutoConfirm() {
    for (auto& [_, download] :  m_impl->m_downloads) {
        auto status = download.getStatus();
        if (auto confirm = std::get_if<server::DownloadStatusConfirm>(&status)) {
            for (auto inc : confirm->version.metadata.getIncompatibilities()) {
                // If some mod has an incompatability that is installed,
                // we need to ask for confirmation
                if (inc.mod && (!download.getVersion().has_value() || inc.version.compare(download.getVersion().value()))) {
                    return false;
                }
                for (auto download : ModDownloadManager::get()->getDownloads()) {
                    if (download.isDone() && inc.id == download.getID() && (!download.getVersion().has_value() || inc.version.compare(download.getVersion().value()))) {
                        return false;
                    }
                }
            }
            // If some installed mod is incompatible with this one,
            // we need to ask for confirmation
            for (auto mod : Loader::get()->getAllMods()) {
                for (auto inc : mod->getMetadata().getIncompatibilities()) {
                    if (inc.id == download.getID() && (!download.getVersion().has_value() || inc.version.compare(download.getVersion().value()))) {
                        return false;
                    }
                }
            }

            // If some newly downloaded mods are incompatible with this one, 
            // we need to ask for confirmation
            for (auto download : ModDownloadManager::get()->getDownloads()) {
                auto status = download.getStatus();
                if (auto done = std::get_if<DownloadStatusDone>(&status)) {
                    for (auto inc : done->version.metadata.getIncompatibilities()) {
                        if (inc.id == download.getID() && inc.version.compare(done->version.metadata.getVersion())) {
                            return false;
                        }
                    }
                }
            }
        }
        // If there are mods we aren't sure about yet, we can't auto-confirm
        else if (std::holds_alternative<DownloadStatusFetching>(status)) {
            return false;
        }
    }

    // If we have reached this point, we can auto-confirm
    this->confirmAll();
    return true;
}

std::vector<ModDownload> ModDownloadManager::getDownloads() const {
    return map::values(m_impl->m_downloads);
}
std::optional<ModDownload> ModDownloadManager::getDownload(std::string const& id) const {
    if (m_impl->m_downloads.contains(id)) {
        return m_impl->m_downloads.at(id);
    }
    return std::nullopt;
}
bool ModDownloadManager::hasActiveDownloads() const {
    for (auto& [_, download] : m_impl->m_downloads) {
        if (download.isActive()) {
            return true;
        }
    }
    return false;
}

bool ModDownloadManager::wantsRestart() const {
    for (auto& [key, v] : m_impl->m_downloads) {
        if (v.isDone()) {
            return true;
        }
    }
    return false;
}

ModDownloadManager* ModDownloadManager::get() {
    static auto inst = new ModDownloadManager();
    return inst;
}
ModDownloadManager::ModDownloadManager() : m_impl(std::make_unique<Impl>()) {}
ModDownloadManager::~ModDownloadManager() = default;
