#include <Geode/ui/SceneManager.hpp>
#include <Geode/utils/cocos.hpp>

using namespace geode::prelude;

SceneManager* SceneManager::get() {
    static SceneManager* inst = nullptr;
    if (!inst) {
        inst = new SceneManager();
    }
    return inst;
}

SceneManager::~SceneManager() {}

void SceneManager::keepAcrossScenes(CCNode* node) {
    if (ranges::contains(m_persistedNodes, node)) {
        return;
    }
    if (m_lastScene) {
        node->removeFromParentAndCleanup(false);
        m_lastScene->addChild(node);
    }
    m_persistedNodes.push_back(node);
}

void SceneManager::forget(CCNode* node) {
    std::erase(m_persistedNodes, node);
}

std::span<Ref<CCNode> const> SceneManager::getPersistedNodes() {
    return m_persistedNodes;
}

void SceneManager::willSwitchToScene(CCScene* scene) {
    for (auto& node : m_persistedNodes) {
        // no cleanup in order to keep actions running
        node->removeFromParentAndCleanup(false);
        scene->addChild(node);
    }
    m_lastScene = scene;
}
