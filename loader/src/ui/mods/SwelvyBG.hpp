#pragma once

#include <Geode/ui/General.hpp>
#include <Geode/utils/ColorProvider.hpp>

using namespace geode::prelude;

class SwelvyBG : public CCNode {
protected:
    bool init();

    void updateSpritePosition(float dt);

    std::vector<EventListener<ColorProvidedFilter>> m_colorListeners;

public:
    static SwelvyBG* create();
};
