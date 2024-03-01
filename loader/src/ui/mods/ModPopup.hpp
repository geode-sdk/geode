#pragma once

#include <Geode/ui/Popup.hpp>
#include "ModSource.hpp"

using namespace geode::prelude;

class ModPopup : public Popup<ModSource&&> {
protected:
    ModSource m_source;

    bool setup(ModSource&& src) override;

public:
    static ModPopup* create(ModSource&& src);
};
