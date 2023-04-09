#include <Geode/loader/Event.hpp>
#include <Geode/utils/ranges.hpp>
#include <mutex>

using namespace geode::prelude;

bool DefaultEventListenerPool::add(EventListenerProtocol* listener) {
    if (m_locked) {
        m_toAdd.push_back(listener);
    }
    else {
        // insert listeners at the start so new listeners get priority
        m_listeners.insert(m_listeners.begin(), listener);
    }
    return true;
}

void DefaultEventListenerPool::remove(EventListenerProtocol* listener) {
    for (size_t i = 0; i < m_listeners.size(); i++) {
        if (m_listeners[i] == listener) {
            m_listeners[i] = nullptr;
        }
    }
}

ListenerResult DefaultEventListenerPool::handle(Event* event) {
    auto res = ListenerResult::Propagate;
    m_locked += 1;
    for (auto h : m_listeners) {
        // if an event listener gets destroyed in the middle of this loop, it 
        // gets set to null
        if (h && h->handle(event) == ListenerResult::Stop) {
            res = ListenerResult::Stop;
            break;
        }
    }
    m_locked -= 1;
    // only mutate listeners once nothing is iterating 
    // (if there are recursive handle calls)
    if (m_locked == 0) {
        ranges::remove(m_listeners, nullptr);
        for (auto listener : m_toAdd) {
            m_listeners.insert(m_listeners.begin(), listener);
        }
        m_toAdd.clear();
    }
    return res;
}

DefaultEventListenerPool* DefaultEventListenerPool::get() {
    static auto inst = new DefaultEventListenerPool();
    return inst;
}

EventListenerPool* EventListenerProtocol::getPool() const {
    return DefaultEventListenerPool::get();
}

bool EventListenerProtocol::enable() {
    // virtual calls from destructors always call the base class so we gotta 
    // store the subclass' pool in a member to be able to access it in disable
    // this is actually better because now regardless of what getPool() does 
    // we can always be assured that whatever pool it returns this listener 
    // will be removed from that pool and can't be in multiple pools at once
    if (m_pool || !(m_pool = this->getPool())) {
        return false;
    }
    return m_pool->add(this);
}

void EventListenerProtocol::disable() {
    if (m_pool) {
        m_pool->remove(this);
        m_pool = nullptr;
    }
}

EventListenerProtocol::~EventListenerProtocol() {
    this->disable();
}

Event::~Event() {}

EventListenerPool* Event::getPool() const {
    return DefaultEventListenerPool::get();
}

ListenerResult Event::postFromMod(Mod* m) {
    if (m) this->sender = m;
    return this->getPool()->handle(this);
}
