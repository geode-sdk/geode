#pragma once

#include <Event.hpp>
#include <string>
#include <tuple>
#include <functional>

namespace geode {
	// Mod interoperability

	template <typename ...Args>
	class DispatchEvent : public Event {
		std::string m_selector;
		std::tuple<Args...> m_args;
	public:
		DispatchEvent(std::string const& name, Args... args)
		: m_selector(name), m_args(std::make_tuple(args...)) {}

		std::string const& selector() {
			return m_selector;
		}
	};

	template <typename ...Args>
	class DispatchHandler : public EventHandler<DispatchEvent<Args...>> {
		std::string m_selector;
		std::function<void(Args...)> m_callback;

		DispatchHandler(std::string const& name, std::function<void(Args...)> callback)
		: m_selector(name), m_callback(callback) {}
	 public:
	 	bool handle(DispatchEvent<Args...>* ev) {
	 		if (ev->name() == m_selector) {
	 			std::apply(m_callback, ev->m_args);
	 		}
	 		return true;
	 	}

	 	static DispatchHandler* create(std::string const& name, std::function<void(Args...)> callback) {
	 		return new DispatchHandler(name, callback);
	 	}
	};
}