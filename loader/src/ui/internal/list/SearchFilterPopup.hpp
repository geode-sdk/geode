#pragma once

#include <Geode/ui/Popup.hpp>
#include <Geode/ui/ScrollLayer.hpp>

using namespace geode::prelude;

class ModListLayer;
enum class ModListType;

class SearchFilterPopup : public Popup<ModListLayer*, ModListType> {
protected:
    ModListLayer* m_modLayer;
    CCNode* m_platformsContainer;
    CCNode* m_optionsContainer;
    ScrollLayer* m_tagLayer;

    bool setup(ModListLayer* layer, ModListType type) override;
    CCMenuItemToggler* addToggle(const char* title, SEL_MenuHandler selector, bool toggled, int tag, CCNode* target);
    CCMenuItemToggler* addPlatformToggle(const char* title, PlatformID id);

    void onPlatformToggle(CCObject*);
    void onShowInstalled(CCObject*);
    void onShowInvalid(CCObject*);
    void onTag(CCObject*);

    void enable(CCMenuItemToggler* toggle, ModListType type);

    void onClose(CCObject*) override;

public:
    static SearchFilterPopup* create(ModListLayer* layer, ModListType type);
};
