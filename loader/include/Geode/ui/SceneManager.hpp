#pragma once

#include "../DefaultInclude.hpp"

#include <cocos2d.h>
#include <vector>
#include <span>
#include <Geode/utils/cocos.hpp>

namespace geode {
    class GEODE_DLL SceneManager final {
    protected:
        std::vector<Ref<cocos2d::CCNode>> m_persistedNodes;
        cocos2d::CCScene* m_lastScene = nullptr;

        virtual ~SceneManager();

    public:
        static SceneManager* get();

        /**
         * Adds a node to the list of persisted nodes, which are kept across scene changes.
         * @param node The node to keep across scenes.
         */
        void keepAcrossScenes(cocos2d::CCNode* node);

        /**
         * Removes a node from the list of persisted nodes.
         * @param node The node to forget.
         */
        void forget(cocos2d::CCNode* node);

        /**
         * Gets a span of the persisted nodes. To add new nodes to the list, use keepAcrossScenes.
         */
        std::span<Ref<cocos2d::CCNode> const> getPersistedNodes();

        // This method should only be called by geode itself
        // TODO(v4): hide this
        void willSwitchToScene(cocos2d::CCScene* scene);
    };
}
