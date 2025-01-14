#include <Geode/ui/SceneManager.hpp>
#include <Geode/modify/CCDirector.hpp>

using namespace geode::prelude;

#ifdef GEODE_IS_WINDOWS
#include <Geode/modify/AppDelegate.hpp>
#else
#include <Geode/modify/AchievementNotifier.hpp>
#endif

namespace geode {

#ifdef GEODE_IS_WINDOWS
struct SceneSwitch : Modify<SceneSwitch, AppDelegate> {
    GEODE_FORWARD_COMPAT_DISABLE_HOOKS("persist disabled")
    void willSwitchToScene(CCScene* scene) {
        AppDelegate::willSwitchToScene(scene);
        SceneManager::get()->willSwitchToScene(scene);
    }
};

#else
struct SceneSwitch : Modify<SceneSwitch, AchievementNotifier> {
    GEODE_FORWARD_COMPAT_DISABLE_HOOKS("persist disabled")
    void willSwitchToScene(CCScene* scene) {
        AchievementNotifier::willSwitchToScene(scene);
        SceneManager::get()->willSwitchToScene(scene);
    }
};

#endif

struct SceneSwitch2 : Modify<SceneSwitch2, CCDirector> {
    GEODE_FORWARD_COMPAT_DISABLE_HOOKS("persist disabled")
    // CCDirector does not call willSwitchToScene in these 2 instances,
    // so we have to do it ourselves to make everything behave as expected
    void popScene() {
        CCDirector::popScene();
    #ifdef GEODE_IS_WINDOWS
        AppDelegate::get()->willSwitchToScene(m_pNextScene);
    #else
        AchievementNotifier::get()->willSwitchToScene(m_pNextScene);
    #endif
    }

    void popToSceneStackLevel(int level) {
        CCDirector::popToSceneStackLevel(level);
    #ifdef GEODE_IS_WINDOWS
        AppDelegate::get()->willSwitchToScene(m_pNextScene);
    #else
        AchievementNotifier::get()->willSwitchToScene(m_pNextScene);
    #endif
    }
};

}