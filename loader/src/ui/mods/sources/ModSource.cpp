#include "ModSource.hpp"
#include <Geode/ui/GeodeUI.hpp>
#include <server/DownloadManager.hpp>

ModSource::ModSource(Mod* mod) : m_value(mod) {}
ModSource::ModSource(server::ServerModMetadata&& metadata) : m_value(metadata) {}

std::string ModSource::getID() const {
    return std::visit(makeVisitor {
        [](Mod* mod) {
            return mod->getID();
        },
        [](server::ServerModMetadata const& metadata) {
            // Versions should be guaranteed to have at least one item
            return metadata.id;
        }
    }, m_value);
}
ModMetadata ModSource::getMetadata() const {
    return std::visit(makeVisitor {
        [](Mod* mod) {
            return mod->getMetadata();
        },
        [](server::ServerModMetadata const& metadata) {
            // Versions should be guaranteed to have at least one item
            return metadata.versions.front().metadata;
        }
    }, m_value);
}
CCNode* ModSource::createModLogo() const {
    return std::visit(makeVisitor {
        [](Mod* mod) {
            return geode::createModLogo(mod);
        },
        [](server::ServerModMetadata const& metadata) {
            return createServerModLogo(metadata.id);
        }
    }, m_value);
}
bool ModSource::wantsRestart() const {
    // If some download has been done for this mod, always want a restart
    auto download = server::ModDownloadManager::get()->getDownload(this->getID());
    if (download && download->isDone()) {
        return true;
    }
    return std::visit(makeVisitor {
        [](Mod* mod) {
            return mod->getRequestedAction() != ModRequestedAction::None;
        },
        [](server::ServerModMetadata const& metdata) {
            return false;
        }
    }, m_value);
}
std::optional<server::ServerModUpdate> ModSource::hasUpdates() const {
    return m_availableUpdate;
}

ModSource ModSource::tryConvertToMod() const {
    return std::visit(makeVisitor {
        [](Mod* mod) {
            return ModSource(mod);
        },
        [](server::ServerModMetadata const& metadata) {
            if (auto mod = Loader::get()->getInstalledMod(metadata.id)) {
                return ModSource(mod);
            }
            return ModSource(server::ServerModMetadata(metadata));
        }
    }, m_value);
}

Mod* ModSource::asMod() const {
    auto mod = std::get_if<Mod*>(&m_value);
    return mod ? *mod : nullptr;
}
server::ServerModMetadata const* ModSource::asServer() const {
    return std::get_if<server::ServerModMetadata>(&m_value);
}

server::ServerRequest<std::optional<std::string>> ModSource::fetchAbout() const {
    return std::visit(makeVisitor {
        [](Mod* mod) {
            return server::ServerRequest<std::optional<std::string>>::immediate(Ok(mod->getMetadata().getDetails()));
        },
        [](server::ServerModMetadata const& metadata) {
            return server::getMod(metadata.id).map(
                [](auto* result) -> Result<std::optional<std::string>, server::ServerError> {
                    if (result->isOk()) {
                        return Ok(result->unwrap().about);
                    }
                    return Err(result->unwrapErr());
                }
            );
        }
    }, m_value);
}
server::ServerRequest<std::optional<std::string>> ModSource::fetchChangelog() const {
    return std::visit(makeVisitor {
        [](Mod* mod) {
            return server::ServerRequest<std::optional<std::string>>::immediate(Ok(mod->getMetadata().getChangelog()));
        },
        [](server::ServerModMetadata const& metadata) {
            return server::getMod(metadata.id).map(
                [](auto* result) -> Result<std::optional<std::string>, server::ServerError> {
                    if (result->isOk()) {
                        return Ok(result->unwrap().changelog);
                    }
                    return Err(result->unwrapErr());
                }
            );
        }
    }, m_value);
}
server::ServerRequest<server::ServerModMetadata> ModSource::fetchServerInfo() const {
    // Request the info even if this is already a server mod because this might 
    // not have the full details (for example changelog) and the server cache 
    // should deal with performance issues
    return server::getMod(this->getID());
}
server::ServerRequest<std::unordered_set<std::string>> ModSource::fetchValidTags() const {
    return std::visit(makeVisitor {
        [](Mod* mod) {
            return server::getTags().map(
                [mod](auto* result) -> Result<std::unordered_set<std::string>, server::ServerError> {
                    if (result->isOk()) {
                        // Filter out invalid tags
                        auto modTags = mod->getMetadata().getTags();
                        auto finalTags = std::unordered_set<std::string>();
                        for (auto& tag : modTags) {
                            if (result->unwrap().contains(tag)) {
                                finalTags.insert(tag);
                            }
                        }
                        return Ok(finalTags);
                    }
                    return *result;
                },
                [](server::ServerProgress* progress) {
                    return *progress;
                }
            );
        },
        [](server::ServerModMetadata const& metadata) {
            // Server info tags are always certain to be valid since the server has already validated them
            return server::ServerRequest<std::unordered_set<std::string>>::immediate(Ok(metadata.tags));
        }
    }, m_value);
}
server::ServerRequest<std::optional<server::ServerModUpdate>> ModSource::checkUpdates() {
    m_availableUpdate = std::nullopt;
    return std::visit(makeVisitor {
        [this](Mod* mod) {
            return server::checkUpdates(mod).map(
                [this](auto* result) -> Result<std::optional<server::ServerModUpdate>, server::ServerError> {
                    if (result->isOk()) {
                        m_availableUpdate = result->unwrap();
                        return Ok(m_availableUpdate);
                    }
                    return Err(result->unwrapErr());
                }
            );
        },
        [](server::ServerModMetadata const& metadata) {
            // Server mods aren't installed so you can't install updates for them
            return server::ServerRequest<std::optional<server::ServerModUpdate>>::immediate(Ok(std::nullopt));
        }
    }, m_value);
}
