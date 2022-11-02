#include <Geode/loader/Event.hpp>
#include <Geode/utils/vector.hpp>

USE_GEODE_NAMESPACE();

std::unordered_set<BasicEventHandler*> Event::s_handlers = {};

void BasicEventHandler::listen() {
    Event::s_handlers.insert(this);
}

void BasicEventHandler::unlisten() {
    Event::s_handlers.erase(this);
}

BasicEventHandler::~BasicEventHandler() {
	this->unlisten();
}

Event::~Event() {}

void Event::postFrom(Mod* m) {
    if (m) m_sender = m;

    for (auto h : Event::s_handlers) {
        if (h->passThrough(this) == PassThrough::Stop) {
            break;
        }
    }
}

Mod* Event::getSender() {
    return m_sender;
}

std::unordered_set<BasicEventHandler*> const& Event::getHandlers() {
    return Event::s_handlers;
}
