#pragma once

#include <Geode/ui/General.hpp>

using namespace geode::prelude;

class SwelvyBG : public CCNode {
protected:
    bool init();

    void updateSpritePosition(float dt);

public:
    static SwelvyBG* create();
};
