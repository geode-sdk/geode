#pragma once

#include <any>
#include <vector>
#include <concepts>
#include <functional>
#include <variant>
#include <memory>
#include <thread>
#include <algorithm>
#include <mutex>
#include <atomic>
#include <asp/ptr/PtrSwap.hpp>
#include "Log.hpp"

#include "../utils/casts.hpp"

namespace geode::event {
    template <class T>
    struct RefOrVoid {
        using type = void(T const&);
    };
    template <>
    struct RefOrVoid<void> {
        using type = void();
    };
    template <class T>
    using RefOrVoidType = typename RefOrVoid<T>::type;

    using ReceiverHandle = size_t;

    template <class Callable>
    struct PortCallable {
        Callable m_callable;
        int m_priority;
        ReceiverHandle m_handle;

        bool operator<(PortCallable const& other) const noexcept {
            return m_priority < other.m_priority;
        }

        template <class ...Args>
        bool call(Args&&... args) const noexcept(std::is_nothrow_invocable_v<Callable, Args...>) {
            if constexpr (std::is_same_v<void, decltype(std::invoke(m_callable, std::forward<Args>(args)...))>) {
                std::invoke(m_callable, std::forward<Args>(args)...);
                return false;
            }
            return std::invoke(m_callable, std::forward<Args>(args)...);
        }
    };

    template <class Callable>
    class Port {
    protected:
        std::vector<PortCallable<Callable>> m_receivers;
    public:
        ReceiverHandle addReceiver(Callable receiver, int priority = 0) noexcept {
            ReceiverHandle handle = m_receivers.empty() ? 1 : m_receivers.back().handle + 1;
            for (auto it = m_receivers.begin(); it != m_receivers.end(); ++it) {
                if (priority < it->m_priority) {
                    m_receivers.insert(it, {std::move(receiver), priority, handle});
                    return handle;
                }
            }
            m_receivers.push_back({std::move(receiver), priority, handle});
            return handle;
        }

        void removeReceiver(ReceiverHandle handle) noexcept {
            for (int i = 0; i < m_receivers.size(); ++i) {
                if (m_receivers[i].handle == handle) {
                    m_receivers.erase(m_receivers.begin() + i);
                    return;
                }
            }
        }

        template <class ...Args>
        requires std::invocable<Callable, Args...>
        void send(Args&&... value) const noexcept(std::is_nothrow_invocable_v<Callable, Args...>) {
            for (auto& callable : m_receivers) {
                if (callable.call(std::forward<Args>(value)...)) {
                    break;
                }
            }
        }
    };

    template <class Callable>
    class OncePort : protected Port<Callable> {
        bool m_sent = false;
    public:
        using Port<Callable>::addReceiver;
        using Port<Callable>::removeReceiver;

        template <class ...Args>
        void send(Args&&... args) noexcept(std::is_nothrow_invocable_v<Callable, Args...>) {
            if (m_sent) return;
            m_sent = true;
            Port<Callable>::send(std::forward<Args>(args)...);
        }
        bool isSent() const noexcept {
            return m_sent;
        }
    };

    template <class Callable>
    class QueuedPort : protected Port<Callable> {
        std::vector<std::function<void()>> m_queue;
    public:
        using Port<Callable>::addReceiver;
        using Port<Callable>::removeReceiver;

        template <class ...Args>
        requires std::invocable<Callable, Args...>
        void send(Args&&... args) const noexcept(std::is_nothrow_invocable_v<Callable, Args...>) {
            m_queue.push_back([=, this] { 
                Port<Callable>::send(std::forward<Args>(args)...); 
            });
        }

        void flush() noexcept {
            for (auto& q : m_queue) {
                std::invoke(q);
            }
            m_queue.clear();
        }
    };

    template <class Callable>
    class QueuedOncePort : protected QueuedPort<Callable> {
        bool m_sent = false;
    public:
        using QueuedPort<Callable>::addReceiver;
        using QueuedPort<Callable>::removeReceiver;
        using QueuedPort<Callable>::flush;

        template <class ...Args>
        void send(Args&&... args) noexcept(std::is_nothrow_invocable_v<Callable, Args...>) {
            if (m_sent) return;
            m_sent = true;
            QueuedPort<Callable>::send(std::forward<Args>(args)...);
        }
        bool isSent() const noexcept {
            return m_sent;
        }
    };

    template <class Callable>
    class ThreadsafePort  {
        using VectorType = std::vector<PortCallable<Callable>>;
        asp::PtrSwap<VectorType> m_receivers;
    public:
        ThreadsafePort() : m_receivers(asp::make_shared<VectorType>()) {}

        ReceiverHandle addReceiver(Callable receiver, int priority = 0) noexcept {
            auto currentReceivers = m_receivers.load();
            auto newReceivers = asp::make_shared<VectorType>(*currentReceivers.get());
            ReceiverHandle handle = newReceivers->empty() ? 1 : newReceivers->back().m_handle + 1;
            for (auto it = newReceivers->begin(); it != newReceivers->end(); ++it) {
                if (priority < it->m_priority) {
                    newReceivers->insert(it, {std::move(receiver), priority, handle});
                    m_receivers.store(std::move(newReceivers));
                    return handle;
                }
            }
            newReceivers->push_back({std::move(receiver), priority, handle});
            m_receivers.store(std::move(newReceivers));
            return handle;
        }

        size_t removeReceiver(ReceiverHandle handle) noexcept {
            auto currentReceivers = m_receivers.load();
            auto newReceivers = asp::make_shared<VectorType>(*currentReceivers.get());
            auto size = newReceivers->size();
            for (int i = 0; i < size; ++i) {
                if ((*newReceivers)[i].m_handle == handle) {
                    newReceivers->erase(newReceivers->begin() + i);
                    m_receivers.store(std::move(newReceivers));
                    return size - 1;
                }
            }
            return size;
        }

        template <class ...Args>
        requires std::invocable<Callable, Args...>
        void send(Args&&... value) noexcept(std::is_nothrow_invocable_v<Callable, Args...>) {
            auto currentReceivers = m_receivers.load();
            for (auto& callable : *currentReceivers) {
                if (callable.call(std::forward<Args>(value)...)) {
                    break;
                }
            }
        }
    };

    class EventCenter;

    class OpaquePortBase {
    public:
        virtual ~OpaquePortBase() noexcept = default;
    };

    class BaseFilter {
    public:
        virtual ~BaseFilter() noexcept = default;
        virtual bool operator==(BaseFilter const& other) const noexcept = 0;
        virtual size_t hash() const noexcept = 0;
        virtual BaseFilter* clone() const noexcept = 0;
        virtual OpaquePortBase* getPort() const noexcept = 0;
    };

    class BaseFilterHash {
    public:
        size_t operator()(BaseFilter const* filter) const noexcept {
            return filter->hash();
        }
        size_t operator()(std::shared_ptr<BaseFilter> const& filter) const noexcept {
            return filter->hash();
        }

        using is_transparent = void;
    };

    class BaseFilterEqual {
    public:
        bool operator()(std::shared_ptr<BaseFilter> const& a, std::shared_ptr<BaseFilter> const& b) const noexcept {
            return *a == *b;
        }
        bool operator()(BaseFilter const* a, std::shared_ptr<BaseFilter> const& b) const noexcept {
            return *a == *b;
        }
        bool operator()(std::shared_ptr<BaseFilter> const& a, BaseFilter const* b) const noexcept {
            return *a == *b;
        }
        bool operator()(BaseFilter const* a, BaseFilter const* b) const noexcept {
            return *a == *b;
        }
        using is_transparent = void;
    };

    template<class Marker, template <class> class PortTemplate, class Func, class... FArgs>
    class EventFilter {};

    class ListenerHandle {
    private:
        BaseFilter const* m_filter;
        ReceiverHandle m_handle;
        using RemoverType = void(BaseFilter const*, ReceiverHandle);
        RemoverType* m_remover;

        ListenerHandle(BaseFilter const* filter, ReceiverHandle handle, RemoverType* remover) noexcept
            : m_filter(filter), m_handle(handle), m_remover(remover) {}

        friend class EventCenter;
        
    public:
        ListenerHandle() noexcept : m_filter(nullptr), m_handle(ReceiverHandle()), m_remover(nullptr) {}

        ListenerHandle(ListenerHandle const&) = delete;
        ListenerHandle(ListenerHandle&& other) noexcept
            : m_filter(other.m_filter), m_handle(other.m_handle), m_remover(other.m_remover) {
            other.m_filter = nullptr;
            other.m_handle = ReceiverHandle();
            other.m_remover = nullptr;
        }
        ListenerHandle& operator=(ListenerHandle const&) = delete;
        ListenerHandle& operator=(ListenerHandle&& other) noexcept {
            if (this != &other) {
                m_filter = other.m_filter;
                m_handle = other.m_handle;
                m_remover = other.m_remover;
                other.m_filter = nullptr;
                other.m_handle = ReceiverHandle();
                other.m_remover = nullptr;
            }
            return *this;
        }

        ~ListenerHandle() noexcept {
            if (m_filter && m_remover) {
                m_remover(m_filter, m_handle);
                m_filter = nullptr;
                m_handle = ReceiverHandle();
                m_remover = nullptr;
            }
        }

        template <class Marker, template <class> class PortTemplate, class Func, class... FArgs>
        friend class EventFilter;
    };

    template<class Marker, template <class> class PortTemplate, class... PArgs, class... FArgs>
    class EventFilter<Marker, PortTemplate, bool(PArgs...), FArgs...> : public BaseFilter {
    protected:
        using Self = EventFilter<Marker, PortTemplate, bool(PArgs...), FArgs...>;
        struct CloneMarker {};

        std::tuple<FArgs...> const m_filter;
        
        bool operator==(BaseFilter const& other) const noexcept override {
            auto* o = geode::cast::exact_cast<Self const*>(&other);
            if (!o) return false;

            return m_filter == o->m_filter;
        }
        BaseFilter* clone() const noexcept override {
            return new Self(CloneMarker{}, m_filter);
        }

        OpaquePortBase* getPort() const noexcept override;

        size_t hash() const noexcept override {
            // TODO: implement hash combining
            // return geode::combineHash(geode::typeHash<Self>(), filter);
            return 0;
        }

        ListenerHandle addReceiver(std::function<bool(PArgs...)> rec, int priority = 0) const noexcept;
        size_t removeReceiver(ReceiverHandle handle) const noexcept;

        static void removeReceiverStatic(BaseFilter const* filter, ReceiverHandle handle) noexcept {
            auto* self = static_cast<EventFilter const*>(filter);
            geode::log::debug("Removing receiver from filter {}", (void*)self);
            geode::log::flush();
            if (self) {
                self->removeReceiver(handle);
            }
        }

        EventFilter(CloneMarker, std::tuple<FArgs...> const& value) noexcept : m_filter(value) {}

    public:
        EventFilter(FArgs&&... value) noexcept : m_filter(std::forward<FArgs>(value)...) {}

        void send(PArgs&&... args) noexcept(std::is_nothrow_invocable_v<std::function<bool(PArgs...)>, PArgs...>);

        template<class Callable>
        ListenerHandle listen(Callable listener, int priority = 0) const noexcept {
            if constexpr (std::is_convertible_v<std::invoke_result_t<Callable, PArgs...>, bool>) {
                auto handle = this->addReceiver([listener = std::move(listener)](PArgs... args) {
                    return static_cast<bool>(std::invoke(listener, std::forward<PArgs>(args)...));
                }, priority);
                handle.m_remover = &Self::removeReceiverStatic;
                return handle;
            }
            else {
                auto handle = this->addReceiver([listener = std::move(listener)](PArgs... args) {
                    std::invoke(listener, std::forward<PArgs>(args)...);
                    return false;
                }, priority);
                handle.m_remover = &Self::removeReceiverStatic;
                return handle;
            }
        }
    };

    template <class... Args>
    class Dispatch : public EventFilter<Dispatch<Args...>, ThreadsafePort, bool(Args...), std::string> {
    public:
        using EventFilter<Dispatch<Args...>, ThreadsafePort, bool(Args...), std::string>::EventFilter;
    };

    template <template <class> class PortTemplate, class... PArgs>
    class OpaqueEventPort : public OpaquePortBase {
        PortTemplate<std::function<bool(PArgs...)>> m_port;

    public:
        template <class... Args>
        void send(Args&&... args) noexcept(std::is_nothrow_invocable_v<std::function<bool(PArgs...)>, Args...>) {
            m_port.send(std::forward<Args>(args)...);
        }

        ReceiverHandle addReceiver(std::function<bool(PArgs...)> rec, int priority = 0) noexcept {
            return m_port.addReceiver(std::move(rec), priority);
        }

        size_t removeReceiver(ReceiverHandle handle) noexcept {
            return m_port.removeReceiver(handle);
        }
    };

    class EventCenter {
        using KeyType = std::shared_ptr<BaseFilter>;
        using ValueType = std::shared_ptr<OpaquePortBase>;
        using MapType = std::unordered_map<KeyType, ValueType, BaseFilterHash, BaseFilterEqual>;
        asp::PtrSwap<MapType> m_ports;

        EventCenter() : m_ports(asp::make_shared<MapType>()) {}
    public:
        static EventCenter& get() {
            static EventCenter instance;
            return instance;
        }

        template <class Callable>
        requires std::is_invocable_v<Callable, OpaquePortBase*>
        void send(BaseFilter const* filter, Callable func) noexcept(std::is_nothrow_invocable_v<Callable, OpaquePortBase*>) {
            auto p = m_ports.load();
            auto it = p->find(filter);
            if (it != p->end()) {
                return std::invoke(func, it->second.get());
            }
            return;
        }

        template <class Callable>
        requires std::is_invocable_v<Callable, OpaquePortBase*>
        ListenerHandle addReceiver(BaseFilter const* filter, Callable func) noexcept {
            auto p = m_ports.load();
            auto it = p->find(filter);
            if (it != p->end()) {
                return ListenerHandle(it->first.get(), std::invoke(func, it->second.get()), nullptr);
            }
            else {
                auto clonedFilter = KeyType(filter->clone());
                auto newFilter = clonedFilter.get();
                auto port = ValueType(clonedFilter->getPort());
                ReceiverHandle handle = std::invoke(func, port.get());
                auto newPorts = asp::make_shared<MapType>(*p.get());
                newPorts->emplace(std::move(clonedFilter), std::move(port));
                m_ports.store(std::move(newPorts));
                return ListenerHandle(newFilter, handle, nullptr);
            }
        }

        template <class Callable>
        requires std::is_invocable_v<Callable, OpaquePortBase*>
        size_t removeReceiver(BaseFilter const* filter, Callable func) noexcept {
            auto p = m_ports.load();
            auto it = p->find(filter);
            if (it != p->end()) {
                auto size = std::invoke(func, it->second.get());
                if (size == 0) {
                    auto newPorts = asp::make_shared<MapType>(*p.get());
                    newPorts->erase(filter);
                    m_ports.store(std::move(newPorts));
                }
                return size;
            }
            return (size_t)-1;
        }
    };

    template <class Marker, template <class> class PortTemplate, class... PArgs, class... FArgs>
    void EventFilter<Marker, PortTemplate, bool(PArgs...), FArgs...>::send(PArgs&&... args) noexcept(std::is_nothrow_invocable_v<std::function<bool(PArgs...)>, PArgs...>) {
        return EventCenter::get().send(this, [&](OpaquePortBase* opaquePort) {
            auto port = static_cast<OpaqueEventPort<PortTemplate, PArgs...>*>(opaquePort);
            return port->send(std::forward<PArgs>(args)...);
        });
    }

    template <class Marker, template <class> class PortTemplate, class... PArgs, class... FArgs>
    ListenerHandle EventFilter<Marker, PortTemplate, bool(PArgs...), FArgs...>::addReceiver(std::function<bool(PArgs...)> rec, int priority) const noexcept {
        return EventCenter::get().addReceiver(this, [&](OpaquePortBase* opaquePort) {
            auto port = static_cast<OpaqueEventPort<PortTemplate, PArgs...>*>(opaquePort);
            return port->addReceiver(std::move(rec), priority);
        });
    }

    template <class Marker, template <class> class PortTemplate, class... PArgs, class... FArgs>
    size_t EventFilter<Marker, PortTemplate, bool(PArgs...), FArgs...>::removeReceiver(ReceiverHandle handle) const noexcept {
        return EventCenter::get().removeReceiver(this, [&](OpaquePortBase* opaquePort) {
            auto port = static_cast<OpaqueEventPort<PortTemplate, PArgs...>*>(opaquePort);
            return port->removeReceiver(handle);
        });
    }

    template <class Marker, template <class> class PortTemplate, class... PArgs, class... FArgs>
    OpaquePortBase* EventFilter<Marker, PortTemplate, bool(PArgs...), FArgs...>::getPort() const noexcept {
        return new OpaqueEventPort<PortTemplate, PArgs...>();
    };
}
