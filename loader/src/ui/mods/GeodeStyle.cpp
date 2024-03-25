#include "GeodeStyle.hpp"
#include <Geode/utils/cocos.hpp>
#include <Geode/utils/ColorProvider.hpp>

$execute {
    ColorProvider::get()->define("mod-list-bg"_spr, { 25, 17, 37, 255 });
    ColorProvider::get()->define("mod-list-version-label"_spr, ccc3(86, 235, 41));
    ColorProvider::get()->define("mod-list-restart-required-label"_spr, ccc3(153, 245, 245));
    ColorProvider::get()->define("mod-list-restart-required-label-bg"_spr, ccc3(123, 156, 163));
    ColorProvider::get()->define("mod-list-search-bg"_spr, { 83, 65, 109, 255 });
    ColorProvider::get()->define("mod-list-tab-selected-bg"_spr, { 168, 147, 185, 255 });
    ColorProvider::get()->define("mod-list-tab-selected-bg-alt"_spr, { 147, 163, 185, 255 });
}

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

IconButtonSprite* createGeodeButton(CCNode* icon, std::string const& text, std::string const& bg) {
    return IconButtonSprite::create(bg.c_str(), icon, text.c_str(), "bigFont.fnt");
}

CCNode* createGeodeButton(CCNode* icon, float width, std::string const& text, std::string const& bgSpr) {
    auto bg = CCScale9Sprite::create(bgSpr.c_str());
    bg->setContentSize({ width, 35 });

    limitNodeSize(icon, { 20, 20 }, 1.f, .1f);
    bg->addChildAtPosition(icon, Anchor::Left, { 35.f / 2, 0 }, { .5f, .5f });

    auto label = CCLabelBMFont::create(text.c_str(), "bigFont.fnt");
    limitNodeSize(label, { width - 42.5f, 20 }, .6f, .1f);
    bg->addChildAtPosition(label, Anchor::Left, { 35, 0 }, { 0, .5f });

    return bg;
}

ButtonSprite* createGeodeButton(std::string const& text, std::string const& bg) {
    return ButtonSprite::create(text.c_str(), "bigFont.fnt", bg.c_str(), .8f);
}

CircleButtonSprite* createGeodeCircleButton(const char* topFrameName) {
    return CircleButtonSprite::createWithSpriteFrameName(topFrameName, 1.f, CircleBaseColor::DarkPurple);
}

ButtonSprite* createGeodeTagLabel(std::string const& text, ccColor3B color, ccColor3B bg) {
    auto label = ButtonSprite::create(text.c_str(), "bigFont.fnt", "white-square.png"_spr, .8f);
    label->m_label->setColor(color);
    label->m_BGSprite->setColor(bg);
    return label;
}

std::pair<ccColor3B, ccColor3B> geodeTagColor(std::string_view const& text) {
    static std::array TAG_COLORS {
        std::make_pair(ccc3(240, 233, 255), ccc3(130, 123, 163)),
        std::make_pair(ccc3(234, 255, 245), ccc3(123, 163, 136)),
        std::make_pair(ccc3(240, 252, 255), ccc3(123, 152, 163)),
        std::make_pair(ccc3(255, 253, 240), ccc3(163, 157, 123)),
        std::make_pair(ccc3(255, 242, 240), ccc3(163, 128, 123)),
    };
    return TAG_COLORS[hash(text) % 5932 % TAG_COLORS.size()];
}

bool GeodeTabSprite::init(const char* iconFrame, const char* text, float width, bool altColor) {
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
    m_selectedBG->setColor(to3B(ColorProvider::get()->color(
        altColor ? 
            "mod-list-tab-selected-bg-alt"_spr : 
            "mod-list-tab-selected-bg"_spr
    )));
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

GeodeTabSprite* GeodeTabSprite::create(const char* iconFrame, const char* text, float width, bool altColor) {
    auto ret = new GeodeTabSprite();
    if (ret && ret->init(iconFrame, text, width, altColor)) {
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
