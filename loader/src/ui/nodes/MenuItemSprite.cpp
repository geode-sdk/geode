#include <Geode/ui/MenuItemSprite.hpp>

using namespace geode::prelude;

MenuItemSprite* MenuItemSprite::create(
    cocos2d::CCNode* normalSprite,
    cocos2d::CCObject* target, cocos2d::SEL_MenuHandler callback
) {
    return MenuItemSprite::create(normalSprite, nullptr, target, callback);
}

MenuItemSprite* MenuItemSprite::create(
    cocos2d::CCNode* normalSprite, cocos2d::CCNode* disabledSprite,
    cocos2d::CCObject* target, cocos2d::SEL_MenuHandler callback
) {
    auto ret = new MenuItemSprite();
    if (ret && ret->init(normalSprite, disabledSprite, target, callback)) {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

MenuItemSprite::MenuItemSprite() {}

MenuItemSprite::~MenuItemSprite() {
}

void MenuItemSprite::setImage(cocos2d::CCNode* image) {
    if (image) {
        image->setAnchorPoint({ 0.5f, 0.5f });
        image->setPosition(image->getContentSize() / 2);
    }
}

// why are the members private like what
void MenuItemSprite::setNormalImage(cocos2d::CCNode* image) {
    CCMenuItemSprite::setNormalImage(image);
    this->setImage(image);
}
void MenuItemSprite::setDisabledImage(cocos2d::CCNode* image) {
    CCMenuItemSprite::setDisabledImage(image);
    this->setImage(image);
}
void MenuItemSprite::setSelectedImage(cocos2d::CCNode* image) {
    CCMenuItemSprite::setSelectedImage(image);
    this->setImage(image);
}

bool MenuItemSprite::init(
    cocos2d::CCNode* normalSprite, cocos2d::CCNode* disabledSprite,
    cocos2d::CCObject* target, cocos2d::SEL_MenuHandler callback
) {
    if (!CCMenuItemSprite::initWithNormalSprite(normalSprite, disabledSprite, nullptr, target, callback)) return false;

    m_selectedAction = []() -> cocos2d::CCAction* {
        return cocos2d::CCEaseBounceOut::create(cocos2d::CCScaleTo::create(0.3f, 1.26f));
    };
    m_unselectedAction = []() -> cocos2d::CCAction* {
        return cocos2d::CCEaseBounceOut::create(cocos2d::CCScaleTo::create(0.4f, 1.0f));
    };
    m_activateAction = []() -> cocos2d::CCAction* {
        return cocos2d::CCScaleTo::create(0.f, 1.0f);
    };

    return true;
}

void MenuItemSprite::runActionOnSprite(cocos2d::CCNode* sprite, cocos2d::CCAction* generator) {
    if (sprite) {
        sprite->stopAllActions();
        if (generator) sprite->runAction(generator);
    }
}

void MenuItemSprite::runActionOnSprites(MiniFunction<cocos2d::CCAction*()> generator) {
    this->runActionOnSprite(this->getNormalImage(), generator());
    this->runActionOnSprite(this->getDisabledImage(), generator());
    this->runActionOnSprite(this->getSelectedImage(), generator());
}

void MenuItemSprite::activate() {
    this->runActionOnSprites(m_activateAction);
    CCMenuItem::activate();
}
void MenuItemSprite::selected() {
    if (!m_bEnabled) return;
    CCMenuItem::selected();

    this->runActionOnSprites(m_selectedAction);
}
void MenuItemSprite::unselected() {
    if (!m_bEnabled) return;
    CCMenuItem::unselected();

    this->runActionOnSprites(m_unselectedAction);
}