#pragma once

#include "Mod.hpp"

#include <Geode/DefaultInclude.hpp>
#include <type_traits>
#include <unordered_set>

namespace geode {
    class Mod;
    class Event;

    enum class ListenerResult {
        Propagate,
        Stop
    };

    struct GEODE_DLL EventListenerProtocol {
        virtual void enable();
        virtual void disable();
        virtual ListenerResult passThrough(Event*) = 0;
        virtual ~EventListenerProtocol();
    };

    template <typename C, typename T>
    struct to_member;

    template <typename C, typename R, typename ...Args>
    struct to_member<C, R(Args...)> {
        using value = R(C::*)(Args...);
    };

    template <typename T>
    concept is_event = std::is_base_of_v<Event, T>;

    template <is_event T>
    class EventFilter {
    public:
        using Callback = ListenerResult(T*);
        using Event = T;

        ListenerResult handle(std::function<Callback> fn, T* e) {
            return fn(e);
        }
    };
 
    template <typename T>
    concept is_filter =
        std::is_base_of_v<EventFilter<typename T::Event>, T> && 
        requires(T a) {
            a.handle(std::declval<typename T::Callback>(), std::declval<typename T::Event*>());
        };

    template <is_filter T>
    class EventListener : public EventListenerProtocol {
    public:
        using Callback = typename T::Callback;
        template <typename C> requires std::is_class_v<C>
        using MemberFn = typename to_member<C, Callback>::value;

        ListenerResult passThrough(Event* e) override {
            if (auto myev = dynamic_cast<typename T::Event*>(e)) {
                return m_filter.handle(m_callback, myev);
            }
            return ListenerResult::Propagate;
        }

        EventListener(T filter = T()) {
            this->enable();
        }
        EventListener(std::function<Callback> fn, T filter = T()) : m_callback(fn), m_filter(filter) {
            this->enable();
        }
        EventListener(Callback* fnptr, T filter = T()) : m_callback(fnptr), m_filter(filter) {
            this->enable();
        }

        template <class C>
        EventListener(C* cls, MemberFn<C> fn, T filter = T()) : EventListener(std::bind(fn, cls, std::placeholders::_1), filter) {
            this->enable();
        }

        void bind(std::function<Callback> fn) {
            m_callback = fn;
        }
        template <typename C>
        void bind(C* cls, MemberFn<C> fn) {
            m_callback = std::bind(fn, cls, std::placeholders::_1);
        }
    protected:
        std::function<Callback> m_callback;
        T m_filter;
    };

    class GEODE_DLL Event {
        static std::unordered_set<EventListenerProtocol*> s_listeners;
        Mod* m_sender;
        friend EventListenerProtocol;
    public:
        void postFrom(Mod* sender);

        inline void post() {
            postFrom(Mod::get());
        }

        Mod* getSender();

        virtual ~Event();
    };
}
