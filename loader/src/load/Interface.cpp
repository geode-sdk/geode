#include <Geode/loader/Mod.hpp>
#include <Geode/loader/Interface.hpp>
#include <Geode/loader/Hook.hpp>
#include <Geode/loader/Log.hpp>
#include <Geode/loader/Loader.hpp>

USE_GEODE_NAMESPACE();

void Interface::init(Mod* mod) {
	if (!m_mod) {
		m_mod = mod;
		for (auto const& hook : m_scheduledHooks) {
			std::invoke(hook.m_addFunction, m_mod, hook.m_displayName, hook.m_address);
		}
		m_scheduledHooks.clear();

		for (auto const& fn : m_scheduledFunctions) {
			std::invoke(fn);
		}
		m_scheduledFunctions.clear();
	}
}

void Interface::scheduleOnLoad(ScheduledFunction function) {
	m_scheduledFunctions.push_back(function);
}

Interface* Interface::create() {
	return new Interface;
}
