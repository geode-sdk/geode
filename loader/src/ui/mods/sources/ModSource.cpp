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

server::ServerPromise<server::ServerModMetadata> ModSource::fetchServerInfo() const {
    return std::visit(makeVisitor {
        [](Mod* mod) {
            return server::ServerResultCache<&server::getMod>::shared().get(mod->getID());
        },
        [](server::ServerModMetadata const& metadata) {
            return server::ServerPromise<server::ServerModMetadata>([&metadata](auto resolve, auto) {
                resolve(metadata);
            });
        }
    }, m_value);
}
server::ServerPromise<std::unordered_set<std::string>> ModSource::fetchValidTags() const {
    return std::visit(makeVisitor {
        [](Mod* mod) {
            return server::ServerResultCache<&server::getTags>::shared().get()
                .then<std::unordered_set<std::string>>([mod](std::unordered_set<std::string> validTags) {
                    // Filter out invalid tags
                    auto modTags = mod->getMetadata().getTags();
                    auto finalTags = std::unordered_set<std::string>();
                    for (auto& tag : modTags) {
                        if (validTags.contains(tag)) {
                            finalTags.insert(tag);
                        }
                    }
                    return finalTags;
                });
        },
        [](server::ServerModMetadata const& metadata) {
            // Server info tags are always certain to be valid since the server has already validated them
            return server::ServerPromise<std::unordered_set<std::string>>([&metadata](auto resolve, auto) {
                resolve(metadata.tags);
            });
        }
    }, m_value);
}
server::ServerPromise<std::optional<server::ServerModUpdate>> ModSource::checkUpdates() {
    m_availableUpdate = std::nullopt;
    return server::ServerResultCache<&server::checkUpdates>::shared()
        .get({ this->getID() })
        .then<std::optional<server::ServerModUpdate>>([this](auto updates) -> std::optional<server::ServerModUpdate> {
            if (!updates.empty()) {
                auto update = std::move(std::move(updates).at(0));
                if (update.version > this->getMetadata().getVersion()) {
                    m_availableUpdate = update;
                    return m_availableUpdate;
                }
            }
            return std::nullopt;
        });
}
