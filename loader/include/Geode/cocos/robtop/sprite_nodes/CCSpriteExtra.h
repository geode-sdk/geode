#ifndef __CCSPRITEEXTRA_H__
#define __CCSPRITEEXTRA_H__

#include "../../include/ccMacros.h"
#include "../../sprite_ndoes/CCSprite.h"

NS_CC_BEGIN

// @note RobTop Addition

class CC_DLL CCSpriteExtra : public CCSprite {
public:
    CCSpriteExtra() {}
    GEODE_CUSTOM_CONSTRUCTOR_COCOS(CCSpriteExtra, CCSprite);

    inline float getXOffset() const { return m_fXOffset; }
    inline float getYOffset() const { return m_fYOffset; }

    inline void setXOffset(float offset) { m_fXOffset = offset; }
    inline void setYOffset(float offset) { m_fYOffset = offset; }

protected:
    float m_fXOffset;
    float m_fYOffset;
};

NS_CC_END

#endif // __CCSPRITEEXTRA_H__
