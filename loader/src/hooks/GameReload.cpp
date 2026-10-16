#include <Geode/loader/GameEvent.hpp>
#include <Geode/modify/GameManager.hpp>

using namespace geode::prelude;

struct GameReloadHook : Modify<GameReloadHook, GameManager> {
    GEODE_FORWARD_COMPAT_DISABLE_HOOKS("GameManager textures unloaded event")

    void reloadAllStep5() {
        GameManager::reloadAllStep5();
        GameEvent(GameEventType::TexturesUnloaded).send();
    }
};
