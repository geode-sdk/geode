#include <Geode/ui/WindowResizeEvent.hpp>
#include <Geode/modify/CCEGLViewProtocol.hpp>
#include <cocos2d.h>

using namespace geode::prelude;

class $modify(CCEGLViewProtocol) {
    void setFrameSize(float width, float height) override {
        CCEGLViewProtocol::setFrameSize(width, height);
        WindowResizeEvent().send(CCDirector::get()->getWinSize());
    }
};