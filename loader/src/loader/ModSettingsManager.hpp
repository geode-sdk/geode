#pragma once

#include <Geode/DefaultInclude.hpp>
#include <Geode/loader/SettingV3.hpp>

using namespace geode::prelude;

// This class should NEVER be exposed in a header!!!
// It is an implementation detail!!!

class ModSettingsManager final {
private:
    struct SettingInfo final {
        std::unique_ptr<SettingV3> v3;
        std::unique_ptr<SettingValue> legacy;
    };

    std::unordered_map<std::string, std::unique_ptr<SettingV3>> m_v3;
    // todo: remove in v4
    std::unordered_map<std::string, std::unique_ptr<SettingValue>> m_legacy;

public:
    SettingV3* get(std::string const& id);
    SettingValue* getLegacy(std::string const& id);
};
