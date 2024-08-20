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

    ModSettingsManager(ModSettingsManager&&);
    ModSettingsManager(ModSettingsManager const&) = delete;

    /**
     * Load setting values from savedata.
     * The format of the savedata should be an object with the keys being 
     * setting IDs and then the values the values of the saved settings
     * @returns Ok if no horrible errors happened. Note that a setting value 
     * missing is not considered a horrible error, but will instead just log a 
     * warning into the console!
     */
    Result<> load(matjson::Value const& json);
    /**
     * Save setting values to savedata.
     * The format of the savedata will be an object with the keys being 
     * setting IDs and then the values the values of the saved settings
     * @note If saving a setting fails, it will log a warning to the console
     */
    void save(matjson::Value& json);

    Result<> registerCustomSetting(std::string_view key, std::shared_ptr<SettingV3> ptr);
    // remove in v4
    Result<> registerLegacyCustomSetting(std::string_view key, std::unique_ptr<SettingValue>&& ptr);

    std::shared_ptr<SettingV3> get(std::string_view key);
    std::shared_ptr<SettingValue> getLegacy(std::string_view key);
    std::optional<Setting> getLegacyDefinition(std::string_view key);
};
