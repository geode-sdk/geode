#include <Geode/ui/MenuItemSpriteExtra.hpp>

using namespace geode::prelude;

MenuItemSpriteExtra* MenuItemSpriteExtra::create(
    cocos2d::CCNode* normalSprite,
    cocos2d::CCObject* target, cocos2d::SEL_MenuHandler callback
) {
    return MenuItemSpriteExtra::create(normalSprite, nullptr, target, callback);
}

MenuItemSpriteExtra* MenuItemSpriteExtra::create(
    cocos2d::CCNode* normalSprite, cocos2d::CCNode* disabledSprite,
    cocos2d::CCObject* target, cocos2d::SEL_MenuHandler callback
) {
    auto ret = new MenuItemSpriteExtra();
    if (ret && ret->init(normalSprite, disabledSprite, target, callback)) {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

MenuItemSpriteExtra::MenuItemSpriteExtra() : m_scaleSprites(false) {}

MenuItemSpriteExtra::~MenuItemSpriteExtra() {
}

void MenuItemSpriteExtra::setImage(cocos2d::CCNode* image) {
    if (image) {
        image->setAnchorPoint({ 0.5f, 0.5f });
        image->setPosition(image->getContentSize() / 2);
    }
}

// why are the members private like what
void MenuItemSpriteExtra::setNormalImage(cocos2d::CCNode* image) {
    CCMenuItemSprite::setNormalImage(image);
    this->setImage(image);
}
void MenuItemSpriteExtra::setDisabledImage(cocos2d::CCNode* image) {
    CCMenuItemSprite::setDisabledImage(image);
    this->setImage(image);
}
void MenuItemSpriteExtra::setSelectedImage(cocos2d::CCNode* image) {
    CCMenuItemSprite::setSelectedImage(image);
    this->setImage(image);
}

bool MenuItemSpriteExtra::init(
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

void MenuItemSpriteExtra::runActionOnSprite(cocos2d::CCNode* sprite, cocos2d::CCAction* generator) {
    if (sprite) {
        sprite->stopAllActions();
        if (generator) sprite->runAction(generator);
    }
}

void MenuItemSpriteExtra::runActionOnSprites(MiniFunction<cocos2d::CCAction*()> generator) {
    if (m_scaleSprites) {
        this->runActionOnSprite(this->getNormalImage(), generator());
        this->runActionOnSprite(this->getDisabledImage(), generator());
        this->runActionOnSprite(this->getSelectedImage(), generator());
    }
    else {
        if (auto action = generator()) this->runAction(action);
    }
}

void MenuItemSpriteExtra::activate() {
    this->runActionOnSprites(m_activateAction);
    CCMenuItem::activate();
}
void MenuItemSpriteExtra::selected() {
    if (!m_bEnabled) return;
    CCMenuItem::selected();

    this->runActionOnSprites(m_selectedAction);
}
void MenuItemSpriteExtra::unselected() {
    if (!m_bEnabled) return;
    CCMenuItem::unselected();

    this->runActionOnSprites(m_unselectedAction);
}

void MenuItemSpriteExtra::setScaleSprites(bool scale) {
    m_scaleSprites = scale;
}