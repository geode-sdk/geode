#include <Geode/platform/cplatform.h>
#ifdef GEODE_IS_MACOS
#include <Geode/utils/ObjcHook.hpp>
#include <objc/runtime.h>

using namespace geode::prelude;

// This fixes a crash on macOS when the game is closed,
// where the game tries to call Steam API functions
// after the Steam API has been shut down.

static void(*s_originalShutdownGame)(void* self, SEL sel);

void shutdownGameHook(void* self, SEL sel) {
    auto director = CCDirector::sharedDirector();
    director->pause();
    director->getScheduler()->unscheduleAll();
    s_originalShutdownGame(self, sel);
}

$execute {
    if (auto original = hook::replaceObjcMethod("AppController", "shutdownGame", reinterpret_cast<void*>(&shutdownGameHook))) {
        s_originalShutdownGame = reinterpret_cast<decltype(s_originalShutdownGame)>(original.unwrap());
    }
};
#endif
