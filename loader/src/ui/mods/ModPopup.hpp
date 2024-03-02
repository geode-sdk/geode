#pragma once

#include <Geode/ui/Popup.hpp>
#include <Geode/ui/MDTextArea.hpp>
#include "ModSource.hpp"
#include "GeodeStyle.hpp"

using namespace geode::prelude;

class ModPopup : public Popup<ModSource&&> {
protected:
    enum class Tab {
        Details,
        Changelog,
        Versions,
    };

    ModSource m_source;
    CCNode* m_rightColumn;
    CCNode* m_currentTabPage = nullptr;
    std::unordered_map<Tab, std::pair<GeodeTabSprite*, Ref<CCNode>>> m_tabs;

    bool setup(ModSource&& src) override;

    void loadTab(Tab tab);
    void onTab(CCObject* sender);

public:
    static ModPopup* create(ModSource&& src);
};
