#pragma once

#include <Geode/DefaultInclude.hpp>
#include <Geode/binding/ButtonSprite.hpp>
#include <Geode/ui/General.hpp>
#include <Geode/ui/IconButtonSprite.hpp>
#include <Geode/ui/BasedButtonSprite.hpp>
#include <Geode/ui/Popup.hpp>
#include <Geode/utils/ZStringView.hpp>
#include <Geode/loader/Mod.hpp>
#include <server/Server.hpp>

using namespace geode::prelude;

enum class GeodePopupStyle {
    Default,
    Alt,
    Alt2,
};

bool isGeodeTheme(bool forceDisableTheme = false);

class GeodePopup : public Popup {
protected:
    bool m_forceDisableTheme = false;

    bool init(float width, float height, GeodePopupStyle style = GeodePopupStyle::Default, bool forceDisableTheme = false);
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
IconButtonSprite* createGeodeButton(CCNode* icon, ZStringView text, GeodeButtonSprite bg = GeodeButtonSprite::Default, bool forceDisableTheme = false);
ButtonSprite* createGeodeButton(ZStringView text, int width, bool absolute = false, bool gold = false, GeodeButtonSprite bg = GeodeButtonSprite::Default, bool forceDisableTheme = false);
ButtonSprite* createGeodeButton(ZStringView text, bool gold = false, GeodeButtonSprite bg = GeodeButtonSprite::Default, bool forceDisableTheme = false);

CircleButtonSprite* createGeodeCircleButton(CCSprite* top, float scale = 1.f, CircleBaseSize size = CircleBaseSize::Medium, bool altColor = false, bool forceDisableTheme = false);

ButtonSprite* createTagLabel(ZStringView text, std::pair<ccColor3B, ccColor3B> const& color);
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
