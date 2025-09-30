#include <Geode/ui/OverlayManager.hpp>
#include <Geode/utils/cocos.hpp>
#include <Geode/utils/ranges.hpp>

using namespace geode::prelude;

OverlayManager* OverlayManager::get() {
    static OverlayManager* inst = nullptr;
    if (!inst) {
        inst = new OverlayManager();
        inst->onEnter();
    }
    return inst;
}