#pragma once

#include <cocos2d.h>

class CCWaveAction : public cocos2d::CCActionInterval
{
public:
    static CCWaveAction* create(float period, float distX, float distY, float phase);

    bool init(float period, float distX, float distY, float phase);

    void startWithTarget(cocos2d::CCNode* target) override;

    void update(float t) override;

    cocos2d::CCActionInterval* reverse() override;

private:
    float m_distX;
    float m_distY;
    float m_phase;
    float m_baseX;
    float m_baseY;
};