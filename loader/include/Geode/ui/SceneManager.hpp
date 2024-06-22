#pragma once

#include "../DefaultInclude.hpp"

#include <cocos2d.h>

namespace cocos2d {
    class CCArray;
    class CCNode;
}

namespace geode {
    class GEODE_DLL SceneManager final {
    protected:
        cocos2d::CCArray* m_persistedNodes;
        cocos2d::CCScene* m_lastScene = nullptr;

        bool setup();

        virtual ~SceneManager();

    public:
        static SceneManager* get();

        void keepAcrossScenes(cocos2d::CCNode* node);
        void forget(cocos2d::CCNode* node);

        void willSwitchToScene(cocos2d::CCScene* scene);
    };
}
