#include <Geode/Modify.hpp>
#include <Geode/ui/SceneManager.hpp>

USE_GEODE_NAMESPACE();

class $modify(AchievementNotifier) {
    void willSwitchToScene(CCScene* scene) {
        AchievementNotifier::willSwitchToScene(scene);
        SceneManager::get()->willSwitchToScene(scene);
    }
};
