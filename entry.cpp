// included by default in every geode project

#include <Geode/Geode.hpp>

GEODE_API bool GEODE_CALL geode_implicit_load(geode::Mod* m) {
	geode::Interface::get()->init(m);
	return true;
}
