#pragma once

#include <Geode/ui/General.hpp>
#include <Geode/ui/SimpleAxisLayout.hpp>
#include <Geode/ui/Popup.hpp>
#include <Geode/ui/NineSlice.hpp>

using namespace geode::prelude;

enum class ModListDisplay {
    SmallList,
    BigList,
    Grid,
};

class ModListItem : public CCNode {
protected:
    NineSlice* m_bg;
    ModListDisplay m_display = ModListDisplay::SmallList;
    float m_targetWidth = 300.0f;
    CCNode* m_viewMenu;

    bool init();

    virtual void updateState();

public:
    virtual void updateDisplay(float width, ModListDisplay display);
};
