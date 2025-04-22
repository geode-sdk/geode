#include <Geode/platform/cplatform.h>
#ifdef GEODE_IS_MACOS
#include <Geode/loader/Mod.hpp>
#include <Geode/utils/ObjcHook.hpp>
#include <objc/NSObject.h>

using namespace geode::prelude;

// This fixes a crash on macOS when the game is closed,
// where the game tries to call Steam API functions
// after the Steam API has been shut down.

void shutdownGameHook(NSObject* self, SEL sel) {
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
        &shutdownGameHook
    )) {
        (void) Mod::get()->claimHook(hook.unwrap());
    }
};
#endif
