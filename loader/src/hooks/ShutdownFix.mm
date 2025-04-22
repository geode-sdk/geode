#include <Geode/platform/cplatform.h>
#ifdef GEODE_IS_MACOS
#include <Geode/utils/ObjcHook.hpp>
#include <objc/runtime.h>

using namespace geode::prelude;

// This fixes a crash on macOS when the game is closed,
// where the game tries to call Steam API functions
// after the Steam API has been shut down.

void shutdownGameHook(id self, SEL sel) {
    auto director = CCDirector::sharedDirector();
    director->pause();
    director->getScheduler()->unscheduleAll();
    // call the original
    [self performSelector:sel];
}

$execute {
    if (auto hook = ObjcHook::create(
        "AppController",
        "shutdownGame",
        shutdownGameHook,
    )) {
        Mod::get()->claimHook(hook);
    }
};
#endif
