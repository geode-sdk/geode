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

	static cocos2d::CCLightning* lightningWithStrikePoint(cocos2d::CCPoint strikePoint, cocos2d::CCPoint strikePoint2, float duration);
	static cocos2d::CCLightning* lightningWithStrikePoint(cocos2d::CCPoint strikePoint);

	bool initWithStrikePoint(cocos2d::CCPoint strikePoint, cocos2d::CCPoint strikePoint2, float duration);
	bool initWithStrikePoint(cocos2d::CCPoint strikePoint) {
		return initWithStrikePoint(strikePoint, {0, 0}, 0.5f);
	}

	void strike();
	void strikeFinished();

	void strikeRandom() {
		m_seed = rand();
		this->strike();
	}

	void strikeWithSeed(uint64_t seed) {
		m_seed = seed;
		this->strike();
	}

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
	cocos2d::_ccColor3B const& getColor();
	cocos2d::_ccColor3B const& getDisplayedColor();
	void setColor(cocos2d::_ccColor3B const&);
	void updateDisplayedColor(cocos2d::_ccColor3B const&);
	bool isCascadeOpacityEnabled();
	void setCascadeColorEnabled(bool);
    
protected:
    cocos2d::CCPoint m_strikePoint;
	cocos2d::CCPoint m_strikePoint2;
	bool m_split;
	PAD(7);
	uint32_t m_displacement;
	uint64_t m_seed;
	float m_lineWidth;
	PAD(4);
	float m_duration;
	float m_extraOpacity;
	cocos2d::CCPoint* m_lightningPoints;
	uint32_t m_numPoints;
	uint8_t m_unk148;
	uint8_t m_opacity;
	PAD(3);
	cocos2d::CCColor3B m_tColor;
	PAD(3);
};

NS_CC_END


#endif //__CCLIGHTNING_H__