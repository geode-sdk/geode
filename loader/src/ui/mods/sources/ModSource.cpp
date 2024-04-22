#include "ModSource.hpp"
#include <Geode/ui/GeodeUI.hpp>

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
std::optional<std::string> ModSource::getAbout() const {
    return std::visit(makeVisitor {
        [](Mod* mod) {
            return mod->getMetadata().getDetails();
        },
        [](server::ServerModMetadata const& metadata) {
            // Versions should be guaranteed to have at least one item
            return metadata.about;
        }
    }, m_value);
}
std::optional<std::string> ModSource::getChangelog() const {
    return std::visit(makeVisitor {
        [](Mod* mod) {
            return mod->getMetadata().getChangelog();
        },
        [](server::ServerModMetadata const& metadata) {
            // Versions should be guaranteed to have at least one item
            return metadata.changelog;
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
    return std::visit(makeVisitor {
        [](Mod* mod) {
            return mod->getRequestedAction() != ModRequestedAction::None;
        },
        [](server::ServerModMetadata const& metdata) {
            // todo: check if the mod has been installed
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

server::ServerRequest<server::ServerModMetadata> ModSource::fetchServerInfo() const {
    return std::visit(makeVisitor {
        [](Mod* mod) {
            return server::getMod(mod->getID());
        },
        [](server::ServerModMetadata const& metadata) {
            return server::ServerRequest<server::ServerModMetadata>::immediate(Ok(metadata));
        }
    }, m_value);
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
    return server::checkUpdates({ this->getID() }).map(
        [this](auto* result) -> Result<std::optional<server::ServerModUpdate>, server::ServerError> {
            if (result->isOk()) {
                auto updates = result->unwrap();
                if (!updates.empty()) {
                    auto update = std::move(std::move(updates).at(0));
                    if (update.version > this->getMetadata().getVersion()) {
                        m_availableUpdate = update;
                        return Ok(m_availableUpdate);
                    }
                }
                return Ok(std::nullopt);
            }
            return Err(result->unwrapErr());
        }
    );
}
