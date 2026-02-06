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
    ModListDisplay m_display = ModListDisplay::SmallList;
    float m_targetWidth = 300;
    NineSlice* m_bg;
    CCMenu* m_viewMenu;

    bool init();

    virtual void updateState();

public:
    void updateDisplay(float width, ModListDisplay display);
};
