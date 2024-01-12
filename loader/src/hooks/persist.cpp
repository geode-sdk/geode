#include <Geode/ui/SceneManager.hpp>

using namespace geode::prelude;

#include <Geode/modify/AchievementNotifier.hpp>

struct SceneSwitch : Modify<SceneSwitch, AchievementNotifier> {
    static void onModify(const auto& self) {
        if (!Loader::get()->isForwardCompatMode())
            return;
        log::warn("persist disabled in forward compat");
        for (const auto& [_, hook] : self.m_hooks) {
            hook->setAutoEnable(false);
        }
    }

    void willSwitchToScene(CCScene* scene) {
        AchievementNotifier::willSwitchToScene(scene);
        SceneManager::get()->willSwitchToScene(scene);
    }
};
