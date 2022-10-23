#include <Geode/DefaultInclude.hpp>

#ifdef GEODE_IS_MACOS

#include <cocos2d.h>
using namespace cocos2d;

// https://github.com/cocos2d/cocos2d-x/blob/cocos2d-x-2.2.3/cocos2dx/cocoa/CCArray.cpp
void CCArray::exchangeObject(CCObject* object1, CCObject* object2) {
    unsigned int index1 = ccArrayGetIndexOfObject(data, object1);
    if(index1 == UINT_MAX)
    {
        return;
    }

    unsigned int index2 = ccArrayGetIndexOfObject(data, object2);
    if(index2 == UINT_MAX)
    {
        return;
    }

    ccArraySwapObjectsAtIndexes(data, index1, index2);
}

#endif
