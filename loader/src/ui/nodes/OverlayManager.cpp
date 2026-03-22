#include <Geode/loader/ModEvent.hpp>
#include <Geode/Loader.hpp>
#include <Geode/ui/OverlayManager.hpp>

using namespace geode::prelude;

OverlayManager* OverlayManager::get() {
    static OverlayManager* inst = nullptr;
    if (!inst) {
        inst = new OverlayManager();
        inst->onEnter();
    }
    return inst;
}

$on_mod(Loaded) {
    queueInMainThread([] {
        CCDirector::get()->m_pNotificationNode = OverlayManager::get();
    });
}
