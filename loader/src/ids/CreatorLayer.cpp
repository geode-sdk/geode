#include <Geode/Modify.hpp>
#include <Geode/Bindings.hpp>
#include <Geode/utils/cocos.hpp>
#include "AddIDs.hpp"

USE_GEODE_NAMESPACE();

$register_ids(CreatorLayer) {
    setIDSafe<CCSprite>(this, 0, "background");

    if (auto menu = getChildOfType<CCMenu>(this, 0)) {
        menu->setID("creator-buttons-menu");

        setIDSafe(menu, 0, "create-button");
        setIDSafe(menu, 1, "saved-button");
        setIDSafe(menu, 2, "scores-button");
        setIDSafe(menu, 3, "quests-button");
        setIDSafe(menu, 4, "daily-button");
        setIDSafe(menu, 5, "weekly-button");
        setIDSafe(menu, 6, "featured-button");
        setIDSafe(menu, 7, "hall-of-fame-button");
        setIDSafe(menu, 8, "map-packs-button");
        setIDSafe(menu, 9, "search-button");
        setIDSafe(menu, 10,"gauntlets-button");

        // move vault button to its own menu
        if (auto lockBtn = setIDSafe(menu, -2, "vault-button")) {
            detachAndCreateMenu(this, "top-right-menu", 
                ColumnLayout::create(5.f, 0.f)->setAlignment(Alignment::Begin),
                lockBtn
            );
        }

        // move treasure room button to its own menu
        if (auto roomBtn = setIDSafe(menu, -1, "treasure-room-button")) {
            detachAndCreateMenu(this, "bottom-right-menu", 
                ColumnLayout::create(5.f, 0.f)->setAlignment(Alignment::End),
                roomBtn
            );
        }
    }

    if (auto menu = getChildOfType<CCMenu>(this, 1)) {
        menu->setID("exit-menu");
        setIDSafe(menu, 0, "exit-button");
    }
}

class $modify(CreatorLayer) {
    bool init() {
        if (!CreatorLayer::init())
            return false;
        
        NodeIDs::get()->provide(this);

        return true;
    }
};
