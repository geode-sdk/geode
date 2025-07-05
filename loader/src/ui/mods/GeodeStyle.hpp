#pragma once

#include <Geode/DefaultInclude.hpp>
#include <Geode/binding/ButtonSprite.hpp>
#include <Geode/ui/General.hpp>
#include <Geode/ui/IconButtonSprite.hpp>
#include <Geode/ui/BasedButtonSprite.hpp>
#include <Geode/ui/Popup.hpp>
#include <Geode/loader/Mod.hpp>
#include <server/Server.hpp>

using namespace geode::prelude;

enum class GeodePopupStyle {
    Default,
    Alt,
    Alt2,
};

bool isGeodeTheme(bool forceDisableTheme = false);

template <class... Args>
class GeodePopup : public Popup<Args...> {
protected:
    bool m_forceDisableTheme = false;

    bool init(float width, float height, Args... args, GeodePopupStyle style = GeodePopupStyle::Default, bool forceDisableTheme = false) {
        m_forceDisableTheme = forceDisableTheme;
        const bool geodeTheme = isGeodeTheme(forceDisableTheme);
        const char* bg;
        switch (style) {
            default:
            case GeodePopupStyle::Default: bg = geodeTheme ? "GE_square01.png"_spr : "GJ_square01.png"; break;
            case GeodePopupStyle::Alt:     bg = geodeTheme ? "GE_square02.png"_spr : "GJ_square02.png"; break;
            case GeodePopupStyle::Alt2:    bg = geodeTheme ? "GE_square03.png"_spr : "GJ_square02.png"; break;
        }
        if (!Popup<Args...>::initAnchored(width, height, std::forward<Args>(args)..., bg))
            return false;

        this->setCloseButtonSpr(
            CircleButtonSprite::createWithSpriteFrameName(
                "close.png"_spr, .85f,
                (geodeTheme ?
                    (style == GeodePopupStyle::Default ? CircleBaseColor::DarkPurple : CircleBaseColor::DarkAqua) :
                    CircleBaseColor::Green
                )
            )
        );

        return true;
    }
};

class GeodeSquareSprite : public CCSprite {
protected:
    bool* m_stateSrc = nullptr;
    bool m_state = false;
    bool m_forceDisableTheme = false;
    CCSprite* m_topSprite;

    bool init(CCSprite* top, bool* state, bool forceDisableTheme = false);

    void update(float dt) override;
    void updateImage();

public:
    static GeodeSquareSprite* create(const char* top, bool* state = nullptr, bool forceDisableTheme = false);
    static GeodeSquareSprite* createWithSpriteFrameName(const char* top, bool* state = nullptr, bool forceDisableTheme = false);

    CCSprite* getTopSprite() const;
    void setState(bool state);
};

CCNode* createLoadingCircle(float sideLength, const char* id = "loading-spinner");

enum class GeodeButtonSprite {
    Default,
    Install,
    Delete,
    Enable,
    Gray,
};
const char* getGeodeButtonSpriteName(GeodeButtonSprite spr, bool forceDisableTheme = false);
IconButtonSprite* createGeodeButton(CCNode* icon, std::string const& text, GeodeButtonSprite bg = GeodeButtonSprite::Default, bool forceDisableTheme = false);
ButtonSprite* createGeodeButton(std::string const& text, int width, bool absolute = false, bool gold = false, GeodeButtonSprite bg = GeodeButtonSprite::Default, bool forceDisableTheme = false);
ButtonSprite* createGeodeButton(std::string const& text, bool gold = false, GeodeButtonSprite bg = GeodeButtonSprite::Default, bool forceDisableTheme = false);

CircleButtonSprite* createGeodeCircleButton(CCSprite* top, float scale = 1.f, CircleBaseSize size = CircleBaseSize::Medium, bool altColor = false, bool forceDisableTheme = false);

ButtonSprite* createTagLabel(std::string const& text, std::pair<ccColor3B, ccColor3B> const& color);
ButtonSprite* createGeodeTagLabel(server::ServerTag const& tag);
std::pair<ccColor3B, ccColor3B> geodeTagColors(server::ServerTag const& tag);

ListBorders* createGeodeListBorders(CCSize const& size, bool forceDisableTheme = false);

class GeodeTabSprite : public CCNode {
protected:
    CCScale9Sprite* m_deselectedBG;
    CCScale9Sprite* m_selectedBG;
    CCSprite* m_icon;
    CCLabelBMFont* m_label;

    bool init(const char* iconFrame, const char* text, float width, bool altColor);

public:
    static GeodeTabSprite* create(const char* iconFrame, const char* text, float width, bool altColor = false);

    void select(bool selected);
    void disable(bool disabled);
};
