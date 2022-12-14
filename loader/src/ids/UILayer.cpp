#include <Geode/modify/UILayer.hpp>
#include <Geode/Bindings.hpp>
#include <Geode/utils/cocos.hpp>
#include "AddIDs.hpp"

USE_GEODE_NAMESPACE();

$register_ids(UILayer) {
	if (auto menu = getChildOfType<CCMenu>(this, 0)) {
		menu->setID("pause-button-menu");

		setIDs(menu, 0, "pause-button");
	}

	if (auto menu = getChildOfType<CCMenu>(this, 1)) {
		menu->setID("checkpoint-menu");

		setIDs(menu, 0,
			"add-checkpoint-button",
			"remove-checkpoint-button"
		);
	}
}

class $modify(UILayer) {
    bool init() {
        if (!UILayer::init())
            return false;
        
        NodeIDs::get()->provide(this);

        return true;
    }
};
