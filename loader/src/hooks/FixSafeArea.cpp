#include <Geode/Geode.hpp>

#ifdef GEODE_IS_IOS
#include <Geode/modify/AppDelegate.hpp>

using namespace geode::prelude;

class $modify(FixSafeAreaAppDelegate, AppDelegate) {
    void setupGLView() {
        AppDelegate::setupGLView();
        m_needsSafeArea = geode::utils::getSafeAreaRect().size != CCDirector::get()->getWinSize();
    }
};
#endif
