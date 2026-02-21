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
#include "../utils/hash.hpp"
// #include "../utils/ZStringView.hpp"
// #include "Types.hpp"

// namespace geode::console {
//     void log(ZStringView msg, Severity severity);
// }

namespace geode::comm {
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
            if (!m_callable) return false;

            if constexpr (std::is_same_v<void, decltype(std::invoke(m_callable, std::forward<Args>(args)...))>) {
                std::invoke(m_callable, std::forward<Args>(args)...);
                return false;
            } else {
                return std::invoke(m_callable, std::forward<Args>(args)...);
            }
        }
    };

    template <class Callable>
    struct PortCallableMove : PortCallableCopy<Callable> {
        template <class ...Args>
        bool call(Args&&... args) noexcept(std::is_nothrow_invocable_v<Callable, Args...>) {
            if (!this->m_callable) return false;

            if constexpr (std::is_same_v<void, decltype(std::invoke(this->m_callable, std::forward<Args>(args)...))>) {
                std::invoke(std::move(this->m_callable), std::forward<Args>(args)...);
                return false;
            }
            return std::invoke(std::move(this->m_callable), std::forward<Args>(args)...);
        }
    };

    class EventCenterThreadLocal;
    class EventCenterGlobal;

    // Okay so even though the Event system is fully header only,
    // we can still version it. One caveat/hackiness is that
    // Ports should be backwards ABI compatible, meaning no member
    // reordering or removing, but we can add new members at the end.
    // For every new version, we need to add a migration system
    // for the previous version, which is basically just a function 
    // that moves the data. Continue reading from OpaqueEventPort.
    template <class Callable, bool ThreadSafe=false, template <class> class Container = PortCallableCopy>
    class Port {
    protected:
        std::vector<Container<Callable>> m_receivers;
        std::vector<typename std::vector<Container<Callable>>::iterator> m_toRemove;
        std::vector<Container<Callable>> m_toAdd;
        size_t m_nextID = 1;
        size_t m_sending = 0;
    public:
        using CallableType = Callable;
        using EventCenterType = EventCenterThreadLocal;

        void migrateFromV1(Port&& other) noexcept {
            m_receivers = std::move(other.m_receivers);
            other.m_receivers.clear();
        }

        ReceiverHandle addReceiver(Callable receiver, int priority = 0) noexcept {
            ReceiverHandle handle = static_cast<ReceiverHandle>(m_nextID++);
            if (m_sending > 0) {
                // geode::console::log(fmt::format("Added handler with id {} to toAdd", handle), Severity::Debug);
                m_toAdd.push_back({std::move(receiver), priority, handle});
                return handle;
            }
            for (auto it = m_receivers.begin(); it != m_receivers.end(); ++it) {
                if (priority < it->m_priority) {
                    // geode::console::log(fmt::format("Added handler with id {} to receivers", handle), Severity::Debug);
                    m_receivers.insert(it, {std::move(receiver), priority, handle});
                    return handle;
                }
            }
            m_receivers.push_back({std::move(receiver), priority, handle});
            return handle;
        }

        size_t removeReceiver(ReceiverHandle handle) noexcept {
            auto size = m_receivers.size();
            for (int i = 0; i < size; ++i) {
                if (m_receivers[i].m_handle == handle) {
                    if (m_sending > 0) {
                        // geode::console::log(fmt::format("Added handler with id {} to toRemove", handle), Severity::Debug);
                        m_toRemove.push_back(m_receivers.begin() + i);
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

        size_t getReceiverCount() const noexcept {
            return m_receivers.size() + m_toAdd.size() - m_toRemove.size();
        }

        template <class ...Args>
        requires std::invocable<Callable, Args...>
        bool send(Args&&... value) noexcept(std::is_nothrow_invocable_v<Callable, Args...>) {
            m_sending++;
            bool ret = false;
            for (auto& callable : m_receivers) {
                if (std::find_if(m_toRemove.begin(), m_toRemove.end(), [&callable](auto& it) {
                    return it->m_handle == callable.m_handle;
                }) != m_toRemove.end()) {
                    // geode::console::log(fmt::format("Skipping handler with id {} because it is in toRemove", callable.m_handle), Severity::Debug);
                    continue;
                }
                if (callable.call(value...)) {
                    ret = true;
                    break;
                }
            }
            m_sending--;

            if (m_sending == 0) {
                // geode::console::log(fmt::format("Flushing {} handlers from toRemove", m_toRemove.size()), Severity::Debug);
                for (auto& it : m_toRemove) {
                    m_receivers.erase(it);
                }
                m_toRemove.clear();

                // geode::console::log(fmt::format("Flushing {} handlers from toAdd", m_toAdd.size()), Severity::Debug);
                m_receivers.insert(m_receivers.end(), std::make_move_iterator(m_toAdd.begin()), std::make_move_iterator(m_toAdd.end()));
                m_toAdd.clear();
                std::sort(m_receivers.begin(), m_receivers.end(), [](auto& a, auto& b) {
                    return a.m_priority < b.m_priority;
                });
            }

            return ret;
        }
    };

    template <class Callable, template <class> class Container>
    class Port<Callable, true, Container>  {
        using VectorType = std::vector<Container<Callable>>;
        asp::PtrSwap<VectorType> m_receivers;
    public:
        using CallableType = Callable;
        using EventCenterType = EventCenterGlobal;

        Port() : m_receivers(asp::make_shared<VectorType>()) {}

        void migrateFromV1(Port&& other) noexcept {
            m_receivers.store(other.m_receivers.load());
        }

        ReceiverHandle addReceiver(Callable receiver, int priority = 0) noexcept {
            ReceiverHandle handle = {};
            m_receivers.rcu([&](auto const& ptr) {
                auto newReceivers = asp::make_shared<VectorType>(*ptr.get());
                handle = newReceivers->empty() ? 1 : newReceivers->back().m_handle + 1;
                for (auto it = newReceivers->begin(); it != newReceivers->end(); ++it) {
                    if (priority < it->m_priority) {
                        newReceivers->insert(it, {std::move(receiver), priority, handle});
                        return newReceivers;
                    }
                }
                newReceivers->push_back({std::move(receiver), priority, handle});
                return newReceivers;
            });
            
            return handle;
        }

        size_t removeReceiver(ReceiverHandle handle) noexcept {
            size_t size = 0;
            m_receivers.rcu([&](auto const& ptr) {
                auto newReceivers = asp::make_shared<VectorType>(*ptr.get());
                size = newReceivers->size();
                for (int i = 0; i < size; ++i) {
                    if ((*newReceivers)[i].m_handle == handle) {
                        newReceivers->erase(newReceivers->begin() + i);
                        size--;
                        return newReceivers;
                    }
                }
                return newReceivers;
            });
            return size;
        }

        size_t getReceiverCount() const noexcept {
            return m_receivers.load()->size();
        }

        template <class ...Args>
        requires std::invocable<Callable, Args...>
        bool send(Args&&... value) noexcept(std::is_nothrow_invocable_v<Callable, Args...>) {
            auto currentReceivers = m_receivers.load();
            for (auto& callable : *currentReceivers) {
                if (callable.call(value...)) {
                    return true;
                }
            }
            return false;
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
        bool send(Args&&... args) noexcept(std::is_nothrow_invocable_v<Callable, Args...>) {

            if constexpr (ThreadSafe) {
                if (m_sent.test_and_set())
                    return false;
            } else {
                if (m_sent) return false;
                m_sent = true;
            }

            return Port<Callable, ThreadSafe>::send(std::forward<Args>(args)...);
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
        bool send(Args&&... args) noexcept(std::is_nothrow_invocable_v<Callable, Args...>) {
            auto lam = [=, this] {
                return Port<Callable, ThreadSafe>::send(args...);
            };

            if constexpr (ThreadSafe) {
                m_queue.rcu([&](auto const& ptr) {
                    auto newQueue = asp::make_shared<VectorType>(*ptr.get());
                    newQueue->push_back(lam);
                    return newQueue;
                });
            } else {
                m_queue.push_back(lam);
            }
            return false;
        }

        void flush() noexcept {
            if constexpr (ThreadSafe) {
                m_queue.rcu([&](auto const& ptr) {
                    auto newQueue = asp::make_shared<VectorType>(*ptr.get());
                    for (auto& q : *newQueue) {
                        std::invoke(q);
                    }
                    newQueue->clear();
                    return newQueue;
                });
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
        bool send(Args&&... args) noexcept(std::is_nothrow_invocable_v<Callable, Args...>) {
            if constexpr (ThreadSafe) {
                if (m_sent.test_and_set())
                    return false;
            } else {
                if (m_sent) return false;
                m_sent = true;
            }
            return QueuedPort<Callable, ThreadSafe>::send(std::forward<Args>(args)...);
        }
        bool isSent() const noexcept {
            if constexpr (ThreadSafe)
                return m_sent.test();
            else
                return m_sent;
        }
    };

    template <template <class, bool, template <class> class> class PortType, bool ThreadSafe, template <class> class Container>
    struct PortWrapper {
        template <class Callable>
        using type = PortType<Callable, ThreadSafe, Container>;
    };

    static_assert(PortTemplateFor<PortWrapper<Port, true, PortCallableCopy>::type, geode::CopyableFunction<void()>>, "Port type is not a valid port");

    class EventCenter;

    class OpaquePortBase {
    public:
        virtual ~OpaquePortBase() noexcept = default;
    };

    template <template <class> class PortTemplate, class... PArgs>
    requires PortTemplateFor<PortTemplate, geode::CopyableFunction<bool(PArgs...)>>
    class OpaqueEventPortV2;

    // In order to version Ports, we need to make a new EventPort class for every version,
    // and subclass the previous one. For example a V3 would subclass V2, which subclasses V1.
    // This is because we dont have a virtual version check function (i forgot) wait actually
    // maybe i can add it now i'll think anyway, and you add a migrate function into the port
    // that you call in the event migration code. Go to Event migratePort function.
    template <template <class> class PortTemplate, class... PArgs>
    requires PortTemplateFor<PortTemplate, geode::CopyableFunction<bool(PArgs...)>>
    class OpaqueEventPort : public OpaquePortBase {
    protected:
        PortTemplate<geode::CopyableFunction<bool(PArgs...)>> m_port;

    public:
        using EventCenterType = typename PortTemplate<geode::CopyableFunction<bool(PArgs...)>>::EventCenterType;

        OpaqueEventPort() {}

        ~OpaqueEventPort() noexcept override {}

        template <class... Args>
        bool send(Args&&... args) noexcept(std::is_nothrow_invocable_v<geode::CopyableFunction<bool(PArgs...)>, Args...>) {
            return m_port.send(std::forward<Args>(args)...);
        }

        ReceiverHandle addReceiver(geode::CopyableFunction<bool(PArgs...)> rec, int priority = 0) noexcept {
            return m_port.addReceiver(std::move(rec), priority);
        }

        size_t getReceiverCount() const noexcept {
            return m_port.getReceiverCount();
        }

        size_t removeReceiver(ReceiverHandle handle) noexcept {
            return m_port.removeReceiver(handle);
        }

        friend class OpaqueEventPortV2<PortTemplate, PArgs...>;
    };

    template <template <class> class PortTemplate, class... PArgs>
    requires PortTemplateFor<PortTemplate, geode::CopyableFunction<bool(PArgs...)>>
    class OpaqueEventPortV2 : public OpaqueEventPort<PortTemplate, PArgs...> {
    public:
        OpaqueEventPortV2() {}
        ~OpaqueEventPortV2() noexcept override {}

        void migrateFromV1(OpaqueEventPort<PortTemplate, PArgs...>* oldPort) noexcept {
            this->m_port.migrateFromV1(std::move(oldPort->m_port));
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

    class [[nodiscard("ListenerHandle is immediately destroyed unless stored or .leak() is called")]] ListenerHandle {
    private:
        std::weak_ptr<BaseFilter> m_filter;
        ReceiverHandle m_handle = ReceiverHandle{};
        using RemoverType = void(BaseFilter const*, ReceiverHandle);
        RemoverType* m_remover = nullptr;
        bool m_active = false;

        ListenerHandle(std::shared_ptr<BaseFilter> filter, ReceiverHandle handle, RemoverType* remover, bool active = true) noexcept
            : m_filter(std::move(filter)), m_handle(handle), m_remover(remover), m_active(active) {}

        ListenerHandle(std::weak_ptr<BaseFilter> filter, ReceiverHandle handle, RemoverType* remover, bool active = true) noexcept
            : m_filter(std::move(filter)), m_handle(handle), m_remover(remover), m_active(active) {}

        friend class EventCenter;
        friend class EventCenterThreadLocal;
        friend class EventCenterGlobal;

    public:
        ListenerHandle() noexcept {}

        ListenerHandle(ListenerHandle const&) = delete;
        ListenerHandle(ListenerHandle&& other) noexcept
            : m_filter(std::move(other.m_filter)), m_handle(other.m_handle), m_remover(other.m_remover), m_active(other.m_active) {
            other.m_handle = ReceiverHandle();
            other.m_remover = nullptr;
            other.m_active = false;
        }
        ListenerHandle& operator=(ListenerHandle const&) = delete;
        ListenerHandle& operator=(ListenerHandle&& other) noexcept {
            if (this != &other) {
                m_filter = std::move(other.m_filter);
                m_handle = other.m_handle;
                m_remover = other.m_remover;
                m_active = other.m_active;
                other.m_handle = ReceiverHandle();
                other.m_remover = nullptr;
                other.m_active = false;
            }
            return *this;
        }

        ListenerHandle downgrade() noexcept {
            return ListenerHandle(m_filter, m_handle, m_remover, false);
        }

        ListenerHandle* leak() {
            return new ListenerHandle(std::move(*this));
        }

        void destroy() {
            if (auto filter = m_filter.lock()) {
                if (m_remover) {
                    m_remover(filter.get(), m_handle);
                }
                m_filter.reset();
            }
            m_handle = ReceiverHandle();
            m_remover = nullptr;
            m_active = false;
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
        using KeyType = std::shared_ptr<BaseFilter>;
        using ValueType = std::shared_ptr<OpaquePortBase>;
        using MapType = std::unordered_map<KeyType, ValueType, BaseFilterHash, BaseFilterEqual>;
        using IteratorType = typename MapType::iterator;
        using OpaqueEventType = OpaqueEventPort<PortTemplate, PArgs...>;
        using OpaqueEventV2Type = OpaqueEventPortV2<PortTemplate, PArgs...>;
        using LatestOpaqueEventType = OpaqueEventV2Type;
        using EventCenterType = LatestOpaqueEventType::EventCenterType;

        // Here we migrate the port version if needed. This is what I meant by versioning,
        // we need to check for previous versions and move them into the current version.
        // Go to getPort definition.
        static OpaquePortBase* migratePort(OpaquePortBase* port) {
            if (!geode::cast::typeinfo_cast<OpaqueEventV2Type*>(port)) {
                auto oldPort = static_cast<OpaqueEventType*>(port);
                auto newPort = new OpaqueEventV2Type();
                newPort->migrateFromV1(oldPort);
                return newPort;
            }
            return nullptr;
        }

        using Self = BasicEvent<Marker, PortTemplate, PReturn(PArgs...), FArgs...>;
        struct CloneMarker {};

        std::tuple<FArgs...> const m_filter;

        bool operator==(BaseFilter const& other) const noexcept override {
            // geode::console::log(fmt::format("Comparing BasicEvent filters {}, {}", (void*)this, (void*)&other), Severity::Debug);
            // geode::console::log(fmt::format("Self type: {}", cast::getRuntimeTypeName(this)), Severity::Debug);
            // geode::console::log(fmt::format("Other type: {}", cast::getRuntimeTypeName(&other)), Severity::Debug);
            auto* o = geode::cast::typeinfo_cast<Self const*>(&other);
            if (!o) return false;

            auto ret = m_filter == o->m_filter;

            // geode::console::log(fmt::format("Comparison result: {}", ret), Severity::Debug);
            return ret;
        }
        BaseFilter* clone() const noexcept override {
            return new (std::nothrow) Self(CloneMarker{}, m_filter);
        }

        // All of the normal functions do static cast version, but that is not strictly needed,
        // what is needed however is updating this getPort function.
        OpaquePortBase* getPort() const noexcept override {
            return new (std::nothrow) OpaqueEventV2Type();
        }

        size_t hash() const noexcept override {
            auto seed = typenameHash<Marker>();
            std::apply([&seed](auto const&... elems) {
                (hashCombine(seed, elems), ...);
            }, m_filter);
            return seed;
        }

        ListenerHandle addReceiver(geode::CopyableFunction<PReturn(PArgs...)> rec, int priority = 0) const noexcept;
        size_t removeReceiver(ReceiverHandle handle) const noexcept;

        static void removeReceiverStatic(BaseFilter const* filter, ReceiverHandle handle) noexcept {
            auto* self = static_cast<BasicEvent const*>(filter);
            // geode::console::log(fmt::format("Static removing receiver from BasicEvent {}, {}", (void*)self, typeid(Marker).name()), Severity::Debug);
            if (self) {
                // geode::console::log(fmt::format("afssfd from BasicEvent {}, {}", (void*)self, typeid(Marker).name()), Severity::Debug);
                self->removeReceiver(handle);
            }
        }

        BasicEvent(CloneMarker, std::tuple<FArgs...> const& value) noexcept : m_filter(value) {
            // geode::console::log(fmt::format("Cloning BasicEvent {}, {}", (void*)this, typeid(Marker).name()), Severity::Debug);
        }

    public:
        BasicEvent(FArgs... value) noexcept : m_filter(std::move(value)...) {
            // geode::console::log(fmt::format("Creating BasicEvent {}, {}", (void*)this, typeid(Marker).name()), Severity::Debug);
        }
        ~BasicEvent() noexcept override {
            // geode::console::log(fmt::format("Destroying BasicEvent {}, {}", (void*)this, typeid(Marker).name()), Severity::Debug);
        }

        bool send(PArgs... args) noexcept(std::is_nothrow_invocable_v<geode::CopyableFunction<PReturn(PArgs...)>, PArgs...>);

        size_t getReceiverCount() const noexcept;

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

    class GEODE_DLL EventCenterThreadLocal {
    private:
        class Impl;
        std::unique_ptr<Impl> m_impl;

        EventCenterThreadLocal();
        ~EventCenterThreadLocal();
    
    public:
        static EventCenterThreadLocal* get();

        using SendFuncType = geode::Function<bool(OpaquePortBase*)>;
        using AddFuncType = geode::Function<ReceiverHandle(OpaquePortBase*)>;
        using SizeFuncType = geode::Function<size_t(OpaquePortBase*)>;
        using RemoveFuncType = geode::Function<size_t(OpaquePortBase*)>;
        using MigrateFuncType = geode::Function<OpaquePortBase*(OpaquePortBase*)>;

        bool send(BaseFilter const* filter, SendFuncType func, MigrateFuncType migratePort) noexcept;
        ListenerHandle addReceiver(BaseFilter const* filter, AddFuncType func, MigrateFuncType migratePort) noexcept;
        size_t getReceiverCount(BaseFilter const* filter, SizeFuncType func, MigrateFuncType migratePort) noexcept;
        size_t removeReceiver(BaseFilter const* filter, RemoveFuncType func, MigrateFuncType migratePort) noexcept;
    };

    class GEODE_DLL EventCenterGlobal {
    private:
        class Impl;
        std::unique_ptr<Impl> m_impl;

        EventCenterGlobal();
        ~EventCenterGlobal();
    
    public:
        static EventCenterGlobal* get();

        using SendFuncType = geode::Function<bool(OpaquePortBase*)>;
        using AddFuncType = geode::Function<ReceiverHandle(OpaquePortBase*)>;
        using SizeFuncType = geode::Function<size_t(OpaquePortBase*)>;
        using RemoveFuncType = geode::Function<size_t(OpaquePortBase*)>;
        using MigrateFuncType = geode::Function<OpaquePortBase*(OpaquePortBase*)>;

        bool send(BaseFilter const* filter, SendFuncType func, MigrateFuncType migratePort) noexcept;
        ListenerHandle addReceiver(BaseFilter const* filter, AddFuncType func, MigrateFuncType migratePort) noexcept;
        size_t getReceiverCount(BaseFilter const* filter, SizeFuncType func, MigrateFuncType migratePort) noexcept;
        size_t removeReceiver(BaseFilter const* filter, RemoveFuncType func, MigrateFuncType migratePort) noexcept;
    };

    class EventCenter {
        using KeyType = std::shared_ptr<BaseFilter>;
        using ValueType = std::shared_ptr<OpaquePortBase>;
        using MapType = std::unordered_map<KeyType, ValueType, BaseFilterHash, BaseFilterEqual>;
        asp::PtrSwap<MapType> m_ports;

        EventCenter() : m_ports(asp::make_shared<MapType>()) {}
    public:
        GEODE_DLL static EventCenter* get();

        template <class Callable, class Callable2>
        requires std::is_invocable_v<Callable, OpaquePortBase*>
        bool send(BaseFilter const* filter, Callable func, Callable2 migratePort) noexcept(std::is_nothrow_invocable_v<Callable, OpaquePortBase*>) {
            // geode::console::log(fmt::format("EventCenter sending event for filter {}, {}", (void*)filter, cast::getRuntimeTypeName(filter)), Severity::Debug);
            auto p = m_ports.load();
            auto it = p->find(filter);
            if (it != p->end()) {
                if (auto newFilter = std::invoke(migratePort, it->second.get())) {
                    it->second.reset(newFilter);
                }
                auto newFilter = it->first.get();
                return std::invoke(func, it->second.get());
            }
            return false;
        }

        template <class Callable, class Callable2>
        requires std::is_invocable_v<Callable, OpaquePortBase*>
        ListenerHandle addReceiver(BaseFilter const* filter, Callable func, Callable2 migratePort) noexcept {
            // geode::console::log(fmt::format("EventCenter adding receiver for filter {}, {}", (void*)filter, cast::getRuntimeTypeName(filter)), Severity::Debug);
            auto p = m_ports.load();
            auto it = p->find(filter);
            if (it != p->end()) {
                if (auto newFilter = std::invoke(migratePort, it->second.get())) {
                    it->second.reset(newFilter);
                }
                return ListenerHandle(it->first, std::invoke(func, it->second.get()), nullptr);
            }
            else {
                auto clonedFilter = KeyType(filter->clone());
                if (!clonedFilter) return ListenerHandle();
                auto filter2 = clonedFilter.get();
                // geode::console::log(fmt::format("Cloned filter for adding receiver {}, {}", (void*)filter2, cast::getRuntimeTypeName(filter2)), Severity::Debug);

                auto port = ValueType(clonedFilter->getPort());
                if (!port) return ListenerHandle();

                ReceiverHandle handle = std::invoke(func, port.get());
                auto ret = ListenerHandle(clonedFilter, handle, nullptr);

                m_ports.rcu([&](auto const& ptr) {
                    auto newPorts = asp::make_shared<MapType>(*ptr.get());
                    newPorts->emplace(std::move(clonedFilter), std::move(port));
                    return newPorts;
                });
                return ret;
            }
        }

        template <class Callable, class Callable2>
        requires std::is_invocable_v<Callable, OpaquePortBase*>
        size_t getReceiverCount(BaseFilter const* filter, Callable func, Callable2 migratePort) noexcept {
            auto p = m_ports.load();
            auto it = p->find(filter);
            if (it != p->end()) {
                if (auto newFilter = std::invoke(migratePort, it->second.get())) {
                    it->second.reset(newFilter);
                }
                return std::invoke(func, it->second.get());
            }
            return 0;
        }

        template <class Callable, class Callable2>
        requires std::is_invocable_v<Callable, OpaquePortBase*>
        size_t removeReceiver(BaseFilter const* filter, Callable func, Callable2 migratePort) noexcept {
            // geode::console::log(fmt::format("EventCenter removing receiver for filter {}, {}", (void*)filter, cast::getRuntimeTypeName(filter)), Severity::Debug);
            auto p = m_ports.load();
            auto it = p->find(filter);
            if (it != p->end()) {
                if (auto newFilter = std::invoke(migratePort, it->second.get())) {
                    it->second.reset(newFilter);
                }
                auto size = std::invoke(func, it->second.get());
                if (size == 0) {
                    // geode::console::log(fmt::format("Removing port for filter type {}", cast::getRuntimeTypeName(filter)), Severity::Debug);
                    m_ports.rcu([&](auto const& ptr) {
                        auto newPorts = asp::make_shared<MapType>(*ptr.get());
                        for (auto& [filt, _] : *newPorts) {
                            if (*filt == *filter) {
                                newPorts->erase(filt);
                                break;
                            }
                        }
                        return newPorts;
                    });
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
    bool BasicEvent<Marker, PortTemplate, PReturn(PArgs...), FArgs...>::send(PArgs... args) noexcept(std::is_nothrow_invocable_v<geode::CopyableFunction<PReturn(PArgs...)>, PArgs...>) {
        auto ret = EventCenterType::get()->send(this, [&](OpaquePortBase* opaquePort) {
            auto port = static_cast<LatestOpaqueEventType*>(opaquePort);
            return port->send(args...);
        }, &BasicEvent::migratePort);

        if (ret) return true;

        // fallback on the old event center
        return EventCenter::get()->send(this, [&](OpaquePortBase* opaquePort) {
            auto port = static_cast<OpaqueEventType*>(opaquePort);
            return port->send(std::forward<PArgs>(args)...);
        }, &BasicEvent::migratePort);
    }

    template <class Marker, template <class> class PortTemplate, class PReturn, class... PArgs, class... FArgs>
    requires requires {
        typename OpaqueEventPort<PortTemplate, PArgs...>;
        std::is_convertible_v<PReturn, bool> || std::is_same_v<PReturn, void>;
    }
    ListenerHandle BasicEvent<Marker, PortTemplate, PReturn(PArgs...), FArgs...>::addReceiver(geode::CopyableFunction<PReturn(PArgs...)> rec, int priority) const noexcept {
        return EventCenterType::get()->addReceiver(this, [&](OpaquePortBase* opaquePort) {
            auto port = static_cast<LatestOpaqueEventType*>(opaquePort);
            return port->addReceiver(std::move(rec), priority);
        }, &BasicEvent::migratePort);
    }

    template <class Marker, template <class> class PortTemplate, class PReturn, class... PArgs, class... FArgs>
    requires requires {
        typename OpaqueEventPort<PortTemplate, PArgs...>;
        std::is_convertible_v<PReturn, bool> || std::is_same_v<PReturn, void>;
    }
    size_t BasicEvent<Marker, PortTemplate, PReturn(PArgs...), FArgs...>::getReceiverCount() const noexcept {
        return EventCenterType::get()->getReceiverCount(this, [&](OpaquePortBase* opaquePort) {
            auto port = static_cast<LatestOpaqueEventType*>(opaquePort);
            return port->getReceiverCount();
        }, &BasicEvent::migratePort);
    }

    template <class Marker, template <class> class PortTemplate, class PReturn, class... PArgs, class... FArgs>
    requires requires {
        typename OpaqueEventPort<PortTemplate, PArgs...>;
        std::is_convertible_v<PReturn, bool> || std::is_same_v<PReturn, void>;
    }
    size_t BasicEvent<Marker, PortTemplate, PReturn(PArgs...), FArgs...>::removeReceiver(ReceiverHandle handle) const noexcept {
        return EventCenterType::get()->removeReceiver(this, [&](OpaquePortBase* opaquePort) {
            auto port = static_cast<LatestOpaqueEventType*>(opaquePort);
            return port->removeReceiver(handle);
        }, &BasicEvent::migratePort);
    }
}

namespace geode {
    using ListenerHandle = comm::ListenerHandle;

    template<class Marker, class PFunc, class... FArgs>
    struct Event : public comm::BasicEvent<Marker, comm::PortWrapper<comm::Port, false, comm::PortCallableCopy>::type, PFunc, FArgs...> {
        using comm::BasicEvent<Marker, comm::PortWrapper<comm::Port, false, comm::PortCallableCopy>::type, PFunc, FArgs...>::BasicEvent;
    };

    template<class Marker, class PFunc, class... FArgs>
    struct ThreadSafeEvent : public comm::BasicEvent<Marker, comm::PortWrapper<comm::Port, true, comm::PortCallableCopy>::type, PFunc, FArgs...> {
        using comm::BasicEvent<Marker, comm::PortWrapper<comm::Port, true, comm::PortCallableCopy>::type, PFunc, FArgs...>::BasicEvent;
    };

namespace comm {
    template<class Marker, bool ThreadSafe, class GFunc, class PFunc, class... FArgs>
    struct BasicGlobalEvent {};

    // I LOVE TEMPLATE ABUSE
    // here we misuse PFunc param as the FArg1, because every BasicGlobalEvent at least has one FArg anyway
    template<class Marker, bool ThreadSafe, class PReturn, class... PArgs, class FArg1, class... FArgs>
    struct BasicGlobalEvent<Marker, ThreadSafe, PReturn(PArgs...), FArg1, FArgs...> : public BasicGlobalEvent<Marker, ThreadSafe, PReturn(FArg1, FArgs..., PArgs...), PReturn(PArgs...), FArg1, FArgs...> {
        using BasicGlobalEvent<Marker, ThreadSafe, PReturn(FArg1, FArgs..., PArgs...), PReturn(PArgs...), FArg1, FArgs...>::BasicGlobalEvent;
    };

    template<class Marker, bool ThreadSafe, class GReturn, class... GArgs, class PReturn, class... PArgs, class... FArgs>
    struct BasicGlobalEvent<Marker, ThreadSafe, GReturn(GArgs...), PReturn(PArgs...), FArgs...> {
    private:
        using Event1Type = std::conditional_t<ThreadSafe,
            ThreadSafeEvent<Marker, PReturn(PArgs...), FArgs...>,
            Event<Marker, PReturn(PArgs...), FArgs...>
        >;
        using Event2Type = std::conditional_t<ThreadSafe,
            ThreadSafeEvent<Marker, bool(FArgs..., PArgs...)>,
            Event<Marker, bool(FArgs..., PArgs...)>
        >;
        std::optional<std::tuple<FArgs...>> m_filter;

    public:
        BasicGlobalEvent() noexcept : m_filter(std::nullopt) {}
        ~BasicGlobalEvent() noexcept = default;

        BasicGlobalEvent(FArgs... args) noexcept : m_filter(std::in_place, std::move(args)...) {}

        template<class Callable>
        requires std::is_invocable_v<Callable, PArgs...>
        ListenerHandle listen(Callable listener, int priority = 0) const noexcept {
            if (m_filter.has_value()) {
                return std::apply([&](auto&&... fargs) {
                    return Event1Type(std::move(fargs)...).listen(std::move(listener), priority);
                }, *m_filter);
            }
            else {
                if constexpr (std::is_convertible_v<std::invoke_result_t<Callable, PArgs...>, bool>) {
                    return Event2Type().listen([listener = std::move(listener)](FArgs... fargs, PArgs... pargs) {
                        return static_cast<bool>(std::invoke(listener, std::forward<PArgs>(pargs)...));
                    }, priority);
                }
                else {
                    return Event2Type().listen([listener = std::move(listener)](FArgs... fargs, PArgs... pargs) {
                        std::invoke(listener, std::forward<PArgs>(pargs)...);
                        return false;
                    }, priority);
                }
            }
        }

        template <class Callable>
        requires std::is_invocable_v<Callable, FArgs..., PArgs...>
        ListenerHandle listen(Callable listener, int priority = 0) const noexcept {
            if (!m_filter.has_value()) {
                return Event2Type().listen(std::move(listener), priority);
            }
            else {
                return std::apply([&](auto&&... fargs) {
                    if constexpr (std::is_convertible_v<std::invoke_result_t<Callable, FArgs..., PArgs...>, bool>) {
                        return Event1Type(fargs...).listen([listener = std::move(listener), ...fargs = std::move(fargs)](PArgs... pargs) {
                            return static_cast<bool>(std::invoke(listener, std::move(fargs)..., std::forward<PArgs>(pargs)...));
                        }, priority);
                    }
                    else {
                        return Event1Type(fargs...).listen([listener = std::move(listener), ...fargs = std::move(fargs)](PArgs... pargs) {
                            std::invoke(listener, std::move(fargs)..., std::forward<PArgs>(pargs)...);
                            return false;
                        }, priority);
                    }
                }, *m_filter);
            }
        }

        bool send(PArgs... args) noexcept(std::is_nothrow_invocable_v<geode::CopyableFunction<PReturn(PArgs...)>, PArgs...>) {
            if (m_filter.has_value()) {
                auto filterCopy = *m_filter;
                auto ret = std::apply([&](auto&&... fargs) {
                    return Event1Type(std::move(fargs)...).send(std::forward<PArgs>(args)...);
                }, std::move(filterCopy));
                if (ret) return true;

                return std::apply([&](auto&&... fargs) {
                    return Event2Type().send(std::move(fargs)..., std::forward<PArgs>(args)...);
                }, std::move(*m_filter));
            }
            return false;
        }
    };
}

    template<class Marker, class GFunc, class PFunc, class... FArgs>
    struct GlobalEvent : public comm::BasicGlobalEvent<Marker, false, GFunc, PFunc, FArgs...> {
        using comm::BasicGlobalEvent<Marker, false, GFunc, PFunc, FArgs...>::BasicGlobalEvent;
    };

    template<class Marker, class GFunc, class PFunc, class... FArgs>
    struct ThreadSafeGlobalEvent : public comm::BasicGlobalEvent<Marker, true, GFunc, PFunc, FArgs...> {
        using comm::BasicGlobalEvent<Marker, true, GFunc, PFunc, FArgs...>::BasicGlobalEvent;
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

}