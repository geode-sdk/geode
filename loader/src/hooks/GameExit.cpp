#include <Geode/loader/GameEvent.hpp>
#include <Geode/modify/CCDirector.hpp>
#include <Geode/modify/PlatformToolbox.hpp>
#include <cocos2d.h>

#ifdef GEODE_IS_MACOS
#include <Geode/utils/ObjcHook.hpp>
#include <objc/message.h>
#endif

using namespace geode::prelude;

namespace geode {

static void triggerEvent() {
    GameEvent(GameEventType::Exiting).send();
}

#ifdef GEODE_IS_MACOS

void gameDidSaveHook(id self, SEL sel) {
    // gameDidSave only prompts to close the game when saveCalled_ is true
    // (this is set in applicationShouldTerminateExec)
    // unlike the previous PlatformToolbox::shutdownGame hook, this is always called when the game closes

    auto iVar = object_getInstanceVariable(self, "saveCalled_", nullptr);
    if (iVar) {
        auto offset = ivar_getOffset(iVar);
        auto saveCalled = *reinterpret_cast<bool*>(reinterpret_cast<uintptr_t>(self) + offset);

        if (saveCalled) {
            triggerEvent();
        }
    } else {
        geode::log::warn("gameDidSaveHook: Failed to get the offset of saveCalled_ to check close event");
    }

    reinterpret_cast<void(*)(id, SEL)>(objc_msgSend)(self, sel);
}

$execute {
    if (auto hook = ObjcHook::create(
        "AppController",
        "gameDidSave",
        &gameDidSaveHook
    )) {
        (void) Mod::get()->claimHook(hook.unwrap());
    }
};

#else

struct GameExitHook : Modify<GameExitHook, CCDirector> {
    void purgeDirector() {
        triggerEvent();
        CCDirector::purgeDirector();
    }
};

#endif

}
