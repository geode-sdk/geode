#include "KeybindsPopup.hpp"
#include <Geode/loader/ModSettingsManager.hpp>

static KeybindsPopupTab POPUP_TAB = KeybindsPopupTab::All;

static std::vector<Mod*> getModsSorted() {
    auto mods = Loader::get()->getAllMods();
    std::erase_if(mods, [](Mod* mod) {
        return !mod->isLoaded();
    });

    // Sort alphabetically
    std::ranges::sort(mods, [](Mod* a, Mod* b) {
        return a->getName() > b->getName();
    });

    return mods;
}

bool KeybindsPopup::init(std::optional<KeybindsPopupTab> tab, Mod* mod, bool forceDisableTheme) {
    if (!BaseSettingsPopup::init(forceDisableTheme, true)) return false;

    m_tab = tab.value_or(POPUP_TAB);

    this->setTitle("Mod Keybinds");
    m_searchInput->setPlaceholder("Search keybinds...");
    for (auto mod : getModsSorted()) {
        bool addedSubtitle = false;
        for (auto& key : mod->getSettingKeys()) {
            if (auto sett = typeinfo_pointer_cast<KeybindSettingV3>(mod->getSetting(key))) {
                if (!addedSubtitle) {
                    m_settings.push_back(TitleSettingNodeV3::create(
                        mod->getName(), m_listSize.width
                    ));
                    addedSubtitle = true;
                }
                m_settings.push_back(sett->createNode(m_listSize.width));
            }
        }
    }

    m_tabsMenu = CCNode::create();
    m_tabsMenu->setID("tabs-menu");
    m_tabsMenu->setAnchorPoint({.5f, .5f});
    m_tabsMenu->setContentWidth(m_listSize.width);

    struct TabInfo {
        KeybindsPopupTab id;
        const char* sprName;
        const char* label;
    };

    static constexpr auto tabs = std::array{
        TabInfo{
            KeybindsPopupTab::All,
            "keybinds.png"_spr,
            "All",
        },
        TabInfo{
            KeybindsPopupTab::Universal,
            "GJ_sMagicIcon_001.png",
            "Universal",
        },
        TabInfo{
            KeybindsPopupTab::Gameplay,
            "controllerBtn_DPad_Right_001.png",
            "Gameplay",
        },
        TabInfo{
            KeybindsPopupTab::Editor,
            "GJ_hammerIcon_001.png",
            "Editor",
        },
    };

    for (auto& tab : tabs) {
        auto tabSpr = GeodeTabSprite::create(tab.sprName, tab.label, 120.f);
        tabSpr->select(tab.id == m_tab);

        auto tabBtn = Button::createWithNode(tabSpr, [this, id = tab.id](auto) {
            m_tab = id;
            POPUP_TAB = m_tab;

            for (auto tab : CCArrayExt<Button*>(m_tabsMenu->getChildren())) {
                auto tabId = static_cast<KeybindsPopupTab>(tab->getTag());
                static_cast<GeodeTabSprite*>(tab->getDisplayNode())->select(tabId == m_tab);
                tab->setEnabled(tabId != m_tab);
            }

            this->updateState(nullptr);
        });
        tabBtn->setScale(.7f);
        tabBtn->setTag(static_cast<int>(tab.id));
        tabBtn->setEnabled(tab.id != m_tab);
        m_tabsMenu->addChild(tabBtn);
    }

    m_tabsMenu->setLayout(RowLayout::create());
    m_mainLayer->addChildAtPosition(m_tabsMenu, Anchor::Center, {.0f, 95.f});

    this->onSelectTab(nullptr);
    if (mod) {
        this->focusMod(mod);
    }

    this->updateState();

    return true;
}

bool KeybindsPopup::shouldShow(SettingNode* node) const {
    if (m_tab == KeybindsPopupTab::All) {
        return true;
    }
    if (auto title = typeinfo_cast<TitleSettingNodeV3*>(node)) {
        // working with what geode has cuz i dont wanna make any big changes
        auto it = std::ranges::find_if(m_settings, [node] (auto& ref) {
            return ref == node;
        });
        // check if any of the titles settings match the category (this could be done in a far cleaner way but once again im working with what geode has)
        while (it != m_settings.end()) {
            auto ptr = (*it).data();
            if (ptr != node && typeinfo_cast<TitleSettingNodeV3*>(ptr)) {
                return false;
            }
            else if (auto sett = typeinfo_pointer_cast<KeybindSettingV3>(ptr->getSetting()); sett && sett->getCategory() == static_cast<KeybindCategory>(m_tab)) {
                return true;
            }
            it++;
        }
        return false;
    }
    if (auto sett = typeinfo_pointer_cast<KeybindSettingV3>(node->getSetting())) {
        return sett->getCategory() == static_cast<KeybindCategory>(m_tab);
    }
    return true;
}

void KeybindsPopup::focusMod(Mod* mod) {
    for (auto node : m_list->m_contentLayer->getChildrenExt()) {
        auto titleNode = typeinfo_cast<TitleSettingNodeV3*>(node);
        if (!titleNode) continue;

        bool matching = titleNode->getNameLabel()->getString() == mod->getName();
        titleNode->setCollapsed(!matching);
    }
}

void KeybindsPopup::onSelectTab(CCObject* sender) {
}

KeybindsPopup* KeybindsPopup::create(bool forceDisableTheme) {
    return create(KeybindsPopupTab::All, nullptr, forceDisableTheme);
}

KeybindsPopup* KeybindsPopup::create(std::optional<KeybindsPopupTab> tab, Mod* mod, bool forceDisableTheme) {
    auto ret = new KeybindsPopup();
    if (ret->init(tab, mod, forceDisableTheme)) {
        ret->autorelease();
        return ret;
    }
    delete ret;
    return nullptr;
}
