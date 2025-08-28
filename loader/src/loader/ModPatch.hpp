#pragma once

#include <Geode/loader/Mod.hpp>

using namespace geode::prelude;

class ModPatch {
public:
    Mod* m_owner = nullptr;
    bool m_enabled = false;
    bool m_autoEnable = true;

    [[nodiscard]] Mod* getOwner() const;
    Result<> setOwner(Mod* mod);

    [[nodiscard]] bool isEnabled() const;
    virtual Result<> enable() = 0;
    virtual Result<> disable() = 0;
    virtual Result<> toggle() = 0;
    virtual Result<> toggle(bool enable) = 0;

    [[nodiscard]] bool getAutoEnable() const;
    void setAutoEnable(bool autoEnable);
};
