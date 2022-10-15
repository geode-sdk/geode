#include <Geode/ui/IconButtonSprite.hpp>
#include <Geode/utils/cocos.hpp>

USE_GEODE_NAMESPACE();

bool IconButtonSprite::init(
    const char* bg,
    bool bgIsFrame,
    cocos2d::CCNode* icon,
    const char* text,
    const char* font
) {
    if (!CCSprite::init())
        return false;
    
    if (bgIsFrame) {
        m_bg = CCScale9Sprite::createWithSpriteFrameName(bg);
    } else {
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
    static constexpr const float PAD = 7.5f;

    auto size = CCSize { 20.f, 20.f };
    if (m_label->getString() && strlen(m_label->getString())) {
        m_label->limitLabelWidth(100.f, .6f, .1f);
        size.width += m_label->getScaledContentSize().width;
    }
    if (m_icon) {
        limitNodeSize(m_icon, size, 1.f, .1f);
    }
    size.height += 15.f;

    if (m_icon) {
        size.width += m_icon->getScaledContentSize().width + PAD;
    }

    this->setContentSize(size);
    m_bg->setContentSize(size / m_bg->getScale());
    m_bg->setPosition(m_obContentSize / 2);

    if (m_icon) {
        m_label->setPosition(
            size.height / 2 + m_icon->getScaledContentSize().width / 2 + PAD,
            size.height / 2 + 1.f
        );
        m_icon->setPosition(size.height / 2, size.height / 2);
    } else {
        m_label->setPosition(size.height / 2, size.height / 2);
    }
}

IconButtonSprite* IconButtonSprite::create(
    const char* bg,
    cocos2d::CCNode* icon,
    const char* text,
    const char* font
) {
    auto ret = new IconButtonSprite();
    if (ret && ret->init(bg, false, icon, text, font)) {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

IconButtonSprite* IconButtonSprite::createWithSpriteFrameName(
    const char* bg,
    cocos2d::CCNode* icon,
    const char* text,
    const char* font
) {
    auto ret = new IconButtonSprite();
    if (ret && ret->init(bg, true, icon, text, font)) {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

void IconButtonSprite::setBG(const char* bg, bool isFrame) {
    if (m_bg) {
        m_bg->removeFromParent();
    }
    if (isFrame) {
        m_bg = CCScale9Sprite::createWithSpriteFrameName(bg);
    } else {
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

void IconButtonSprite::setString(const char* label) {
    m_label->setString(label);
    this->updateLayout();
}

const char* IconButtonSprite::getString() {
    return m_label->getString();
}

