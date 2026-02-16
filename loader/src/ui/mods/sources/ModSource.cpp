#include "ModSource.hpp"

#include <Geode/loader/ModMetadata.hpp>
#include <Geode/loader/ModSettingsManager.hpp>
#include <Geode/ui/GeodeUI.hpp>
#include <server/DownloadManager.hpp>
#include <Geode/binding/GameObject.hpp>
#include <unordered_set>

ModSource::ModSource(Mod* mod) : m_value(mod) {}
ModSource::ModSource(server::ServerModMetadata&& metadata) : m_value(metadata) {}

std::string ModSource::getID() const {
    return std::visit(makeVisitor {
        [](Mod* mod) {
            return std::string{mod->getID()};
        },
        [](server::ServerModMetadata const& metadata) {
            return metadata.id;
        },
    }, m_value);
}
ModMetadata const& ModSource::getMetadata() const {
    return std::visit(makeVisitor {
        // the return type annotation is super important here or else for some unknown to me reason
        // those lambdas decide to return a value and thus the function will return a ref to a temporary
        [](Mod* mod) -> ModMetadata const& {
            return mod->getMetadata();
        },
        [](server::ServerModMetadata const& metadata) -> ModMetadata const& {
            // Versions should be guaranteed to have at least one item
            return metadata.versions.front().metadata;
        },
    }, m_value);
}

std::string ModSource::formatDevelopers() const {
    return std::visit(makeVisitor {
        [](Mod* mod) {
            return ModMetadata::formatDeveloperDisplayString(mod->getMetadata().getDevelopers());
        },
        [](server::ServerModMetadata const& metadata) {
            // Versions should be guaranteed to have at least one item
            return metadata.formatDevelopersToString();
        },
    }, m_value);
}

CCNode* ModSource::createModLogo() const {
    return std::visit(makeVisitor {
        [](Mod* mod) {
            return geode::createModLogo(mod);
        },
        [](server::ServerModMetadata const& metadata) {
            return createServerModLogo(metadata.id);
        },
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
            return mod->getRequestedAction() != ModRequestedAction::None ||
                ModSettingsManager::from(mod)->restartRequired();
        },
        [](server::ServerModMetadata const& metadata) {
            return false;
        },
    }, m_value);
}
server::ServerModUpdateOneCheck ModSource::hasUpdates() const {
    return m_availableUpdate;
}

ModSource ModSource::convertForPopup() const {
    return std::visit(makeVisitor {
        [](Mod* mod) {
            return ModSource(mod);
        },
        [](server::ServerModMetadata const& metadata) {
            if (auto mod = Loader::get()->getInstalledMod(metadata.id)) {
                return ModSource(mod);
            }
            return ModSource(server::ServerModMetadata(metadata));
        },
    }, m_value);
}

Mod* ModSource::asMod() const {
    auto mod = std::get_if<Mod*>(&m_value);
    return mod ? *mod : nullptr;
}
server::ServerModMetadata const* ModSource::asServer() const {
    return std::get_if<server::ServerModMetadata>(&m_value);
}

server::ServerFuture<std::optional<std::string>> ModSource::fetchAbout() const {
    // todo: write as visit
    if (!this->hasUpdates().update) {
        if (auto mod = this->asMod()) {
            co_return Ok(mod->getMetadata().getDetails());
        }
    }
    auto result = co_await server::getMod(this->getID());
    if (result.isOk()) {
        co_return Ok(result.unwrap().about);
    }
    co_return Err(result.unwrapErr());
}
server::ServerFuture<std::optional<std::string>> ModSource::fetchChangelog() const {
    if (!this->hasUpdates().update) {
        if (auto mod = this->asMod()) {
            co_return Ok(mod->getMetadata().getChangelog());
        }
    }

    auto result = co_await server::getMod(this->getID());
    if (result.isOk()) {
        co_return Ok(result.unwrap().changelog);
    }
    co_return Err(result.unwrapErr());
}
server::ServerFuture<server::ServerModMetadata> ModSource::fetchServerInfo() const {
    // Request the info even if this is already a server mod because this might
    // not have the full details (for example changelog) and the server cache
    // should deal with performance issues
    return server::getMod(this->getID());
}

server::ServerFuture<std::vector<server::ServerTag>> ModSource::fetchValidTags() const {
    std::unordered_set<std::string> modTags;
    std::visit(makeVisitor {
        [&](Mod* mod) {
            modTags = mod->getMetadata().getTags();
        },
        [&](server::ServerModMetadata const& metadata) {
            modTags = metadata.tags;
        },
    }, m_value);

    // This does two things:
    // 1. For installed mods, it filters out invalid tags
    // 2. For everything else, it gets the rest of the tag info (display name) from the server
    auto fetched = ARC_CO_UNWRAP(co_await server::getTags());

    std::vector<server::ServerTag> finalTags;
    // Filter out invalid tags
    for (auto& tag : modTags) {
        auto stag = ranges::find(fetched, [&tag](server::ServerTag const& stag) {
            return stag.name == tag;
        });
        if (stag) {
            finalTags.push_back(*stag);
        }
    }

    co_return Ok(std::move(finalTags));
}
server::ServerFuture<server::ServerModUpdateOneCheck> ModSource::checkUpdates() {
    m_availableUpdate = server::ServerModUpdateOneCheck();
    if (std::holds_alternative<server::ServerModMetadata>(m_value)) {
        // Server mods aren't installed so you can't install updates for them
        co_return Ok(m_availableUpdate);
    }

    auto mod = std::get<Mod*>(m_value);
    m_availableUpdate = std::move(ARC_CO_UNWRAP(co_await server::checkUpdates(mod)));
    co_return Ok(m_availableUpdate);
}
void ModSource::startInstall() {
    if (auto update = this->hasUpdates().update) {
        server::ModDownloadManager::get()->startDownload(this->getID(), update->version);
    } else {
        server::ModDownloadManager::get()->startDownload(
            this->getID(),
            this->asServer()
                ? std::optional{this->asServer()->latestVersion().getVersion()}
                : std::nullopt
        );
    }
}
