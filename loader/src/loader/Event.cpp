#include <Geode/loader/Event.hpp>
#include <Geode/utils/ranges.hpp>
#include <mutex>

using namespace geode::prelude;

bool DefaultEventListenerPool::add(EventListenerProtocol* listener) {
    if (m_locked) {
        m_toAdd.insert(listener);
    }
    else {
        m_listeners.push_back(listener);
    }
    return true;
}

void DefaultEventListenerPool::remove(EventListenerProtocol* listener) {
    if (m_locked) {
        m_toRemove.insert(listener);
    }
    else {
        ranges::remove(m_listeners, listener);
    }
}

void DefaultEventListenerPool::handle(Event* event) {
    m_locked += 1;
    for (auto h : m_listeners) {
        // if an event listener gets destroyed in the middle of this loop, we 
        // need to handle that
        if (m_toRemove.contains(h)) continue;
        if (h->handle(event) == ListenerResult::Stop) {
            break;
        }
    }
    m_locked -= 1;
    // only clear listeners once nothing is iterating (if there are recursive handle calls)
    if (m_locked == 0) {
        for (auto listener : m_toAdd) {
            m_listeners.push_back(listener);
        }
        for (auto listener : m_toRemove) {
            ranges::remove(m_listeners, listener);
        }
        m_toAdd.clear();
        m_toRemove.clear();
    }
}

DefaultEventListenerPool* DefaultEventListenerPool::get() {
    static auto inst = new DefaultEventListenerPool();
    return inst;
}

EventListenerPool* EventListenerPool::getDefault() {
    return DefaultEventListenerPool::get();
}

EventListenerPool* EventListenerProtocol::getPool() const {
    return EventListenerPool::getDefault();
}

bool EventListenerProtocol::enable() {
    return this->getPool()->add(this);
}

void EventListenerProtocol::disable() {
    this->getPool()->remove(this);
}

EventListenerProtocol::~EventListenerProtocol() {
    this->disable();
}

Event::~Event() {}

EventListenerPool* Event::getPool() const {
    return EventListenerPool::getDefault();
}

void Event::postFrom(Mod* m) {
    if (m) this->sender = m;
    this->getPool()->handle(this);
}
