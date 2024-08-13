#include "ModSettingsManager.hpp"

SettingV3* ModSettingsManager::get(std::string const& id) {}

SettingValue* ModSettingsManager::getLegacy(std::string const& id) {
    // If this setting has alreay been given a legacy interface, give that
    if (m_legacy.count(id)) {
        return m_legacy.at(id).get();
    }
    if (m_v3.count(id)) {}
}
