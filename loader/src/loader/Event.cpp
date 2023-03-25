#include <Geode/loader/Event.hpp>
#include <Geode/utils/ranges.hpp>

using namespace geode::prelude;

void EventListenerProtocol::enable() {
    Event::listeners().push_back(this);
}

void EventListenerProtocol::disable() {
    ranges::remove(Event::listeners(), this);
}

EventListenerProtocol::~EventListenerProtocol() {
    this->disable();
}

Event::~Event() {}

void Event::postFrom(Mod* m) {
    if (m) this->sender = m;

    std::vector<EventListenerProtocol*> listeners_copy = Event::listeners();

    for (auto h : listeners_copy) {
        if (h->passThrough(this) == ListenerResult::Stop) {
            break;
        }
    }
}

std::vector<EventListenerProtocol*>& Event::listeners() {
    static std::vector<EventListenerProtocol*> listeners;
    return listeners;
}

void Event::prioritize(EventListenerProtocol* listener) {
    ranges::move(Event::listeners(), listener, 0);
}
