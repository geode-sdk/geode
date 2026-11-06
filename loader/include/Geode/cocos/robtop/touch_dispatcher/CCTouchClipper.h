#pragma once
#include "../../cocoa/CCGeometry.h"

NS_CC_BEGIN

class CC_DLL CCTouchClipper
{
    GEODE_FRIEND_MODIFY

public:
    virtual ~CCTouchClipper() {}
    virtual bool touchInsideClip(cocos2d::CCPoint const&) = 0;
};

NS_CC_END
