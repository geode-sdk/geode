#include <Geode/modify/AchievementNotifier.hpp>
#include <Geode/ui/SceneManager.hpp>

struct SceneSwitch : Modify<SceneSwitch, AchievementNotifier> {
    GEODE_FORWARD_COMPAT_DISABLE_HOOKS("Persist Disabled")
    void willSwitchToScene(CCScene* scene) {
        AchievementNotifier::willSwitchToScene(scene);
        SceneManager::get()->willSwitchToScene(scene);
    }
};
