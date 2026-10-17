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
#include <asp/iter.hpp>
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

    struct OpaqueCallable {
        virtual ~OpaqueCallable() = default;

        virtual int getPriority() const noexcept = 0;
        virtual ReceiverHandle getHandle() const noexcept = 0;
        virtual void setPriority(int priority) noexcept = 0;
        virtual void setHandle(ReceiverHandle handle) noexcept = 0;
        virtual size_t getVersion() const noexcept = 0;
        virtual size_t getMinorVersion() const noexcept = 0;

        using SendFunction = geode::CopyableFunction<bool(OpaqueCallable*)>;
    };

    template <class Callable>
    struct TypedCallable : OpaqueCallable {
        Callable m_callable;
        int m_priority = 0;
        ReceiverHandle m_handle = 0;

        TypedCallable(Callable callable) : m_callable(callable) {}
        ~TypedCallable() override = default;

        int getPriority() const noexcept override {
            return m_priority;
        }

        ReceiverHandle getHandle() const noexcept override {
            return m_handle;
        }

        void setPriority(int priority) noexcept override {
            m_priority = priority;
        }

        void setHandle(ReceiverHandle handle) noexcept override {
            m_handle = handle;
        }

        size_t getVersion() const noexcept override {
            return 1;
        }

        size_t getMinorVersion() const noexcept override {
            return 0;
        }

        template <class ...Args>
        bool call(Args&&... args) const noexcept(std::is_nothrow_invocable_v<Callable, Args...>) {
            if (!m_callable) return false;
            if (this->getVersion() != 1) return false;

            if constexpr (std::is_same_v<void, decltype(std::invoke(m_callable, std::forward<Args>(args)...))>) {
                std::invoke(m_callable, std::forward<Args>(args)...);
                return false;
            } else {
                return std::invoke(m_callable, std::forward<Args>(args)...);
            }
        }
    };

    class EventCenterGlobal;

    class OpaquePort final {
        class Impl;
        std::unique_ptr<Impl> m_impl;

    public:
        OpaquePort();
        ~OpaquePort();

        ReceiverHandle addReceiver(OpaqueCallable* opaqueCallable, int priority) noexcept;

        size_t removeReceiver(ReceiverHandle handle) noexcept;

        size_t getReceiverCount() const noexcept;

        bool send(OpaqueCallable::SendFunction opaqueFunction);
    };

    class BaseFilter {
    public:
        virtual ~BaseFilter() noexcept = default;
        virtual bool operator==(BaseFilter const& other) const noexcept = 0;
        virtual size_t hash() const noexcept = 0;
        virtual BaseFilter* clone() const noexcept = 0;
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

    template<class Marker, class Func, class... FArgs>
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
                if (m_active) this->destroy();

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

        template <class Marker, class Func, class... FArgs>
        friend class BasicEvent;
    };

    template<class Marker, class PReturn, class... PArgs, class... FArgs>
    class BasicEvent<Marker, PReturn(PArgs...), FArgs...> : public BaseFilter {
    protected:
        using CallableType = geode::CopyableFunction<PReturn(PArgs...)>;

        using Self = BasicEvent<Marker, PReturn(PArgs...), FArgs...>;
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

        size_t hash() const noexcept override {
            auto seed = typenameHash<Marker>();
            std::apply([&seed](auto const&... elems) {
                (hashCombine(seed, elems), ...);
            }, m_filter);
            return seed;
        }

        ListenerHandle addReceiver(CallableType receiver, int priority = 0) const noexcept;
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

        bool send(PArgs... args);

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

    class GEODE_DLL EventCenterGlobal {
    private:
        class Impl;
        std::unique_ptr<Impl> m_impl;

        EventCenterGlobal();
        ~EventCenterGlobal();

    public:
        static EventCenterGlobal* get();

        using SendFuncType = geode::Function<bool(OpaquePort*)>;
        using AddFuncType = geode::Function<ReceiverHandle(OpaquePort*)>;
        using SizeFuncType = geode::Function<size_t(OpaquePort*)>;
        using RemoveFuncType = geode::Function<size_t(OpaquePort*)>;

        bool send(BaseFilter const* filter, OpaqueCallable::SendFunction sendFunction);
        ListenerHandle addReceiver(BaseFilter const* filter, OpaqueCallable* opaqueCallable, int priority) noexcept;
        size_t getReceiverCount(BaseFilter const* filter) noexcept;
        size_t removeReceiver(BaseFilter const* filter, ReceiverHandle handle) noexcept;
    };

    template <class Marker, class PReturn, class... PArgs, class... FArgs>
    bool BasicEvent<Marker, PReturn(PArgs...), FArgs...>::send(PArgs... args) {
        return EventCenterGlobal::get()->send(this, [&](OpaqueCallable* opaqueCallable) {
            auto typedCallable = static_cast<TypedCallable<CallableType>*>(opaqueCallable);
            return typedCallable->call(args...);
        });
    }

    template <class Marker, class PReturn, class... PArgs, class... FArgs>
    ListenerHandle BasicEvent<Marker, PReturn(PArgs...), FArgs...>::addReceiver(CallableType receiver, int priority) const noexcept {
        return EventCenterGlobal::get()->addReceiver(this, new TypedCallable<CallableType>(std::move(receiver)), priority);
    }

    template <class Marker, class PReturn, class... PArgs, class... FArgs>
    size_t BasicEvent<Marker, PReturn(PArgs...), FArgs...>::getReceiverCount() const noexcept {
        return EventCenterGlobal::get()->getReceiverCount(this);
    }

    template <class Marker, class PReturn, class... PArgs, class... FArgs>
    size_t BasicEvent<Marker, PReturn(PArgs...), FArgs...>::removeReceiver(ReceiverHandle handle) const noexcept {
        return EventCenterGlobal::get()->removeReceiver(this, handle);
    }
}

namespace geode {
    using ListenerHandle = comm::ListenerHandle;

    template<class Marker, class PFunc, class... FArgs>
    struct Event : public comm::BasicEvent<Marker, PFunc, FArgs...> {
        using comm::BasicEvent<Marker, PFunc, FArgs...>::BasicEvent;
    };

    template<class Marker, class PFunc, class... FArgs>
    struct ThreadSafeEvent : public comm::BasicEvent<Marker, PFunc, FArgs...> {
        using comm::BasicEvent<Marker, PFunc, FArgs...>::BasicEvent;
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
                    return Event1Type(std::move(fargs)...).send(args...);
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
