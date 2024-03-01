#include "ModSource.hpp"
#include <Geode/ui/GeodeUI.hpp>

ModSource::ModSource(Mod* mod) : m_value(mod) {}
ModSource::ModSource(server::ServerModMetadata&& metadata) : m_value(metadata) {}

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

Mod* ModSource::asMod() const {
    auto mod = std::get_if<Mod*>(&m_value);
    return mod ? *mod : nullptr;
}
server::ServerModMetadata const* ModSource::asServer() const {
    return std::get_if<server::ServerModMetadata>(&m_value);
}
