#include <Geode/loader/ModSettingsManager.hpp>
#include <Geode/utils/JsonValidation.hpp>
#include "ModImpl.hpp"

using namespace geode::prelude;

// All setting type generators are put in a shared pool for two reasons:
// #1 no need to duplicate the built-in settings between all mods
// #2 easier lookup of custom settings if a mod uses another mod's custom setting type


namespace {
    auto changeToGenerator(auto&& function) {
        return [function](
            std::string const& key,
            std::string const& modID,
            matjson::Value const& json
        ) -> Result<std::shared_ptr<SettingV3>> {
            return function(key, modID, json).map([](auto&& ptr) {
                return std::shared_ptr<SettingV3>(ptr);
            });
        };
    }
}
class SharedSettingTypesPool final {
private:
    std::unordered_map<std::string, SettingGenerator> m_types;

    SharedSettingTypesPool() : m_types({
        { "title", changeToGenerator(TitleSettingV3::parse) },
        { "bool", changeToGenerator(BoolSettingV3::parse) },
        { "int", changeToGenerator(IntSettingV3::parse) },
        { "float", changeToGenerator(FloatSettingV3::parse) },
        { "string", changeToGenerator(StringSettingV3::parse) },
        { "file", changeToGenerator(FileSettingV3::parse) },
        { "folder", changeToGenerator(FileSettingV3::parse) },
        { "path", changeToGenerator(FileSettingV3::parse) },
        { "rgb", changeToGenerator(Color3BSettingV3::parse) },
        { "color", changeToGenerator(Color3BSettingV3::parse) },
        { "rgba", changeToGenerator(Color4BSettingV3::parse) },
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
        std::shared_ptr<Setting> v3 = nullptr;
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
            if (sett.v3->save(value)) {
                this->savedata[key] = value;
            }
            else {
                log::error("Unable to save setting '{}' for mod {}", key, this->modID);
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
                setting.v3 = v3.unwrap();
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
    for (auto const& [key, json] : metadata.getSettings()) {
        auto setting = Impl::SettingInfo();
        setting.json = json;
        auto root = checkJson(json, "setting");
        root.needs("type").into(setting.type);
        if (root) {
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

Result<> ModSettingsManager::load(matjson::Value const& json) {
    if (json.isObject()) {
        // Save this so when custom settings are registered they can load their
        // values properly
        m_impl->savedata = json;
        for (auto const& [key, _] : json) {
           m_impl->loadSettingValueFromSave(key);
        }
    }
    return Ok();
}
matjson::Value ModSettingsManager::save() {
    for (auto& [key, _] : m_impl->settings) {
        m_impl->saveSettingValueToSave(key);
    }
    // Doing this since `ModSettingsManager` is expected to manage savedata fully
    return m_impl->savedata;
}
matjson::Value& ModSettingsManager::getSaveData() {
    return m_impl->savedata;
}

std::shared_ptr<Setting> ModSettingsManager::get(std::string_view key) {
    auto id = std::string(key);
    return m_impl->settings.count(id) ? m_impl->settings.at(id).v3 : nullptr;
}

bool ModSettingsManager::restartRequired() const {
    return m_impl->restartRequired;
}
