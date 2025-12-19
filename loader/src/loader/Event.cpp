#include <Geode/loader/Event.hpp>
#include <Geode/utils/ranges.hpp>
#include <mutex>

using namespace geode::prelude;

class DefaultEventListenerPool::Impl {
public:
    std::atomic_size_t m_locked = 0;
    std::mutex m_mutex;
    std::deque<EventListenerProtocol*> m_listeners;
    std::vector<EventListenerProtocol*> m_toAdd;
};

DefaultEventListenerPool::DefaultEventListenerPool() : m_impl(new Impl) {}
DefaultEventListenerPool::~DefaultEventListenerPool() = default;

bool DefaultEventListenerPool::add(EventListenerProtocol* listener) {
    if (!m_impl) m_impl = std::make_unique<Impl>();

    std::unique_lock lock(m_impl->m_mutex);
    if (ranges::contains(m_impl->m_listeners, listener) || ranges::contains(m_impl->m_toAdd, listener)) {
        return false;
    }

    if (m_impl->m_locked) {
        m_impl->m_toAdd.push_back(listener);
    }
    else {
        // insert listeners at the start so new listeners get priority
        m_impl->m_listeners.push_front(listener);
    }
    return true;
}

void DefaultEventListenerPool::remove(EventListenerProtocol* listener) {
    if (!m_impl) m_impl = std::make_unique<Impl>();

    std::unique_lock lock(m_impl->m_mutex);
    if (m_impl->m_locked) {
        for (size_t i = 0 ; i < m_impl->m_listeners.size(); i++) {
            if (m_impl->m_listeners[i] == listener) {
                m_impl->m_listeners[i] = nullptr;
            }
        }
    }
    else {
        ranges::remove(m_impl->m_listeners, listener);
    }
    ranges::remove(m_impl->m_toAdd, listener);
}

ListenerResult DefaultEventListenerPool::handle(Event* event) {
    if (!m_impl) m_impl = std::make_unique<Impl>();

    auto res = ListenerResult::Propagate;
    m_impl->m_locked += 1;
    std::unique_lock lock(m_impl->m_mutex);
    for (auto h : m_impl->m_listeners) {
        lock.unlock();
        if (h && h->handle(event) == ListenerResult::Stop) {
            res = ListenerResult::Stop;
            lock.lock();
            break;
        }
        lock.lock();
    }
    m_impl->m_locked -= 1;
    // only mutate listeners once nothing is iterating
    // (if there are recursive handle calls)
    if (m_impl->m_locked == 0) {
        ranges::remove(m_impl->m_listeners, nullptr);
        for (auto listener : m_impl->m_toAdd) {
            m_impl->m_listeners.push_front(listener);
        }
        m_impl->m_toAdd.clear();
    }
    return res;
}

std::shared_ptr<DefaultEventListenerPool> DefaultEventListenerPool::create() {
    return std::shared_ptr<DefaultEventListenerPool>(new DefaultEventListenerPool(), [](DefaultEventListenerPool* p) { delete p; });
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
