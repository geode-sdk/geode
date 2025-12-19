#pragma once

#include "../utils/casts.hpp"

#include <Geode/DefaultInclude.hpp>
#include <Geode/utils/function.hpp>
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
    private:
        class Impl;
        std::unique_ptr<Impl> m_impl;

        DefaultEventListenerPool();
        ~DefaultEventListenerPool() override;

    public:
        bool add(EventListenerProtocol* listener) override;
        void remove(EventListenerProtocol* listener) override;
        ListenerResult handle(Event* event) override;

        static DefaultEventListenerPool* get();

        template <class E>
        static EventListenerPool* getForEvent() {
            static std::shared_ptr<DefaultEventListenerPool> pool = DefaultEventListenerPool::create();
            return pool.get();
        }

        static std::shared_ptr<DefaultEventListenerPool> create();   
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

        virtual ListenerResult handle(geode::Function<Callback> fn, T* e) {
            return fn(e);
        }

        virtual EventListenerPool* getPool() const {
            return DefaultEventListenerPool::get();
        }

        virtual void setListener(EventListenerProtocol* listener) {
            m_listener = listener;
        }
        virtual EventListenerProtocol* getListener() const {
            return m_listener;
        }

        virtual ~EventFilter() = default;
    };

    template <typename T>
    concept is_filter = // # no need to do this IMO - HJfod # std::is_base_of_v<EventFilter<typename T::Event>, T> &&
        requires(T a, T const& ca) {
            typename T::Callback;
            typename T::Event;
            { a.handle(std::declval<geode::Function<typename T::Callback>&>(), std::declval<typename T::Event*>()) } -> std::same_as<ListenerResult>;
            { ca.getPool() } -> std::convertible_to<EventListenerPool*>;
            { a.setListener(std::declval<EventListenerProtocol*>()) } -> std::same_as<void>;
            { ca.getListener() } -> std::convertible_to<EventListenerProtocol*>;
        };

    template <is_filter T>
    class EventListener : public EventListenerProtocol {
    public:
        using Callback = typename T::Callback;
        template <typename C>
            requires std::is_class_v<C>
        using MemberFn = typename to_member<C, Callback>::value;

        ListenerResult handle(Event* e) override {
            if (m_callback) {
                if (auto myev = cast::typeinfo_cast<typename T::Event*>(e)) {
                    return m_filter.handle(m_callback, myev);
                }
            }
            return ListenerResult::Propagate;
        }

        EventListenerPool* getPool() const override {
            return m_filter.getPool();
        }

        EventListener(T filter = T()) : m_filter(filter) {
            m_filter.setListener(this);
            this->enable();
        }

        EventListener(geode::Function<Callback> fn, T filter = T())
          : m_callback(std::move(fn)), m_filter(filter)
        {
            m_filter.setListener(this);
            this->enable();
        }

        template <class C>
        EventListener(C* cls, MemberFn<C> fn, T filter = T()) :
            EventListener(std::bind(fn, cls, std::placeholders::_1), filter)
        {
            m_filter.setListener(this);
            this->enable();
        }

        EventListener(EventListener&& other)
          : m_callback(std::move(other.m_callback)),
            m_filter(std::move(other.m_filter))
        {
            m_filter.setListener(this);
            other.disable();
            this->enable();
        }

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

        void bind(geode::Function<Callback> fn) {
            m_callback = std::move(fn);
        }

        template <typename C>
        void bind(C* cls, MemberFn<C> fn) {
            m_callback = std::bind(fn, cls, std::placeholders::_1);
        }

        void setFilter(T filter) {
            m_filter = std::move(filter);
            m_filter.setListener(this);
        }

        T& getFilter() {
            return m_filter;
        }

        T const& getFilter() const {
            return m_filter;
        }

        geode::Function<Callback>& getCallback() {
            return m_callback;
        }

    protected:
        geode::Function<Callback> m_callback = nullptr;
        T m_filter;
    };

    class GEODE_DLL [[nodiscard]] Event {
    private:
        friend EventListenerProtocol;

    protected:
        virtual EventListenerPool* getPool() const;

    public:
        Mod* sender;

        ListenerResult postFromMod(Mod* sender);
        
        template <class _ = void>
        inline ListenerResult post() {
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