#include <Geode/ui/IconButtonSprite.hpp>
#include <Geode/utils/cocos.hpp>
#include <cocos-ext.h>

using namespace geode::prelude;

class IconButtonSprite::Impl {
public:
    cocos2d::extension::CCScale9Sprite* bg = nullptr;
    cocos2d::CCLabelBMFont* label = nullptr;
    cocos2d::CCNode* icon = nullptr;
};

IconButtonSprite::IconButtonSprite() : m_impl(std::make_unique<Impl>()) { }

IconButtonSprite::~IconButtonSprite() { }

bool IconButtonSprite::init(
    char const* bg, bool bgIsFrame, cocos2d::CCNode* icon, char const* text, char const* font
) {
    if (!CCSprite::init()) return false;

    if (bgIsFrame) {
        m_impl->bg = CCScale9Sprite::createWithSpriteFrameName(bg);
    }
    else {
        m_impl->bg = CCScale9Sprite::create(bg);
    }
    this->addChild(m_impl->bg);

    m_impl->label = CCLabelBMFont::create(text, font);
    m_impl->label->setAnchorPoint({ .0f, .5f });
    m_impl->label->setZOrder(1);
    this->addChild(m_impl->label);

    if (icon) {
        m_impl->icon = icon;
        icon->setZOrder(1);
        this->addChild(icon);
    }

    this->updateLayout();

    return true;
}

void IconButtonSprite::updateLayout() {
    static constexpr float const PAD = 7.5f;

    bool hasText = m_impl->label->getString() && strlen(m_impl->label->getString());

    auto size = CCSize { 20.f, 20.f };
    if (hasText) {
        m_impl->label->limitLabelWidth(100.f, .6f, .1f);
        size.width += m_impl->label->getScaledContentSize().width;
        if (m_impl->icon) {
            size.width += PAD;
        }
    }
    if (m_impl->icon) {
        limitNodeSize(m_impl->icon, { 20, 20 }, 1.f, .1f);
    }
    size.height += 15.f;

    if (m_impl->icon) {
        size.width += m_impl->icon->getScaledContentSize().width;
    }

    this->setContentSize(size);
    m_impl->bg->setContentSize(size / m_impl->bg->getScale());
    m_impl->bg->setPosition(m_obContentSize / 2);

    if (m_impl->icon) {
        if (hasText) {
            m_impl->label->setPosition(
                size.height / 2 + m_impl->icon->getScaledContentSize().width / 2 + PAD,
                size.height / 2 + 1.f
            );
            m_impl->icon->setPosition(size.height / 2, size.height / 2);
        }
        else {
            m_impl->icon->setPosition(size.width / 2, size.height / 2);
        }
    }
    else {
        m_impl->label->setPosition(size.height / 2, size.height / 2);
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
    if (m_impl->bg) {
        m_impl->bg->removeFromParent();
    }
    if (isFrame) {
        m_impl->bg = CCScale9Sprite::createWithSpriteFrameName(bg);
    }
    else {
        m_impl->bg = CCScale9Sprite::create(bg);
    }
    this->addChild(m_impl->bg);
    this->updateLayout();
}

void IconButtonSprite::setIcon(cocos2d::CCNode* icon) {
    if (m_impl->icon) {
        m_impl->icon->removeFromParent();
    }
    m_impl->icon = icon;
    m_impl->icon->setZOrder(1);
    this->addChild(icon);
    this->updateLayout();
}

cocos2d::CCNode* IconButtonSprite::getIcon() const {
    return m_impl->icon;
}

void IconButtonSprite::setString(char const* label) {
    m_impl->label->setString(label);
    this->updateLayout();
}

char const* IconButtonSprite::getString() {
    return m_impl->label->getString();
}

void IconButtonSprite::setColor(cocos2d::ccColor3B const& color) {
    CCSprite::setColor(color);
    m_impl->bg->setColor(color);
    m_impl->label->setColor(color);
    if (auto icon = typeinfo_cast<CCRGBAProtocol*>(m_impl->icon)) {
        icon->setColor(color);
    }
}

void IconButtonSprite::setOpacity(GLubyte opacity) {
    CCSprite::setOpacity(opacity);
    m_impl->bg->setOpacity(opacity);
    m_impl->label->setOpacity(opacity);
    if (auto icon = typeinfo_cast<CCRGBAProtocol*>(m_impl->icon)) {
        icon->setOpacity(opacity);
    }
}

cocos2d::extension::CCScale9Sprite* IconButtonSprite::getBg() {
    return m_impl->bg;
}

cocos2d::CCLabelBMFont* IconButtonSprite::getLabel() {
    return m_impl->label;
}

cocos2d::CCNode* IconButtonSprite::getIcon() {
    return m_impl->icon;
}