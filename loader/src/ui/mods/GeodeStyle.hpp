#pragma once

#include <Geode/DefaultInclude.hpp>
#include <Geode/ui/IconButtonSprite.hpp>
#include <Geode/ui/BasedButtonSprite.hpp>

using namespace geode::prelude;

template <class... Args>
class GeodePopup : public Popup<Args...> {
protected:
    bool init(float width, float height, Args... args) {
        if (!Popup<Args...>::initAnchored(width, height, std::forward<Args>(args)..., "GE_square01.png"_spr))
            return false;
        
        // Replace the close button with a Geode style one
        auto origSize = Popup<Args...>::m_closeBtn->getContentSize();
        auto orig = Ref(Popup<Args...>::m_closeBtn->getNormalImage());
        auto spr = CircleButtonSprite::createWithSpriteFrameName("close.png"_spr, 1.f, CircleBaseColor::DarkPurple);
        Popup<Args...>::m_closeBtn->setNormalImage(spr);
        spr->setScale(orig->getScale());
        spr->setPosition(orig->getPosition());
        spr->setAnchorPoint(orig->getAnchorPoint());
        Popup<Args...>::m_closeBtn->setContentSize(origSize);

        return true;
    }
};

class GeodeSquareSprite : public CCSprite {
protected:
    bool* m_stateSrc = nullptr;
    bool m_state = false;

    bool init(CCSprite* top, bool* state);

    void update(float dt) override;

public:
    static GeodeSquareSprite* create(const char* top, bool* state = nullptr);
    static GeodeSquareSprite* createWithSpriteFrameName(const char* top, bool* state = nullptr);
};

IconButtonSprite* createGeodeButton(CCNode* icon, std::string const& text, std::string const& bg = "GE_button_05.png"_spr);
CCNode* createGeodeButton(CCNode* icon, float width, std::string const& text, std::string const& bg = "GE_button_05.png"_spr);
ButtonSprite* createGeodeButton(std::string const& text, std::string const& bg = "GE_button_05.png"_spr);

CircleButtonSprite* createGeodeCircleButton(const char* topFrameName);

ButtonSprite* createGeodeTagLabel(std::string const& text, ccColor3B color, ccColor3B bg);
std::pair<ccColor3B, ccColor3B> geodeTagColor(std::string_view const& text);

class GeodeTabSprite : public CCNode {
protected:
    CCScale9Sprite* m_deselectedBG;
    CCScale9Sprite* m_selectedBG;
    CCSprite* m_icon;
    CCLabelBMFont* m_label;

    bool init(const char* iconFrame, const char* text, float width);

public:
    static GeodeTabSprite* create(const char* iconFrame, const char* text, float width);

    void select(bool selected);
    void disable(bool disabled);
};
