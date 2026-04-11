#include <Geode/ui/IconButtonSprite.hpp>
#include <Geode/utils/cocos.hpp>
#include <cocos-ext.h>

using namespace geode::prelude;

class IconButtonSprite::Impl final {
public:
    NineSlice* bg = nullptr;
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
        m_impl->bg = NineSlice::createWithSpriteFrameName(bg);
    }
    else {
        m_impl->bg = NineSlice::create(bg);
    }
    this->addChild(m_impl->bg);

    m_impl->label = CCLabelBMFont::create(text, font);
    m_impl->label->setZOrder(1);
    this->addChildAtPosition(m_impl->label, Anchor::Center);

    if (icon) {
        m_impl->icon = icon;
        icon->setZOrder(1);
        this->addChildAtPosition(icon, Anchor::Center);
    }

    this->updateLayout();

    return true;
}

void IconButtonSprite::updateLayout() {
    bool hasText = m_impl->label->getString() && strlen(m_impl->label->getString());

    constexpr float PADDING = 7.5f;

    CCSize size = ccp(PADDING * 2, 35);
    if (hasText) {
        m_impl->label->limitLabelWidth(100, .6f, .1f);
        size.width += m_impl->label->getScaledContentWidth();
        if (m_impl->icon) {
            size.width += PADDING;
        }
    }
    if (m_impl->icon) {
        limitNodeSize(m_impl->icon, ccp(20, 20), 1.f, .1f);
        size.width += m_impl->icon->getScaledContentWidth();
    }

    this->setContentSize(size);
    m_impl->bg->setContentSize(size / m_impl->bg->getScale());
    m_impl->bg->setPosition(m_obContentSize / 2);

    if (m_impl->icon) {
        if (hasText) {
            m_impl->label->updateAnchoredPosition(
                Anchor::Left,
                ccp(m_impl->icon->getScaledContentWidth() + PADDING * 2, 1), ccp(0, .5f)
            );
            m_impl->icon->updateAnchoredPosition(Anchor::Left, ccp(PADDING, 0), ccp(0, .5f));
        }
        else {
            m_impl->icon->updateAnchoredPosition(Anchor::Center, ccp(0, 0), ccp(.5f, .5f));
        }
    }
    else {
        m_impl->label->updateAnchoredPosition(Anchor::Center, ccp(0, 0), ccp(.5f, .5f));
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
        m_impl->bg = NineSlice::createWithSpriteFrameName(bg);
    }
    else {
        m_impl->bg = NineSlice::create(bg);
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

NineSlice* IconButtonSprite::getBg() {
    return m_impl->bg;
}

cocos2d::CCLabelBMFont* IconButtonSprite::getLabel() {
    return m_impl->label;
}

cocos2d::CCNode* IconButtonSprite::getIcon() {
    return m_impl->icon;
}