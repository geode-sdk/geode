#include "AddIDs.hpp"

#include <Geode/Bindings.hpp>
#include <Geode/modify/GJGarageLayer.hpp>
#include <Geode/utils/cocos.hpp>

using namespace geode::prelude;

$register_ids(GJGarageLayer) {
    // the lock does not exist for not logged in users
    auto loggedInOffset = GJAccountManager::get()->m_accountID == GJAccountManager::get()->m_playerID ? -1 : 0;

    setIDSafe(this, 2, "username-label");
    setIDSafe(this, 6 + loggedInOffset, "player-icon");

    auto winSize = CCDirector::get()->getWinSize();

    if (auto menu = getChildOfType<CCMenu>(this, 0)) {
        menu->setID("category-menu");

        setIDs(
            menu,
            0,
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

        menu->setContentSize({ 320.f, 50.f });
        menu->setLayout(
            RowLayout::create()
                ->setAxisAlignment(AxisAlignment::Start)
                ->setGap(-4.f)
        );
    }

    setIDs(
        this,
        10 + loggedInOffset,
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

    if (auto menu = getChildOfType<CCMenu>(this, 1)) {
        menu->setID("top-left-menu");

        setIDs(menu, 0, "back-button", "shop-button", "shards-button");

        auto backBtn = menu->getChildByID("back-button");
        auto backMenu = detachAndCreateMenu(
            this,
            "back-menu",
            RowLayout::create()
                ->setAxisAlignment(AxisAlignment::Start),
            backBtn
        );
        backMenu->setContentSize({ 100.f, 50.f });
        backMenu->setPositionX(
            backMenu->getPositionX() + 100.f / 2 - 
                getSizeSafe(backBtn).width / 2
        );
        backMenu->updateLayout();

        auto shardsBtn = menu->getChildByID("shards-button");
        auto shardsMenu = detachAndCreateMenu(
            this,
            "shards-menu",
            ColumnLayout::create()
                ->setAxisReverse(true)
                ->setAxisAlignment(AxisAlignment::End),
            shardsBtn
        );
        shardsMenu->setContentSize({ 50.f, 100.f });
        shardsMenu->setPositionY(
            shardsMenu->getPositionY() - 100.f / 2 + 
                getSizeSafe(shardsBtn).height / 2
        );
        shardsMenu->updateLayout();
    }

    auto bottomLeftMenu = CCMenu::create();
    bottomLeftMenu->setID("bottom-left-menu");
    bottomLeftMenu->setContentSize({ 50.f, 70.f });
    bottomLeftMenu->setPosition(30.f, 115.f);
    bottomLeftMenu->setLayout(
        ColumnLayout::create()
            ->setAxisAlignment(AxisAlignment::Start)
    );
    this->addChild(bottomLeftMenu);

    auto bottomRightMenu = CCMenu::create();
    bottomRightMenu->setID("bottom-right-menu");
    bottomRightMenu->setContentSize({ 50.f, 110.f });
    bottomRightMenu->setPosition(winSize.width - 30.f, 135.f);
    bottomRightMenu->setLayout(
        ColumnLayout::create()
            ->setAxisAlignment(AxisAlignment::Start)
    );
    this->addChild(bottomRightMenu);

    // aspect ratio responsiveness
    if (winSize.width / winSize.height <= 5.1f / 3.f) {
        bottomLeftMenu->setPosition(15.f, 115.f);
        bottomRightMenu->setPosition(winSize.width - 15.f, 135.f);

        if (auto shardsMenu = this->getChildByID("shards-menu")) {
            shardsMenu->setContentSize({ 110.f, 50.f });
            shardsMenu->setPosition(
                shardsMenu->getPosition() + ccp(50.f, 30.f)
            );
            shardsMenu->setLayout(
                RowLayout::create()
                    ->setAxisAlignment(AxisAlignment::Start)
            );
        }
    }
    if (winSize.width / winSize.height <= 4.1f / 3.f) {
        bottomLeftMenu->setContentSize({ 90.f, 50.f });
        bottomLeftMenu->setPosition(
            15.f + 110.f / 2,
            85.f
        );
        bottomLeftMenu->setLayout(
            RowLayout::create()
                ->setAxisAlignment(AxisAlignment::Start)
        );

        bottomRightMenu->setContentSize({ 90.f, 50.f });
        bottomRightMenu->setPosition(
            winSize.width - 15.f - 110.f / 2,
            85.f
        );
        bottomRightMenu->setLayout(
            RowLayout::create()
                ->setAxisReverse(true)
                ->setAxisAlignment(AxisAlignment::End)
        );
    }
}

struct GJGarageLayerIDs : Modify<GJGarageLayerIDs, GJGarageLayer> {
    static void onModify(auto& self) {
        if (!self.setHookPriority("GJGarageLayer::init", GEODE_ID_PRIORITY)) {
            log::warn("Failed to set GJGarageLayer::init hook priority, node IDs may not work properly");
        }
    }

    bool init() {
        if (!GJGarageLayer::init()) return false;

        NodeIDs::get()->provide(this);

        return true;
    }
};
