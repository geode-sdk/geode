#include <Geode/platform/cplatform.h>
#ifdef GEODE_IS_MACOS
#include <Geode/binding/PlatformToolbox.hpp>
#include <Geode/loader/Mod.hpp>
#include <Geode/utils/ObjcHook.hpp>
#include <objc/message.h>

using namespace geode::prelude;

// This fixes a crash on macOS when the game is closed,
// where the game tries to call Steam API functions
// after the Steam API has been shut down.

void shutdownGameHook(void* self, SEL sel) {
    auto director = CCDirector::sharedDirector();
    director->pause();
    director->getScheduler()->unscheduleAll();
    // call the original
    reinterpret_cast<void(*)(void*, SEL)>(objc_msgSend)(self, sel);
}

// This fixes the game saving twice on exit,
// which can cause issues with mods that handle saving before shutdown.

void applicationShouldTerminateExecHook(void* self, SEL sel) {
    *reinterpret_cast<bool*>(reinterpret_cast<uintptr_t>(self) + 0x19) = true;
    PlatformToolbox::gameDidSave();
}

$execute {
    if (auto hook = ObjcHook::create(
        "AppController",
        "shutdownGame",
        &shutdownGameHook
    )) {
        (void) Mod::get()->claimHook(hook.unwrap());
    }
    if (auto hook = ObjcHook::create(
        "AppController",
        "applicationShouldTerminateExec",
        &applicationShouldTerminateExecHook
    )) {
        (void) Mod::get()->claimHook(hook.unwrap());
    }
};
#endif
