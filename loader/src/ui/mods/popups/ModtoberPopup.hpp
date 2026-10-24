#pragma once

#include "../../GeodeStyle.hpp"

#include <Geode/ui/Popup.hpp>

using namespace geode::prelude;

class ModtoberPopup : public GeodePopup {
protected:
    bool init() override;

    void onDiscord(CCObject*);

public:
    static ModtoberPopup* create();
};