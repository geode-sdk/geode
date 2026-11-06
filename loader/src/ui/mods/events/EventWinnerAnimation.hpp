#pragma once

#include <Geode/utils/cocos.hpp>
#include <Geode/cocos/layers_scenes_transitions_nodes/CCTransition.h>

using namespace geode::prelude;

// NOTE: this has a generic name because we should have modtober info

class EventWinnerAnimation : public CCMenu {
protected:
    CCLayerColor* m_bg;

    bool init();

    void onClose(CCObject*);

public:
    static EventWinnerAnimation* create();
};
