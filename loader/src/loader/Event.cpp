#include <Geode/loader/Event.hpp>

USE_GEODE_NAMESPACE();

std::unordered_set<EventListenerProtocol*> Event::s_listeners = {};

void EventListenerProtocol::enable() {
    std::cout << "enable " << this << ": " << typeid(*this).name() << "\n";
    Event::s_listeners.insert(this);
}

void EventListenerProtocol::disable() {
    std::cout << "disable " << this << "\n";
    Event::s_listeners.erase(this);
}

EventListenerProtocol::~EventListenerProtocol() {
    this->disable();
}

Event::~Event() {}

void Event::postFrom(Mod* m) {
    if (m) this->sender = m;

    for (auto h : Event::s_listeners) {
        try {
            std::cout << h << ": " << typeid(*h).name() << "\n";
            if (h->passThrough(this) == ListenerResult::Stop) {
                break;
            }
        } catch(std::exception& e) {
            std::cout << "fuck: " << h << ": " << e.what() << "\n";
        }
    }
}
