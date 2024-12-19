#ifndef __CCFONTSPRITE_H__
#define __CCFONTSPRITE_H__

#include "../../include/ccMacros.h"
#include "CCSpriteExtra.h"

NS_CC_BEGIN

// @note RobTop Addition
class CC_DLL CCFontSprite : public CCSpriteExtra {
public:
    CCFontSprite() {}
    virtual ~CCFontSprite() {}
    GEODE_CUSTOM_CONSTRUCTOR_COCOS(CCFontSprite, CCSpriteExtra);

public:
    bool m_bUseInstant;
    float m_fInstantTime;
    float m_fDelay;
    float m_fShakesPerSecond;
    float m_fShakeIntensity;
    float m_fShakeElapsed;
    int m_nShakeIndex;
    CCPoint m_obShakePosition;
};

NS_CC_END

#endif // __CCFONTSPRITE_H__
