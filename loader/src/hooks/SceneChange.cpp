#include <Geode/ui/SceneEvent.hpp>
#include <Geode/modify/CCDirector.hpp>
#include <cocos2d.h>

using namespace geode::prelude;

class SceneChangeHook : Modify<SceneChangeHook, cocos2d::CCDirector> {
	void willSwitchToScene(CCScene* scene) {
		SceneEvent().send(scene);
		CCDirector::willSwitchToScene(scene);
	}
};
