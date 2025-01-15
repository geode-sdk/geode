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
        ListenerResult handle(std::function<Callback> fn, GameEvent* event);

        GameEventFilter(GameEventType type);
        GameEventFilter(GameEventFilter const&) = default;
    };
}