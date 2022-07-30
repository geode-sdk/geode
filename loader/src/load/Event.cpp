#include <Geode/loader/Event.hpp>
#include <Geode/utils/vector.hpp>

USE_GEODE_NAMESPACE();

std::vector<BasicEventHandler*> Event::handlers = {};

void BasicEventHandler::listen() {
	if (!vector_utils::contains(Event::handlers, this))
		Event::handlers.push_back(this);
}

void BasicEventHandler::unlisten() {
	vector_utils::erase(Event::handlers, this);
}

Event::~Event() {}

void Event::postFrom(Mod* m) {
	if (m)
		m_sender = m;

	for (auto h : Event::handlers) {
		if (!h->onEvent(this))
			break;
	}
}

std::vector<BasicEventHandler*> const& Event::getHandlers() {
	return Event::handlers;
}
