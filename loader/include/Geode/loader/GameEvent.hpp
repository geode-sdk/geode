#pragma once

#include "Event.hpp"

namespace geode {
    enum class GameEventType {
        /// Fired when the game finishes loading, and only once.
        /// The event is triggered right after MenuLayer is initialized and already
        /// available, so popups can be shown.
        Loaded,
    };

    class GEODE_DLL GameEvent final : public Event {
    protected:
        GameEventType m_type;

    public:
        GameEvent(GameEventType type);
        GameEventType getType() const;
    };

    class GEODE_DLL GameEventFilter final : public EventFilter<GameEvent> {
    public:
        using Callback = void(GameEvent*);

    protected:
        GameEventType m_type;

    public:
        ListenerResult handle(geode::Function<Callback>& fn, GameEvent* event);

        GameEventFilter(GameEventType type);
        GameEventFilter(GameEventFilter const&) = default;
    };
}

// clang-format off
#define $on_game(type) \
template<class> void GEODE_CONCAT(geodeExecFunction, __LINE__)(geode::GameEvent*); \
namespace { struct GEODE_CONCAT(ExecFuncUnique, __LINE__) {}; } \
static inline auto GEODE_CONCAT(Exec, __LINE__) = (new geode::EventListener( \
    &GEODE_CONCAT(geodeExecFunction, __LINE__)<GEODE_CONCAT(ExecFuncUnique, __LINE__)>, \
    geode::GameEventFilter(geode::GameEventType::type) \
), 0); \
template<class> void GEODE_CONCAT(geodeExecFunction, __LINE__)(geode::GameEvent*)
// clang-format on
