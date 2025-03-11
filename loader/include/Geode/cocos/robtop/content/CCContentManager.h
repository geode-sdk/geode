#ifndef __CCCONTENT_MANAGER_H__
#define __CCCONTENT_MANAGER_H__

#include "../../include/ccMacros.h"
#include "../../cocoa/CCObject.h"
#include "../../cocoa/CCDictionary.h"


// not in cocos2d namespace
// @note RobTop Addition
class CC_DLL CCContentManager : public cocos2d::CCObject 
{
public:
    static CCContentManager* sharedManager();
    CCContentManager();
    bool init();

    cocos2d::CCDictionary* addDict(const char* dict, bool unk);
    cocos2d::CCDictionary* addDictDS(const char* dict);

    void clearCache();
public:
    cocos2d::CCDictionary* m_pDictCache;
};

#endif
