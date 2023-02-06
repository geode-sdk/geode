#include "AddIDs.hpp"

#include <Geode/Bindings.hpp>
#include <Geode/modify/CreatorLayer.hpp>
#include <Geode/utils/cocos.hpp>

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
        setIDSafe(menu, 10, "gauntlets-button");

        // move vault button to its own menu
        if (auto lockBtn = setIDSafe(menu, -2, "vault-button")) {
            auto menu = detachAndCreateMenu(
                this,
                "top-right-menu",
                ColumnLayout::create()
                    ->setAxisAlignment(AxisAlignment::End),
                lockBtn
            );
            menu->setPositionY(menu->getPositionY() - 125.f / 2);
            menu->setContentSize({ 60.f, 125.f });
            menu->updateLayout();
        }

        // move treasure room button to its own menu
        if (auto roomBtn = setIDSafe(menu, -1, "treasure-room-button")) {
            auto menu = detachAndCreateMenu(
                this,
                "bottom-right-menu",
                ColumnLayout::create()
                    ->setAxisAlignment(AxisAlignment::Start),
                roomBtn
            );
            menu->setPositionY(menu->getPositionY() + 125.f / 2);
            menu->setContentSize({ 60.f, 125.f });
            menu->updateLayout();
        }
    }

    if (auto menu = getChildOfType<CCMenu>(this, 1)) {
        menu->setID("exit-menu");
        setIDSafe(menu, 0, "exit-button");
        menu->setPositionY(menu->getPositionY() - 125.f / 2);
        menu->setContentSize({ 60.f, 125.f });
        menu->setLayout(
            ColumnLayout::create()
                ->setAxisAlignment(AxisAlignment::End)
        );
    }

    // add a menu to the bottom left corner that is empty but prolly a place mods 
    // want to add stuff to
    auto menu = CCMenu::create();
    menu->setPosition(24.f, 24.f + 125.f / 2);
    menu->setID("bottom-left-menu");
    menu->setContentSize({ 60.f, 125.f });
    menu->setLayout(
        ColumnLayout::create()
            ->setAxisAlignment(AxisAlignment::Start)
    );
    this->addChild(menu);
}

struct CreatorLayerIDs : Modify<CreatorLayerIDs, CreatorLayer> {
    static void onModify(auto& self) {
        if (!self.setHookPriority("CreatorLayer::init", GEODE_ID_PRIORITY)) {
            log::warn("Failed to set CreatorLayer::init hook priority, node IDs may not work properly");
        }
    }

    bool init() {
        if (!CreatorLayer::init()) return false;

        NodeIDs::get()->provide(this);

        return true;
    }
};
