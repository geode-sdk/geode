#pragma once

#include <Geode/loader/SettingV3.hpp>

using namespace geode::prelude;

// todo in v4: this header can be fully removed and the impl moved back into SettingV3.cpp
// for now it has to be exposed for ModSettingsManager legacy compatibility

class UnresolvedCustomSettingV3::Impl final {
public:
    matjson::Value json;
    // todo: remove in v4
    // this is for compatability with legacy custom settings
    // in v3 settings custom settings just replace the definition fully like a normal person
    std::shared_ptr<SettingValue> legacyValue = nullptr;
};

