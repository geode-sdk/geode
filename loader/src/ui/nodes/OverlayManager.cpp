#include "Geode/ui/WindowResizeEvent.hpp"
#include <Geode/loader/ModEvent.hpp>
#include <Geode/Loader.hpp>
#include <Geode/ui/OverlayManager.hpp>

using namespace geode::prelude;

bool OverlayManager::init() {
    if (!CCNode::init()) return false;
    this->setContentSize(CCDirector::get()->getWinSize());
    this->setLayout(AnchorLayout::create());
    this->addEventListener(WindowResizeEvent(), [this](const CCSize& size){
        setContentSize(size);
        updateLayout();
    }, 0);
    return true;
}

OverlayManager* OverlayManager::get() {
    static OverlayManager* inst = nullptr;
    if (!inst) {
        inst = new OverlayManager();
        inst->init();
        inst->onEnter();
    }
    return inst;
}

$on_mod(Loaded) {
    queueInMainThread([] {
        CCDirector::get()->m_pNotificationNode = OverlayManager::get();
    });
}
