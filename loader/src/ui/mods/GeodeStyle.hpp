#pragma once

#include <Geode/DefaultInclude.hpp>

using namespace geode::prelude;

class GeodeButtonSprite : public CCSprite {
protected:
    bool* m_stateSrc = nullptr;
    bool m_state = false;

    bool init(CCSprite* top, bool* state);

    void update(float dt) override;

public:
    static GeodeButtonSprite* create(const char* top, bool* state = nullptr);
    static GeodeButtonSprite* createWithSpriteFrameName(const char* top, bool* state = nullptr);
};
