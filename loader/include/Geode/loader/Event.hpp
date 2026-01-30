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
#include "../utils/function.hpp"
#include "../utils/casts.hpp"
#include "../utils/general.hpp"

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

    template <class Port>
    concept IsPort = requires(Port p, typename Port::CallableType c, ReceiverHandle h) {
        { p.addReceiver(std::move(c), 0) } -> std::convertible_to<ReceiverHandle>;
        { p.removeReceiver(h) } -> std::convertible_to<size_t>;
    };

    template <template <class> class Port, class Callable>
    concept PortTemplateFor = IsPort<Port<Callable>>;

    template <class Callable>
    struct PortCallableCopy {
        Callable m_callable;
        int m_priority;
        ReceiverHandle m_handle;

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
    struct PortCallableMove : PortCallableCopy<Callable> {
        template <class ...Args>
        bool call(Args&&... args) noexcept(std::is_nothrow_invocable_v<Callable, Args...>) {
            if constexpr (std::is_same_v<void, decltype(std::invoke(this->m_callable, std::forward<Args>(args)...))>) {
                std::invoke(std::move(this->m_callable), std::forward<Args>(args)...);
                return false;
            }
            return std::invoke(std::move(this->m_callable), std::forward<Args>(args)...);
        }
    };


    template <class Callable, bool ThreadSafe=false, template <class> class Container = PortCallableCopy>
    class Port {
    protected:
        std::vector<Container<Callable>> m_receivers;
    public:
        using CallableType = Callable;

        ReceiverHandle addReceiver(Callable receiver, int priority = 0) noexcept {
            ReceiverHandle handle = m_receivers.empty() ? 1 : m_receivers.back().m_handle + 1;
            for (auto it = m_receivers.begin(); it != m_receivers.end(); ++it) {
                if (priority < it->m_priority) {
                    m_receivers.insert(it, {std::move(receiver), priority, handle});
                    return handle;
                }
            }
            m_receivers.push_back({std::move(receiver), priority, handle});
            return handle;
        }

        size_t removeReceiver(ReceiverHandle handle) noexcept {
            for (int i = 0; i < m_receivers.size(); ++i) {
                if (m_receivers[i].m_handle == handle) {
                    m_receivers.erase(m_receivers.begin() + i);
                    return i;
                }
            }
            return m_receivers.size();
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

    template <class Callable, template <class> class Container>
    class Port<Callable, true, Container>  {
        using VectorType = std::vector<Container<Callable>>;
        asp::PtrSwap<VectorType> m_receivers;
    public:
        using CallableType = Callable;

        Port() : m_receivers(asp::make_shared<VectorType>()) {}

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

    template <class Callable, bool ThreadSafe = false>
    class OncePort : protected Port<Callable, ThreadSafe, PortCallableMove> {
        std::conditional_t<ThreadSafe, std::atomic_flag, bool> m_sent = false;
    public:
        using CallableType = Callable;

        using Port<Callable, ThreadSafe>::addReceiver;
        using Port<Callable, ThreadSafe>::removeReceiver;

        template <class ...Args>
        void send(Args&&... args) noexcept(std::is_nothrow_invocable_v<Callable, Args...>) {

            if constexpr (ThreadSafe) {
                if (m_sent.test_and_set())
                    return;
            } else {
                if (m_sent) return;
                m_sent = true;
            }

            Port<Callable, ThreadSafe>::send(std::forward<Args>(args)...);
        }
        bool isSent() const noexcept {
            if constexpr (ThreadSafe)
                return m_sent.test();
            else
                return m_sent;
        }
    };

    template <class Callable, bool ThreadSafe = false, template <class> class Container = PortCallableCopy>
    class QueuedPort : protected Port<Callable, ThreadSafe, Container> {
        using VectorType = std::vector<geode::CopyableFunction<void()>>;
        std::conditional_t<ThreadSafe, asp::PtrSwap<VectorType>, VectorType> m_queue;
    public:
        using CallableType = Callable;

        using Port<Callable, ThreadSafe>::addReceiver;
        using Port<Callable, ThreadSafe>::removeReceiver;

        template <class ...Args>
        requires std::invocable<Callable, Args...>
        void send(Args&&... args) noexcept(std::is_nothrow_invocable_v<Callable, Args...>) {
            auto lam = [=, this] { 
                Port<Callable, ThreadSafe>::send(args...); 
            };

            if constexpr (ThreadSafe) {
                auto newQueue = asp::make_shared<VectorType>(*m_queue.load().get());
                newQueue->push_back(lam);
                m_queue.store(newQueue);
            } else {
                m_queue.push_back(lam);
            }
        }

        void flush() noexcept {
            if constexpr (ThreadSafe) {
                auto newQueue = asp::make_shared<VectorType>(*m_queue.load().get());
                for (auto& q : *newQueue) {
                    std::invoke(q);
                }
                newQueue->clear();
                m_queue.store(newQueue);
            } else {
                for (auto& q : m_queue) {
                    std::invoke(q);
                }
                m_queue.clear();
            }
        }
    };

    template <class Callable, bool ThreadSafe = false>
    class QueuedOncePort : protected QueuedPort<Callable, ThreadSafe, PortCallableMove> {
        std::conditional_t<ThreadSafe, std::atomic_flag, bool> m_sent = false;
    public:
        using CallableType = Callable;

        using QueuedPort<Callable, ThreadSafe>::addReceiver;
        using QueuedPort<Callable, ThreadSafe>::removeReceiver;
        using QueuedPort<Callable, ThreadSafe>::flush;

        template <class ...Args>
        void send(Args&&... args) noexcept(std::is_nothrow_invocable_v<Callable, Args...>) {
            if constexpr (ThreadSafe) {
                if (m_sent.test_and_set())
                    return;
            } else {
                if (m_sent) return;
                m_sent = true;
            }
            QueuedPort<Callable, ThreadSafe>::send(std::forward<Args>(args)...);
        }
        bool isSent() const noexcept {
            if constexpr (ThreadSafe)
                return m_sent.test();
            else
                return m_sent;
        }
    };

    template <template <class, bool> class PortType, bool ThreadSafe>
    struct PortWrapper {
        template <class Callable>
        using type = PortType<Callable, ThreadSafe>;
    };

    static_assert(PortTemplateFor<PortWrapper<Port, true>::type, geode::CopyableFunction<void()>>, "Port type is not a valid port");

    class EventCenter;

    class OpaquePortBase {
    public:
        virtual ~OpaquePortBase() noexcept = default;
    };

    template <template <class> class PortTemplate, class... PArgs>
    requires PortTemplateFor<PortTemplate, geode::CopyableFunction<bool(PArgs...)>>
    class OpaqueEventPort : public OpaquePortBase {
        PortTemplate<geode::CopyableFunction<bool(PArgs...)>> m_port;

    public:
        template <class... Args>
        void send(Args&&... args) noexcept(std::is_nothrow_invocable_v<geode::CopyableFunction<bool(PArgs...)>, Args...>) {
            m_port.send(std::forward<Args>(args)...);
        }

        ReceiverHandle addReceiver(geode::CopyableFunction<bool(PArgs...)> rec, int priority = 0) noexcept {
            return m_port.addReceiver(std::move(rec), priority);
        }

        size_t removeReceiver(ReceiverHandle handle) noexcept {
            return m_port.removeReceiver(handle);
        }
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
    class BasicEvent {
    private:
        static_assert(std::is_same_v<Marker, void>, "BasicEvent specialization missing");
    };

    class ListenerHandle {
    private:
        BaseFilter const* m_filter = nullptr;
        ReceiverHandle m_handle = ReceiverHandle{};
        using RemoverType = void(BaseFilter const*, ReceiverHandle);
        RemoverType* m_remover = nullptr;
        bool m_active = false;

        ListenerHandle(BaseFilter const* filter, ReceiverHandle handle, RemoverType* remover, bool active = true) noexcept
            : m_filter(filter), m_handle(handle), m_remover(remover), m_active(active) {}

        friend class EventCenter;
        
    public:
        ListenerHandle() noexcept {}

        ListenerHandle(ListenerHandle const&) = delete;
        ListenerHandle(ListenerHandle&& other) noexcept
            : m_filter(other.m_filter), m_handle(other.m_handle), m_remover(other.m_remover), m_active(other.m_active) {
            other.m_filter = nullptr;
            other.m_handle = ReceiverHandle();
            other.m_remover = nullptr;
            other.m_active = false;
        }
        ListenerHandle& operator=(ListenerHandle const&) = delete;
        ListenerHandle& operator=(ListenerHandle&& other) noexcept {
            if (this != &other) {
                m_filter = other.m_filter;
                m_handle = other.m_handle;
                m_remover = other.m_remover;
                m_active = other.m_active;
                other.m_filter = nullptr;
                other.m_handle = ReceiverHandle();
                other.m_remover = nullptr;
                other.m_active = false;
            }
            return *this;
        }

        ListenerHandle downgrade() noexcept {
            return ListenerHandle(m_filter, m_handle, m_remover, false);
        }

        ListenerHandle* leak() noexcept {
            return new ListenerHandle(std::move(*this));
        }

        void destroy() {
            if (m_filter && m_remover) {
                m_remover(m_filter, m_handle);
                m_filter = nullptr;
                m_handle = ReceiverHandle();
                m_remover = nullptr;
                m_active = false;
            }
        }

        ~ListenerHandle() noexcept {
            if (m_active) {
                this->destroy();
                m_active = false;
            }
        }

        template <class Marker, template <class> class PortTemplate, class Func, class... FArgs>
        friend class BasicEvent;
    };

    template<class Marker, template <class> class PortTemplate, class PReturn, class... PArgs, class... FArgs>
    requires requires {
        typename OpaqueEventPort<PortTemplate, PArgs...>;
        std::is_convertible_v<PReturn, bool> || std::is_same_v<PReturn, void>;
    }
    class BasicEvent<Marker, PortTemplate, PReturn(PArgs...), FArgs...> : public BaseFilter {
    protected:
        using Self = BasicEvent<Marker, PortTemplate, bool(PArgs...), FArgs...>;
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
            auto seed = typenameHash<Marker>();
            std::apply([&seed](auto const&... elems) {
                (hashCombine(seed, elems), ...);
            }, m_filter);
            return seed;
        }

        ListenerHandle addReceiver(geode::CopyableFunction<bool(PArgs...)> rec, int priority = 0) const noexcept;
        size_t removeReceiver(ReceiverHandle handle) const noexcept;

        static void removeReceiverStatic(BaseFilter const* filter, ReceiverHandle handle) noexcept {
            auto* self = static_cast<BasicEvent const*>(filter);
            if (self) {
                self->removeReceiver(handle);
            }
        }

        BasicEvent(CloneMarker, std::tuple<FArgs...> const& value) noexcept : m_filter(value) {}

    public:
        BasicEvent(FArgs&&... value) noexcept : m_filter(std::forward<FArgs>(value)...) {}

        void send(PArgs&&... args) noexcept(std::is_nothrow_invocable_v<geode::CopyableFunction<bool(PArgs...)>, PArgs...>);

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

    class EventCenter {
        using KeyType = std::shared_ptr<BaseFilter>;
        using ValueType = std::shared_ptr<OpaquePortBase>;
        using MapType = std::unordered_map<KeyType, ValueType, BaseFilterHash, BaseFilterEqual>;
        asp::PtrSwap<MapType> m_ports;

        EventCenter() : m_ports(asp::make_shared<MapType>()) {}
    public:
        GEODE_DLL static EventCenter* get();

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
                    for (auto& [filt, _] : *newPorts) {
                        if (*filt == *filter) {
                            newPorts->erase(filt);
                            break;
                        }
                    }
                    m_ports.store(std::move(newPorts));
                }
                return size;
            }
            return (size_t)-1;
        }
    };

    template <class Marker, template <class> class PortTemplate, class PReturn, class... PArgs, class... FArgs>
    requires requires {
        typename OpaqueEventPort<PortTemplate, PArgs...>;
        std::is_convertible_v<PReturn, bool> || std::is_same_v<PReturn, void>;
    }
    void BasicEvent<Marker, PortTemplate, PReturn(PArgs...), FArgs...>::send(PArgs&&... args) noexcept(std::is_nothrow_invocable_v<geode::CopyableFunction<bool(PArgs...)>, PArgs...>) {
        return EventCenter::get()->send(this, [&](OpaquePortBase* opaquePort) {
            auto port = static_cast<OpaqueEventPort<PortTemplate, PArgs...>*>(opaquePort);
            return port->send(std::forward<PArgs>(args)...);
        });
    }

    template <class Marker, template <class> class PortTemplate, class PReturn, class... PArgs, class... FArgs>
    requires requires {
        typename OpaqueEventPort<PortTemplate, PArgs...>;
        std::is_convertible_v<PReturn, bool> || std::is_same_v<PReturn, void>;
    }
    ListenerHandle BasicEvent<Marker, PortTemplate, PReturn(PArgs...), FArgs...>::addReceiver(geode::CopyableFunction<bool(PArgs...)> rec, int priority) const noexcept {
        return EventCenter::get()->addReceiver(this, [&](OpaquePortBase* opaquePort) {
            auto port = static_cast<OpaqueEventPort<PortTemplate, PArgs...>*>(opaquePort);
            return port->addReceiver(std::move(rec), priority);
        });
    }

    template <class Marker, template <class> class PortTemplate, class PReturn, class... PArgs, class... FArgs>
    requires requires {
        typename OpaqueEventPort<PortTemplate, PArgs...>;
        std::is_convertible_v<PReturn, bool> || std::is_same_v<PReturn, void>;
    }
    size_t BasicEvent<Marker, PortTemplate, PReturn(PArgs...), FArgs...>::removeReceiver(ReceiverHandle handle) const noexcept {
        return EventCenter::get()->removeReceiver(this, [&](OpaquePortBase* opaquePort) {
            auto port = static_cast<OpaqueEventPort<PortTemplate, PArgs...>*>(opaquePort);
            return port->removeReceiver(handle);
        });
    }

    template <class Marker, template <class> class PortTemplate, class PReturn, class... PArgs, class... FArgs>
    requires requires {
        typename OpaqueEventPort<PortTemplate, PArgs...>;
        std::is_convertible_v<PReturn, bool> || std::is_same_v<PReturn, void>;
    }
    OpaquePortBase* BasicEvent<Marker, PortTemplate, PReturn(PArgs...), FArgs...>::getPort() const noexcept {
        return new OpaqueEventPort<PortTemplate, PArgs...>();
    };
}

namespace geode {
    template<class Marker, class PFunc, class... FArgs>
    struct Event : public event::BasicEvent<Marker, event::PortWrapper<event::Port, false>::type, PFunc, FArgs...> {
        using event::BasicEvent<Marker, event::PortWrapper<event::Port, false>::type, PFunc, FArgs...>::BasicEvent;
    };

    template<class Marker, class PFunc, class... FArgs>
    struct ThreadSafeEvent : public event::BasicEvent<Marker, event::PortWrapper<event::Port, true>::type, PFunc, FArgs...> {
        using event::BasicEvent<Marker, event::PortWrapper<event::Port, true>::type, PFunc, FArgs...>::BasicEvent;
    };

    template<class Marker, class... PArgs>
    struct SimpleEvent : public event::BasicEvent<Marker, event::PortWrapper<event::Port, false>::type, bool(PArgs...)> {
        using event::BasicEvent<Marker, event::PortWrapper<event::Port, false>::type, bool(PArgs...)>::BasicEvent;
    };

    struct ListenerResult {
        static constexpr bool Propagate = false;
        static constexpr bool Stop = true;
        
        constexpr ListenerResult() noexcept = default;
        constexpr ListenerResult(bool value) noexcept : m_value(value) {}
        constexpr ListenerResult(ListenerResult const&) noexcept = default;
        constexpr ListenerResult(ListenerResult&&) noexcept = default;
        constexpr ListenerResult& operator=(ListenerResult const&) noexcept = default;
        constexpr ListenerResult& operator=(ListenerResult&&) noexcept = default;

        constexpr operator bool() const noexcept {
            return m_value;
        }

        bool m_value = false;
    };

    using ListenerHandle = event::ListenerHandle;
}