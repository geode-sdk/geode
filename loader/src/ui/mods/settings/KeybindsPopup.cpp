#include "KeybindsPopup.hpp"
#include <Geode/loader/ModSettingsManager.hpp>

static std::vector<Mod*> getModsSorted() {
    auto mods = Loader::get()->getAllMods();

    // Sort alphabetically
    std::ranges::sort(mods, [](Mod* a, Mod* b) {
        return a->getName() < b->getName();
    });

    // Move Custom Keybinds to be first because it'll have all the vanilla 
    // keybinds :-)
    auto cb = std::ranges::find_if(mods, [](Mod* mod) {
        return mod->getID() == "geode-sdk.custom-keybinds";
    });
    if (cb != mods.end()) {
        std::ranges::rotate(mods, cb);
    }
    return mods;
}

bool KeybindsPopup::init(bool forceDisableTheme) {
    if (!BaseSettingsPopup::init(forceDisableTheme))
        return false;
    
    this->setTitle("Keybinds");
    m_searchInput->setPlaceholder("Search keybinds...");
    for (auto mod : getModsSorted()) {
        bool addedSubtitle = false;
        for (auto key : mod->getSettingKeys()) {
            if (auto sett = typeinfo_pointer_cast<KeybindSettingV3>(mod->getSetting(key))) {
                if (!addedSubtitle) {
                    m_settings.push_back(TitleSettingNodeV3::create(
                        mod->getName(), "Test popup", m_layerSize.width
                    ));
                    addedSubtitle = true;
                }
                m_settings.push_back(sett->createNode(m_layerSize.width));
            }
        }
    }

    this->updateState();
    
    return true;
}

KeybindsPopup* KeybindsPopup::create(bool forceDisableTheme) {
    auto ret = new KeybindsPopup();
    if (ret->init(forceDisableTheme)) {
        ret->autorelease();
        return ret;
    }
    delete ret;
    return nullptr;
}
