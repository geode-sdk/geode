#pragma once

#include <Geode/loader/Mod.hpp>

using namespace geode::prelude;

class ModPatch {
public:
    Mod* m_owner = nullptr;
    bool m_enabled = false;
    bool m_autoEnable = true;

    [[nodiscard]] Mod* getOwner() const;
    Result<void, HookError> setOwner(Mod* mod);

    [[nodiscard]] bool isEnabled() const;
    virtual Result<void, HookError> enable() = 0;
    virtual Result<void, HookError> disable() = 0;
    virtual Result<void, HookError> toggle() = 0;
    virtual Result<void, HookError> toggle(bool enable) = 0;

    [[nodiscard]] bool getAutoEnable() const;
    void setAutoEnable(bool autoEnable);
};
