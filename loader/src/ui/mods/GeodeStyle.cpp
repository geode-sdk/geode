#include "GeodeStyle.hpp"
#include <Geode/utils/cocos.hpp>

bool GeodeSquareSprite::init(CCSprite* top, bool* state) {
    if (!CCSprite::initWithFile("GE_button_05.png"_spr))
        return false;
    
    m_stateSrc = state;

    limitNodeSize(top, m_obContentSize * .65f, 2.f, .1f);
    this->addChildAtPosition(top, Anchor::Center);

    // Only schedule update if there is a need to do so
    if (state) {
        this->scheduleUpdate();
    }

    return true;
}

void GeodeSquareSprite::update(float dt) {
    CCSprite::update(dt);
    if (m_stateSrc && m_state != *m_stateSrc) {
        m_state = *m_stateSrc;
        this->setTexture(CCTextureCache::get()->addImage(
            (m_state ? "GJ_button_02.png" : "GE_button_05.png"_spr), true
        ));
    }
}

GeodeSquareSprite* GeodeSquareSprite::create(const char* top, bool* state) {
    auto ret = new GeodeSquareSprite();
    if (ret && ret->init(CCSprite::create(top), state)) {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

GeodeSquareSprite* GeodeSquareSprite::createWithSpriteFrameName(const char* top, bool* state) {
    auto ret = new GeodeSquareSprite();
    if (ret && ret->init(CCSprite::createWithSpriteFrameName(top), state)) {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

ButtonSprite* createGeodeButton(std::string const& text, std::string const& bg) {
    return ButtonSprite::create(text.c_str(), "bigFont.fnt", bg.c_str(), .8f);
}

bool GeodeTabSprite::init(const char* iconFrame, const char* text, float width) {
    if (!CCNode::init())
        return false;
    
    const CCSize itemSize { width, 35 };
    const CCSize iconSize { 18, 18 };

    this->setContentSize(itemSize);
    this->setAnchorPoint({ .5f, .5f });

    m_deselectedBG = CCScale9Sprite::createWithSpriteFrameName("tab-bg.png"_spr);
    m_deselectedBG->setScale(.8f);
    m_deselectedBG->setContentSize(itemSize / .8f);
    m_deselectedBG->setColor({ 26, 24, 29 });
    this->addChildAtPosition(m_deselectedBG, Anchor::Center);

    m_selectedBG = CCScale9Sprite::createWithSpriteFrameName("tab-bg.png"_spr);
    m_selectedBG->setScale(.8f);
    m_selectedBG->setContentSize(itemSize / .8f);
    m_selectedBG->setColor({ 168, 147, 185 });
    this->addChildAtPosition(m_selectedBG, Anchor::Center);

    m_icon = CCSprite::createWithSpriteFrameName(iconFrame);
    limitNodeSize(m_icon, iconSize, 3.f, .1f);
    this->addChildAtPosition(m_icon, Anchor::Left, ccp(16, 0), false);

    m_label = CCLabelBMFont::create(text, "bigFont.fnt");
    m_label->limitLabelWidth(this->getContentWidth() - 34, .55f, .1f);
    m_label->setAnchorPoint({ .0f, .5f });
    this->addChildAtPosition(m_label, Anchor::Left, ccp(28, 0), false);
    
    return true;
}

GeodeTabSprite* GeodeTabSprite::create(const char* iconFrame, const char* text, float width) {
    auto ret = new GeodeTabSprite();
    if (ret && ret->init(iconFrame, text, width)) {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

void GeodeTabSprite::select(bool selected) {
    m_deselectedBG->setVisible(!selected);
    m_selectedBG->setVisible(selected);
}

void GeodeTabSprite::disable(bool disabled) {
    auto color = disabled ? ccc3(95, 95, 95) : ccc3(255, 255, 255);
    m_deselectedBG->setColor(color);
    m_selectedBG->setColor(color);
    m_icon->setColor(color);
    m_label->setColor(color);
}
