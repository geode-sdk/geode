#pragma once

#include "../utils/casts.hpp"
#include "../utils/MiniFunction.hpp"

#include <Geode/DefaultInclude.hpp>
#include <type_traits>
#include <unordered_set>

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
        virtual void handle(Event* event) = 0;
        virtual ~EventListenerPool() = default;

        EventListenerPool() = default;
        EventListenerPool(EventListenerPool const&) = delete;
        EventListenerPool(EventListenerPool&&) = delete;
    };
    
    class GEODE_DLL DefaultEventListenerPool : public EventListenerPool {
    protected:
        std::atomic_size_t m_locked = 0;
        std::vector<EventListenerProtocol*> m_listeners;
        std::vector<EventListenerProtocol*> m_toAdd;

    public:
        bool add(EventListenerProtocol* listener) override;
        void remove(EventListenerProtocol* listener) override;
        void handle(Event* event) override;

        static DefaultEventListenerPool* get();
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
    public:
        using Callback = ListenerResult(T*);
        using Event = T;

        ListenerResult handle(utils::MiniFunction<Callback> fn, T* e) {
            return fn(e);
        }

        EventListenerPool* getPool() const {
            return DefaultEventListenerPool::get();
        }
    };

    template <typename T>
    concept is_filter = std::is_base_of_v<EventFilter<typename T::Event>, T> &&
        requires(T a) {
            a.handle(std::declval<typename T::Callback>(), std::declval<typename T::Event*>());
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

        EventListener(T filter = T()) {
            this->enable();
        }

        EventListener(utils::MiniFunction<Callback> fn, T filter = T())
          : m_callback(fn), m_filter(filter)
        {
            this->enable();
        }

        EventListener(Callback* fnptr, T filter = T()) : m_callback(fnptr), m_filter(filter) {
            this->enable();
        }

        template <class C>
        EventListener(C* cls, MemberFn<C> fn, T filter = T()) :
            EventListener(std::bind(fn, cls, std::placeholders::_1), filter) {
            this->enable();
        }

        EventListener(EventListener&& other)
          : m_callback(std::move(other.m_callback)),
            m_filter(std::move(other.m_filter))
        {
            other.disable();
            this->enable();
        }

        EventListener(EventListener const& other)
          : m_callback(other.m_callback),
            m_filter(other.m_filter)
        {
            other.disable();
            this->enable();
        }

        void bind(utils::MiniFunction<Callback> fn) {
            m_callback = fn;
        }

        template <typename C>
        void bind(C* cls, MemberFn<C> fn) {
            m_callback = std::bind(fn, cls, std::placeholders::_1);
        }

        void setFilter(T filter) {
            m_filter = filter;
        }

        T getFilter() const {
            return m_filter;
        }

        utils::MiniFunction<Callback>& getCallback() {
            return m_callback;
        }

    protected:
        utils::MiniFunction<Callback> m_callback = nullptr;
        T m_filter;
    };

    class GEODE_DLL [[nodiscard]] Event {
    private:
        friend EventListenerProtocol;

    protected:
        virtual EventListenerPool* getPool() const;

    public:
        Mod* sender;

        void postFrom(Mod* sender);
        template<class = void>
        void post() {
            postFrom(getMod());
        }
        
        virtual ~Event();
    };
}
