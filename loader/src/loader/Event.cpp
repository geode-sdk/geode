#include <Geode/loader/Event.hpp>
#include <Geode/utils/ranges.hpp>
#include <mutex>

using namespace geode::prelude;
using namespace geode::comm;

// EventCenterThreadLocal

class EventCenterThreadLocal::Impl {
public:
    using KeyType = std::shared_ptr<BaseFilter>;
    using ValueType = std::shared_ptr<OpaquePortBase>;
    using MapType = std::unordered_map<KeyType, ValueType, BaseFilterHash, BaseFilterEqual>;

    MapType m_ports;
};

EventCenterThreadLocal::EventCenterThreadLocal() : m_impl(std::make_unique<Impl>()) {}
EventCenterThreadLocal::~EventCenterThreadLocal() = default;

EventCenterThreadLocal* EventCenterThreadLocal::get() {
    static thread_local EventCenterThreadLocal instance;
    return &instance;
}

bool EventCenterThreadLocal::send(BaseFilter const* filter, OpaqueFuncType func, MigrateFuncType migratePort) noexcept {
    // log::debug("EventCenterThreadLocal sending event for filter {}, {}", (void*)filter, cast::getRuntimeTypeName(filter));

    auto it = m_impl->m_ports.find(filter);
    if (it != m_impl->m_ports.end()) {
        if (auto newPort = std::invoke(migratePort, it->second.get())) {
            it->second.reset(newPort);
        }
        return std::invoke(func, it->second.get());
    }
    return false;
}
ListenerHandle EventCenterThreadLocal::addReceiver(BaseFilter const* filter, OpaqueFuncType func, MigrateFuncType migratePort) noexcept {
    log::debug("EventCenterThreadLocal adding receiver for filter {}, {}", (void*)filter, cast::getRuntimeTypeName(filter));

    auto it = m_impl->m_ports.find(filter);
    if (it != m_impl->m_ports.end()) {
        if (auto newPort = std::invoke(migratePort, it->second.get())) {
            it->second.reset(newPort);
        }
        return ListenerHandle(it->first, std::invoke(func, it->second.get()), nullptr);
    }
    else {
        auto clonedFilter = Impl::KeyType(filter->clone());
        if (!clonedFilter) return ListenerHandle();
        auto filter2 = clonedFilter.get();
        // geode::console::log(fmt::format("Cloned filter for adding receiver {}, {}", (void*)filter2, cast::getRuntimeTypeName(filter2)), Severity::Debug);

        auto port = Impl::ValueType(clonedFilter->getPort());
        if (!port) return ListenerHandle();

        ReceiverHandle handle = std::invoke(func, port.get());
        auto ret = ListenerHandle(clonedFilter, handle, nullptr);

        m_impl->m_ports.emplace(std::move(clonedFilter), std::move(port));
        return ret;
    }
}
size_t EventCenterThreadLocal::getReceiverCount(BaseFilter const* filter, OpaqueFuncType func, MigrateFuncType migratePort) noexcept {
    auto it = m_impl->m_ports.find(filter);
    if (it != m_impl->m_ports.end()) {
        if (auto newPort = std::invoke(migratePort, it->second.get())) {
            it->second.reset(newPort);
        }
        return std::invoke(func, it->second.get());
    }
    return 0;
}
size_t EventCenterThreadLocal::removeReceiver(BaseFilter const* filter, OpaqueFuncType func, MigrateFuncType migratePort) noexcept {
    log::debug("EventCenterThreadLocal removing receiver for filter {}, {}", (void*)filter, cast::getRuntimeTypeName(filter));

    auto it = m_impl->m_ports.find(filter);
    if (it != m_impl->m_ports.end()) {
        if (auto newPort = std::invoke(migratePort, it->second.get())) {
            it->second.reset(newPort);
        }
        auto size = std::invoke(func, it->second.get());
        if (size == 0) {
            // geode::console::log(fmt::format("Removing port for filter type {}", cast::getRuntimeTypeName(filter)), Severity::Debug);
            m_impl->m_ports.erase(it);
        }
        return size;
    }
    return (size_t)-1;
}

// EventCenterGlobal

class EventCenterGlobal::Impl {
public:
    using KeyType = std::shared_ptr<BaseFilter>;
    using ValueType = std::shared_ptr<OpaquePortBase>;
    using MapType = std::unordered_map<KeyType, ValueType, BaseFilterHash, BaseFilterEqual>;

    std::mutex m_mutex;
    MapType m_ports;
};

EventCenterGlobal::EventCenterGlobal() : m_impl(std::make_unique<Impl>()) {}
EventCenterGlobal::~EventCenterGlobal() = default;

EventCenterGlobal* EventCenterGlobal::get() {
    static EventCenterGlobal instance;
    return &instance;
}

bool EventCenterGlobal::send(BaseFilter const* filter, OpaqueFuncType func, MigrateFuncType migratePort) noexcept {
    // log::debug("EventCenterGlobal sending event for filter {}, {}", (void*)filter, cast::getRuntimeTypeName(filter));

    auto lock = std::unique_lock<std::mutex>(m_impl->m_mutex);
    auto it = m_impl->m_ports.find(filter);
    auto const end = m_impl->m_ports.end();
    lock.unlock();

    if (it != end) {
        if (auto newPort = std::invoke(migratePort, it->second.get())) {
            it->second.reset(newPort);
        }
        return std::invoke(func, it->second.get());
    }
    return false;
}
ListenerHandle EventCenterGlobal::addReceiver(BaseFilter const* filter, OpaqueFuncType func, MigrateFuncType migratePort) noexcept {
    log::debug("EventCenterGlobal adding receiver for filter {}, {}", (void*)filter, cast::getRuntimeTypeName(filter));

    auto lock = std::unique_lock<std::mutex>(m_impl->m_mutex);
    auto it = m_impl->m_ports.find(filter);
    auto const end = m_impl->m_ports.end();

    if (it != end) {
        if (auto newPort = std::invoke(migratePort, it->second.get())) {
            it->second.reset(newPort);
        }
        return ListenerHandle(it->first, std::invoke(func, it->second.get()), nullptr);
    }
    else {
        auto clonedFilter = Impl::KeyType(filter->clone());
        if (!clonedFilter) return ListenerHandle();
        auto filter2 = clonedFilter.get();
        // geode::console::log(fmt::format("Cloned filter for adding receiver {}, {}", (void*)filter2, cast::getRuntimeTypeName(filter2)), Severity::Debug);

        auto port = Impl::ValueType(clonedFilter->getPort());
        if (!port) return ListenerHandle();

        ReceiverHandle handle = std::invoke(func, port.get());
        auto ret = ListenerHandle(clonedFilter, handle, nullptr);

        m_impl->m_ports.emplace(std::move(clonedFilter), std::move(port));
        return ret;
    }
}
size_t EventCenterGlobal::getReceiverCount(BaseFilter const* filter, OpaqueFuncType func, MigrateFuncType migratePort) noexcept {
    auto lock = std::unique_lock<std::mutex>(m_impl->m_mutex);
    auto it = m_impl->m_ports.find(filter);
    auto const end = m_impl->m_ports.end();
    lock.unlock();
    
    if (it != end) {
        if (auto newPort = std::invoke(migratePort, it->second.get())) {
            it->second.reset(newPort);
        }
        return std::invoke(func, it->second.get());
    }
    return 0;
}
size_t EventCenterGlobal::removeReceiver(BaseFilter const* filter, OpaqueFuncType func, MigrateFuncType migratePort) noexcept {
    log::debug("EventCenterGlobal removing receiver for filter {}, {}", (void*)filter, cast::getRuntimeTypeName(filter));

    auto lock = std::unique_lock<std::mutex>(m_impl->m_mutex);
    auto it = m_impl->m_ports.find(filter);
    auto const end = m_impl->m_ports.end();

    if (it != end) {
        if (auto newPort = std::invoke(migratePort, it->second.get())) {
            it->second.reset(newPort);
        }
        auto size = std::invoke(func, it->second.get());
        if (size == 0) {
            // geode::console::log(fmt::format("Removing port for filter type {}", cast::getRuntimeTypeName(filter)), Severity::Debug);
            m_impl->m_ports.erase(it);
        }
        return size;
    }
    return (size_t)-1;
}