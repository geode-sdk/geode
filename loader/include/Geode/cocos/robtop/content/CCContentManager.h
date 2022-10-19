#ifndef __CCCONTENT_MANAGER_H__
#define __CCCONTENT_MANAGER_H__

#include "../../include/ccMacros.h"
#include "../../cocoa/CCObject.h"
#include "../../cocoa/CCDictionary.h"

RT_ADD(
    // not in cocos2d namespace
    class CC_DLL CCContentManager : public cocos2d::CCObject 
    {
    public:
        static CCContentManager* sharedManager();
        CCContentManager();
        bool init();

        cocos2d::CCDictionary* addDict(const char* dict, bool unk);
        cocos2d::CCDictionary* addDictDS(const char* dict);

        void clearCache();
    };
);

#endif
