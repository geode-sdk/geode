#pragma once

#include <Interface.hpp>
#include <string>
#include <typeinfo>
#include <memory>
#include <exception>
#include <queue>
#include <gen/Header.hpp>

#ifndef GEODE_DLL
#define GEODE_DLL
#endif

namespace geode {
    class NotificationCenter;
    class Mod;

    class GEODE_DLL Notification {
     protected:
        std::string m_selector;
        std::unique_ptr<char> m_object;
        std::string m_object_type;
        Mod* m_sender;
     public:
        template <typename T>
        bool is_type() const {
            return m_object_type == typeid(T).name();
        }

        template <typename T>
        T& unwrap_object() {
            if (is_type<T>()) {
                return reinterpret_cast<T&>(m_object.get());
            } else {
                throw std::exception();
            }
        }

        inline std::string const& selector() const { return m_selector; }
        inline Mod* sender() const { return m_sender; }

        template <typename T>
        Notification(std::string sel, T obj, Mod* sender) : 
            m_selector(sel),
            m_object(reinterpret_cast<char*>(&obj)),
            m_object_type(typeid(T).name()),
            m_sender(sender) {}

        Notification(std::string sel, Mod* sender) : Notification(sel, nullptr, sender) {}
        Notification(std::string sel) : Notification(sel, Interface::get()->mod()) {}

        Notification(Notification&& a) : m_selector(a.m_selector), m_object_type(a.m_object_type), m_sender(a.m_sender), m_object(std::move(a.m_object)) {}

        friend class NotificationCenter;
    };

    struct Observer {
        std::string m_selector;
        Mod* m_mod;
        std::function<void(Notification const&)> m_callback;
    };

    class GEODE_DLL NotificationCenter : public cocos2d::CCObject {
     protected:
        std::queue<std::pair<Mod*, Notification>> m_queue;
        std::vector<Observer> m_observers;
        static NotificationCenter* shared;
     public:
        using callback_t = std::function<void(Notification const&)>;

        NotificationCenter();
        static NotificationCenter* get();

        void sendSync(Notification n, Mod* m);
        void broadcastSync(Notification n);
        void send(Notification n, Mod* m);
        void broadcast(Notification n);

        Observer* registerObserver(Mod* m, std::string selector, callback_t cb);
        inline Observer* registerObserver(std::string selector, callback_t cb) {
            return registerObserver(Interface::get()->mod(), selector, cb);
        }
        void unregisterObserver(Observer* ob);
        std::vector<Observer*> getObservers(std::string selector, Mod* m);
     protected:
        void update(float dt);
    };
}
