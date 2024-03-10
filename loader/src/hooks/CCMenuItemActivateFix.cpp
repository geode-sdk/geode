#include <Geode/Geode.hpp>
#include <Geode/modify/CCMenuItem.hpp>

using namespace geode::prelude;

class $modify(CCMenuItem) {
    void activate() {
        this->retain();
        CCMenuItem::activate();
        this->release();
    }
};