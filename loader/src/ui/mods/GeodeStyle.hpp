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

ButtonSprite* createGeodeButton(std::string const& text);

class GeodeButtonSprite : public ButtonSprite {
protected:
    bool init(std::string const& text);

public:
    static GeodeButtonSprite* create(std::string const& text);
};
