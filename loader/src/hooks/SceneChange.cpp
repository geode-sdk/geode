#include <Geode/ui/SceneEvent.hpp>
#include <Geode/modify/CCDirector.hpp>

using namespace geode::prelude;

struct SceneChangeHook : Modify<SceneChangeHook, cocos2d::CCDirector> {
	void willSwitchToScene(CCScene* scene) {
		SceneEvent().send(scene);
		CCDirector::willSwitchToScene(scene);
	}
};
