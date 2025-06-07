#include "ModSettingsPopup.hpp"
#include <Geode/binding/ButtonSprite.hpp>
#include <Geode/loader/Mod.hpp>
#include <Geode/loader/ModSettingsManager.hpp>
#include <Geode/ui/ScrollLayer.hpp>
#include <Geode/utils/cocos.hpp>
#include <Geode/ui/General.hpp>
#include <Geode/ui/Scrollbar.hpp>
#include <Geode/loader/Setting.hpp>
#include <loader/SettingNode.hpp>
// needed for weightedFuzzyMatch
#include <ui/mods/sources/ModListSource.hpp>

static bool matchSearch(SettingNode* node, std::string const& query) {
    if (typeinfo_cast<TitleSettingNode*>(node)) {
        return true;
    }
    bool addToList = false;
    auto setting = node->getSetting();
    double weighted = 0;
    if (auto name = setting->getName()) {
        addToList |= weightedFuzzyMatch(setting->getKey(), query, 0.5, weighted);
        addToList |= weightedFuzzyMatch(*name, query, 1, weighted);
    }
    // If there's no name, give full weight to key
    else {
        addToList |= weightedFuzzyMatch(setting->getKey(), query, 1, weighted);
    }
    if (weighted < 60 + 10 * query.size()) {
        addToList = false;
    }
    return addToList;
}

bool ModSettingsPopup::setup(Mod* mod) {
    m_noElasticity = true;
    m_mod = mod;

    this->setTitle(("Settings for " + mod->getName()).c_str());

    auto winSize = CCDirector::sharedDirector()->getWinSize();

    auto const layerSize = CCSize(346, 200);

    auto layerBG = CCLayerColor::create({ 0, 0, 0, 75 });
    layerBG->setContentSize(layerSize);
    layerBG->ignoreAnchorPointForPosition(false);
    m_mainLayer->addChildAtPosition(layerBG, Anchor::Center);

    auto searchContainer = CCMenu::create();
    searchContainer->setContentSize({ layerSize.width, 30 });

    m_searchInput = TextInput::create((layerSize.width - 15) / .7f - 40, "Search Settings...");
    m_searchInput->setTextAlign(TextInputAlign::Left);
    m_searchInput->setScale(.7f);
    m_searchInput->setCallback([this](auto const&) {
        this->updateState();
        m_list->moveToTop();
    });
    m_searchInput->setID("search-input");
    searchContainer->addChildAtPosition(m_searchInput, Anchor::Left, ccp(7.5f, 0), ccp(0, .5f));

    auto searchClearSpr = GeodeSquareSprite::createWithSpriteFrameName("GJ_deleteIcon_001.png", nullptr, m_forceDisableTheme);
    searchClearSpr->setScale(.45f);
    m_searchClearBtn = CCMenuItemSpriteExtra::create(
        searchClearSpr, this, menu_selector(ModSettingsPopup::onClearSearch)
    );
    m_searchClearBtn->setID("clear-search-button");
    searchContainer->addChildAtPosition(m_searchClearBtn, Anchor::Right, ccp(-20, 0));

    layerBG->addChildAtPosition(searchContainer, Anchor::Top, ccp(0, 0), ccp(.5f, 1));

    m_list = ScrollLayer::create(layerSize - ccp(0, searchContainer->getContentHeight()));
    m_list->setTouchEnabled(true);

    for (auto& key : mod->getSettingKeys()) {
        SettingNode* node;
        if (auto sett = mod->getSetting(key)) {
            node = sett->createNode(layerSize.width);
        }
        else {
            node = UnresolvedCustomSettingNode::create(key, mod, layerSize.width);
        }

        m_settings.push_back(node);
        m_list->m_contentLayer->addChild(node);
    }
    m_list->m_contentLayer->setLayout(
        ColumnLayout::create()
            ->setAxisReverse(true)
            ->setAutoGrowAxis(m_list->getContentHeight())
            ->setCrossAxisOverflow(false)
            ->setAxisAlignment(AxisAlignment::End)
            ->setGap(0)
    );
    m_list->moveToTop();

    const int buttonPriority = m_list->getTouchPriority() - 1;

    m_buttonMenu->setTouchPriority(buttonPriority);
    searchContainer->setTouchPriority(buttonPriority);

    layerBG->addChildAtPosition(m_list, Anchor::BottomLeft);

    // layer borders

    m_mainLayer->addChildAtPosition(createGeodeListBorders(layerSize, m_forceDisableTheme), Anchor::Center);

    auto scrollBar = Scrollbar::create(m_list);
    m_mainLayer->addChildAtPosition(
        scrollBar, Anchor::Center, ccp(layerBG->getContentWidth() / 2 + 10, 0)
    );

    // buttons

    m_applyMenu = CCMenu::create();
    m_applyMenu->setContentWidth(150);
    m_applyMenu->setLayout(RowLayout::create());
    m_applyMenu->getLayout()->ignoreInvisibleChildren(true);
    m_applyMenu->setTouchPriority(buttonPriority);

    auto restartBtnSpr = createGeodeButton("Restart Now", true, GeodeButtonSprite::Default, m_forceDisableTheme);
    restartBtnSpr->setScale(.6f);
    m_restartBtn = CCMenuItemSpriteExtra::create(
        restartBtnSpr, this, menu_selector(ModSettingsPopup::onRestart)
    );
    m_applyMenu->addChildAtPosition(m_restartBtn, Anchor::Bottom, ccp(0, 20));

    m_applyBtnSpr = createGeodeButton("Apply", true, GeodeButtonSprite::Default, m_forceDisableTheme);
    m_applyBtnSpr->setScale(.6f);
    m_applyBtn = CCMenuItemSpriteExtra::create(
        m_applyBtnSpr, this, menu_selector(ModSettingsPopup::onApply)
    );
    m_applyMenu->addChildAtPosition(m_applyBtn, Anchor::Bottom, ccp(0, 20));

    m_mainLayer->addChildAtPosition(m_applyMenu, Anchor::Bottom, ccp(0, 20));

    auto resetBtnSpr = createGeodeButton("Reset All", true, GeodeButtonSprite::Default, m_forceDisableTheme);
    resetBtnSpr->setScale(.6f);

    auto resetBtn = CCMenuItemSpriteExtra::create(
        resetBtnSpr, this, menu_selector(ModSettingsPopup::onResetAll)
    );
    m_buttonMenu->addChildAtPosition(resetBtn, Anchor::BottomLeft, ccp(45, 20));

    auto foldersMenu = CCMenu::create();
    foldersMenu->setContentSize({ 0, 30 });
    foldersMenu->setAnchorPoint(ccp(1, 0));
    foldersMenu->setLayout(RowLayout::create()
        ->setAxisReverse(true)
        ->setGap(5)
        ->setAutoGrowAxis(30)
        ->setGrowCrossAxis(true));
    m_buttonMenu->addChildAtPosition(foldersMenu, Anchor::BottomRight, ccp(-5, 10));

    auto createFolderButton = [&, this](auto subSprName, auto callback) {
        auto folderSpr = CCSprite::createWithSpriteFrameName("folderIcon_001.png");
        auto folderSprSub = CCSprite::createWithSpriteFrameName(subSprName);
        folderSprSub->setColor(ccBLACK);
        folderSprSub->setOpacity(155);
        folderSprSub->setScale(.55f);
        folderSpr->addChildAtPosition(folderSprSub, Anchor::Center, ccp(0, -3));
        auto buttonSpr = createGeodeButton(folderSpr, "", GeodeButtonSprite::Default, m_forceDisableTheme);
        buttonSpr->setScale(.6f);
        buttonSpr->getIcon()->setScale(buttonSpr->getIcon()->getScale() * 1.4f);
        auto folderBtn = CCMenuItemSpriteExtra::create(
            buttonSpr, this, static_cast<SEL_MenuHandler>(callback)
        );
        foldersMenu->addChild(folderBtn);
        return folderBtn;
    };

    createFolderButton("save.png"_spr, &ModSettingsPopup::onOpenSaveDirectory);
    createFolderButton("settings.png"_spr, &ModSettingsPopup::onOpenConfigDirectory);
    auto persistentBtn = createFolderButton("persistent.png"_spr, &ModSettingsPopup::onOpenPersistentDirectory);
    if (!std::filesystem::exists(mod->getPersistentDir(false))) {
        persistentBtn->setEnabled(false);
        auto spr = static_cast<CCSprite*>(persistentBtn->getNormalImage());
        spr->setCascadeColorEnabled(true);
        spr->setCascadeOpacityEnabled(true);
        spr->setColor(ccGRAY);
        spr->setOpacity(155);
    }

    foldersMenu->updateLayout();

    m_changeListener.bind([this](auto* ev) {
        this->updateState(ev->getNode());
        return ListenerResult::Propagate;
    });
    this->updateState();

    return true;
}

void ModSettingsPopup::onApply(CCObject*) {
    bool someChangesMade = false;
    for (auto& sett : m_settings) {
        if (sett->hasUncommittedChanges()) {
            sett->commit();
            someChangesMade = true;
        }
    }
    if (!someChangesMade) {
        FLAlertLayer::create("Info", "No changes have been made.", "OK")->show();
    }
}
void ModSettingsPopup::onRestart(CCObject*) {
    // Update button state to let user know it's restarting but it might take a bit
    m_restartBtn->setEnabled(false);
    static_cast<ButtonSprite*>(m_restartBtn->getNormalImage())->setString("Restarting...");
    m_restartBtn->updateSprite();

    // Actually restart
    Loader::get()->queueInMainThread([] {
        // Delayed by 2 frames - one is needed to render the "Restarting text"
        Loader::get()->queueInMainThread([] {
            // the other never finishes rendering because the game actually restarts at this point
            game::restart(true);
        });
    });
}
void ModSettingsPopup::onResetAll(CCObject*) {
    createQuickPopup(
        "Reset All",
        "Are you sure you want to <cr>reset</c> ALL settings "
        "to <cy>default</c>?",
        "Cancel", "Reset",
        [this](auto, bool btn2) {
            if (btn2) {
                for (auto& sett : m_settings) {
                    sett->resetToDefault();
                }
            }
        }
    );
}
void ModSettingsPopup::onOpenSaveDirectory(CCObject*) {
    file::openFolder(m_mod->getSaveDir());
}
void ModSettingsPopup::onOpenConfigDirectory(CCObject*) {
    file::openFolder(m_mod->getConfigDir());
}
void ModSettingsPopup::onOpenPersistentDirectory(CCObject*) {
    file::openFolder(m_mod->getPersistentDir());
}
void ModSettingsPopup::onClearSearch(CCObject*) {
    m_searchInput->setString("");
    this->updateState();
    m_list->moveToTop();
}

void ModSettingsPopup::updateState(SettingNode* invoker) {
    auto search = m_searchInput->getString();
    auto hasSearch = !search.empty();

    m_restartBtn->setVisible(ModSettingsManager::from(m_mod)->restartRequired());
    m_applyMenu->updateLayout();

    auto listPosBefore = m_list->m_contentLayer->getPositionY();
    auto listHeightBefore = m_list->m_contentLayer->getContentHeight();

    // Update search visibility + all settings with "enable-if" schemes +
    // checkerboard BG
    TitleSettingNode* lastTitle = nullptr;
    bool bg = false;
    for (auto& sett : m_settings) {
        if (auto asTitle = typeinfo_cast<TitleSettingNode*>(sett.data())) {
            lastTitle = asTitle;
        }
        sett->removeFromParent();
        if (
            // Show if the setting is not a title and is not subject to a collapsed title
            !(lastTitle && lastTitle != sett && lastTitle->isCollapsed()) &&
            // Show if there's no search query or if the setting matches it
            (!hasSearch || matchSearch(sett, search))
        ) {
            m_list->m_contentLayer->addChild(sett);
            sett->setDefaultBGColor(ccc4(0, 0, 0, bg ? 60 : 20));
            bg = !bg;
        }
        // Avoid infinite loops
        if (sett == invoker) {
            continue;
        }
        if (sett->getSetting() && sett->getSetting()->getEnableIf()) {
            sett->updateState(nullptr);
        }
    }
    m_list->m_contentLayer->updateLayout();

    // Preserve relative list position if something has been collapsed
    m_list->m_contentLayer->setPositionY(
        listPosBefore +
            (listHeightBefore - m_list->m_contentLayer->getContentHeight())
    );

    m_applyBtnSpr->setCascadeColorEnabled(true);
    m_applyBtnSpr->setCascadeOpacityEnabled(true);
    if (this->hasUncommitted()) {
        m_applyBtnSpr->setColor(ccWHITE);
        m_applyBtnSpr->setOpacity(255);
        m_applyBtn->setEnabled(true);
    }
    else {
        m_applyBtnSpr->setColor(ccGRAY);
        m_applyBtnSpr->setOpacity(155);
        m_applyBtn->setEnabled(false);
    }

    auto clearSpr = static_cast<GeodeSquareSprite*>(m_searchClearBtn->getNormalImage());
    m_searchClearBtn->setEnabled(hasSearch);
    clearSpr->setColor(hasSearch ? ccWHITE : ccGRAY);
    clearSpr->setOpacity(hasSearch ? 255 : 90);
    clearSpr->getTopSprite()->setColor(hasSearch ? ccWHITE : ccGRAY);
    clearSpr->getTopSprite()->setOpacity(hasSearch ? 255 : 90);
}

bool ModSettingsPopup::hasUncommitted() const {
    for (auto& sett : m_settings) {
        if (sett->hasUncommittedChanges()) {
            return true;
        }
    }
    return false;
}

void ModSettingsPopup::onClose(CCObject* sender) {
    if (this->hasUncommitted()) {
        createQuickPopup(
            "Unsaved Changes",
            "You have <cr>unsaved changes</c>! Are you sure you "
            "want to exit?",
            "Cancel", "Discard",
            [this](FLAlertLayer*, bool btn2) {
                if (btn2) {
                    GeodePopup::onClose(nullptr);
                }
            }
        );
        return;
    }
    GeodePopup::onClose(sender);
}

ModSettingsPopup* ModSettingsPopup::create(Mod* mod, bool forceDisableTheme) {
    auto ret = new ModSettingsPopup();
    if (ret->init(440, 280, mod, GeodePopupStyle::Default, forceDisableTheme)) {
        ret->autorelease();
        return ret;
    }
    delete ret;
    return nullptr;
}
