#include <Geode/ui/SceneManager.hpp>
#include <Geode/utils/cocos.hpp>

using namespace geode::prelude;

bool SceneManager::setup() {
    m_persistedNodes = CCArray::create();
    m_persistedNodes->retain();
    return true;
}

SceneManager* SceneManager::get() {
    static SceneManager* inst = nullptr;
    if (!inst) {
        inst = new SceneManager();
        inst->setup();
    }
    return inst;
}

SceneManager::~SceneManager() {
    m_persistedNodes->release();
}

void SceneManager::keepAcrossScenes(CCNode* node) {
    if (m_lastScene) {
        node->removeFromParentAndCleanup(false);
        m_lastScene->addChild(node);
    }
    m_persistedNodes->addObject(node);
}

void SceneManager::forget(CCNode* node) {
    m_persistedNodes->removeObject(node);
}

void SceneManager::willSwitchToScene(CCScene* scene) {
    for (auto node : CCArrayExt<CCNode>(m_persistedNodes)) {
        // no cleanup in order to keep actions running
        node->removeFromParentAndCleanup(false);
        scene->addChild(node);
    }
    m_lastScene = scene;
}
