#include <Geode/modify/GJGarageLayer.hpp>
#include <Geode/Bindings.hpp>
#include <Geode/utils/cocos.hpp>
#include "AddIDs.hpp"

USE_GEODE_NAMESPACE();

$register_ids(GJGarageLayer) {
	setIDSafe(this, 2, "username-label");
    setIDSafe(this, 6, "player-icon");

    if (auto menu = getChildOfType<CCMenu>(this, 0)) {
        menu->setID("icon-select-menu");

        setIDs(menu, 0,
            "cube-button",
            "ship-button",
            "ball-button",
            "ufo-button",
            "wave-button",
            "robot-button",
            "spider-button",
            "trail-button",
            "death-effect-button"
        );
    }

    setIDs(this, 10,
        "cube-selection-menu",
        "ship-selection-menu",
        "ball-selection-menu",
        "ufo-selection-menu",
        "wave-selection-menu",
        "robot-selection-menu",
        "spider-selection-menu",
        "trail-selection-menu",
        "death-effect-selection-menu",

        "color-1-cursor",
        "color-2-cursor",

        "color-selection-menu"
    );

    if (auto menu = getChildOfType<CCMenu>(this, 11)) {
        menu->setID("top-left-menu");

        setIDs(menu, 0,
            "back-button",
            "shop-button",
            "shards-button"
        );

        detachAndCreateMenu(
            menu,
            "shards-button-menu",
            ColumnLayout::create(),
            menu->getChildByID("shards-button")
        );
    }
}

class $modify(GJGarageLayer) {
    bool init() {
        if (!GJGarageLayer::init())
            return false;
        
        NodeIDs::get()->provide(this);

        return true;
    }
};
