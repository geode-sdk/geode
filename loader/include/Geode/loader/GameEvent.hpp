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

    class GEODE_DLL GameEvent final : public Event<GameEvent, bool(), GameEventType> {
    public:
        using Event::Event;
    };
}

// clang-format off
#define $on_game(type) \
template<class>                                                        \
void GEODE_CONCAT(geodeExecFunction, __LINE__)();                      \
namespace {                                                            \
	struct GEODE_CONCAT(ExecFuncUnique, __LINE__) {};                  \
}                                                                      \
static inline auto GEODE_CONCAT(Exec, __LINE__) =                      \
    geode::GameEvent(geode::GameEventType::type)                       \
    .listen(&GEODE_CONCAT(geodeExecFunction, __LINE__)                 \
        <GEODE_CONCAT(ExecFuncUnique, __LINE__)>).leak();              \
template<class>                                                        \
void GEODE_CONCAT(geodeExecFunction, __LINE__)()
// clang-format on
