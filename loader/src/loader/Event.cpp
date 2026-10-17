#include <Geode/loader/Event.hpp>
#include <Geode/utils/ranges.hpp>
#include <mutex>

using namespace geode::prelude;
using namespace geode::comm;

// Port2Opaque
// // only threadsafe ver for now before i can handle it all
class Port2Opaque::Impl {
    mutable std::mutex m_mutex;

    std::vector<OpaqueCallable::Pair> m_receivers;
    std::vector<OpaqueCallable::Pair> m_toAdd;
    std::vector<ReceiverHandle> m_toRemove;

    size_t m_nextID = 1;
    std::atomic_size_t m_sending = 0;

    ReceiverHandle addReceiver(OpaqueCallable* opaqueCallable, OpaqueCallable::SendFunction opaqueFunction, int priority) noexcept;

    size_t removeReceiver(ReceiverHandle handle) noexcept;

    size_t getReceiverCount() const noexcept;

    void cleanup() noexcept;

    OpaqueCallable::Pair* getCallable(size_t& index) noexcept;
}

Port2Opaque::Port2Opaque() : m_impl(new Port2Opaque::Impl) {}
virtual Port2Opaque::~Port2Opaque() = default;

ReceiverHandle Port2Opaque::Impl::addReceiver(OpaqueCallable* opaqueCallable, OpaqueCallable::SendFunction opaqueFunction, int priority) noexcept {
    auto lock = std::unique_lock<std::mutex>(m_mutex);
    ReceiverHandle handle = static_cast<ReceiverHandle>(m_nextID++);
    if (m_sending > 0) {
        opaqueCallable->setPriority(priority);
        opaqueCallable->setHandle(handle);
        // geode::console::log(fmt::format("Added handler with id {} to toAdd", handle), Severity::Debug);
        m_toAdd.push_back({opaqueCallable, std::move(opaqueFunction)});
        return handle;
    }
    for (auto it = m_receivers->begin(); it != m_receivers->end(); ++it) {
        if (priority < it->getPriority()) {
            opaqueCallable->setPriority(priority);
            opaqueCallable->setHandle(handle);
            // geode::console::log(fmt::format("Added handler with id {} to receivers", handle), Severity::Debug);
            m_receivers.insert(it, {opaqueCallable, std::move(opaqueFunction)});
            return handle;
        }
    }
    m_receivers.push_back({std::move(receiver), priority, handle});
    return handle;
}

size_t Port2Opaque::Impl::removeReceiver(ReceiverHandle handle) noexcept {
    auto lock = std::unique_lock<std::mutex>(m_mutex);
    auto size = m_receivers->size();
    for (int i = 0; i < size; ++i) {
        if (m_receivers[i].m_handle == handle) {
            if (m_sending > 0) {
                // geode::console::log(fmt::format("Added handler with id {} to toRemove", handle), Severity::Debug);
                m_toRemove.push_back(handle);
            } else {
                // geode::console::log(fmt::format("Removed handler with id {} from receivers", handle), Severity::Debug);
                m_receivers.erase(receivers.begin() + i);
            }
            // size - 1, return for symmetry
            return size - 1;
        }
    }
    // size
    return size;
}

void Port2Opaque::Impl::cleanup() noexcept {
    auto lock = std::unique_lock<std::mutex>(m_mutex);
    if (m_sending == 0) {
        // is it n^2? yes, do i care? no
        for (auto handle : m_toRemove) {
            std::erase_if(m_receivers.begin(), m_receivers.end(), [](auto& receiver) {
                return receiver->getHandle() == handle;
            });
        }
        m_toRemove.clear();

        m_receivers->insert(m_receivers.end(), std::make_move_iterator(m_toAdd.begin()), std::make_move_iterator(m_toAdd.end()));
        m_toAdd.clear();

        std::sort(m_receivers.begin(), m_receivers.end(), [](auto& a, auto& b) {
            return a.m_priority < b.m_priority;
        });
    }
}

size_t Port2Opaque::Impl::getReceiverCount() const noexcept {
    auto lock = std::unique_lock<std::mutex>(m_mutex);
    return m_receivers.size() + m_toAdd.size() - m_toRemove.size();
}

OpaqueCallable::Pair* Port2Opaque::getCallable(size_t& index) noexcept {
    if (index >= m_receivers.size()) return nullptr;
    auto* ptr = &m_receivers[index];

    while (std::find(m_toRemove.begin(), m_toRemove.end(), ptr->first->getHandle()) != m_toRemove.end())) {
        ++index;
        if (index >= m_receivers.size()) return nullptr;
        ptr = &m_receivers[index];
    }
    return ptr;
}

ReceiverHandle Port2Opaque::addReceiver(OpaqueCallable* opaqueCallable, OpaqueCallable::SendFunction opaqueFunction, int priority) noexcept {
    return m_impl->addReceiver(opaqueCallable, std::move(opaqueFunction), priority);
}

size_t Port2Opaque::removeReceiver(ReceiverHandle handle) noexcept {
    return m_impl->removeReceiver(handle);
}

size_t Port2Opaque::getReceiverCount() const noexcept {
    return m_impl->getReceiverCount();
}

void Port2Opaque::cleanup() noexcept {
    m_impl->cleanup();
}

void Port2Opaque::incrementSending() noexcept {
    m_impl->m_sending += 1;
}
void Port2Opaque::decrementSending() noexcept {
    m_impl->m_sending -= 1;
}
OpaqueCallable::Pair* Port2Opaque::getCallable(size_t& index) noexcept {
    return m_impl->getCallable(index);
}


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
    // TODO: make this back threadlocal
    static auto s_instance = new EventCenterThreadLocal();
    return s_instance;
}

bool EventCenterThreadLocal::send(BaseFilter const* filter, SendFuncType func, MigrateFuncType migratePort) noexcept {
    // log::debug("EventCenterThreadLocal sending event for filter {}, {}", (void*)filter, cast::getRuntimeTypeName(filter));
    // log::debug("hash {} threadid {}", BaseFilterHash{}(filter), std::this_thread::get_id());

    auto it = m_impl->m_ports.find(filter);
    if (it != m_impl->m_ports.end()) {
        // log::debug("found port for filter {}", (void*)it->first.get());
        if (auto newPort = std::invoke(migratePort, it->second.get())) {
            it->second.reset(newPort);
        }

        auto port = it->second;
        return std::invoke(func, port.get());
    }
    return false;
}
ListenerHandle EventCenterThreadLocal::addReceiver(BaseFilter const* filter, AddFuncType func, MigrateFuncType migratePort) noexcept {
    // log::debug("EventCenterThreadLocal adding receiver for filter {}, {}", (void*)filter, cast::getRuntimeTypeName(filter));
    // log::debug("hash {} threadid {}", BaseFilterHash{}(filter), std::this_thread::get_id());

    auto it = m_impl->m_ports.find(filter);

    // if (std::string(cast::getRuntimeTypeName(filter)).find("UpdateModListStateEvent") != std::string::npos) {
    // if (it == m_impl->m_ports.end()) {
    //     log::debug("in ports, not found");
    // }
    // else {
    //     log::debug("in ports, found {} -> {}, ({})", (void*)it->first.get(), (void*)it->second.get(), cast::getRuntimeTypeName(it->first.get()));
    // }}

    if (it != m_impl->m_ports.end()) {
        if (auto newPort = std::invoke(migratePort, it->second.get())) {
            it->second.reset(newPort);
        }
        return ListenerHandle(it->first, std::invoke(func, it->second.get()), nullptr);
    }
    else {
        auto clonedFilter = Impl::KeyType(filter->clone());
        // if (std::string(cast::getRuntimeTypeName(filter)).find("UpdateModListStateEvent") != std::string::npos) {
        // log::debug("cloned filter {}, typename {}", (void*)clonedFilter.get(), cast::getRuntimeTypeName(clonedFilter.get()));
        // }
        if (!clonedFilter) return ListenerHandle();
        auto filter2 = clonedFilter.get();
        // geode::console::log(fmt::format("Cloned filter for adding receiver {}, {}", (void*)filter2, cast::getRuntimeTypeName(filter2)), Severity::Debug);

        auto port = Impl::ValueType(clonedFilter->getPort());
        if (!port) return ListenerHandle();

        ReceiverHandle handle = std::invoke(func, port.get());
        // if (std::string(cast::getRuntimeTypeName(filter)).find("UpdateModListStateEvent") != std::string::npos) {
        // log::debug("handle {}", handle);
        // }
        auto ret = ListenerHandle(clonedFilter, handle, nullptr);

        m_impl->m_ports.emplace(std::move(clonedFilter), std::move(port));

        auto it2 = m_impl->m_ports.find(filter);
    //     if (std::string(cast::getRuntimeTypeName(filter)).find("UpdateModListStateEvent") != std::string::npos) {
    //     if (it2 == m_impl->m_ports.end()) {
    //         log::debug("added into ports, not found");
    //     }
    //     else {
    //         log::debug("added into ports, found {} -> {}", (void*)it2->first.get(), (void*)it2->second.get());
    //     }
    // }
        return ret;
    }
}
size_t EventCenterThreadLocal::getReceiverCount(BaseFilter const* filter, SizeFuncType func, MigrateFuncType migratePort) noexcept {
    auto it = m_impl->m_ports.find(filter);
    if (it != m_impl->m_ports.end()) {
        if (auto newPort = std::invoke(migratePort, it->second.get())) {
            it->second.reset(newPort);
        }
        return std::invoke(func, it->second.get());
    }
    return 0;
}
size_t EventCenterThreadLocal::removeReceiver(BaseFilter const* filter, RemoveFuncType func, MigrateFuncType migratePort) noexcept {
    // log::debug("EventCenterThreadLocal removing receiver for filter {}, {}", (void*)filter, cast::getRuntimeTypeName(filter));
    // log::debug("hash {} threadid {}", BaseFilterHash{}(filter), std::this_thread::get_id());

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

        // if (std::string(cast::getRuntimeTypeName(filter)).find("UpdateModListStateEvent") != std::string::npos) {
        //     log::debug("port size {}", size);
        // }
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
    static auto s_instance = new EventCenterGlobal();
    return s_instance;
}

bool EventCenterGlobal::send(BaseFilter const* filter, SendFuncType func, MigrateFuncType migratePort) noexcept {
    // log::debug("EventCenterGlobal sending event for filter {}, {}", (void*)filter, cast::getRuntimeTypeName(filter));

    auto lock = std::unique_lock<std::mutex>(m_impl->m_mutex);
    auto it = m_impl->m_ports.find(filter);
    auto const end = m_impl->m_ports.end();

    if (it != end) {
        if (auto newPort = std::invoke(migratePort, it->second.get())) {
            it->second.reset(newPort);
        }

        auto port = it->second;
        lock.unlock();
        return std::invoke(func, port.get());
    }
    return false;
}
ListenerHandle EventCenterGlobal::addReceiver(BaseFilter const* filter, AddFuncType func, MigrateFuncType migratePort) noexcept {
    // log::debug("EventCenterGlobal adding receiver for filter {}, {}", (void*)filter, cast::getRuntimeTypeName(filter));

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
size_t EventCenterGlobal::getReceiverCount(BaseFilter const* filter, SizeFuncType func, MigrateFuncType migratePort) noexcept {
    auto lock = std::unique_lock<std::mutex>(m_impl->m_mutex);
    auto it = m_impl->m_ports.find(filter);
    auto const end = m_impl->m_ports.end();

    if (it != end) {
        if (auto newPort = std::invoke(migratePort, it->second.get())) {
            it->second.reset(newPort);
        }

        auto port = it->second;
        lock.unlock();
        return std::invoke(func, port.get());
    }
    return 0;
}
size_t EventCenterGlobal::removeReceiver(BaseFilter const* filter, RemoveFuncType func, MigrateFuncType migratePort) noexcept {
    // log::debug("EventCenterGlobal removing receiver for filter {}, {}", (void*)filter, cast::getRuntimeTypeName(filter));

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
