#include <Geode/loader/ModSettingsManager.hpp>
#include <Geode/utils/JsonValidation.hpp>
#include "ModImpl.hpp"

using namespace geode::prelude;

// All setting type generators are put in a shared pool for two reasons:
// #1 no need to duplicate the built-in settings between all mods
// #2 easier lookup of custom settings if a mod uses another mod's custom setting type

class SharedSettingTypesPool final {
private:
    std::unordered_map<std::string, SettingGenerator> m_types;

    SharedSettingTypesPool() : m_types({
        // todo in v4: remove this
        { "custom", &LegacyCustomSettingV3::parse },
        { "title", &TitleSettingV3::parse },
        { "bool", &BoolSettingV3::parse },
        { "int", &IntSettingV3::parse },
        { "float", &FloatSettingV3::parse },
        { "string", &StringSettingV3::parse },
        { "file", &FileSettingV3::parse },
        { "folder", &FileSettingV3::parse },
        { "path", &FileSettingV3::parse },
        { "rgb", &Color3BSettingV3::parse },
        { "color", &Color3BSettingV3::parse },
        { "rgba", &Color4BSettingV3::parse },
    }) {}

public:
    static SharedSettingTypesPool& get() {
        static auto inst = SharedSettingTypesPool();
        return inst;
    }

    Result<> add(std::string_view modID, std::string_view type, SettingGenerator generator) {
        // Limit type to just [a-z0-9\-]+
        if (type.empty() || !std::all_of(type.begin(), type.end(), +[](char c) {
            return
                ('a' <= c && c <= 'z') ||
                ('0' <= c && c <= '9') ||
                (c == '-');
        })) {
            return Err("Custom setting types must match the regex [a-z0-9\\-]+");
        }
        auto full = fmt::format("{}/{}", modID, type);
        if (m_types.contains(full)) {
            return Err("Type \"{}\" has already been registered for mod {}", type, modID);
        }
        m_types.emplace(full, generator);
        return Ok();
    }
    std::optional<SettingGenerator> find(std::string_view modID, std::string_view fullType) {
        // Find custom settings via namespaced lookup
        if (fullType.starts_with("custom:")) {
            auto full = std::string(fullType.substr(fullType.find(':') + 1));
            // If there's no mod ID in the type name, use the current mod's ID
            if (full.find('/') == std::string_view::npos) {
                full = fmt::format("{}/{}", modID, full);
            }
            if (m_types.contains(full)) {
                return m_types.at(full);
            }
        }
        // Otherwise find a built-in setting
        else {
            auto full = std::string(fullType);
            if (m_types.contains(full)) {
                return m_types.at(full);
            }
        }
        // Return null if nothing was found
        return std::nullopt;
    }
};

class ModSettingsManager::Impl final {
public:
    struct SettingInfo final {
        std::string type;
        matjson::Value json;
        std::shared_ptr<SettingV3> v3 = nullptr;
        // todo: remove in v4
        std::shared_ptr<SettingValue> legacy = nullptr;
    };  
    std::string modID;
    std::unordered_map<std::string, SettingInfo> settings;
    // Stored so custom settings registered after the fact can be loaded
    // If the ability to unregister custom settings is ever added, remember to 
    // update this by calling saveSettingValueToSave
    matjson::Value savedata;
    bool restartRequired = false;

    void loadSettingValueFromSave(std::string const& key) {
        if (this->savedata.contains(key) && this->settings.contains(key)) {
            auto& sett = this->settings.at(key);
            if (!sett.v3) return;
            try {
                if (!sett.v3->load(this->savedata[key])) {
                    log::error("Unable to load setting '{}' for mod {}", key, this->modID);
                }
            }
            // matjson::JsonException doesn't catch all possible json errors
            catch(std::exception const& e) {
                log::error("Unable to load setting '{}' for mod {} (JSON exception): {}", key, this->modID, e.what());
            }
        }
    }
    void saveSettingValueToSave(std::string const& key) {
        if (this->settings.contains(key)) {
            auto& sett = this->settings.at(key);
            if (!sett.v3) return;
            // Store the value in an intermediary so if `save` fails the existing 
            // value loaded from disk isn't overwritten
            matjson::Value value;
            try {
                if (sett.v3->save(value)) {
                    this->savedata[key] = value;
                }
                else {
                    log::error("Unable to save setting '{}' for mod {}", key, this->modID);
                }
            }
            catch(matjson::JsonException const& e) {
                log::error("Unable to save setting '{}' for mod {} (JSON exception): {}", key, this->modID, e.what());
            }
        }
    }

    void createSettings() {
        for (auto& [key, setting] : settings) {
            if (setting.v3) {
                continue;
            }
            auto gen = SharedSettingTypesPool::get().find(modID, setting.type);
            // The type was not found, meaning it probably hasn't been registered yet
            if (!gen) {
                continue;
            }
            if (auto v3 = (*gen)(key, modID, setting.json)) {
                setting.v3 = *v3;
                this->loadSettingValueFromSave(key);
            }
            else {
                log::error(
                    "Unable to parse setting '{}' for mod {}: {}",
                    key, modID, v3.unwrapErr()
                );
            }
        }
    }
};

ModSettingsManager* ModSettingsManager::from(Mod* mod) {
    if (!mod) return nullptr;
    return ModImpl::getImpl(mod)->m_settings.get();
}

ModSettingsManager::ModSettingsManager(ModMetadata const& metadata)
  : m_impl(std::make_unique<Impl>())
{
    m_impl->modID = metadata.getID();
    for (auto const& [key, json] : metadata.getSettingsV3()) {
        auto setting = Impl::SettingInfo();
        setting.json = json;
        auto root = checkJson(json, "setting");
        root.needs("type").into(setting.type);
        if (root) {
            if (setting.type == "custom") {
                log::warn(
                    "Setting \"{}\" in mod {} has the old \"custom\" type - "
                    "this type has been deprecated and will be removed in Geode v4.0.0. "
                    "Use the new \"custom:type-name-here\" syntax for defining custom "
                    "setting types - see more in "
                    "https://docs.geode-sdk.org/mods/settings/#custom-settings",
                    key, m_impl->modID
                );
            }
            m_impl->settings.emplace(key, setting);
        }
        else {
            log::error("Setting '{}' in mod {} is missing type", key, m_impl->modID);
        }
    }
    m_impl->createSettings();
}
ModSettingsManager::~ModSettingsManager() {}
ModSettingsManager::ModSettingsManager(ModSettingsManager&&) = default;

void ModSettingsManager::markRestartRequired() {
    m_impl->restartRequired = true;
}

Result<> ModSettingsManager::registerCustomSettingType(std::string_view type, SettingGenerator generator) {
    GEODE_UNWRAP(SharedSettingTypesPool::get().add(m_impl->modID, type, generator));
    m_impl->createSettings();
    return Ok();
}
Result<> ModSettingsManager::registerLegacyCustomSetting(std::string_view key, std::unique_ptr<SettingValue>&& ptr) {
    auto id = std::string(key);
    if (!m_impl->settings.count(id)) {
        return Err("No such setting '{}' in mod {}", id, m_impl->modID);
    }
    auto& sett = m_impl->settings.at(id);
    if (auto custom = typeinfo_pointer_cast<LegacyCustomSettingV3>(sett.v3)) {
        if (!custom->getValue()) {
            custom->setValue(std::move(ptr));
            m_impl->loadSettingValueFromSave(id);
        }
        else {
            return Err("Setting '{}' in mod {} has already been registed", id, m_impl->modID);
        }
    }
    else {
        return Err("Setting '{}' in mod {} is not a legacy custom setting", id, m_impl->modID);
    }
    return Ok();
}

Result<> ModSettingsManager::load(matjson::Value const& json) {
    if (json.is_object()) {
        // Save this so when custom settings are registered they can load their 
        // values properly
        m_impl->savedata = json.as_object();
        for (auto const& [key, _] : json.as_object()) {
           m_impl->loadSettingValueFromSave(key);
        }
    }
    return Ok();
}
void ModSettingsManager::save(matjson::Value& json) {
    for (auto& [key, _] : m_impl->settings) {
        m_impl->saveSettingValueToSave(key);
    }
    // Doing this since `ModSettingsManager` is expected to manage savedata fully
    json = m_impl->savedata;
}
matjson::Value& ModSettingsManager::getSaveData() {
    return m_impl->savedata;
}

std::shared_ptr<SettingV3> ModSettingsManager::get(std::string_view key) {
    auto id = std::string(key);
    return m_impl->settings.count(id) ? m_impl->settings.at(id).v3 : nullptr;
}
std::shared_ptr<SettingValue> ModSettingsManager::getLegacy(std::string_view key) {
    auto id = std::string(key);
    if (!m_impl->settings.count(id)) {
        return nullptr;
    }
    auto& info = m_impl->settings.at(id);
    // If this setting has alreay been given a legacy interface, give that
    if (info.legacy) {
        return info.legacy;
    }
    // Uninitialized settings are null
    if (!info.v3) {
        return nullptr;
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

bool ModSettingsManager::restartRequired() const {
    return m_impl->restartRequired;
}
