#include <Geode/ui/OverlayManager.hpp>
#include <Geode/modify/CCDirector.hpp>
#include <Geode/modify/CCEGLView.hpp>

using namespace geode::prelude;

#ifdef GEODE_IS_WINDOWS
#include <Geode/modify/AppDelegate.hpp>
#else
#include <Geode/modify/AchievementNotifier.hpp>
#endif

namespace geode {

struct DrawOverlay : Modify<DrawOverlay, CCEGLView> {
    GEODE_FORWARD_COMPAT_DISABLE_HOOKS("persist disabled")
    static void onModify(auto& self) {
        if (auto res = self.setHookPriorityPost("cocos2d::CCEGLView::swapBuffers", Priority::First); !res) {
            geode::log::warn("Failed to set hook priority: {}", res.unwrapErr());
        }
    }
    void swapBuffers() {
        OverlayManager::get()->visit();
        CCEGLView::swapBuffers();
    }
};

struct SceneSwitch2 : Modify<SceneSwitch2, CCDirector> {
    GEODE_FORWARD_COMPAT_DISABLE_HOOKS("persist disabled")
    // CCDirector does not call willSwitchToScene in these 2 instances,
    // so we have to do it ourselves to make everything behave as expected
    void popScene() {
        CCDirector::popScene();
    #ifdef GEODE_IS_WINDOWS
        AppDelegate::get()->willSwitchToScene(m_pNextScene);
    #else
        AchievementNotifier::sharedState()->willSwitchToScene(m_pNextScene);
    #endif
    }

    void popToSceneStackLevel(int level) {
        CCDirector::popToSceneStackLevel(level);
    #ifdef GEODE_IS_WINDOWS
        AppDelegate::get()->willSwitchToScene(m_pNextScene);
    #else
        AchievementNotifier::sharedState()->willSwitchToScene(m_pNextScene);
    #endif
    }
};


}
