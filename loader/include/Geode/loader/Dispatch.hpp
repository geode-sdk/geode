#pragma once

#include "Event.hpp"

#include <functional>
#include <string>
#include <tuple>

namespace geode {
    // Mod interoperability

    GEODE_DLL std::unordered_map<std::string, EventListenerPool*>& dispatchPools();

    template <class... Args>
    class DispatchEvent : public Event {
    protected:
        std::string m_id;
        std::tuple<Args...> m_args;
    
    public:
        DispatchEvent(std::string const& id, Args... args)
          : m_id(id), m_args(std::make_tuple(args...)) {}
        
        std::tuple<Args...> getArgs() const {
            return m_args;
        }

        std::string getID() const {
            return m_id;
        }

        EventListenerPool* getPool() const override {
            if (dispatchPools().count(m_id) == 0) {
                dispatchPools()[m_id] = DefaultEventListenerPool::create();
            }
            return dispatchPools()[m_id];
        }
    };

    template <class... Args>
    class DispatchFilter : public EventFilter<DispatchEvent<Args...>> {
    protected:
        std::string m_id;

    public:
        using Ev = DispatchEvent<Args...>;
        using Callback = ListenerResult(Args...);

        EventListenerPool* getPool() const {
            if (dispatchPools().count(m_id) == 0) {
                dispatchPools()[m_id] = DefaultEventListenerPool::create();
            }
            return dispatchPools()[m_id];
        }

        ListenerResult handle(utils::MiniFunction<Callback> fn, Ev* event) {
            if (event->getID() == m_id) {
                return std::apply(fn, event->getArgs());
            }
            return ListenerResult::Propagate;
        }

        DispatchFilter(std::string const& id) : m_id(id) {}
        DispatchFilter(DispatchFilter const&) = default;
    };
}