#include <Geode/loader/GameEvent.hpp>
#include <Geode/modify/CCDirector.hpp>
#include <Geode/modify/PlatformToolbox.hpp>
#include <cocos2d.h>

using namespace geode::prelude;

namespace geode {

static void triggerEvent() {
    GameEvent(GameEventType::Exiting).send();
}

#ifdef GEODE_IS_MACOS

struct GameExitHook : Modify<GameExitHook, PlatformToolbox> {
    static void platformShutdown() {
        triggerEvent();
        PlatformToolbox::platformShutdown();
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
