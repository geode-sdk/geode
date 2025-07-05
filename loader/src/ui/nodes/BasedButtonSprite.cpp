#include <Geode/ui/BasedButtonSprite.hpp>
#include <Geode/loader/Mod.hpp>
#include <Geode/utils/cocos.hpp>

using namespace geode::prelude;

const char* geode::baseEnumToString(CircleBaseSize value) {
    switch (value) {
        case CircleBaseSize::Tiny: return "Tiny";
        case CircleBaseSize::Small: return "Small";
        case CircleBaseSize::SmallAlt: return "SmallAlt";
        case CircleBaseSize::Medium: return "Medium";
        case CircleBaseSize::MediumAlt: return "MediumAlt";
        case CircleBaseSize::Big: return "Big";
        case CircleBaseSize::BigAlt: return "BigAlt";
        case CircleBaseSize::Large: return "Large";
    }
    return "Unknown";
}

const char* geode::baseEnumToString(CircleBaseColor value) {
    switch (value) {
        case CircleBaseColor::Green: return "Green";
        case CircleBaseColor::Pink: return "Pink";
        case CircleBaseColor::Gray: return "Gray";
        case CircleBaseColor::Blue: return "Blue";
        case CircleBaseColor::Cyan: return "Cyan";
        case CircleBaseColor::DarkPurple: return "DarkPurple";
        case CircleBaseColor::DarkAqua: return "DarkAqua";
    }
    return "Unknown";
}

const char* geode::baseEnumToString(CrossBaseSize value) {
    switch (value) {
        case CrossBaseSize::Small: return "Small";
        case CrossBaseSize::Huge: return "Huge";
    }
    return "Unknown";
}

const char* geode::baseEnumToString(CrossBaseColor value) {
    switch (value) {
        case CrossBaseColor::Green: return "Green";
    }
    return "Unknown";
}

const char* geode::baseEnumToString(AccountBaseSize value) {
    switch (value) {
        case AccountBaseSize::Normal: return "Normal";
    }
    return "Unknown";
}

const char* geode::baseEnumToString(AccountBaseColor value) {
    switch (value) {
        case AccountBaseColor::Blue: return "Blue";
        case AccountBaseColor::Gray: return "Gray";
        case AccountBaseColor::Purple: return "Purple";
    }
    return "Unknown";
}

const char* geode::baseEnumToString(IconSelectBaseSize value) {
    switch (value) {
        case IconSelectBaseSize::Normal: return "Normal";
    }
    return "Unknown";
}

const char* geode::baseEnumToString(IconSelectBaseColor value) {
    switch (value) {
        case IconSelectBaseColor::Unselected: return "Unselected";
        case IconSelectBaseColor::Selected: return "Selected";
    }
    return "Unknown";
}

const char* geode::baseEnumToString(EditorBaseSize value) {
    switch (value) {
        case EditorBaseSize::Normal: return "Normal";
    }
    return "Unknown";
}

const char* geode::baseEnumToString(EditorBaseColor value) {
    switch (value) {
        case EditorBaseColor::LightBlue: return "LightBlue";
        case EditorBaseColor::Green: return "Green";
        case EditorBaseColor::Orange: return "Orange";
        case EditorBaseColor::DarkGray: return "DarkGray";
        case EditorBaseColor::Gray: return "Gray";
        case EditorBaseColor::Pink: return "Pink";
        case EditorBaseColor::Teal: return "Teal";
        case EditorBaseColor::Aqua: return "Aqua";
        case EditorBaseColor::Cyan: return "Cyan";
        case EditorBaseColor::Magenta: return "Magenta";
        case EditorBaseColor::DimGreen: return "DimGreen";
        case EditorBaseColor::BrightGreen: return "BrightGreen";
        case EditorBaseColor::Salmon: return "Salmon";
    }
    return "Unknown";
}

const char* geode::baseEnumToString(TabBaseSize value) {
    switch (value) {
        case TabBaseSize::Normal: return "Normal";
    }
    return "Unknown";
}

const char* geode::baseEnumToString(TabBaseColor value) {
    switch (value) {
        case TabBaseColor::Unselected: return "Unselected";
        case TabBaseColor::UnselectedDark: return "UnselectedDark";
        case TabBaseColor::Selected: return "Selected";
    }
    return "Unknown";
}

const char* geode::baseEnumToString(LeaderboardBaseSize value) {
    switch (value) {
        case LeaderboardBaseSize::Normal: return "Normal";
    }
    return "Unknown";
}

const char* geode::baseEnumToString(LeaderboardBaseColor value) {
    switch (value) {
        case LeaderboardBaseColor::Blue: return "Blue";
    }
    return "Unknown";
}

const char* geode::baseEnumToString(CategoryBaseSize value) {
    switch (value) {
        case CategoryBaseSize::Big: return "Big";
    }
    return "Unknown";
}

const char* geode::baseEnumToString(CategoryBaseColor value) {
    switch (value) {
        case CategoryBaseColor::Green: return "Green";
    }
    return "Unknown";
}

const char* geode::baseEnumToString(BaseType value) {
    switch (value) {
        case BaseType::Circle: return "Circle";
        case BaseType::Cross: return "Cross";
        case BaseType::Account: return "Account";
        case BaseType::IconSelect: return "IconSelect";
        case BaseType::Leaderboard: return "Leaderboard";
        case BaseType::Editor: return "Editor";
        case BaseType::Tab: return "Tab";
        case BaseType::Category: return "Category";
    }
    return "Unknown";
}

static std::string baseEnumsToString(BaseType type, int size, int color) {
#define ENUMS_TO_STRING(ty_)    \
    case BaseType::ty_: {       \
        sizeStr = baseEnumToString(static_cast<ty_##BaseSize>(size));\
        colorStr = baseEnumToString(static_cast<ty_##BaseColor>(color));\
    } break

    const char* typeStr = baseEnumToString(type);
    const char* sizeStr;
    const char* colorStr;
    switch (type) {
        ENUMS_TO_STRING(Circle);
        ENUMS_TO_STRING(Cross);
        ENUMS_TO_STRING(Account);
        ENUMS_TO_STRING(IconSelect);
        ENUMS_TO_STRING(Leaderboard);
        ENUMS_TO_STRING(Editor);
        ENUMS_TO_STRING(Tab);
        ENUMS_TO_STRING(Category);
    }
    return fmt::format("base{}_{}_{}.png", typeStr, sizeStr, colorStr);
}

bool BasedButtonSprite::init(CCNode* ontop, BaseType type, int size, int color) {
    if (!CCSprite::initWithSpriteFrameName(
        Mod::get()->expandSpriteName(baseEnumsToString(type, size, color)).data()
    )) return false;

    m_type = type;
    m_size = size;
    m_color = color;

    if (ontop) {
        m_onTop = ontop;
        m_onTop->setPosition(this->getContentSize() / 2 + m_topOffset);
        limitNodeSize(m_onTop, this->getMaxTopSize(), 999.f, .1f);
        m_onTop->setScale(m_onTop->getScale() * m_onTopRelativeScale);
        this->addChild(m_onTop);
    }

    this->setCascadeColorEnabled(true);
    this->setCascadeOpacityEnabled(true);

    return true;
}

CCSize BasedButtonSprite::getMaxTopSize() const {
    return m_obContentSize - CCSize(18.f, 18.f);
}

void BasedButtonSprite::setTopOffset(CCPoint const& offset) {
    m_topOffset = offset;
    if (m_onTop) {
        m_onTop->setPosition(this->getContentSize() / 2 + offset);
    }
}
void BasedButtonSprite::setTopRelativeScale(float scale) {
    m_onTopRelativeScale = scale;
    if (m_onTop) {
        limitNodeSize(m_onTop, this->getMaxTopSize(), 999.f, .1f);
        m_onTop->setScale(m_onTop->getScale() * m_onTopRelativeScale);
    }
}

bool BasedButtonSprite::initWithSprite(
    char const* sprName, float sprScale, BaseType type, int size, int color
) {
    auto spr = CCSprite::create(sprName);
    if (!spr) return false;
    m_onTopRelativeScale = sprScale;
    return this->init(spr, type, size, color);
}

bool BasedButtonSprite::initWithSpriteFrameName(
    char const* sprName, float sprScale, BaseType type, int size, int color
) {
    auto spr = CCSprite::createWithSpriteFrameName(sprName);
    if (!spr) return false;
    m_onTopRelativeScale = sprScale;
    return this->init(spr, type, size, color);
}

CCNode* BasedButtonSprite::getTopNode() const {
    return m_onTop;
}

BasedButtonSprite::~BasedButtonSprite() {}

BasedButtonSprite* BasedButtonSprite::create(CCNode* ontop, BaseType type, int size, int color) {
    auto ret = new BasedButtonSprite();
    if (ret->init(ontop, type, size, color)) {
        ret->autorelease();
        return ret;
    }
    delete ret;
    return nullptr;
}

#define DECL_BASED_CREATE(ty_) \
    ty_##ButtonSprite* ty_##ButtonSprite::create(                       \
        cocos2d::CCNode* top, ty_##BaseColor color, ty_##BaseSize size  \
    ) {                                                                 \
        auto ret = new ty_##ButtonSprite();                             \
        if (ret->init(                                           \
            top, BaseType::ty_,                                         \
            static_cast<int>(size), static_cast<int>(color)             \
        )) {                                                            \
            ret->autorelease();                                         \
            return ret;                                                 \
        }                                                               \
        delete ret;                                            \
        return nullptr;                                                 \
    }

#define DECL_BASED_CREATE_WITH_SPRITE(ty_)                  \
    ty_##ButtonSprite* ty_##ButtonSprite::createWithSprite( \
        const char* sprName, float sprScale,                \
        ty_##BaseColor color, ty_##BaseSize size            \
    ) {                                                     \
        auto ret = new ty_##ButtonSprite();                 \
        if (ret->initWithSprite(                     \
            sprName, sprScale, BaseType::ty_,               \
            static_cast<int>(size), static_cast<int>(color) \
        )) {                                                \
            ret->autorelease();                             \
            return ret;                                     \
        }                                                   \
        delete ret;                                \
        return nullptr;                                     \
    }

#define DECL_BASED_CREATE_WITH_SPRITE_FRAME_NAME(ty_)               \
    ty_##ButtonSprite* ty_##ButtonSprite::createWithSpriteFrameName(\
        const char* sprName, float sprScale,                        \
        ty_##BaseColor color, ty_##BaseSize size                    \
    ) {                                                             \
        auto ret = new ty_##ButtonSprite();                         \
        if (ret->initWithSpriteFrameName(                    \
            sprName, sprScale, BaseType::ty_,                       \
            static_cast<int>(size), static_cast<int>(color)         \
        )) {                                                        \
            ret->autorelease();                                     \
            return ret;                                             \
        }                                                           \
        delete ret;                                        \
        return nullptr;                                             \
    }

#define DECL_BASED_CREATE_FUNS(ty_)                 \
    DECL_BASED_CREATE(ty_);                         \
    DECL_BASED_CREATE_WITH_SPRITE(ty_);             \
    DECL_BASED_CREATE_WITH_SPRITE_FRAME_NAME(ty_);  \

DECL_BASED_CREATE_FUNS(Circle);
DECL_BASED_CREATE_FUNS(Cross);
DECL_BASED_CREATE_FUNS(Account);
DECL_BASED_CREATE_FUNS(IconSelect);
DECL_BASED_CREATE_FUNS(Leaderboard);
DECL_BASED_CREATE_FUNS(Editor);
DECL_BASED_CREATE_FUNS(Category);

CCSize CircleButtonSprite::getMaxTopSize() const {
    return m_obContentSize * .65f;
}

CCSize EditorButtonSprite::getMaxTopSize() const {
    return m_obContentSize - CCSize { 8.f, 8.f };
}

TabButtonSprite* TabButtonSprite::create(char const* text, TabBaseColor color, TabBaseSize size) {
    auto ret = new TabButtonSprite();
    auto label = CCLabelBMFont::create(text, "bigFont.fnt");
    label->limitLabelWidth(75.f, .6f, .1f);
    if (ret->init(
        label, BaseType::Tab,
        static_cast<int>(size), static_cast<int>(color)
    )) {
        ret->autorelease();
        return ret;
    }
    delete ret;
    return nullptr;
}
