#include <Geode/loader/Event.hpp>

USE_GEODE_NAMESPACE();

void EventListenerProtocol::enable() {
    Event::listeners().insert(this);
}

void EventListenerProtocol::disable() {
    Event::listeners().erase(this);
}

EventListenerProtocol::~EventListenerProtocol() {
    this->disable();
}

Event::~Event() {}

void Event::postFrom(Mod* m) {
    if (m) this->sender = m;

    for (auto h : Event::listeners()) {
        if (h->passThrough(this) == ListenerResult::Stop) {
            break;
        }
    }
}

std::unordered_set<EventListenerProtocol*>& Event::listeners() {
    static std::unordered_set<EventListenerProtocol*> listeners;
    return listeners;
}
