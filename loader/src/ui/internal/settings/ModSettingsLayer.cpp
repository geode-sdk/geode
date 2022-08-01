/*#include "ModSettingsLayer.hpp"
#include "../settings/ModSettingsList.hpp"

bool ModSettingsLayer::init(Mod* mod) {
    m_noElasticity = true;
    m_mod = mod;

    auto winSize = CCDirector::sharedDirector()->getWinSize();
	const CCSize size { 440.f, 290.f };

    if (!this->initWithColor({ 0, 0, 0, 105 })) return false;
    m_mainLayer = CCLayer::create();
    this->addChild(m_mainLayer);

    auto bg = CCScale9Sprite::create("GJ_square01.png", { 0.0f, 0.0f, 80.0f, 80.0f });
    bg->setContentSize(size);
    bg->setPosition(winSize.width / 2, winSize.height / 2);
    m_mainLayer->addChild(bg);

    m_buttonMenu = CCMenu::create();
    m_buttonMenu->setZOrder(10);
    m_mainLayer->addChild(m_buttonMenu);

    CCDirector::sharedDirector()->getTouchDispatcher()->incrementForcePrio(2);
    this->registerWithTouchDispatcher();

	auto nameStr = m_mod->getName() + " Settings";
    auto nameLabel = CCLabelBMFont::create(
        nameStr.c_str(), "bigFont.fnt"
    );
    nameLabel->setPosition(winSize.width / 2, winSize.height / 2 + 120.f);
    nameLabel->setScale(.7f);
    m_mainLayer->addChild(nameLabel, 2); 

	const CCSize listSize { 350.f, 200.f };

    auto bgSprite = CCScale9Sprite::create(
        "square02b_001.png", { 0.0f, 0.0f, 80.0f, 80.0f }
    );
    bgSprite->setScale(.5f);
    bgSprite->setColor({ 0, 0, 0 });
    bgSprite->setOpacity(75);
    bgSprite->setContentSize(listSize * 2);
    bgSprite->setPosition(winSize.width / 2, winSize.height / 2);
    m_mainLayer->addChild(bgSprite);

	m_list = ModSettingsList::create(mod, this, listSize.width, listSize.height);
	m_list->setPosition(winSize.width / 2 - listSize.width / 2, winSize.height / 2 - listSize.height / 2);
	m_mainLayer->addChild(m_list);

    {
        auto topSprite = CCSprite::createWithSpriteFrameName("GJ_commentTop_001.png");
        topSprite->setPosition({ winSize.width / 2, winSize.height / 2 + listSize.height / 2 - 5.f });
        m_mainLayer->addChild(topSprite);

        auto bottomSprite = CCSprite::createWithSpriteFrameName("GJ_commentTop_001.png");
        bottomSprite->setFlipY(true);
        bottomSprite->setPosition({ winSize.width / 2, winSize.height / 2 - listSize.height / 2 + 5.f });
        m_mainLayer->addChild(bottomSprite);

        auto leftSprite = CCSprite::createWithSpriteFrameName("GJ_commentSide_001.png");
        leftSprite->setPosition({ winSize.width / 2 - listSize.width / 2 + 1.5f, winSize.height / 2 });
        leftSprite->setScaleY(5.7f);
        m_mainLayer->addChild(leftSprite);

        auto rightSprite = CCSprite::createWithSpriteFrameName("GJ_commentSide_001.png");
        rightSprite->setFlipX(true);
        rightSprite->setPosition({ winSize.width / 2 + listSize.width / 2 - 1.5f, winSize.height / 2 });
        rightSprite->setScaleY(5.7f);
        m_mainLayer->addChild(rightSprite);
    }

    m_applyBtnSpr = ButtonSprite::create("Apply", "goldFont.fnt", "GJ_button_01.png", .8f);
    m_applyBtnSpr->setScale(.8f);

    auto applyBtn = CCMenuItemSpriteExtra::create(
        m_applyBtnSpr,
        this,
        menu_selector(ModSettingsLayer::onApply)
    );
    applyBtn->setPosition(size.width / 2 - 80.f, -size.height / 2 + 25.f);
    m_buttonMenu->addChild(applyBtn);


    auto resetBtnSpr = ButtonSprite::create("Reset to\nDefault", "bigFont.fnt", "GJ_button_05.png", .8f);
    resetBtnSpr->setScale(.4f);

    auto resetBtn = CCMenuItemSpriteExtra::create(
        resetBtnSpr,
        this,
        menu_selector(ModSettingsLayer::onResetAllToDefault)
    );
    resetBtn->setPosition(size.width / 2 - 150.f, -size.height / 2 + 25.f);
    m_buttonMenu->addChild(resetBtn);


    auto closeSpr = CCSprite::createWithSpriteFrameName("GJ_closeBtn_001.png");
    closeSpr->setScale(.8f);

    auto closeBtn = CCMenuItemSpriteExtra::create(
        closeSpr,
        this,
        menu_selector(ModSettingsLayer::onClose)
    );
    closeBtn->setPosition(-size.width / 2 + 3.f, size.height / 2 - 3.f);
    m_buttonMenu->addChild(closeBtn);

    this->setKeypadEnabled(true);
    this->setTouchEnabled(true);

    this->updateState();

    return true;
}

void ModSettingsLayer::updateState() {
    if (m_list->hasUnsavedModifiedSettings()) {
        m_applyBtnSpr->setOpacity(255);
        m_applyBtnSpr->setColor({ 255, 255, 255 });
    } else {
        m_applyBtnSpr->setOpacity(155);
        m_applyBtnSpr->setColor({ 155, 155, 155 });
    }
}

void ModSettingsLayer::FLAlert_Clicked(FLAlertLayer* layer, bool btn2) {
    if (btn2) {
        switch (layer->getTag()) {
            case 0: this->close(); break;
            case 1: m_list->resetAllToDefault(); break;
        }
    }
}

void ModSettingsLayer::onResetAllToDefault(CCObject*) {
    auto layer = FLAlertLayer::create(
        this, "Reset Settings",
        "Are you sure you want to <co>reset</c> ALL settings?",
        "Cancel", "Reset", 400.f
    );
    layer->setTag(1);
    layer->show();
}

void ModSettingsLayer::onApply(CCObject*) {
    if (!m_list->hasUnsavedModifiedSettings()) {
        FLAlertLayer::create(
            "No Changes",
            "No changes were made.",
            "OK"
        )->show();
    }
    m_list->applyChanges();
    this->updateState();
}

void ModSettingsLayer::keyDown(enumKeyCodes key) {
    if (key == KEY_Escape)
        return this->onClose(nullptr);
    if (key == KEY_Space)
        return;
    return FLAlertLayer::keyDown(key);
}

void ModSettingsLayer::onClose(CCObject*) {
    if (m_list->hasUnsavedModifiedSettings()) {
        auto layer = FLAlertLayer::create(
            this, "Unsaved Changes",
            "You have <cy>unsaved</c> settings! Are you sure "
            "you want to <cr>discard</c> your changes?",
            "Cancel", "Discard", 400.f
        );
        layer->setTag(0);
        layer->show();
    } else {
        this->close();
    }
};

void ModSettingsLayer::close() {
    this->setKeyboardEnabled(false);
    this->removeFromParentAndCleanup(true);
}

ModSettingsLayer* ModSettingsLayer::create(Mod* mod) {
    auto ret = new ModSettingsLayer();
    if (ret && ret->init(mod)) {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}*/

