#include <Geode/Geode.hpp>
#include <Geode/modify/CCScene.hpp>

using namespace geode::prelude;

class $modify(CCSceneCrashFix, CCScene) {
    int getHighestChildZ() {
        if(this->getChildrenCount() == 0) return 0;

        return CCScene::getHighestChildZ();
    }
};