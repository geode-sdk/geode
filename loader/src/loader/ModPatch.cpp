#include "ModPatch.hpp"

Mod* ModPatch::getOwner() const {
    return m_owner;
}

Result<> ModPatch::setOwner(geode::Mod* mod) {
    if (mod && m_owner) {
        return Err("Cannot directly replace owner of an already owned mod");
    }
    m_owner = mod;
    return Ok();
}

bool ModPatch::isEnabled() const {
    return m_enabled;
}

bool ModPatch::getAutoEnable() const {
    return m_autoEnable;
}

void ModPatch::setAutoEnable(bool autoEnable) {
    m_autoEnable = autoEnable;
}
