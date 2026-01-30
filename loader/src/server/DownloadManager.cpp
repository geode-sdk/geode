#include "DownloadManager.hpp"
#include "Geode/loader/Mod.hpp"
#include <Geode/loader/Dirs.hpp>
#include <Geode/utils/map.hpp>
#include <Geode/utils/StringMap.hpp>
#include <fmt/format.h>
#include <optional>
#include <hash/hash.hpp>
#include <loader/LoaderImpl.hpp>
#include <loader/ModImpl.hpp>

using namespace server;

class ModDownload::Impl final {
public:
    std::string m_id;
    std::optional<VersionInfo> m_version;
    std::optional<DependencyFor> m_dependencyFor;
    std::optional<std::string> m_replacesMod;
    DownloadStatus m_status;
    async::TaskHolder<web::WebResponse> m_downloadListener;
    async::TaskHolder<ServerResult<ServerModVersion>> m_infoListener;
    unsigned int m_scheduledEventForFrame = 0;

    Impl(
        std::string id,
        std::optional<VersionInfo> version,
        std::optional<DependencyFor> dependencyFor,
        std::optional<std::string> replacesMod
    )
      : m_id(std::move(id)),
        m_version(std::move(version)),
        m_dependencyFor(std::move(dependencyFor)),
        m_replacesMod(std::move(replacesMod)),
        m_status(DownloadStatusFetching {
            .percentage = 0,
        })
    {
        auto fetchVersion = version.has_value() ? ModVersion(*version) : ModVersion(ModVersionLatest());
        m_infoListener.spawn(
            getModVersion(m_id, std::move(fetchVersion)),
            [this](ServerResult<ServerModVersion> result) {
                if (result.isOk()) {
                    auto& data = result.unwrap();
                    m_version = data.metadata.getVersion();

                    // Start downloads for any missing required dependencies
                    for (auto dep : data.metadata.getDependencies()) {
                        if (!dep.getMod() && dep.getImportance() != ModMetadata::Dependency::Importance::Suggested) {
                            ModDownloadManager::get()->startDownload(
                                dep.getID(), dep.getVersion().getUnderlyingVersion(),
                                std::make_pair(m_id, dep.getImportance())
                            );
                        }
                    }

                    m_status = DownloadStatusConfirm {
                        .version = std::move(result).unwrap(),
                    };
                }
                else {
                    m_status = DownloadStatusError {
                        .details = std::move(result.unwrapErr().details),
                    };
                }


                // TODO: v5 web progress
                //     else if (auto progress = event->getProgress()) {
                //         m_status = DownloadStatusFetching {
                //             .percentage = progress->percentage.value_or(0),
                //         };
                //     }
                //     else if (event->isCancelled()) {
                //         m_status = DownloadStatusCancelled();
                //         m_infoListener.setFilter(ServerFuture<ServerModVersion>());
                //     }
                //
                //     if (!ModDownloadManager::get()->checkAutoConfirm()) {
                //         // Throttle events to only once per frame to not cause a
                //         // billion UI updates at once
                //         if (m_scheduledEventForFrame != CCDirector::get()->getTotalFrames()) {
                //             m_scheduledEventForFrame = CCDirector::get()->getTotalFrames();
                //             Loader::get()->queueInMainThread([id = m_id]() {
                //                 GlobalModDownloadEvent().send(std::string_view(id));
                //                 ModDownloadEvent(std::string(id)).send();
                //             });
                //         }
                //     }
            }
        );

        Loader::get()->queueInMainThread([id = m_id] {
            GlobalModDownloadEvent().send(std::string_view(id));
            ModDownloadEvent(std::string(id)).send();
        });
    }

    void onFinished(web::WebResponse response, ServerModVersion version) {
        if (!response.ok()) {
            if (response.code() == -1) {
                m_status = DownloadStatusError {
                    .details = fmt::format(
                        "Failed to make request to download endpoint. Error: {}",
                        response.string().unwrapOr("No message")
                    )
                };
            } else {
                m_status = DownloadStatusError {
                    .details = fmt::format(
                        "Server returned error {} with message: {}",
                        response.code(),
                        response.string().unwrapOr("No message")
                    )
                };
            }

            log::error("Failed to download {}, server returned error {}", m_id, response.code());
            log::error("{}", response.string().unwrapOr("No response"));

            const auto& extErr = response.errorMessage();
            if (!extErr.empty()) {
                log::error("Extended error info: {}", extErr);
            }
            return;
        }

        auto actualHash = ::calculateHash(response.data());
        if (actualHash != version.hash) {
            log::error("Failed to download {}, hash mismatch ({} != {})", m_id, actualHash, version.hash);
            m_status = DownloadStatusError {
                .details = "Hash mismatch, downloaded file did not match what was expected",
            };
            return;
        }

        std::string id = m_replacesMod.has_value() ? m_replacesMod.value() : m_id;
        if (auto mod = Loader::get()->getInstalledMod(id)) {
            std::error_code ec;
            std::filesystem::remove(mod->getPackagePath(), ec);
            if (ec) {
                m_status = DownloadStatusError {
                    .details = fmt::format("Unable to delete existing .geode package (code {})", ec),
                };
                return;
            }
            // Mark mod as updated
            ModImpl::getImpl(mod)->m_requestedAction = ModRequestedAction::Update;
        }

        // If this was an update, delete the old file first
        auto geodePath = dirs::getModsDir() / (m_id + ".geode");
        auto data = std::move(response).data();
        auto ok = file::writeBinary(geodePath, data);
        if (!ok) {
            m_status = DownloadStatusError {
                .details = std::move(ok).unwrapErr(),
            };
            return;
        }

        auto metadata = ModMetadata::createFromGeodeFile(geodePath);
        if (metadata.isErr()) {
            m_status = DownloadStatusError {
                .details = std::move(metadata).unwrapErr(),
            };
            return;
        }

        auto okBinary = LoaderImpl::get()->extractBinary(metadata.unwrap());
        if (!okBinary) {
            m_status = DownloadStatusError {
                .details = std::move(okBinary).unwrapErr(),
            };
            return;
        }

        m_status = DownloadStatusDone {
            .version = std::move(version)
        };
    }

    void confirm() {
        auto confirm = std::get_if<DownloadStatusConfirm>(&m_status);
        if (!confirm) return;

        auto version = confirm->version;
        auto downloadURL = version.downloadURL;

        m_status = DownloadStatusDownloading {
            .percentage = 0,
        };
        
        // TODO: v5 web progress
        //     else if (auto progress = event->getProgress()) {
        //         m_status = DownloadStatusDownloading {
        //             .percentage = static_cast<uint8_t>(progress->downloadProgress().value_or(0)),
        //         };
        //     }
        //     else if (event->isCancelled()) {
        //         m_status = DownloadStatusCancelled();
        //     }
        //     // Throttle events to only once per frame to not cause a
        //     // billion UI updates at once

        m_downloadListener.spawn(
            web::WebRequest().userAgent(getServerUserAgent()).get(std::move(downloadURL)),
            [this, version = std::move(version)](web::WebResponse response) {
                this->onFinished(std::move(response), std::move(version));

                // post event
                if (m_scheduledEventForFrame != CCDirector::get()->getTotalFrames()) {
                    m_scheduledEventForFrame = CCDirector::get()->getTotalFrames();
                    Loader::get()->queueInMainThread([id = m_id]() {
                        GlobalModDownloadEvent().send(std::string_view(id));
                        ModDownloadEvent(std::string(id)).send();
                    });
                }
            }
        );

        GlobalModDownloadEvent().send(std::string_view(m_id));
        ModDownloadEvent(std::string(m_id)).send();
    }
};

ModDownload::ModDownload(
    std::string id,
    std::optional<VersionInfo> version,
    std::optional<DependencyFor> dependencyFor,
    std::optional<std::string> replacesMod
) : m_impl(std::make_shared<Impl>(std::move(id), std::move(version), std::move(dependencyFor), std::move(replacesMod))) {}

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
    StringMap<ModDownload> m_downloads;
    async::TaskHolder<Result<std::vector<ServerModUpdate>, server::ServerError>> m_updateAllTask;

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
        m_impl->m_infoListener = {};
        m_impl->m_downloadListener = {};

        // Cancel any dependencies of this mod left over (unless some other
        // installation depends on them still)
        ModDownloadManager::get()->m_impl->cancelOrphanedDependencies();
        GlobalModDownloadEvent().send(std::string_view(m_impl->m_id));
        ModDownloadEvent(std::string(m_impl->m_id)).send();
    }
}

std::optional<ModDownload> ModDownloadManager::startDownload(
    std::string id,
    std::optional<VersionInfo> version,
    std::optional<DependencyFor> dependencyFor,
    std::optional<std::string> replacesMod
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
    auto [it, _] = m_impl->m_downloads.emplace(id, ModDownload(
        std::move(id),
        std::move(version),
        std::move(dependencyFor),
        std::move(replacesMod)
    ));
    return it->second;
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
    m_impl->m_updateAllTask.spawn(checkAllUpdates(), [this](auto result) {
        if (result.isOk()) {
            for (auto& mod : result.unwrap()) {
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
    });
}
void ModDownloadManager::dismissAll() {
    std::erase_if(m_impl->m_downloads, [](auto const& d) {
        return d.second.canRetry();
    });
    GlobalModDownloadEvent().send("");
    ModDownloadEvent("").send();
}
bool ModDownloadManager::checkAutoConfirm() {
    for (auto& [_, download] :  m_impl->m_downloads) {
        auto status = download.getStatus();
        if (auto confirm = std::get_if<server::DownloadStatusConfirm>(&status)) {
            for (auto& inc : confirm->version.metadata.getIncompatibilities()) {
                // If some mod has an incompatability that is installed,
                // we need to ask for confirmation
                if (inc.getMod() && (!download.getVersion().has_value() || inc.getVersion().compare(download.getVersion().value()))) {
                    return false;
                }
                for (auto& download : ModDownloadManager::get()->getDownloads()) {
                    if (download.isDone() && inc.getID() == download.getID() && (!download.getVersion().has_value() || inc.getVersion().compare(download.getVersion().value()))) {
                        return false;
                    }
                }
            }
            // If some installed mod is incompatible with this one,
            // we need to ask for confirmation
            for (auto mod : Loader::get()->getAllMods()) {
                for (auto& inc : mod->getMetadata().getIncompatibilities()) {
                    if (inc.getID() == download.getID() && (!download.getVersion().has_value() || inc.getVersion().compare(download.getVersion().value()))) {
                        return false;
                    }
                }
            }

            // If some newly downloaded mods are incompatible with this one,
            // we need to ask for confirmation
            for (auto& download : ModDownloadManager::get()->getDownloads()) {
                auto status = download.getStatus();
                if (auto done = std::get_if<DownloadStatusDone>(&status)) {
                    for (auto& inc : done->version.metadata.getIncompatibilities()) {
                        if (inc.getID() == download.getID() && inc.getVersion().compare(done->version.metadata.getVersion())) {
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
std::optional<ModDownload> ModDownloadManager::getDownload(std::string_view id) const {
    auto it = m_impl->m_downloads.find(id);
    return it != m_impl->m_downloads.end() ? std::optional<ModDownload>(it->second) : std::nullopt;
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
