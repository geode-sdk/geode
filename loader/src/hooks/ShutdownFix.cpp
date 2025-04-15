#include <Geode/platform/cplatform.h>
#ifdef GEODE_IS_MACOS
#include <Geode/loader/Mod.hpp>
#include <objc/runtime.h>

using namespace geode::prelude;

// This fixes a crash on macOS when the game is closed,
// where the game tries to call Steam API functions
// after the Steam API has been shut down.

static void(*s_originalShutdownGame)(void* self, SEL sel);

void shutdownGameHook(void* self, SEL sel) {
    CCDirector::sharedDirector()->getScheduler()->unscheduleAll();
    s_originalShutdownGame(self, sel);
}

$execute {
    auto appController = objc_getClass("AppController");
    if (!appController) return;

    auto shutdownGame = class_getInstanceMethod(appController, sel_registerName("shutdownGame"));
    if (!shutdownGame) return;

    s_originalShutdownGame = reinterpret_cast<decltype(s_originalShutdownGame)>(method_getImplementation(shutdownGame));
    if (s_originalShutdownGame) (void)Mod::get()->hook(
        reinterpret_cast<void*>(s_originalShutdownGame),
        &shutdownGameHook,
        "AppController::shutdownGame"
    );
};
#endif
