#pragma once

#include <Geode/DefaultInclude.hpp>
#include <Geode/ui/IconButtonSprite.hpp>
#include <Geode/ui/BasedButtonSprite.hpp>

using namespace geode::prelude;

enum class GeodePopupStyle {
    Default,
    Alt,
    Alt2,
};

template <class... Args>
class GeodePopup : public Popup<Args...> {
protected:
    bool init(float width, float height, Args... args, GeodePopupStyle style = GeodePopupStyle::Default) {
        const char* bg;
        switch (style) {
            default:
            case GeodePopupStyle::Default: bg = "GE_square01.png"_spr; break;
            case GeodePopupStyle::Alt:     bg = "GE_square02.png"_spr; break;
            case GeodePopupStyle::Alt2:    bg = "GE_square03.png"_spr; break;
        }
        if (!Popup<Args...>::initAnchored(width, height, std::forward<Args>(args)..., bg))
            return false;
        
        this->setCloseButtonSpr(
            CircleButtonSprite::createWithSpriteFrameName(
                "close.png"_spr, .85f,
                (style == GeodePopupStyle::Default ? CircleBaseColor::DarkPurple : CircleBaseColor::DarkAqua)
            )
        );

        return true;
    }
};

class GeodeSquareSprite : public CCSprite {
protected:
    bool* m_stateSrc = nullptr;
    bool m_state = false;
    CCSprite* m_topSprite;

    bool init(CCSprite* top, bool* state);

    void update(float dt) override;

public:
    static GeodeSquareSprite* create(const char* top, bool* state = nullptr);
    static GeodeSquareSprite* createWithSpriteFrameName(const char* top, bool* state = nullptr);

    CCSprite* getTopSprite() const;
};

CCNode* createLoadingCircle(float sideLength, const char* id = "loading-spinner");

IconButtonSprite* createGeodeButton(CCNode* icon, std::string const& text, std::string const& bg = "GE_button_05.png"_spr);
CCNode* createGeodeButton(CCNode* icon, float width, std::string const& text, std::string const& bg = "GE_button_05.png"_spr);
ButtonSprite* createGeodeButton(std::string const& text, std::string const& bg = "GE_button_05.png"_spr);

CircleButtonSprite* createGeodeCircleButton(const char* topFrameName);

ButtonSprite* createGeodeTagLabel(std::string const& text, std::optional<std::pair<ccColor3B, ccColor3B>> const& color = std::nullopt);
std::pair<ccColor3B, ccColor3B> geodeTagColor(std::string_view const& text);

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
