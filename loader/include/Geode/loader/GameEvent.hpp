#pragma once

#include "Event.hpp"

namespace geode {
    enum class GameEventType {
        /// Fired when the game finishes loading, and only once.
        /// The event is triggered right after MenuLayer is initialized and already
        /// available, so popups can be shown.
        Loaded,

        /// Fired when the game is about to exit.
        /// This can be used as a safe place for cleanup of global state,
        /// as it runs before static destructors are called.
        Exiting,

        /// Fired when the game finishes loading all mods.
        ModsLoaded,

        /// Fired when the game finishes unloading all textures.
        TexturesUnloaded,

        /// Fired when the game finishes loading all textures.
        TexturesLoaded,
    };

    class GameEvent final : public Event<GameEvent, bool(), GameEventType> {
    public:
        using Event::Event;
    };
}

#ifndef GEODE_UNITY_NS_ID
#define GEODE_UNITY_NS_ID _test_no_unity
#endif

#define $on_game(type) $execute_base({                                  \
    geode::GameEvent(geode::GameEventType::type)                 \
    .listen(&GEODE_CONCAT(geodeExecFunctionI, __LINE__)).leak(); \
})
