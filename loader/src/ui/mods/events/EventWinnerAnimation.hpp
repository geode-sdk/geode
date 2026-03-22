#pragma once

#include <Geode/utils/cocos.hpp>

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
