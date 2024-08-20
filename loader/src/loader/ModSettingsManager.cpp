#include "ModSettingsManager.hpp"
#include "SettingV3Impl.hpp"
#include <Geode/utils/JsonValidation.hpp>

class ModSettingsManager::Impl final {
public:
    struct SettingInfo final {
        std::shared_ptr<SettingV3> v3;
        // todo: remove in v4
        std::shared_ptr<SettingValue> legacy = nullptr;
    };
    std::string modID;
    std::unordered_map<std::string, SettingInfo> list;
};

ModSettingsManager::ModSettingsManager(ModMetadata const& metadata)
  : m_impl(std::make_unique<Impl>())
{
    m_impl->modID = metadata.getID();
    for (auto const& [key, json] : metadata.getSettingsV3()) {
        if (auto v3 = SettingV3::parseBuiltin(key, m_impl->modID, json)) {
            auto setting = Impl::SettingInfo();
            setting.v3.swap(*v3);
            m_impl->list.emplace(key, setting);
        }
        else {
            log::error("Unable to parse setting '{}' for mod {}: {}", key, m_impl->modID, v3.unwrapErr());
        }
    }
}
ModSettingsManager::~ModSettingsManager() {}

ModSettingsManager::ModSettingsManager(ModSettingsManager&&) = default;

Result<> ModSettingsManager::registerCustomSetting(std::string_view key, std::shared_ptr<SettingV3> ptr) {
    if (!ptr) {
        return Err("Custom settings must not be null!");
    }
    auto id = std::string(key);
    if (!m_impl->list.count(id)) {
        return Err("No such setting '{}' in mod {}", id, m_impl->modID);
    }
    auto& sett = m_impl->list.at(id);
    sett.v3.swap(ptr);
    return Ok();
}
Result<> ModSettingsManager::registerLegacyCustomSetting(std::string_view key, std::unique_ptr<SettingValue>&& ptr) {
    auto id = std::string(key);
    if (!m_impl->list.count(id)) {
        return Err("No such setting '{}' in mod {}", id, m_impl->modID);
    }
    auto& sett = m_impl->list.at(id);
    if (auto custom = typeinfo_pointer_cast<UnresolvedCustomSettingV3>(sett.v3)) {
        if (!custom->m_impl->legacyValue) {
            custom->m_impl->legacyValue = std::move(ptr);
        }
        else {
            return Err("Setting '{}' in mod {} has already been registed", id, m_impl->modID);
        }
    }
    else {
        return Err("Setting '{}' in mod {} is not a custom setting", id, m_impl->modID);
    }
    return Ok();
}

Result<> ModSettingsManager::load(matjson::Value const& json) {
    auto root = checkJson(json, "Settings");
    for (auto const& [key, value] : root.properties()) {
        if (m_impl->list.contains(key)) {
            try {
                if (!m_impl->list.at(key).v3->load(value.json())) {
                    log::error("Unable to load setting '{}' for mod {}", key, m_impl->modID);
                }
            }
            catch(matjson::JsonException const& e) {
                log::error("Unable to load setting '{}' for mod {} (JSON exception): {}", key, m_impl->modID, e.what());
            }
        }
    }
    return Ok();
}
void ModSettingsManager::save(matjson::Value& json) {
    for (auto& [key, sett] : m_impl->list) {
        // Store the value in an intermediary so if `save` fails the existing 
        // value loaded from disk isn't overwritten
        matjson::Value value;
        try {
            if (sett.v3->save(value)) {
                json[key] = value;
            }
            else {
                log::error("Unable to save setting '{}' for mod {}", key, m_impl->modID);
            }
        }
        catch(matjson::JsonException const& e) {
            log::error("Unable to save setting '{}' for mod {} (JSON exception): {}", key, m_impl->modID, e.what());
        }
    }
}

std::shared_ptr<SettingV3> ModSettingsManager::get(std::string_view key) {
    auto id = std::string(key);
    return m_impl->list.count(id) ? m_impl->list.at(id).v3 : nullptr;
}
std::shared_ptr<SettingValue> ModSettingsManager::getLegacy(std::string_view key) {
    auto id = std::string(key);
    if (!m_impl->list.count(id)) {
        return nullptr;
    }
    auto& info = m_impl->list.at(id);
    // If this setting has alreay been given a legacy interface, give that
    if (info.legacy) {
        return info.legacy;
    }
    // Generate new legacy interface
    if (auto legacy = info.v3->convertToLegacyValue()) {
        info.legacy.swap(*legacy);
        return info.legacy;
    }
    return nullptr;
}
std::optional<Setting> ModSettingsManager::getLegacyDefinition(std::string_view key) {
    if (auto s = this->get(key)) {
        return s->convertToLegacy();
    }
    return std::nullopt;
}
