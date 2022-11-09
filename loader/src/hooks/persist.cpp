#include <Geode/ui/SceneManager.hpp>

USE_GEODE_NAMESPACE();

#include <Geode/modify/AchievementNotifier.hpp>

struct SceneSwitch : Modify<SceneSwitch, AchievementNotifier> {
    void willSwitchToScene(CCScene* scene) {
        AchievementNotifier::willSwitchToScene(scene);
        SceneManager::get()->willSwitchToScene(scene);
    }
};
