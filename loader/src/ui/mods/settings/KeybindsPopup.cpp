#include "KeybindsPopup.hpp"
#include <Geode/loader/ModSettingsManager.hpp>

static KeybindsPopupTab POPUP_TAB = KeybindsPopupTab::All;

static std::vector<Mod*> getModsSorted() {
    auto mods = Loader::get()->getAllMods();

    // Sort alphabetically
    std::ranges::sort(mods, [](Mod* a, Mod* b) {
        return a->getName() > b->getName();
    });

    // Move Custom Keybinds to be first because it'll have all the vanilla 
    // keybinds :-)
    auto cb = std::ranges::find_if(mods, [](Mod* mod) {
        return mod->getID() == "geode.custom-keybinds";
    });
    if (cb != mods.end()) {
        std::ranges::rotate(mods, cb);
    }
    return mods;
}

bool KeybindsPopup::init(bool forceDisableTheme) {
    if (!BaseSettingsPopup::init(forceDisableTheme, true))
        return false;
    
    m_tab = POPUP_TAB;
    
    this->setTitle("Keybinds");
    m_searchInput->setPlaceholder("Search keybinds...");
    for (auto mod : getModsSorted()) {
        bool addedSubtitle = false;
        for (auto key : mod->getSettingKeys()) {
            if (auto sett = typeinfo_pointer_cast<KeybindSettingV3>(mod->getSetting(key))) {
                if (!addedSubtitle) {
                    m_settings.push_back(TitleSettingNodeV3::create(
                        mod->getName(), "Test popup", m_listSize.width
                    ));
                    addedSubtitle = true;
                }
                m_settings.push_back(sett->createNode(m_listSize.width));
            }
        }
    }

    m_tabsMenu = CCMenu::create();
    m_tabsMenu->ignoreAnchorPointForPosition(false);
    m_tabsMenu->setContentWidth(m_listSize.width);

    for (auto tab : {
        std::make_tuple(KeybindsPopupTab::All, "keybinds.png"_spr, "All"),
        std::make_tuple(KeybindsPopupTab::Universal, "GJ_sMagicIcon_001.png", "Universal"),
        std::make_tuple(KeybindsPopupTab::Gameplay, "controllerBtn_DPad_Right_001.png", "Gameplay"),
        std::make_tuple(KeybindsPopupTab::Editor, "GJ_hammerIcon_001.png", "Editor"),
    }) {
        auto id = std::get<0>(tab);
        auto tabSpr = GeodeTabSprite::create(std::get<1>(tab), std::get<2>(tab), 120);
        tabSpr->setScale(.7f);

        auto tabBtn = CCMenuItemSpriteExtra::create(
            tabSpr, this, menu_selector(KeybindsPopup::onSelectTab)
        );
        tabBtn->setTag(static_cast<int>(id));
        m_tabsMenu->addChild(tabBtn);
    }

    m_tabsMenu->setLayout(RowLayout::create());
    m_mainLayer->addChildAtPosition(m_tabsMenu, Anchor::Center, ccp(0, 95));

    this->onSelectTab(nullptr);
    this->updateState();
    
    return true;
}

bool KeybindsPopup::shouldShow(SettingNode* node) const {
    if (m_tab == KeybindsPopupTab::All) {
        return true;
    }
    if (auto sett = typeinfo_pointer_cast<KeybindSettingV3>(node->getSetting())) {
        return sett->getCategory() == static_cast<KeybindCategory>(m_tab);
    }
    return true;
}

void KeybindsPopup::onSelectTab(CCObject* sender) {
    if (sender) {
        m_tab = static_cast<KeybindsPopupTab>(static_cast<CCNode*>(sender)->getTag());
        POPUP_TAB = m_tab;
    }
    for (auto tab : CCArrayExt<CCMenuItemSpriteExtra*>(m_tabsMenu->getChildren())) {
        static_cast<GeodeTabSprite*>(tab->getNormalImage())->select(
            static_cast<KeybindsPopupTab>(tab->getTag()) == m_tab
        );
    }
    this->updateState(nullptr);
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
