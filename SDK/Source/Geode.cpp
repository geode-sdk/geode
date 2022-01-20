#include <InterfaceBase.hpp>
#include <stdexcept>
#include <set>
#include <iostream>

cocos2d::CCDestructor::~CCDestructor() {
	destructorLock.erase(this);
};

GEODE_API bool GEODE_CALL geode_load(geode::Mod* mod) {
    MyMod = mod;
    for (auto i : modContainer.m_mods) {
        mod->addHook(
            (void*)i.hookAddr,
            (void*)i.funcLocation
        );
    }
    return true;
}

GEODE_API void GEODE_CALL geode_unload() {

}
