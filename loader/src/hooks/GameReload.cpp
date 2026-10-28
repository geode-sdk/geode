#include <Geode/loader/GameEvent.hpp>
#include <Geode/modify/GameManager.hpp>

using namespace geode::prelude;

struct GameReloadHook : Modify<GameReloadHook, GameManager> {
    GEODE_FORWARD_COMPAT_DISABLE_HOOKS("GameManager textures unloaded event")

    void reloadAllStep2() {
        GameManager::reloadAllStep2();
        GameEvent(GameEventType::TexturesUnloaded).send();
    }
};
