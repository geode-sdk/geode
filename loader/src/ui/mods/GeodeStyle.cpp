#include "GeodeStyle.hpp"
#include <Geode/utils/cocos.hpp>

bool GeodeButtonSprite::init(CCSprite* top, bool* state) {
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

void GeodeButtonSprite::update(float dt) {
    CCSprite::update(dt);
    if (m_stateSrc && m_state != *m_stateSrc) {
        m_state = *m_stateSrc;
        this->setTexture(CCTextureCache::get()->addImage(
            (m_state ? "GJ_button_02.png" : "GE_button_05.png"_spr), true
        ));
    }
}

GeodeButtonSprite* GeodeButtonSprite::create(const char* top, bool* state) {
    auto ret = new GeodeButtonSprite();
    if (ret && ret->init(CCSprite::create(top), state)) {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

GeodeButtonSprite* GeodeButtonSprite::createWithSpriteFrameName(const char* top, bool* state) {
    auto ret = new GeodeButtonSprite();
    if (ret && ret->init(CCSprite::createWithSpriteFrameName(top), state)) {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}
