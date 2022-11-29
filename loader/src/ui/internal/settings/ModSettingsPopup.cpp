#include "ModSettingsPopup.hpp"

#include <Geode/binding/ButtonSprite.hpp>
#include <Geode/loader/Mod.hpp>
#include <Geode/loader/Setting.hpp>
#include <Geode/ui/ScrollLayer.hpp>
#include <Geode/utils/cocos.hpp>

bool ModSettingsPopup::setup(Mod* mod) {
    m_noElasticity = true;
    m_mod = mod;

    this->setTitle(("Settings for " + mod->getName()).c_str());

    auto winSize = CCDirector::sharedDirector()->getWinSize();

    auto const layerSize = CCSize { 346.f, 220.f };

    auto layerBG = CCLayerColor::create({ 0, 0, 0, 75 });
    layerBG->setContentSize(layerSize);
    layerBG->setPosition(winSize / 2 - layerSize / 2);
    m_mainLayer->addChild(layerBG);

    auto layer = ScrollLayer::create(layerSize);
    layer->setPosition(winSize / 2 - layerSize / 2);

    CCDirector::sharedDirector()->getTouchDispatcher()->incrementForcePrio(2);
    layer->registerWithTouchDispatcher();

    float totalHeight = .0f;
    std::vector<CCNode*> rendered;
    bool hasBG = true;
    for (auto& [_, sett] : mod->getSettings()) {
        auto node = sett->createNode(layerSize.width);
        node->setDelegate(this);

        totalHeight += node->getScaledContentSize().height;

        if (hasBG) {
            auto bg = CCLayerColor::create({ 0, 0, 0, 50 });
            bg->setContentSize(node->getScaledContentSize());
            bg->setPosition(0.f, -totalHeight);
            bg->setZOrder(-10);
            layer->m_contentLayer->addChild(bg);

            rendered.push_back(bg);

            hasBG = false;
        }
        else {
            hasBG = true;
        }

        node->setPosition(0.f, -totalHeight);
        layer->m_contentLayer->addChild(node);

        auto separator = CCLayerColor::create(
            { 0, 0, 0, static_cast<GLubyte>(hasBG ? 100 : 50) }, layerSize.width, 1.f
        );
        separator->setPosition(0.f, -totalHeight);
        layer->m_contentLayer->addChild(separator);
        rendered.push_back(separator);

        rendered.push_back(node);
        m_settings.push_back(node);
    }
    if (totalHeight < layerSize.height) {
        totalHeight = layerSize.height;
    }
    for (auto& node : rendered) {
        node->setPositionY(node->getPositionY() + totalHeight);
    }
    layer->m_contentLayer->setContentSize({ layerSize.width, totalHeight });
    layer->moveToTop();

    m_mainLayer->addChild(layer);

    // layer borders

    auto layerTopSpr = CCSprite::createWithSpriteFrameName("GJ_commentTop_001.png");
    layerTopSpr->setPosition({ winSize.width / 2, winSize.height / 2 + layerSize.height / 2 - 5.f }
    );
    m_mainLayer->addChild(layerTopSpr);

    auto layerBottomSpr = CCSprite::createWithSpriteFrameName("GJ_commentTop_001.png");
    layerBottomSpr->setFlipY(true);
    layerBottomSpr->setPosition({ winSize.width / 2,
                                  winSize.height / 2 - layerSize.height / 2 + 5.f });
    m_mainLayer->addChild(layerBottomSpr);

    auto layerLeftSpr = CCSprite::createWithSpriteFrameName("GJ_commentSide_001.png");
    layerLeftSpr->setScaleY(6.3f);
    layerLeftSpr->setPosition({ winSize.width / 2 - layerSize.width / 2 - .5f, winSize.height / 2 }
    );
    m_mainLayer->addChild(layerLeftSpr);

    auto layerRightSpr = CCSprite::createWithSpriteFrameName("GJ_commentSide_001.png");
    layerRightSpr->setScaleY(6.3f);
    layerRightSpr->setFlipX(true);
    layerRightSpr->setPosition({ winSize.width / 2 + layerSize.width / 2 + .5f, winSize.height / 2 }
    );
    m_mainLayer->addChild(layerRightSpr);

    // buttons

    m_applyBtnSpr = ButtonSprite::create("Apply", "goldFont.fnt", "GJ_button_01.png", .7f);
    m_applyBtnSpr->setScale(.7f);

    m_applyBtn = CCMenuItemSpriteExtra::create(
        m_applyBtnSpr, this, menu_selector(ModSettingsPopup::onApply)
    );
    m_applyBtn->setPosition(.0f, -m_size.height / 2 + 20.f);
    m_buttonMenu->addChild(m_applyBtn);

    auto resetBtnSpr = ButtonSprite::create("Reset All", "goldFont.fnt", "GJ_button_05.png", .7f);
    resetBtnSpr->setScale(.7f);

    auto resetBtn = CCMenuItemSpriteExtra::create(
        resetBtnSpr, this, menu_selector(ModSettingsPopup::onResetAll)
    );
    resetBtn->setPosition(-m_size.width / 2 + 45.f, -m_size.height / 2 + 20.f);
    m_buttonMenu->addChild(resetBtn);

    auto openDirBtnSpr =
        ButtonSprite::create("Open Folder", "goldFont.fnt", "GJ_button_05.png", .7f);
    openDirBtnSpr->setScale(.7f);

    auto openDirBtn = CCMenuItemSpriteExtra::create(
        openDirBtnSpr, this, menu_selector(ModSettingsPopup::onOpenSaveDirectory)
    );
    openDirBtn->setPosition(m_size.width / 2 - 53.f, -m_size.height / 2 + 20.f);
    m_buttonMenu->addChild(openDirBtn);

    this->settingValueChanged(nullptr);

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

void ModSettingsPopup::settingValueChanged(SettingNode*) {
    if (this->hasUncommitted()) {
        m_applyBtnSpr->setColor(cc3x(0xf));
        m_applyBtn->setEnabled(true);
    }
    else {
        m_applyBtnSpr->setColor(cc3x(0x4));
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

void ModSettingsPopup::onOpenSaveDirectory(CCObject*) {
    file::openFolder(m_mod->getSaveDir());
}

ModSettingsPopup* ModSettingsPopup::create(Mod* mod) {
    auto ret = new ModSettingsPopup();
    if (ret && ret->init(440.f, 290.f, mod)) {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}
