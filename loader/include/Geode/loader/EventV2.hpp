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
#include <atomic_shared_ptr/atomic_shared_ptr.h>

#include "../utils/casts.hpp"

namespace geode::event {
    template <typename T>
    struct RefOrVoid {
        using type = void(T const&);
    };
    template <>
    struct RefOrVoid<void> {
        using type = void();
    };
    template <typename T>
    using RefOrVoidType = typename RefOrVoid<T>::type;

    using ReceiverHandle = size_t;

    template <class Callable>
    struct PortCallable {
        Callable callable;
        int priority;
        ReceiverHandle handle;

        bool operator<(PortCallable const& other) const noexcept {
            return priority < other.priority;
        }

        template <typename ...Args>
        bool call(Args&&... args) const noexcept(std::is_nothrow_invocable_v<Callable, Args...>) {
            if constexpr (std::is_same_v<void, decltype(std::invoke(callable, std::forward<Args>(args)...))>) {
                std::invoke(callable, std::forward<Args>(args)...);
                return false;
            }
            return std::invoke(callable, std::forward<Args>(args)...);
        }
    };

    template <class Callable>
    class Port {
    protected:
        std::vector<PortCallable<Callable>> receivers;
    public:
        ReceiverHandle addReceiver(Callable receiver, int priority = 0) noexcept {
            ReceiverHandle handle = receivers.empty() ? 1 : receivers.back().handle + 1;
            receivers.push_back({std::move(receiver), priority, handle});
            std::sort(receivers.begin(), receivers.end());
            return handle;
        }

        void removeReceiver(ReceiverHandle handle) noexcept {
            for (int i = 0; i < receivers.size(); ++i) {
                if (receivers[i].handle == handle) {
                    receivers.erase(receivers.begin() + i);
                    return;
                }
            }
        }

        template <typename ...Args>
        requires std::invocable<Callable, Args...>
        void send(Args&&... value) const noexcept(std::is_nothrow_invocable_v<Callable, Args...>) {
            for (auto& callable : receivers) {
                if (callable.call(std::forward<Args>(value)...)) {
                    break;
                }
            }
        }
    };

    template <class Callable>
    class OncePort : protected Port<Callable> {
        bool sent = false;
    public:
        using Port<Callable>::addReceiver;
        using Port<Callable>::removeReceiver;

        template <typename ...Args>
        void send(Args&&... args) noexcept(std::is_nothrow_invocable_v<Callable, Args...>) {
            if (sent) return;
            sent = true;
            Port<Callable>::send(std::forward<Args>(args)...);
        }
        bool isSent() const noexcept {
            return sent;
        }
    };

    template <class Callable>
    class QueuedPort : protected Port<Callable> {
        std::vector<std::function<void()>> queue;
    public:
        using Port<Callable>::addReceiver;
        using Port<Callable>::removeReceiver;

        template <typename ...Args>
        requires std::invocable<Callable, Args...>
        void send(Args&&... args) const noexcept(std::is_nothrow_invocable_v<Callable, Args...>) {
            queue.push_back([=, this] { 
                Port<Callable>::send(std::forward<Args>(args)...); 
            });
        }

        void flush() noexcept {
            for (auto& q : queue) {
                std::invoke(q);
            }
            queue.clear();
        }
    };

    template <class Callable>
    class QueuedOncePort : protected QueuedPort<Callable> {
        bool sent = false;
    public:
        using QueuedPort<Callable>::addReceiver;
        using QueuedPort<Callable>::removeReceiver;
        using QueuedPort<Callable>::flush;

        template <typename ...Args>
        void send(Args&&... args) noexcept(std::is_nothrow_invocable_v<Callable, Args...>) {
            if (sent) return;
            sent = true;
            QueuedPort<Callable>::send(std::forward<Args>(args)...);
        }
        bool isSent() const noexcept {
            return sent;
        }
    };

    template <class Callable>
    class ThreadsafePort  {
        using VectorType = std::vector<PortCallable<Callable>>;
        LFStructs::AtomicSharedPtr<VectorType> receivers;
    public:
        ReceiverHandle addReceiver(Callable receiver, int priority = 0) noexcept {
            auto newReceivers = new VectorType(*receivers.get().get());
            ReceiverHandle handle = newReceivers->empty() ? 1 : newReceivers->back().handle + 1;
            for (auto it = newReceivers->begin(); it != newReceivers->end(); ++it) {
                if (priority < it->priority) {
                    newReceivers->insert(it, {std::move(receiver), priority, handle});
                    receivers.store(newReceivers);
                    return handle;
                }
            }
            newReceivers->push_back({std::move(receiver), priority, handle});
            receivers.store(newReceivers);
            return handle;
        }

        size_t removeReceiver(ReceiverHandle handle) noexcept {
            auto newReceivers = new VectorType(*receivers.get().get());
            auto size = newReceivers->size();
            for (int i = 0; i < size; ++i) {
                if ((*newReceivers)[i].handle == handle) {
                    newReceivers->erase(newReceivers->begin() + i);
                    receivers.store(newReceivers);
                    return size - 1;
                }
            }
            return size;
        }

        template <typename ...Args>
        requires std::invocable<Callable, Args...>
        void send(Args&&... value) noexcept(std::is_nothrow_invocable_v<Callable, Args...>) {
            auto currentReceivers = receivers.get().get();
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
        BaseFilter const* filter;
        ReceiverHandle handle;
        using RemoverType = void(BaseFilter const*, ReceiverHandle);
        RemoverType* remover;

        ListenerHandle(BaseFilter const* filter, ReceiverHandle handle, RemoverType* remover) noexcept
            : filter(filter), handle(handle), remover(remover) {}
        ListenerHandle() noexcept : filter(nullptr), handle(ReceiverHandle()), remover(nullptr) {}

        friend class EventCenter;
        
    public:
        ListenerHandle(ListenerHandle const&) = delete;
        ListenerHandle(ListenerHandle&& other) noexcept
            : filter(other.filter), handle(other.handle), remover(other.remover) {
            other.filter = nullptr;
            other.handle = ReceiverHandle();
            other.remover = nullptr;
        }
        ListenerHandle& operator=(ListenerHandle const&) = delete;
        ListenerHandle& operator=(ListenerHandle&& other) noexcept {
            if (this != &other) {
                filter = other.filter;
                handle = other.handle;
                remover = other.remover;
                other.filter = nullptr;
                other.handle = ReceiverHandle();
                other.remover = nullptr;
            }
            return *this;
        }

        ~ListenerHandle() noexcept {
            if (filter && remover) {
                remover(filter, handle);
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

        std::tuple<FArgs...> const filter;
        
        bool operator==(BaseFilter const& other) const noexcept override {
            auto* o = geode::cast::exact_cast<Self const*>(&other);
            if (!o) return false;

            return filter == o->filter;
        }
        BaseFilter* clone() const noexcept override {
            return new Self(CloneMarker{}, filter);
        }

        OpaquePortBase* getPort() const noexcept override;

        size_t hash() const noexcept override {
            // TODO: implement hash combining
            // return geode::combineHash(geode::typeHash<Self>(), filter);
            return 0;
        }

        ReceiverHandle addReceiver(std::function<bool(PArgs...)> rec, int priority = 0) const noexcept;
        size_t removeReceiver(ReceiverHandle handle) const noexcept;

        static void removeReceiverStatic(BaseFilter const* filter, ReceiverHandle handle) noexcept {
            auto* self = static_cast<EventFilter const*>(filter);
            if (self) {
                self->removeReceiver(handle);
            }
        }

        EventFilter(CloneMarker, std::tuple<FArgs...> const& value) noexcept : filter(value) {}

    public:
        EventFilter(FArgs&&... value) noexcept : filter(std::forward<FArgs>(value)...) {}

        void send(PArgs&&... args) noexcept(std::is_nothrow_invocable_v<std::function<bool(PArgs...)>, PArgs...>);

        template<class Callable>
        ListenerHandle listen(Callable listener, int priority = 0) const noexcept {
            if constexpr (std::is_convertible_v<std::invoke_result_t<Callable, PArgs...>, bool>) {
                auto handle = this->addReceiver([listener = std::move(listener)](PArgs... args) {
                    return static_cast<bool>(listener(std::forward<PArgs>(args)...));
                }, priority);
                return ListenerHandle(this, handle, &Self::removeReceiverStatic);
            }
            else {
                auto handle = this->addReceiver([listener = std::move(listener)](PArgs... args) {
                    listener(std::forward<PArgs>(args)...);
                    return false;
                }, priority);
                return ListenerHandle(this, handle, &Self::removeReceiverStatic);
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
        PortTemplate<std::function<bool(PArgs...)>> port;

    public:
        template <typename... Args>
        void send(Args&&... args) noexcept(std::is_nothrow_invocable_v<std::function<bool(PArgs...)>, Args...>) {
            port.send(std::forward<Args>(args)...);
        }

        ReceiverHandle addReceiver(std::function<bool(PArgs...)> rec, int priority = 0) noexcept {
            return port.addReceiver(std::move(rec), priority);
        }

        size_t removeReceiver(ReceiverHandle handle) noexcept {
            return port.removeReceiver(handle);
        }
    };

    class EventCenter {
        using KeyType = std::shared_ptr<BaseFilter>;
        using ValueType = std::shared_ptr<OpaquePortBase>;
        using MapType = std::unordered_map<KeyType, ValueType, BaseFilterHash, BaseFilterEqual>;
        LFStructs::AtomicSharedPtr<MapType> ports;
    public:
        static EventCenter& get() {
            static EventCenter instance;
            return instance;
        }

        void send(BaseFilter const* filter, std::function<void(OpaquePortBase*)> func) noexcept(std::is_nothrow_invocable_v<std::function<void(OpaquePortBase*)>, OpaquePortBase*>) {
            auto p = ports.get().get();
            auto it = p->find(filter);
            if (it != p->end()) {
                return std::invoke(func, it->second.get());
            }
            return;
        }

        ReceiverHandle addReceiver(BaseFilter const* filter, std::function<size_t(OpaquePortBase*)> func) noexcept {
            auto p = ports.get().get();
            auto it = p->find(filter);
            if (it != p->end()) {
                return std::invoke(func, it->second.get());
            }
            else {
                auto clonedFilter = KeyType(filter->clone());
                auto port = ValueType(clonedFilter->getPort());
                ReceiverHandle handle = std::invoke(func, port.get());
                auto newPorts = new MapType(*p);
                newPorts->emplace(std::move(clonedFilter), std::move(port));
                ports.store(newPorts);
                return handle;
            }
        }

        size_t removeReceiver(BaseFilter const* filter, std::function<size_t(OpaquePortBase*)> func) noexcept {
            auto p = ports.get().get();
            auto it = p->find(filter);
            if (it != p->end()) {
                auto size = std::invoke(func, it->second.get());
                if (size == 0) {
                    auto newPorts = new MapType(*p);
                    newPorts->erase(filter);
                    ports.store(newPorts);
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
    ReceiverHandle EventFilter<Marker, PortTemplate, bool(PArgs...), FArgs...>::addReceiver(std::function<bool(PArgs...)> rec, int priority) const noexcept {
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

inline void func() {
    using namespace geode::event;
    std::cout << "start" << std::endl;
    auto handle = Dispatch<int>("test").listen([](int val) {
        std::cout << "val: " << val << std::endl;
    });

    auto handle2 = Dispatch<int>("test2").listen([](int val) {
        std::cout << "val2: " << val << std::endl;
    });

    auto handle3 = Dispatch<float>("test").listen([](float val) {
        std::cout << "val3: " << val << std::endl;
    });

    auto handle4 = Dispatch<int>("test").listen([](int val) {
        std::cout << "val4: " << val << std::endl;
    }, -5);

    std::cout << "handle generated" << std::endl;

    Dispatch<int>("test").send(5);
    Dispatch<int>("test2").send(7);
    Dispatch<float>("test").send(9);
}