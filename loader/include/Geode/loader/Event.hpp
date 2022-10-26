#pragma once

#include <Geode/DefaultInclude.hpp>
#include <type_traits>
#include "Mod.hpp"
#include <unordered_set>

namespace geode {
	class Mod;
	class Event;

	enum class PassThrough : bool {
		Propagate,
		Stop,
	};

	struct GEODE_DLL BasicEventHandler {
		virtual PassThrough passThrough(Event*) = 0;

		virtual ~BasicEventHandler();

		void listen();
		void unlisten();
	};

	class GEODE_DLL Event {
		static std::unordered_set<BasicEventHandler*> s_handlers;

	 	friend BasicEventHandler;

	 	Mod* m_sender;

	public:
	 	static std::unordered_set<BasicEventHandler*> const& getHandlers();

	 	void postFrom(Mod* sender);
	 	inline void post() {
	 		postFrom(Mod::get());
	 	}

	 	Mod* getSender();

	 	virtual ~Event();
	};

	template <typename T>
	class EventHandler : public BasicEventHandler {
	public:
		virtual PassThrough handle(T*) = 0;
		PassThrough passThrough(Event* ev) override {
			if (auto myev = dynamic_cast<T*>(ev)) {
				return handle(myev);
			}
			return PassThrough::Propagate;
		}

		EventHandler() {
			listen();
		}
	};
}
