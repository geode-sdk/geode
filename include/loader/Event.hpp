#pragma once

#include <string>
#include <typeinfo>
#include <memory>
#include <exception>
#include <map>
#include <Macros.hpp>
#include <any>
#include <variant>

#ifndef GEODE_DLL
#define GEODE_DLL
#endif

namespace geode {
    class EventCenter;
    class Mod;

    template <typename T = std::monostate>
    struct EventInfo {
        std::string selector;
        inline EventInfo(std::string sel) : selector(sel) {}
        inline EventInfo() {}
    };

    template <typename T = std::monostate>
    struct ConstEventInfo {
        char const* selector;
        inline constexpr ConstEventInfo(char const* sel) : selector(sel) {}
        inline constexpr ConstEventInfo() {}

        inline operator EventInfo<T>() {
            return EventInfo<T>(selector);
        }
    };

    template <typename T = std::monostate>
    class Event {
    protected:
        EventInfo<T> m_info;
        T m_object;
        Mod* m_sender;
    public:
        inline T const& object() const {
        	return m_object;
        }

        inline std::string const& selector() const { return m_info.selector; }
        inline Mod* sender() const { return m_sender; }

        inline Event(EventInfo<T> inf, T obj, Mod* sender) :
            m_info(inf),
            m_object(obj),
            m_sender(sender) {}

        inline Event(std::string sel, T obj, Mod* sender) : Event(EventInfo<T>(sel), obj, sender) {}

        inline Event(std::string sel, Mod* sender) : Event(sel, T(), sender) {}
        inline Event(EventInfo<T> inf, Mod* sender) : Event(inf, T(), sender) {}
        // Event(std::string sel) : Event(sel, Interface::get()->mod()) {}

        inline Event(Event&& a) : m_info(a.m_info), m_sender(a.m_sender), m_object(std::move(a.m_object)) {}

        friend class EventCenter;
    };

    template <typename T = std::monostate>
    struct Observer {
        EventInfo<T> m_info;
        Mod* m_mod;
        std::function<void(Event<T> const&)> m_callback;

        template <typename U = std::monostate>
        inline Observer<U>* into() {
            return reinterpret_cast<Observer<U>*>(this);
        }
    };

    class GEODE_DLL EventCenter {
    public:
        std::map<Mod*, std::map<std::string, std::vector<Observer<std::monostate>*>>> m_observers;
        static EventCenter* shared;
    public:

        EventCenter();
        static EventCenter* get();

        template <typename T>
        void send(Event<T> n, Mod* m) {
            for (auto& obs : m_observers[m][n.selector()]) {
                obs->template into<T>()->m_callback(n);
            }
        }

        template <typename T>
        void broadcast(Event<T> n) {
            for (auto& [k, v] : m_observers) {
                for (auto& obs : v[n.selector()]) {
                    obs->template into<T>()->m_callback(n);
                }
            }
        }

        template <typename T = std::monostate>
        Observer<std::monostate>* registerObserver(Mod* m, EventInfo<T> info, std::function<void(Event<T> const&)> cb) {
            Observer<T>* ob = new Observer<T>;
            ob->m_info = info;
            ob->m_callback = cb;
            ob->m_mod = m;

            m_observers[m][info.selector].push_back(ob->into());

            return ob->into();
        }

        template <typename T = std::monostate>
        Observer<std::monostate>* registerObserver(Mod* m, std::string sel, std::function<void(Event<T> const&)> cb) {
            return registerObserver(m, EventInfo<T>(sel), cb);
        }

        template <typename T = std::monostate>
        inline Observer<std::monostate>* registerObserver(std::string sel, std::function<void(Event<T> const&)> cb);

        template <typename T = std::monostate>
        inline Observer<std::monostate>* registerObserver(EventInfo<T> info, std::function<void(Event<T> const&)> cb);

        void unregisterObserver(Observer<std::monostate>* ob);
        std::vector<Observer<std::monostate>*> getObservers(std::string selector, Mod* m);
    };
}
#define _$observe3(sel, T, data, ctr) \
    void $_observer##ctr(geode::Event<T> const&); \
    static auto $_throw##ctr = (([](){ \
        geode::Interface::get()->scheduleOnLoad(+[](Mod* m) { \
            geode::EventCenter::get()->registerObserver<T>( \
                m, sel, $_observer##ctr \
            ); \
        }); \
    })(), 0); \
    void $_observer##ctr(geode::Event<T> const& data)

#define _$observe1(sel, ctr) _$observe3(sel, std::monostate, , ctr)

#define $observe(...) GEODE_INVOKE(GEODE_CONCAT(_$observe, GEODE_NUMBER_OF_ARGS(__VA_ARGS__)), __VA_ARGS__, __COUNTER__)

