#pragma once

#include "Event.hpp"

namespace geode::event::v2 {
	template <is_event T>
	struct WrapFilter : public EventFilter<T> {
	    geode::Function<bool(T*)> m_filterFunc;

	    WrapFilter(geode::Function<bool(T*)> ff) : m_filterFunc(std::move(ff)) {}

	    ListenerResult handle(geode::Function<ListenerResult(T*)>& fn, T* event) {
	        if (m_filterFunc(event))
	            return fn(event);
	        return ListenerResult::Propagate;
	    }
	};

	template <typename T>
	struct EventHandler : public EventListener<WrapFilter<T>> {
	    EventHandler(geode::Function<bool(T*)> filterFunc) : EventListener<WrapFilter<T>>(WrapFilter(std::move(filterFunc))) {}

		EventHandler& listen(geode::Function<ListenerResult(T*)> fn) {
			bind(std::move(fn));
			return *this;
		}

		static EventHandler* create(geode::Function<bool(T*)> filterFunc) {
			return new EventHandler(std::move(filterFunc));
		}
	};
}
