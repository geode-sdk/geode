#pragma once

#include "Event.hpp"

namespace geode::event::v2 {
	template <is_event T>
	struct WrapFilter : public EventFilter<T> {
	    std::function<bool(T*)> m_filterFunc;

	    WrapFilter(std::function<bool(T*)> ff) : m_filterFunc(ff) {}

	    ListenerResult handle(std::function<ListenerResult(T*)> fn, T* event) {
	        if (m_filterFunc(event))
	            return fn(event);
	        return ListenerResult::Propagate;
	    }
	};

	template <typename T>
	struct EventHandler : public EventListener<WrapFilter<T>> {
	    EventHandler(std::function<bool(T*)> filterFunc) : EventListener<WrapFilter<T>>(WrapFilter(filterFunc)) {}
	};
}
