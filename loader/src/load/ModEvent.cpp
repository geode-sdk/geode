#include <Geode/loader/ModEvent.hpp>

USE_GEODE_NAMESPACE();

ModStateEvent::ModStateEvent(Mod* mod, ModEventType type) : m_mod(mod), m_type(type) {}

ModEventType ModStateEvent::getType() const {
    return m_type;
}

Mod* ModStateEvent::getMod() const {
    return m_mod;
}

PassThrough ModStateEventHandler::handle(ModStateEvent* event) {
    if (event->getMod() == m_mod && event->getType() == m_type) {
        m_consumer();
    }
    return PassThrough::Propagate;
}

ModStateEventHandler::ModStateEventHandler(
    Mod* mod,
    ModEventType type,
    Consumer consumer
) : m_mod(mod), m_type(type), m_consumer(consumer) {}
