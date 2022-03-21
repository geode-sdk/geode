#pragma once

#include <string>
#include <typeinfo>
#include <memory>
#include <exception>
#include <unordered_map>
#include <gen/Header.hpp>
#include <any>
#include <variant>

#ifndef GEODE_DLL
#define GEODE_DLL
#endif

namespace geode {
    class NotificationCenter;
    class Mod;

    template <typename T = std::monostate>
    struct GEODE_DLL NotifInfo {
        std::string selector;
        NotifInfo(std::string sel) : selector(sel) {}
        NotifInfo() {}
    };

    template <typename T = std::monostate>
    class GEODE_DLL Notification {
    protected:
        NotifInfo<T> m_info;
        T m_object;
        Mod* m_sender;
    public:
        T const& object() const {
        	return m_object;
        }

        inline std::string const& selector() const { return m_info.selector; }
        inline Mod* sender() const { return m_sender; }

        Notification(NotifInfo<T> inf, T obj, Mod* sender) :
            m_info(inf),
            m_object(obj),
            m_sender(sender) {}

        Notification(std::string sel, T obj, Mod* sender) : Notification(NotifInfo<T>(sel), obj, sender) {}

        Notification(std::string sel, Mod* sender) : Notification(sel, T(), sender) {}
        Notification(NotifInfo<T> inf, Mod* sender) : Notification(inf, T(), sender) {}
        // Notification(std::string sel) : Notification(sel, Interface::get()->mod()) {}

        Notification(Notification&& a) : m_info(a.m_info), m_sender(a.m_sender), m_object(std::move(a.m_object)) {}

        friend class NotificationCenter;
    };

    template <typename T = std::monostate>
    struct Observer {
        NotifInfo<T> m_info;
        Mod* m_mod;
        std::function<void(Notification<T> const&)> m_callback;

        template <typename U = std::monostate>
        Observer<U>* into() {return reinterpret_cast<Observer<U>*>(this);}
    };

    class GEODE_DLL NotificationCenter {
    protected:
        std::map<Mod*, std::map<std::string, std::vector<Observer<std::monostate>*>>> m_observers;
        static NotificationCenter* shared;
    public:

        NotificationCenter();
        static NotificationCenter* get();

        template <typename T>
        void send(Notification<T> n, Mod* m) {
            for (auto& obs : m_observers[m][n.selector()]) {
                obs->template into<T>()->m_callback(n);
            }
        }

        template <typename T>
        void broadcast(Notification<T> n) {
            for (auto& [k, v] : m_observers) {
                for (auto& obs : v[n.selector()]) {
                    obs->template into<T>()->m_callback(n);
                }
            }
        }

        template <typename T = std::monostate>
        Observer<std::monostate>* registerObserver(Mod* m, NotifInfo<T> info, std::function<void(Notification<T> const&)> cb) {
            Observer<T>* ob = new Observer<T>;
            ob->m_info = info;
            ob->m_callback = cb;
            ob->m_mod = m;

            m_observers[m][info.selector].push_back(ob->into());

            return ob->into();
        }
        // inline Observer* registerObserver(std::string selector, callback_t cb) {
        //     return registerObserver(Interface::get()->mod(), selector, cb);
        // }
        void unregisterObserver(Observer<std::monostate>* ob);
        std::vector<Observer<std::monostate>*> getObservers(std::string selector, Mod* m);
    };
}
