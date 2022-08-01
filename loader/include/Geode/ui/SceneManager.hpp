#pragma once

#include <Geode/Geode.hpp>

namespace geode {
    class GEODE_DLL SceneManager {
    protected:
        cocos2d::CCArray* m_persistedNodes;

        bool setup();

        virtual ~SceneManager();

    public:
        static SceneManager* get();

        void keepAcrossScenes(cocos2d::CCNode* node);
        void forget(cocos2d::CCNode* node);

        void willSwitchToScene(cocos2d::CCScene* scene);
    };
}
