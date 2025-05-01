#include <Geode/loader/Event.hpp>
#include <Geode/utils/ranges.hpp>
#include <mutex>

using namespace geode::prelude;

DefaultEventListenerPool::DefaultEventListenerPool() : m_data(new Data) {}

bool DefaultEventListenerPool::add(EventListenerProtocol* listener) {
    if (!m_data) m_data = std::make_unique<Data>();

    std::unique_lock lock(m_data->m_mutex);
    if (ranges::contains(m_data->m_listeners, listener) || ranges::contains(m_data->m_toAdd, listener)) {
        return false;
    }
    
    if (m_data->m_locked) {
        m_data->m_toAdd.push_back(listener);
    }
    else {
        // insert listeners at the start so new listeners get priority
        m_data->m_listeners.push_front(listener);
    }
    return true;
}

void DefaultEventListenerPool::remove(EventListenerProtocol* listener) {
    if (!m_data) m_data = std::make_unique<Data>();

    std::unique_lock lock(m_data->m_mutex);
    if (m_data->m_locked) {
        for (size_t i = 0 ; i < m_data->m_listeners.size(); i++) {
            if (m_data->m_listeners[i] == listener) {
                m_data->m_listeners[i] = nullptr;
            }
        }
    }
    else {
        ranges::remove(m_data->m_listeners, listener);
    }
    ranges::remove(m_data->m_toAdd, listener);
}

ListenerResult DefaultEventListenerPool::handle(Event* event) {
    if (!m_data) m_data = std::make_unique<Data>();

    auto res = ListenerResult::Propagate;
    m_data->m_locked += 1;
    std::unique_lock lock(m_data->m_mutex);
    for (auto h : m_data->m_listeners) {
        lock.unlock();
        if (h && h->handle(event) == ListenerResult::Stop) {
            res = ListenerResult::Stop;
            lock.lock();
            break;
        }
        lock.lock();
    }
    m_data->m_locked -= 1;
    // only mutate listeners once nothing is iterating 
    // (if there are recursive handle calls)
    if (m_data->m_locked == 0) {
        ranges::remove(m_data->m_listeners, nullptr);
        for (auto listener : m_data->m_toAdd) {
            m_data->m_listeners.push_front(listener);
        }
        m_data->m_toAdd.clear();
    }
    return res;
}

DefaultEventListenerPool* DefaultEventListenerPool::create() {
    return new DefaultEventListenerPool();
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
