#include <Geode/ui/SceneManager.hpp>
#include <Geode/utils/cocos.hpp>

USE_GEODE_NAMESPACE();

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
}
