#ifndef __CCLIGHTNING_H__
#define __CCLIGHTNING_H__

#include "../../include/ccMacros.h"
#include "../../base_nodes/CCNode.h"

NS_CC_BEGIN

// @note RobTop Addition
class CCLightning : public CCNode, public CCRGBAProtocol {
public:
    CCLightning();
    virtual ~CCLightning();
    GEODE_CUTOFF_CONSTRUCTOR_GD(CCLightning, CCNode);

    static CCLightning* lightningWithStrikePoint(CCPoint strikePoint, CCPoint strikePoint2, float duration);
    static CCLightning* lightningWithStrikePoint(CCPoint strikePoint);

    bool initWithStrikePoint(CCPoint strikePoint, CCPoint strikePoint2, float duration);
    bool initWithStrikePoint(CCPoint strikePoint);

    void strike();
    void strikeFinished();

    void strikeRandom();
    void strikeWithSeed(uint64_t seed);

    void draw();

    // cocos2d::CCRGBAProtocol
    bool isOpacityModifyRGB();
    void setOpacityModifyRGB(bool);
    unsigned char getOpacity();
    unsigned char getDisplayedOpacity();
    void setOpacity(unsigned char);
    void updateDisplayedOpacity(unsigned char);
    bool isCascadeColorEnabled();
    void setCascadeOpacityEnabled(bool);
    ccColor3B const& getColor();
    ccColor3B const& getDisplayedColor();
    void setColor(ccColor3B const&);
    void updateDisplayedColor(ccColor3B const&);
    bool isCascadeOpacityEnabled();
    void setCascadeColorEnabled(bool);

protected:
    CCPoint m_strikePoint;
    CCPoint m_strikePoint2;
    bool m_split;
    int m_displacement;
    int m_minDisplacement;
    uint64_t m_seed;
    float m_lineWidth;
    bool m_unkBool;
    bool m_removeAfterFinished;
    float m_duration;
    float m_opacityModify;
    std::array<CCPoint, 200>* m_lightningPoints;
    uint32_t m_numPoints;
    uint8_t m_displayedOpacity;
    uint8_t m_opacity;
    ccColor3B m_displayedColor;
    ccColor3B m_color;
    bool m_cascadeColorEnabled;
    bool m_cascadeOpacityEnabled;
    bool m_opacityModifyEnabled;
};

NS_CC_END

#endif //__CCLIGHTNING_H__
