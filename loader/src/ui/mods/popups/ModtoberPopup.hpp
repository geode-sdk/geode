#pragma once

#include <Geode/ui/Popup.hpp>
#include "../GeodeStyle.hpp"

using namespace geode::prelude;

class ModtoberPopup : public GeodePopup<> {
protected:
    bool setup() override;

    void onDiscord(CCObject*);

public:
    static ModtoberPopup* create();
};
