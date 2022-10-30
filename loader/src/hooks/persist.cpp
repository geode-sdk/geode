#include <Geode/ui/SceneManager.hpp>

USE_GEODE_NAMESPACE();

// clang-format off
#include <Geode/modify/AchievementNotifier.hpp>
class $modify(AchievementNotifier) {
    void willSwitchToScene(CCScene* scene) {
        AchievementNotifier::willSwitchToScene(scene);
        SceneManager::get()->willSwitchToScene(scene);
    }
};
// clang-format on