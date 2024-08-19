#pragma once

#include <Geode/DefaultInclude.hpp>
#include <Geode/loader/SettingV3.hpp>

using namespace geode::prelude;

class ModSettingsManager final {
private:
    class Impl;
    std::unique_ptr<Impl> m_impl;

public:
    ModSettingsManager(ModMetadata const& metadata);
    ~ModSettingsManager();

    Result<> load(matjson::Value const& json);
    void save(matjson::Value& json);

    Result<> registerCustomSetting(std::string_view key, std::shared_ptr<SettingV3> ptr);
    Result<> registerLegacyCustomSetting(std::string_view key, std::unique_ptr<SettingValue>&& ptr);

    std::shared_ptr<SettingV3> get(std::string_view key);
    std::shared_ptr<SettingValue> getLegacy(std::string_view key);
    std::optional<Setting> getLegacyDefinition(std::string_view key);
};
