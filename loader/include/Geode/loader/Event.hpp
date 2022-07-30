#pragma once

#include <Geode/DefaultInclude.hpp>
#include <type_traits>
#include "Mod.hpp"

namespace geode {
	class Mod;
	class Event;

	struct GEODE_DLL BasicEventHandler {
		virtual bool onEvent(Event*) = 0;

		void listen();
		void unlisten();
	};

	class GEODE_DLL Event {
		static std::vector<BasicEventHandler*> handlers;
	 	friend BasicEventHandler;

	 	Mod* m_sender;
	 public:
	 	static std::vector<BasicEventHandler*> const& getHandlers();

	 	void postFrom(Mod* sender);
	 	inline void post() {
	 		postFrom(Mod::get());
	 	}

	 	Mod* sender();

	 	virtual ~Event();
	};

	template <typename T>
	class EventHandler : public BasicEventHandler {
	 public:
		virtual bool handle(T*) = 0;
		bool onEvent(Event* ev) override {
			if (auto myev = dynamic_cast<T*>(ev)) {
				return handle(myev);
			}
			return true;
		}

		EventHandler() {
			listen();
		}
	};
}
