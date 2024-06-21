#ifndef __CCMOUSE_DELEGATE_H__
#define __CCMOUSE_DELEGATE_H__

#include "../../include/ccMacros.h"
#include "../../cocoa/CCObject.h"

NS_CC_BEGIN

// @note RobTop Addition
class CC_DLL CCMouseDelegate 
{
public:
	virtual void rightKeyDown() {}

	virtual void rightKeyUp() {}

	virtual void scrollWheel(float x, float y) {}

	//pretty certain there's no fields, based on initializer
};

// @note RobTop Addition
class CC_DLL CCMouseHandler : public CCObject
{
public:
	GEODE_CUSTOM_CONSTRUCTOR_COCOS(CCMouseHandler, CCObject)
	inline CCMouseHandler() = default;
	
	virtual ~CCMouseHandler();

	CCMouseHandler& operator=(const CCMouseHandler&);

	CCMouseDelegate* getDelegate();

	static CCMouseHandler* handlerWithDelegate(CCMouseDelegate* pDelegate);

	virtual bool initWithDelegate(CCMouseDelegate* pDelegate);

	void setDelegate(CCMouseDelegate* pDelegate);

protected:
	CCMouseDelegate* m_pDelegate;
};

NS_CC_END

#endif
