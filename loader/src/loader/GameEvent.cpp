#include <Geode/loader/GameEvent.hpp>

using namespace geode::prelude;

GameEvent::GameEvent(GameEventType type) : m_type(type) {}

GameEventType GameEvent::getType() const {
    return m_type;
}

ListenerResult GameEventFilter::handle(geode::Function<Callback>& fn, GameEvent* event) {
    if (event->getType() == m_type) {
        fn(event);
    }
    return ListenerResult::Propagate;
}

GameEventFilter::GameEventFilter(GameEventType type) : m_type(type) {}