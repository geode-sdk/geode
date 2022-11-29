#include <Geode/loader/Event.hpp>

USE_GEODE_NAMESPACE();

std::unordered_set<EventListenerProtocol*> Event::s_listeners = {};

void EventListenerProtocol::enable() {
    Event::s_listeners.insert(this);
}

void EventListenerProtocol::disable() {
    Event::s_listeners.erase(this);
}

EventListenerProtocol::~EventListenerProtocol() {
	this->disable();
}

Event::~Event() {}

void Event::postFrom(Mod* m) {
    if (m) m_sender = m;

    for (auto h : Event::s_listeners) {
        if (h->passThrough(this) == ListenerResult::Stop) {
            break;
        }
    }
}

Mod* Event::getSender() {
    return m_sender;
}
