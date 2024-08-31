#include "ModSettingsPopup.hpp"
#include <Geode/binding/ButtonSprite.hpp>
#include <Geode/loader/Mod.hpp>
#include <Geode/loader/Setting.hpp>
#include <Geode/loader/ModSettingsManager.hpp>
#include <Geode/ui/ScrollLayer.hpp>
#include <Geode/utils/cocos.hpp>
#include <Geode/ui/General.hpp>

bool ModSettingsPopup::setup(Mod* mod) {
    m_noElasticity = true;
    m_mod = mod;

    this->setTitle(("Settings for " + mod->getName()).c_str());

    auto winSize = CCDirector::sharedDirector()->getWinSize();

    auto const layerSize = CCSize(346, 200);

    auto layerBG = CCLayerColor::create({ 0, 0, 0, 75 });
    layerBG->setContentSize(layerSize);
    m_mainLayer->addChildAtPosition(layerBG, Anchor::Center, -layerSize / 2);

    auto layer = ScrollLayer::create(layerSize);
    layer->setTouchEnabled(true);

    bool bg = false;
    for (auto& key : mod->getSettingKeys()) {
        bg = !bg;

        SettingNodeV3* node;
        if (auto sett = mod->getSettingV3(key)) {
            node = sett->createNode(layerSize.width);
        }
        else {
            // todo: placeholder node
            continue;
        }
        node->setBGColor(ccc4(0, 0, 0, bg ? 60 : 20));
    
        // auto separator = CCLayerColor::create({ 0, 0, 0, 50 }, layerSize.width, 1.f);
        // separator->setOpacity(bg ? 100 : 50);
        // separator->ignoreAnchorPointForPosition(false);
        // bg->addChildAtPosition(separator, Anchor::Bottom, ccp(0, 0), ccp(.5f, .5f));

        m_settings.push_back(node);
        layer->m_contentLayer->addChild(node);
    }
    layer->m_contentLayer->setLayout(
        ColumnLayout::create()
            ->setAxisReverse(true)
            ->setAutoGrowAxis(layerSize.height)
            ->setCrossAxisOverflow(false)
            ->setAxisAlignment(AxisAlignment::End)
            ->setGap(0)
    );
    layer->moveToTop();

    layerBG->addChild(layer);

    // layer borders

    m_mainLayer->addChildAtPosition(createGeodeListBorders({layerSize.width, layerSize.height - 2}), Anchor::Center);

    // buttons

    m_applyMenu = CCMenu::create();
    m_applyMenu->setContentWidth(150);
    m_applyMenu->setLayout(RowLayout::create());
    m_applyMenu->getLayout()->ignoreInvisibleChildren(true);

    auto restartBtnSpr = createGeodeButton("Restart Now", true);
    restartBtnSpr->setScale(.6f);
    m_restartBtn = CCMenuItemSpriteExtra::create(
        restartBtnSpr, this, menu_selector(ModSettingsPopup::onRestart)
    );
    m_applyMenu->addChildAtPosition(m_restartBtn, Anchor::Bottom, ccp(0, 20));

    m_applyBtnSpr = createGeodeButton("Apply", true);
    m_applyBtnSpr->setScale(.6f);
    m_applyBtn = CCMenuItemSpriteExtra::create(
        m_applyBtnSpr, this, menu_selector(ModSettingsPopup::onApply)
    );
    m_applyMenu->addChildAtPosition(m_applyBtn, Anchor::Bottom, ccp(0, 20));

    m_mainLayer->addChildAtPosition(m_applyMenu, Anchor::Bottom, ccp(0, 20));

    auto resetBtnSpr = createGeodeButton("Reset All", true);
    resetBtnSpr->setScale(.6f);

    auto resetBtn = CCMenuItemSpriteExtra::create(
        resetBtnSpr, this, menu_selector(ModSettingsPopup::onResetAll)
    );
    m_buttonMenu->addChildAtPosition(resetBtn, Anchor::BottomLeft, ccp(45, 20));

    auto configFolderSpr = CCSprite::createWithSpriteFrameName("folderIcon_001.png");
    m_openConfigDirBtnSpr = createGeodeButton(configFolderSpr, "");
    m_openConfigDirBtnSpr->setScale(.6f);
    m_openConfigDirBtnSpr->getIcon()->setScale(m_openConfigDirBtnSpr->getIcon()->getScale() * 1.4f);
    auto openConfigDirBtn = CCMenuItemSpriteExtra::create(
        m_openConfigDirBtnSpr, this, menu_selector(ModSettingsPopup::onOpenConfigDirectory)
    );
    m_buttonMenu->addChildAtPosition(openConfigDirBtn, Anchor::BottomRight, ccp(-50, 20));

    auto settingFolderSpr = CCSprite::createWithSpriteFrameName("folderIcon_001.png");
    auto settingFolderSprSub = CCSprite::createWithSpriteFrameName("settings.png"_spr);
    settingFolderSprSub->setColor(ccBLACK);
    settingFolderSprSub->setOpacity(155);
    settingFolderSprSub->setScale(.55f);
    settingFolderSpr->addChildAtPosition(settingFolderSprSub, Anchor::Center, ccp(0, -3));
    auto openDirBtnSpr = createGeodeButton(settingFolderSpr, "");
    openDirBtnSpr->setScale(.6f);
    openDirBtnSpr->getIcon()->setScale(openDirBtnSpr->getIcon()->getScale() * 1.4f);
    auto openDirBtn = CCMenuItemSpriteExtra::create(
        openDirBtnSpr, this, menu_selector(ModSettingsPopup::onOpenSaveDirectory)
    );
    m_buttonMenu->addChildAtPosition(openDirBtn, Anchor::BottomRight, ccp(-20, 20));

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
            game::restart();
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
    this->updateState();
}

void ModSettingsPopup::updateState(SettingNodeV3* invoker) {
    m_restartBtn->setVisible(ModSettingsManager::from(m_mod)->restartRequired());
    m_applyMenu->updateLayout();

    auto configDirExists = std::filesystem::exists(m_mod->getConfigDir(false));
    m_openConfigDirBtnSpr->setCascadeColorEnabled(true);
    m_openConfigDirBtnSpr->setCascadeOpacityEnabled(true);
    m_openConfigDirBtnSpr->setColor(configDirExists ? ccWHITE : ccGRAY);
    m_openConfigDirBtnSpr->setOpacity(configDirExists ? 255 : 155);

    // Update all settings with "enable-if" schemes
    for (auto& sett : m_settings) {
        // Avoid infinite loops
        if (sett == invoker) {
            continue;
        }
        if (sett->getSetting()->getEnableIf()) {
            sett->updateState();
        }
    }

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
    if (sender && this->hasUncommitted()) {
        createQuickPopup(
            "Unsaved Changes",
            "You have <cr>unsaved changes</c>! Are you sure you "
            "want to exit?",
            "Cancel", "Discard",
            [this](FLAlertLayer*, bool btn2) {
                if (btn2) this->onClose(nullptr);
            }
        );
        return;
    }
    Popup<Mod*>::onClose(sender);
}

ModSettingsPopup* ModSettingsPopup::create(Mod* mod) {
    auto ret = new ModSettingsPopup();
    if (ret->init(440, 280, mod)) {
        ret->autorelease();
        return ret;
    }
    delete ret;
    return nullptr;
}
