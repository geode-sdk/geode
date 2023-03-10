#include "AddIDs.hpp"

#include <Geode/Bindings.hpp>
#include <Geode/modify/UILayer.hpp>
#include <Geode/utils/cocos.hpp>

using namespace geode::prelude;

$register_ids(UILayer) {
    if (auto menu = getChildOfType<CCMenu>(this, 0)) {
        menu->setID("pause-button-menu");

        setIDs(menu, 0, "pause-button");
    }

    if (auto menu = getChildOfType<CCMenu>(this, 1)) {
        menu->setID("checkpoint-menu");

        setIDs(menu, 0, "add-checkpoint-button", "remove-checkpoint-button");
    }
}

struct UILayerIDs : Modify<UILayerIDs, UILayer> {
    static void onModify(auto& self) {
        if (!self.setHookPriority("UILayer::init", GEODE_ID_PRIORITY)) {
            log::warn("Failed to set UILayer::init hook priority, node IDs may not work properly");
        }
    }

    bool init() {
        if (!UILayer::init()) return false;

        NodeIDs::get()->provide(this);

        return true;
    }
};
