#pragma once

#include <Geode/DefaultInclude.hpp>

using namespace geode::prelude;

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

ButtonSprite* createGeodeButton(std::string const& text, std::string const& bg = "GE_button_05.png"_spr);

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
