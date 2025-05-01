#include <Geode/ui/IconButtonSprite.hpp>
#include <Geode/utils/cocos.hpp>
#include <cocos-ext.h>

using namespace geode::prelude;

bool IconButtonSprite::init(
    char const* bg, bool bgIsFrame, cocos2d::CCNode* icon, char const* text, char const* font
) {
    if (!CCSprite::init()) return false;

    if (bgIsFrame) {
        m_bg = CCScale9Sprite::createWithSpriteFrameName(bg);
    }
    else {
        m_bg = CCScale9Sprite::create(bg);
    }
    this->addChild(m_bg);

    m_label = CCLabelBMFont::create(text, font);
    m_label->setAnchorPoint({ .0f, .5f });
    m_label->setZOrder(1);
    this->addChild(m_label);

    if (icon) {
        m_icon = icon;
        icon->setZOrder(1);
        this->addChild(icon);
    }

    this->updateLayout();

    return true;
}

void IconButtonSprite::updateLayout() {
    static constexpr float const PAD = 7.5f;

    bool hasText = m_label->getString() && strlen(m_label->getString());

    auto size = CCSize { 20.f, 20.f };
    if (hasText) {
        m_label->limitLabelWidth(100.f, .6f, .1f);
        size.width += m_label->getScaledContentSize().width;
        if (m_icon) {
            size.width += PAD;
        }
    }
    if (m_icon) {
        limitNodeSize(m_icon, { 20, 20 }, 1.f, .1f);
    }
    size.height += 15.f;

    if (m_icon) {
        size.width += m_icon->getScaledContentSize().width;
    }

    this->setContentSize(size);
    m_bg->setContentSize(size / m_bg->getScale());
    m_bg->setPosition(m_obContentSize / 2);

    if (m_icon) {
        if (hasText) {
            m_label->setPosition(
                size.height / 2 + m_icon->getScaledContentSize().width / 2 + PAD,
                size.height / 2 + 1.f
            );
            m_icon->setPosition(size.height / 2, size.height / 2);
        }
        else {
            m_icon->setPosition(size.width / 2, size.height / 2);
        }
    }
    else {
        m_label->setPosition(size.height / 2, size.height / 2);
    }
}

IconButtonSprite* IconButtonSprite::create(
    char const* bg, cocos2d::CCNode* icon, char const* text, char const* font
) {
    auto ret = new IconButtonSprite();
    if (ret->init(bg, false, icon, text, font)) {
        ret->autorelease();
        return ret;
    }
    delete ret;
    return nullptr;
}

IconButtonSprite* IconButtonSprite::createWithSpriteFrameName(
    char const* bg, cocos2d::CCNode* icon, char const* text, char const* font
) {
    auto ret = new IconButtonSprite();
    if (ret->init(bg, true, icon, text, font)) {
        ret->autorelease();
        return ret;
    }
    delete ret;
    return nullptr;
}

void IconButtonSprite::setBG(char const* bg, bool isFrame) {
    if (m_bg) {
        m_bg->removeFromParent();
    }
    if (isFrame) {
        m_bg = CCScale9Sprite::createWithSpriteFrameName(bg);
    }
    else {
        m_bg = CCScale9Sprite::create(bg);
    }
    this->addChild(m_bg);
    this->updateLayout();
}

void IconButtonSprite::setIcon(cocos2d::CCNode* icon) {
    if (m_icon) {
        m_icon->removeFromParent();
    }
    m_icon = icon;
    m_icon->setZOrder(1);
    this->addChild(icon);
    this->updateLayout();
}

cocos2d::CCNode* IconButtonSprite::getIcon() const {
    return m_icon;
}

void IconButtonSprite::setString(char const* label) {
    m_label->setString(label);
    this->updateLayout();
}

char const* IconButtonSprite::getString() {
    return m_label->getString();
}

void IconButtonSprite::setColor(cocos2d::ccColor3B const& color) {
    CCSprite::setColor(color);
    m_bg->setColor(color);
    m_label->setColor(color);
    if (auto icon = typeinfo_cast<CCRGBAProtocol*>(m_icon)) {
        icon->setColor(color);
    }
}

void IconButtonSprite::setOpacity(GLubyte opacity) {
    CCSprite::setOpacity(opacity);
    m_bg->setOpacity(opacity);
    m_label->setOpacity(opacity);
    if (auto icon = typeinfo_cast<CCRGBAProtocol*>(m_icon)) {
        icon->setOpacity(opacity);
    }
}
