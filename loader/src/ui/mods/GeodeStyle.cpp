#include "GeodeStyle.hpp"
#include <Geode/utils/cocos.hpp>
#include <Geode/utils/ColorProvider.hpp>
#include <Geode/loader/SettingEvent.hpp>
#include <Geode/binding/ButtonSprite.hpp>

$on_mod(Loaded) {
    // todo: these names should probably be shorter so they fit in SSO...
    ColorProvider::get()->define("mod-list-bg"_spr, { 25, 17, 37, 255 });
    ColorProvider::get()->define("mod-list-version-label"_spr, ccc3(112, 235, 41));
    ColorProvider::get()->define("mod-list-version-bg-updates-available"_spr, ccc3(88, 202, 255));
    ColorProvider::get()->define("mod-list-version-label-updates-available"_spr, ccc3(88, 202, 255));
    ColorProvider::get()->define("mod-list-restart-required-label"_spr, ccc3(153, 245, 245));
    ColorProvider::get()->define("mod-list-restart-required-label-bg"_spr, ccc3(123, 156, 163));
    ColorProvider::get()->define("mod-list-search-bg"_spr, { 83, 65, 109, 255 });
    ColorProvider::get()->define("mod-list-updates-available-bg"_spr, { 139, 89, 173, 255 });
    ColorProvider::get()->define("mod-list-updates-available-bg-2"_spr, { 45, 110, 222, 255 });
    ColorProvider::get()->define("mod-list-errors-found"_spr, { 235, 35, 112, 255 });
    ColorProvider::get()->define("mod-list-errors-found-2"_spr, { 245, 27, 27, 255 });
    ColorProvider::get()->define("mod-list-tab-deselected-bg"_spr, { 26, 24, 29, 255 });
    ColorProvider::get()->define("mod-list-tab-selected-bg"_spr, { 168, 147, 185, 255 });
    ColorProvider::get()->define("mod-list-tab-selected-bg-alt"_spr, { 147, 163, 185, 255 });
    ColorProvider::get()->define("mod-list-featured-color"_spr, { 255, 255, 120, 255 });
    ColorProvider::get()->define("mod-list-enabled"_spr, { 120, 255, 100, 255 });
    ColorProvider::get()->define("mod-list-disabled"_spr, { 255, 120, 100, 255 });
    ColorProvider::get()->define("mod-list-recommended-bg"_spr, ccc3(25, 255, 167));
    ColorProvider::get()->define("mod-list-recommended-by"_spr, ccc3(25, 255, 167));
    ColorProvider::get()->define("mod-list-recommended-by-2"_spr, ccc3(47, 255, 255));
    ColorProvider::get()->define("mod-problems-item-bg"_spr, { 255, 255, 255, 15 });
    ColorProvider::get()->define("mod-developer-item-bg"_spr, { 255, 255, 255, 15 });

    // Only used when GD theme is active
    ColorProvider::get()->define("mods-layer-gd-bg"_spr, { 0, 102, 255, 255 });

    auto updateColors = +[](bool enabled) {
        if (enabled) {
            ColorProvider::get()->reset("mod-list-bg"_spr);
            ColorProvider::get()->reset("mod-list-version-bg-updates-available"_spr);
            ColorProvider::get()->reset("mod-list-search-bg"_spr);
            ColorProvider::get()->reset("mod-list-tab-deselected-bg"_spr);
            ColorProvider::get()->reset("mod-list-tab-selected-bg"_spr);
            ColorProvider::get()->reset("mod-list-tab-selected-bg-alt"_spr);
            ColorProvider::get()->reset("mod-list-restart-required-label"_spr);
            ColorProvider::get()->reset("mod-list-restart-required-label-bg"_spr);
            ColorProvider::get()->reset("mod-problems-item-bg"_spr);
            ColorProvider::get()->reset("mod-developer-item-bg"_spr);
        }
        else {
            ColorProvider::get()->override("mod-list-bg"_spr, { 168, 85, 44, 255 });
            ColorProvider::get()->override("mod-list-version-bg-updates-available"_spr, { 220, 190, 0, 120 });
            ColorProvider::get()->override("mod-list-search-bg"_spr, { 114, 63, 31, 255 });
            ColorProvider::get()->override("mod-list-tab-deselected-bg"_spr, { 54, 31, 16, 255 });
            ColorProvider::get()->override("mod-list-tab-selected-bg"_spr, { 248, 200, 43, 255 });
            ColorProvider::get()->override("mod-list-tab-selected-bg-alt"_spr, { 156, 185, 147, 255 });
            ColorProvider::get()->override("mod-list-restart-required-label"_spr, ccc3(10, 226, 255));
            ColorProvider::get()->override("mod-list-restart-required-label-bg"_spr, ccc3(0, 174, 180));
            ColorProvider::get()->override("mod-list-errors-found"_spr, { 255, 0, 0, 255 });
            ColorProvider::get()->override("mod-list-errors-found-2"_spr, { 235, 35, 112, 255 });
            ColorProvider::get()->override("mod-problems-item-bg"_spr, { 0, 0, 0, 75 });
            ColorProvider::get()->override("mod-developer-item-bg"_spr, { 0, 0, 0, 75 });
        }
    };

    // Update colors when the theme is changed
    listenForSettingChanges("enable-geode-theme", updateColors);

    Loader::get()->queueInMainThread([updateColors = updateColors] {
        // this code is ran during static init, where settings aren't loaded yet, and getSettingValue will always return false.
        // because of that, we have to delay it until next frame.
        updateColors(Mod::get()->template getSettingValue<bool>("enable-geode-theme"));
    });
}

bool GeodeSquareSprite::init(CCSprite* top, bool* state) {
    if (!CCSprite::initWithFile(isGeodeTheme() ? "GE_button_05.png"_spr : "GJ_button_01.png"))
        return false;

    m_stateSrc = state;
    m_topSprite = top;

    limitNodeSize(top, m_obContentSize * .65f, 2.f, .1f);
    this->addChildAtPosition(top, Anchor::Center);

    // Only schedule update if there is a need to do so
    if (state) {
        this->scheduleUpdate();
    }

    return true;
}

void GeodeSquareSprite::updateImage() {
    this->setTexture(CCTextureCache::get()->addImage(
        (m_state ? "GJ_button_02.png" : (isGeodeTheme() ? "GE_button_05.png"_spr : "GJ_button_01.png")),
        false
    ));
}
void GeodeSquareSprite::update(float dt) {
    CCSprite::update(dt);
    if (m_stateSrc && m_state != *m_stateSrc) {
        m_state = *m_stateSrc;
        this->updateImage();
    }
}

GeodeSquareSprite* GeodeSquareSprite::create(const char* top, bool* state) {
    auto ret = new GeodeSquareSprite();
    if (ret->init(CCSprite::create(top), state)) {
        ret->autorelease();
        return ret;
    }
    delete ret;
    return nullptr;
}
GeodeSquareSprite* GeodeSquareSprite::createWithSpriteFrameName(const char* top, bool* state) {
    auto ret = new GeodeSquareSprite();
    if (ret->init(CCSprite::createWithSpriteFrameName(top), state)) {
        ret->autorelease();
        return ret;
    }
    delete ret;
    return nullptr;
}

CCSprite* GeodeSquareSprite::getTopSprite() const {
    return m_topSprite;
}

void GeodeSquareSprite::setState(bool state) {
    if (!m_stateSrc) {
        m_state = state;
        this->updateImage();
    }
}

class LoadingSpinner : public CCNode {
protected:
    CCSprite* m_spinner;

    bool init(float sideLength) {
        if (!CCNode::init())
            return false;

        this->setID("loading-spinner");
        this->setContentSize({ sideLength, sideLength });
        this->setAnchorPoint({ .5f, .5f });

        m_spinner = CCSprite::create("loadingCircle.png");
        m_spinner->setBlendFunc({ GL_ONE, GL_ONE });
        limitNodeSize(m_spinner, m_obContentSize, 1.f, .1f);
        this->addChildAtPosition(m_spinner, Anchor::Center);

        this->spin();

        return true;
    }

    void spin() {
        m_spinner->runAction(CCRepeatForever::create(CCRotateBy::create(1.f, 360.f)));
    }

public:
    static LoadingSpinner* create(float sideLength) {
        auto ret = new LoadingSpinner();
        if (ret->init(sideLength)) {
            ret->autorelease();
            return ret;
        }
        delete ret;
        return nullptr;
    }

    void setVisible(bool visible) override {
        CCNode::setVisible(visible);
        if (visible) {
            this->spin();
        }
    }
};

CCNode* createLoadingCircle(float sideLength, const char* id) {
    auto spinner = LoadingSpinner::create(sideLength);
    spinner->setID(id);
    return spinner;
}

const char* getGeodeButtonSpriteName(GeodeButtonSprite spr) {
    if (isGeodeTheme()) {
        switch (spr) {
            default:
            case GeodeButtonSprite::Default: return "GE_button_05.png"_spr;
            case GeodeButtonSprite::Install: return "GE_button_01.png"_spr;
            case GeodeButtonSprite::Delete: return "GJ_button_06.png";
            case GeodeButtonSprite::Enable: return "GJ_button_01.png";
        }
    }
    else {
        switch (spr) {
            default:
            case GeodeButtonSprite::Default: return "GJ_button_01.png";
            case GeodeButtonSprite::Install: return "GE_button_01.png"_spr;
            case GeodeButtonSprite::Delete: return "GJ_button_06.png";
            case GeodeButtonSprite::Enable: return "GJ_button_02.png";
        }
    }
}

IconButtonSprite* createGeodeButton(CCNode* icon, std::string const& text, GeodeButtonSprite bg) {
    return IconButtonSprite::create(getGeodeButtonSpriteName(bg), icon, text.c_str(), "bigFont.fnt");
}
ButtonSprite* createGeodeButton(std::string const& text, int width, bool gold, bool absolute, GeodeButtonSprite bg) {
    return ButtonSprite::create(text.c_str(), width, absolute, gold ? "goldFont.fnt" : "bigFont.fnt", getGeodeButtonSpriteName(bg), 0.0f, .8f);
}
ButtonSprite* createGeodeButton(std::string const& text, bool gold, GeodeButtonSprite bg) {
    return ButtonSprite::create(text.c_str(), gold ? "goldFont.fnt" : "bigFont.fnt", getGeodeButtonSpriteName(bg), .8f);
}

CircleButtonSprite* createGeodeCircleButton(CCSprite* top, float scale, CircleBaseSize size, bool altColor) {
    const auto geodeTheme = isGeodeTheme();
    auto ret = CircleButtonSprite::create(
        top, geodeTheme ? (altColor ? CircleBaseColor::DarkAqua : CircleBaseColor::DarkPurple) : CircleBaseColor::Green, size
    );
    ret->setTopRelativeScale(scale);
    return ret;
}

ButtonSprite* createGeodeTagLabel(std::string const& text, std::optional<std::pair<ccColor3B, ccColor3B>> const& color) {
    auto label = ButtonSprite::create(text.c_str(), "bigFont.fnt", "white-square.png"_spr, .8f);
    if (color) {
        label->m_label->setColor(color->first);
        label->m_BGSprite->setColor(color->second);
    }
    else {
        auto def = geodeTagColor(text);
        label->m_label->setColor(def.first);
        label->m_BGSprite->setColor(def.second);
    }
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

ListBorders* createGeodeListBorders(CCSize const& size) {
    auto ret = ListBorders::create();
    if (isGeodeTheme()) {
        ret->setSpriteFrames("geode-list-top.png"_spr, "geode-list-side.png"_spr, 2);
    }
    ret->setContentSize(size);
    return ret;
}

bool isGeodeTheme() {
    return Mod::get()->template getSettingValue<bool>("enable-geode-theme");
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
    m_deselectedBG->setColor("mod-list-tab-deselected-bg"_cc3b);
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
    m_label->limitLabelWidth(this->getContentWidth() - 45, clamp(width * .0045f, .35f, .55f), .1f);
    m_label->setAnchorPoint({ .5f, .5f });
    this->addChildAtPosition(m_label, Anchor::Left, ccp((itemSize.width - iconSize.width) / 2 + iconSize.width, 0), false);

    return true;
}

GeodeTabSprite* GeodeTabSprite::create(const char* iconFrame, const char* text, float width, bool altColor) {
    auto ret = new GeodeTabSprite();
    if (ret->init(iconFrame, text, width, altColor)) {
        ret->autorelease();
        return ret;
    }
    delete ret;
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
