#include <Geode/Geode.hpp>
#include <Geode/modify/CCMenuItem.hpp>
#include <Geode/modify/CCMenu.hpp>

using namespace geode::prelude;

class $modify(CCMenuItem) {
    void activate() {
        this->retain();
        CCMenuItem::activate();
        this->release();
    }
};

class $modify(CCMenu) {
    void ccTouchEnded(CCTouch* touch, CCEvent* event) {
        this->retain();
        CCMenu::ccTouchEnded(touch, event);
        this->release();
    }
};