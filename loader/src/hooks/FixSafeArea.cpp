#include <Geode/Geode.hpp>

#ifdef GEODE_IS_IOS
#include <Geode/modify/AppDelegate.hpp>

using namespace geode::prelude;

class $modify(FixSafeAreaAppDelegate, AppDelegate) {
    void setupGLView() {
        AppDelegate::setupGLView();
        // just detect if the left inset exists (might be good to change to >= some value but i'm too lazy to see what that value is)
        m_needsSafeArea = geode::utils::getSafeAreaRect().x > 0.0f;
    }
};
#endif
