#include <Geode/ui/SceneManager.hpp>

using namespace geode::prelude;

#include <Geode/modify/AchievementNotifier.hpp>

struct SceneSwitch : Modify<SceneSwitch, AchievementNotifier> {
    void willSwitchToScene(CCScene* scene) {
        AchievementNotifier::willSwitchToScene(scene);
        SceneManager::get()->willSwitchToScene(scene);
    }
};
