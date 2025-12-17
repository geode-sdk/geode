#pragma once

#include "../utils/casts.hpp"

#include <Geode/DefaultInclude.hpp>
#include <functional>
#include <memory>
#include <type_traits>
#include <mutex>
#include <deque>
#include <atomic>
#include <vector>

namespace geode {
    class Mod;
    class Event;
    class EventListenerProtocol;

    Mod* getMod();

    enum class ListenerResult {
        Propagate,
        Stop
    };

    struct GEODE_DLL EventListenerPool {
        virtual bool add(EventListenerProtocol* listener) = 0;
        virtual void remove(EventListenerProtocol* listener) = 0;
        virtual ListenerResult handle(Event* event) = 0;
        virtual ~EventListenerPool() = default;

        EventListenerPool() = default;
        EventListenerPool(EventListenerPool const&) = delete;
        EventListenerPool(EventListenerPool&&) = delete;
    };

    template <class... Args>
    class DispatchEvent;

    template <class... Args>
    class DispatchFilter;

    class GEODE_DLL DefaultEventListenerPool : public EventListenerPool {
    protected:
        // fix this in Geode 4.0.0
        struct Data {
            std::atomic_size_t m_locked = 0;
            std::mutex m_mutex;
            std::deque<EventListenerProtocol*> m_listeners;
            std::vector<EventListenerProtocol*> m_toAdd;
        };
        std::unique_ptr<Data> m_data;

    private:
        static DefaultEventListenerPool* create();
        DefaultEventListenerPool();

    public:
        bool add(EventListenerProtocol* listener) override;
        void remove(EventListenerProtocol* listener) override;
        ListenerResult handle(Event* event) override;

        static DefaultEventListenerPool* get();

        template <class... Args>
        friend class DispatchEvent;

        template <class... Args>
        friend class DispatchFilter;
    };

    class GEODE_DLL EventListenerProtocol {
    private:
        EventListenerPool* m_pool = nullptr;

    public:
        bool enable();
        void disable();

        virtual EventListenerPool* getPool() const;
        virtual ListenerResult handle(Event*) = 0;
        virtual ~EventListenerProtocol();
    };

    template <typename C, typename T>
    struct to_member;

    template <typename C, typename R, typename... Args>
    struct to_member<C, R(Args...)> {
        using value = R (C::*)(Args...);
    };

    template <typename T>
    concept is_event = std::is_base_of_v<Event, T>;

    template <is_event T>
    class EventFilter {
    protected:
        EventListenerProtocol* m_listener = nullptr;

    public:
        using Callback = ListenerResult(T*);
        using Event = T;

        template <typename F> requires (std::is_invocable_r_v<ListenerResult, F, T*>)
        ListenerResult handle(F&& fn, T* e) {
            return fn(e);
        }

        EventListenerPool* getPool() const {
            return DefaultEventListenerPool::get();
        }

        void setListener(EventListenerProtocol* listener) {
            m_listener = listener;
        }
        EventListenerProtocol* getListener() const {
            return m_listener;
        }
    };

    template <typename T>
    concept is_filter = // # no need to do this IMO - HJfod # std::is_base_of_v<EventFilter<typename T::Event>, T> &&
        requires(T a, T const& ca) {
            typename T::Callback;
            typename T::Event;
            { a.handle(std::declval<typename T::Callback>(), std::declval<typename T::Event*>()) } -> std::same_as<ListenerResult>;
            { ca.getPool() } -> std::convertible_to<EventListenerPool*>;
            { a.setListener(std::declval<EventListenerProtocol*>()) } -> std::same_as<void>;
            { ca.getListener() } -> std::convertible_to<EventListenerProtocol*>;
        };

    template <typename T>
    struct into_filter;
    template <is_filter T>
    struct into_filter<T> {
        using type = T;
    };
    template <is_event T>
    struct into_filter<T> {
        using type = EventFilter<T>;
    };

    template <typename T> requires is_filter<T> || is_event<T>
    class EventListener : public EventListenerProtocol {
    public:
        using Filter = into_filter<T>::type;
        using Callback = typename Filter::Callback;
        template <typename C>
            requires std::is_class_v<C>
        using MemberFn = typename to_member<C, Callback>::value;

        ListenerResult handle(Event* e) override {
            if (m_callback) {
                if (auto myev = cast::typeinfo_cast<typename Filter::Event*>(e)) {
                    return m_filter.handle(m_callback, myev);
                }
            }
            return ListenerResult::Propagate;
        }

        EventListenerPool* getPool() const override {
            return m_filter.getPool();
        }

        EventListener(T filter = Filter()) requires is_filter<T> : m_filter(std::move(filter)) {
            m_filter.setListener(this);
            this->enable();
        }

        EventListener(std::function<Callback> fn, T filter = Filter()) requires is_filter<T>
          : m_callback(std::move(fn)), EventListener(std::move(filter)) {}

        EventListener(Callback* fnptr, T filter = Filter()) requires is_filter<T>
          : m_callback(fnptr), EventListener(std::move(filter)) {}

        template <class C>
        EventListener(C* cls, MemberFn<C> fn, T filter = Filter()) requires is_filter<T> :
            EventListener(std::bind(std::move(fn), cls, std::placeholders::_1), std::move(filter)) {}

        EventListener() requires is_event<T> : m_filter(Filter()) {
            m_filter.setListener(this);
            this->enable();
        }

        EventListener(std::function<Callback> fn) requires is_event<T>
          : m_callback(std::move(fn)), EventListener() {}

        EventListener(Callback* fnptr) requires is_event<T>
          : m_callback(fnptr), EventListener() {}

        template <class C>
        EventListener(C* cls, MemberFn<C> fn) requires is_event<T> :
            EventListener(std::bind(std::move(fn), cls, std::placeholders::_1)) {}

        EventListener(EventListener&& other)
          : m_callback(std::move(other.m_callback)),
            m_filter(std::move(other.m_filter))
        {
            m_filter.setListener(this);
            other.disable();
            this->enable();
        }

        EventListener(EventListener const& other)
          : m_callback(other.m_callback),
            EventListener(other.m_filter) {}

        EventListener& operator=(EventListener&& other) {
            if (this == &other) {
                return *this;
            }

            m_callback = std::move(other.m_callback);
            m_filter = std::move(other.m_filter);

            m_filter.setListener(this);
            other.disable();

            return *this;
        }

        void bind(std::function<Callback> fn) {
            m_callback = std::move(fn);
        }

        template <typename C>
        void bind(C* cls, MemberFn<C> fn) {
            m_callback = std::bind(std::move(fn), cls, std::placeholders::_1);
        }

        void setFilter(Filter filter) {
            m_filter = std::move(filter);
            m_filter.setListener(this);
        }

        Filter& getFilter() {
            return m_filter;
        }

        Filter const& getFilter() const {
            return m_filter;
        }

        std::function<Callback>& getCallback() {
            return m_callback;
        }

    protected:
        std::function<Callback> m_callback = nullptr;
        Filter m_filter;
    };

    class GEODE_DLL [[nodiscard]] Event {
    private:
        friend EventListenerProtocol;

    protected:
        virtual EventListenerPool* getPool() const;

    public:
        Mod* sender;

        ListenerResult postFromMod(Mod* sender);
        template<class = void>
        ListenerResult post() {
            return postFromMod(getMod());
        }

        virtual ~Event();
    };

    // Creates an EventListener that is active for the entire lifetime of the game. You have no way of disabling the listener, so only use this if you want to always listen for certain events!
    template <is_filter T>
    void globalListen(typename T::Callback callback, T filter = T()) {
        new EventListener<T>(callback, filter);
    }
}