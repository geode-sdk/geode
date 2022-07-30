#include <Geode/loader/Interface.hpp>
#include <Geode/loader/Mod.hpp>
#include <Geode/loader/Hook.hpp>
#include <Geode/loader/Log.hpp>
#include <Geode/loader/Loader.hpp>

USE_GEODE_NAMESPACE();

void Interface::init(Mod* mod) {
	if (!this->m_mod) {
		this->m_mod = mod;
		for (auto const& hook : this->m_scheduledHooks) {
			std::invoke(hook.m_addFunction, this->m_mod, hook.m_displayName, hook.m_address);
		}
		this->m_scheduledHooks.clear();

		for (auto const& log : this->m_scheduledLogs) {
			this->m_mod->logInfo(log.m_info, log.m_severity);
		}
		this->m_scheduledLogs.clear();

		for (auto const& fn : this->m_scheduledFunctions) {
			std::invoke(fn);
		}
		this->m_scheduledFunctions.clear();
	}
}

void Interface::logInfo(
	std::string const& info,
	Severity severity
) {
	if (this->m_mod) {
		return this->m_mod->logInfo(info, severity);
	}
	this->m_scheduledLogs.push_back({ info, severity });
}

void Interface::scheduleOnLoad(ScheduledFunction function) {
	this->m_scheduledFunctions.push_back(function);
}

Interface* Interface::create() {
	return new Interface;
}
