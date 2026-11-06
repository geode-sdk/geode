#include <Geode/loader/Event.hpp>
#include <Geode/utils/ranges.hpp>
#include <mutex>
#include <vector>

using namespace geode::prelude;
using namespace geode::comm;

// OpaquePort
// // only threadsafe ver for now before i can handle it all
struct OpaquePort::Impl {
    mutable std::mutex m_mutex;

    std::vector<std::unique_ptr<OpaqueCallable>> m_receivers;
    std::vector<std::unique_ptr<OpaqueCallable>> m_toAdd;
    std::vector<ReceiverHandle> m_toRemove;

    size_t m_nextID = 1;
    size_t m_sending = 0;

    ReceiverHandle addReceiver(OpaqueCallable* opaqueCallable, int priority) noexcept;

    size_t removeReceiver(ReceiverHandle handle) noexcept;

    size_t getReceiverCount() const noexcept;

    void cleanup() noexcept;

    OpaqueCallable* getCallable(size_t& index) noexcept;

    bool send(OpaqueCallable::SendFunction opaqueFunction);
};

OpaquePort::OpaquePort() : m_impl(new OpaquePort::Impl) {}
OpaquePort::~OpaquePort() = default;

ReceiverHandle OpaquePort::Impl::addReceiver(OpaqueCallable* opaqueCallable, int priority) noexcept {
    auto lock = std::unique_lock<std::mutex>(m_mutex);
    ReceiverHandle handle = static_cast<ReceiverHandle>(m_nextID++);
    if (m_sending > 0) {
        opaqueCallable->setPriority(priority);
        opaqueCallable->setHandle(handle);
        // geode::console::log(fmt::format("Added handler with id {} to toAdd", handle), Severity::Debug);
        m_toAdd.emplace_back(opaqueCallable);
        return handle;
    }
    for (auto it = m_receivers.begin(); it != m_receivers.end(); ++it) {
        if (priority < (*it)->getPriority()) {
            opaqueCallable->setPriority(priority);
            opaqueCallable->setHandle(handle);
            // geode::console::log(fmt::format("Added handler with id {} to receivers", handle), Severity::Debug);
            m_receivers.insert(it, std::unique_ptr<OpaqueCallable>(opaqueCallable));
            return handle;
        }
    }
    opaqueCallable->setPriority(priority);
    opaqueCallable->setHandle(handle);
    // geode::console::log(fmt::format("Added handler with id {} to receivers", handle), Severity::Debug);
    m_receivers.emplace_back(opaqueCallable);
    return handle;
}

size_t OpaquePort::Impl::removeReceiver(ReceiverHandle handle) noexcept {
    auto lock = std::unique_lock<std::mutex>(m_mutex);
    auto size = m_receivers.size();
    for (int i = 0; i < size; ++i) {
        if (m_receivers[i]->getHandle() == handle) {
            if (m_sending > 0) {
                // geode::console::log(fmt::format("Added handler with id {} to toRemove", handle), Severity::Debug);
                m_toRemove.push_back(handle);
            } else {
                // geode::console::log(fmt::format("Removed handler with id {} from receivers", handle), Severity::Debug);
                m_receivers.erase(m_receivers.begin() + i);
            }
            // size - 1, return for symmetry
            return size - 1;
        }
    }
    // size
    return size;
}

bool OpaquePort::Impl::send(OpaqueCallable::SendFunction opaqueFunction) {
    auto lock = std::unique_lock<std::mutex>(m_mutex);

    m_sending += 1;
    bool ret = false;
    size_t index = 0;
    while (auto* opaqueCallable = this->getCallable(index)) {
        lock.unlock();
        if (std::invoke(opaqueFunction, opaqueCallable)) {
            ret = true;
            lock.lock();
            break;
        }
        ++index;
        lock.lock();
    }

    m_sending -= 1;

    if (m_sending == 0) {
        // is it n^2? yes, do i care? no
        for (auto handle : m_toRemove) {
            std::erase_if(m_receivers, [&](auto const& a) {
                return a->getHandle() == handle;
            });
        }
        m_toRemove.clear();

        m_receivers.insert(m_receivers.end(), std::make_move_iterator(m_toAdd.begin()), std::make_move_iterator(m_toAdd.end()));
        m_toAdd.clear();

        std::sort(m_receivers.begin(), m_receivers.end(), [](auto const& a, auto const& b) {
            return a->getPriority() < b->getPriority();
        });
    }
    return ret;
}

size_t OpaquePort::Impl::getReceiverCount() const noexcept {
    auto lock = std::unique_lock<std::mutex>(m_mutex);
    return m_receivers.size() + m_toAdd.size() - m_toRemove.size();
}

OpaqueCallable* OpaquePort::Impl::getCallable(size_t& index) noexcept {
    if (index >= m_receivers.size()) return nullptr;
    auto* opaqueCallable = m_receivers[index].get();

    while (std::find(m_toRemove.begin(), m_toRemove.end(), opaqueCallable->getHandle()) != m_toRemove.end()) {
        ++index;
        if (index >= m_receivers.size()) return nullptr;
        opaqueCallable = m_receivers[index].get();
    }
    return opaqueCallable;
}

ReceiverHandle OpaquePort::addReceiver(OpaqueCallable* opaqueCallable, int priority) noexcept {
    return m_impl->addReceiver(opaqueCallable, priority);
}

size_t OpaquePort::removeReceiver(ReceiverHandle handle) noexcept {
    return m_impl->removeReceiver(handle);
}

size_t OpaquePort::getReceiverCount() const noexcept {
    return m_impl->getReceiverCount();
}

bool OpaquePort::send(OpaqueCallable::SendFunction opaqueFunction) {
    return m_impl->send(std::move(opaqueFunction));
}

// EventCenterGlobal

class EventCenterGlobal::Impl {
public:
    using KeyType = std::shared_ptr<BaseFilter>;
    using ValueType = std::unique_ptr<OpaquePort>;
    using MapType = std::unordered_map<KeyType, ValueType, BaseFilterHash, BaseFilterEqual>;

    std::mutex m_mutex;
    MapType m_ports;

    bool send(BaseFilter const* filter, OpaqueCallable::SendFunction sendFunction);
    ListenerHandle addReceiver(BaseFilter const* filter, OpaqueCallable* opaqueCallable, int priority) noexcept;
    size_t getReceiverCount(BaseFilter const* filter) noexcept;
    size_t removeReceiver(BaseFilter const* filter, ReceiverHandle handle) noexcept;
};

EventCenterGlobal::EventCenterGlobal() : m_impl(std::make_unique<Impl>()) {}
EventCenterGlobal::~EventCenterGlobal() = default;

EventCenterGlobal* EventCenterGlobal::get() {
    static auto s_instance = new EventCenterGlobal();
    return s_instance;
}

bool EventCenterGlobal::Impl::send(BaseFilter const* filter, OpaqueCallable::SendFunction sendFunction) {
    // log::debug("EventCenterGlobal sending event for filter {}, {}", (void*)filter, cast::getRuntimeTypeName(filter));

    auto lock = std::unique_lock<std::mutex>(m_mutex);
    auto it = m_ports.find(filter);
    auto const end = m_ports.end();

    if (it != end) {
        auto* opaquePort = it->second.get();
        lock.unlock();
        return opaquePort->send(std::move(sendFunction));
    }
    return false;
}
ListenerHandle EventCenterGlobal::Impl::addReceiver(BaseFilter const* filter, OpaqueCallable* opaqueCallable, int priority) noexcept {
    // log::debug("EventCenterGlobal adding receiver for filter {}, {}", (void*)filter, cast::getRuntimeTypeName(filter));

    auto lock = std::unique_lock<std::mutex>(m_mutex);
    auto it = m_ports.find(filter);
    auto const end = m_ports.end();

    if (it != end) {
        auto* opaquePort = it->second.get();
        auto receiverHandle = opaquePort->addReceiver(opaqueCallable, priority);

        return ListenerHandle(it->first, receiverHandle, nullptr);
    }
    else {
        auto clonedFilter = Impl::KeyType(filter->clone());
        if (!clonedFilter) return ListenerHandle();
        auto filter2 = clonedFilter.get();
        // geode::console::log(fmt::format("Cloned filter for adding receiver {}, {}", (void*)filter2, cast::getRuntimeTypeName(filter2)), Severity::Debug);

        auto opaquePort = std::make_unique<OpaquePort>();

        auto receiverHandle = opaquePort->addReceiver(opaqueCallable, priority);
        auto listenerHandle = ListenerHandle(clonedFilter, receiverHandle, nullptr);

        m_ports.emplace(std::move(clonedFilter), std::move(opaquePort));
        return listenerHandle;
    }
}
size_t EventCenterGlobal::Impl::getReceiverCount(BaseFilter const* filter) noexcept {
    auto lock = std::unique_lock<std::mutex>(m_mutex);
    auto it = m_ports.find(filter);
    auto const end = m_ports.end();

    if (it != end) {
        auto* opaquePort = it->second.get();

        lock.unlock();
        return opaquePort->getReceiverCount();
    }
    return 0;
}
size_t EventCenterGlobal::Impl::removeReceiver(BaseFilter const* filter, ReceiverHandle handle) noexcept {
    // log::debug("EventCenterGlobal removing receiver for filter {}, {}", (void*)filter, cast::getRuntimeTypeName(filter));

    auto lock = std::unique_lock<std::mutex>(m_mutex);
    auto it = m_ports.find(filter);
    auto const end = m_ports.end();

    if (it != end) {
        auto* opaquePort = it->second.get();

        auto size = opaquePort->removeReceiver(handle);
        if (size == 0) {
            // geode::console::log(fmt::format("Removing port for filter type {}", cast::getRuntimeTypeName(filter)), Severity::Debug);
            m_ports.erase(it);
        }
        return size;
    }
    return (size_t)-1;
}

bool EventCenterGlobal::send(BaseFilter const* filter, OpaqueCallable::SendFunction sendFunction) {
    return m_impl->send(filter, sendFunction);
}
ListenerHandle EventCenterGlobal::addReceiver(BaseFilter const* filter, OpaqueCallable* opaqueCallable, int priority) noexcept {
    return m_impl->addReceiver(filter, opaqueCallable, priority);
}
size_t EventCenterGlobal::getReceiverCount(BaseFilter const* filter) noexcept {
    return m_impl->getReceiverCount(filter);
}
size_t EventCenterGlobal::removeReceiver(BaseFilter const* filter, ReceiverHandle handle) noexcept {
    return m_impl->removeReceiver(filter, handle);
}
