// included by default in every geode project

#include <Geode/Loader.hpp>

GEODE_API bool GEODE_CALL geode_implicit_load(geode::Mod* m) {
	geode::Mod::setSharedMod(m);
	geode::log::releaseSchedules(m);
	geode::Loader::get()->releaseScheduledFunctions(m);
	return true;
}
