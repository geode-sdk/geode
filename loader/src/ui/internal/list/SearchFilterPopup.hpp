#pragma once

#include <Geode/ui/Popup.hpp>

USE_GEODE_NAMESPACE();

class ModListLayer;
enum class ModListType;

class SearchFilterPopup : public Popup<ModListLayer*, ModListType> {
protected:
    ModListLayer* m_modLayer;

    bool setup(ModListLayer* layer, ModListType type) override;
    CCMenuItemToggler* addToggle(
        char const* title, SEL_MenuHandler selector, bool toggled, int tag, CCPoint& pos
    );
    CCMenuItemToggler* addPlatformToggle(char const* title, PlatformID id, CCPoint& pos);

    void onPlatformToggle(CCObject*);
    void onShowInstalled(CCObject*);
    void onTag(CCObject*);

    void enable(CCMenuItemToggler* toggle, ModListType type);

    void onClose(CCObject*) override;

public:
    static SearchFilterPopup* create(ModListLayer* layer, ModListType type);
};
