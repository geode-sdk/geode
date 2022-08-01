#include "SearchFilterPopup.hpp"
#include "ModListLayer.hpp"
#include "ModListView.hpp"

bool SearchFilterPopup::init(ModListLayer* layer) {
    this->m_noElasticity = true;

    this->m_modLayer = layer;

    auto winSize = CCDirector::sharedDirector()->getWinSize();
	CCSize size { 280.f, 250.f };

    if (!this->initWithColor({ 0, 0, 0, 105 })) return false;
    this->m_mainLayer = CCLayer::create();
    this->addChild(this->m_mainLayer);

    auto bg = CCScale9Sprite::create("GJ_square05.png", { 0.0f, 0.0f, 80.0f, 80.0f });
    bg->setContentSize(size);
    bg->setPosition(winSize.width / 2, winSize.height / 2);
    this->m_mainLayer->addChild(bg);

    this->m_buttonMenu = CCMenu::create();
    this->m_mainLayer->addChild(this->m_buttonMenu);

    auto nameLabel = CCLabelBMFont::create("Search Filters", "goldFont.fnt");
    nameLabel->setPosition(winSize.width / 2, winSize.height / 2 + 100.f);
    nameLabel->setScale(.7f);
    this->m_mainLayer->addChild(nameLabel, 2);

    this->m_pos = CCPoint { winSize.width / 2 - 45.f, 86.f };

    this->addToggle("Name",        ModListView::SearchFlags::Name);
    this->addToggle("ID",          ModListView::SearchFlags::ID);
    this->addToggle("Credits",     ModListView::SearchFlags::Credits);
    this->addToggle("Description", ModListView::SearchFlags::Description);
    this->addToggle("Details",     ModListView::SearchFlags::Details);
    this->addToggle("Developer",   ModListView::SearchFlags::Developer);

    CCDirector::sharedDirector()->getTouchDispatcher()->incrementForcePrio(2);
    this->registerWithTouchDispatcher();
    
    auto closeSpr = CCSprite::createWithSpriteFrameName("GJ_closeBtn_001.png");
    closeSpr->setScale(1.0f);

    auto closeBtn = CCMenuItemSpriteExtra::create(
        closeSpr,
        this,
        (SEL_MenuHandler)&SearchFilterPopup::onClose
    );
    closeBtn->setUserData(reinterpret_cast<void*>(this));

    this->m_buttonMenu->addChild(closeBtn);

    closeBtn->setPosition( - size.width / 2, size.height / 2 );

    this->setKeypadEnabled(true);
    this->setTouchEnabled(true);

    return true;
}

void SearchFilterPopup::addToggle(const char* title, int flag) {
    GameToolbox::createToggleButton(
        title, menu_selector(SearchFilterPopup::onToggle),
        this->m_modLayer->m_searchFlags & flag,
        this->m_buttonMenu, this->m_pos, this,
        this->m_buttonMenu, .5f, .5f, 100.f, 
        { 10.f, .0f }, nullptr, false, flag, nullptr
    )->setTag(flag);
    this->m_pos.y += 25.f;
}

void SearchFilterPopup::keyDown(cocos2d::enumKeyCodes key) {
    if (key == KEY_Escape)
        return onClose(nullptr);
    if (key == KEY_Space)
        return;
    return FLAlertLayer::keyDown(key);
}

void SearchFilterPopup::onClose(cocos2d::CCObject*) {
    this->setKeyboardEnabled(false);
    this->removeFromParentAndCleanup(true);
}

void SearchFilterPopup::onToggle(cocos2d::CCObject* pSender) {
    if (as<CCMenuItemToggler*>(pSender)->isToggled()) {
        this->m_modLayer->m_searchFlags &= ~pSender->getTag();
    } else {
        this->m_modLayer->m_searchFlags |= pSender->getTag();
    }
}

SearchFilterPopup* SearchFilterPopup::create(ModListLayer* layer) {
    auto ret = new SearchFilterPopup();
    if (ret && ret->init(layer)) {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}
