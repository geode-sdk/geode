#include "AddIDs.hpp"

#include <Geode/binding/LevelInfoLayer.hpp>
#include <Geode/modify/LevelInfoLayer.hpp>

$register_ids(LevelInfoLayer) {
    auto winSize = CCDirector::get()->getWinSize();

    setIDSafe<CCSprite>(this, 0, "background");

    size_t iconOffset = 0;

    setIDSafe<CCSprite>(this, 1, "bottom-left-art");
    setIDSafe<CCSprite>(this, 2, "bottom-right-art");

    if (m_level->m_highObjectsEnabled) {
        setIDSafe<CCSprite>(this, 4, "high-object-indicator");
        iconOffset++;
    }

    setIDSafe<CCSprite>(this, 4 + iconOffset, "length-icon");
    setIDSafe<CCSprite>(this, 5 + iconOffset, "downloads-icon");
    setIDSafe<CCSprite>(this, 6 + iconOffset, "orbs-icon");
    setIDSafe<CCSprite>(this, 7 + iconOffset, "likes-icon");
    setIDSafe<CCLabelBMFont>(this, 1, "downloads-label");
    setIDSafe<CCLabelBMFont>(this, 2, "length-label");
    setIDSafe<CCLabelBMFont>(this, 3, "likes-label");
    setIDSafe<CCLabelBMFont>(this, 4, "orbs-label");

    if (m_level->m_stars) {
        setIDSafe<CCSprite>(this, 8 + iconOffset, "stars-icon");
        setIDSafe<CCLabelBMFont>(this, 5, "stars-label");
    }

    setIDSafe<CustomSongWidget>(this, 0, "custom-songs-widget");

    if (auto menu = getChildOfType<CCMenu>(this, 0)) {
        menu->setID("play-menu");
        setIDSafe(menu, 0, "play-button");
    }

    if (auto menu = getChildOfType<CCMenu>(this, 2)) {
        menu->setID("back-menu");
        auto backBtn = setIDSafe(menu, 0, "back-button");
        menu->setPositionX(
            menu->getPositionX() + 100.f / 2 - 
                getSizeSafe(backBtn).width / 2
        );
        menu->setContentSize({ 100.f, 50.f });
        menu->setLayout(
            RowLayout::create()
                ->setAxisAlignment(AxisAlignment::Start)
        );
    }

    if (auto menu = getChildOfType<CCMenu>(this, 1)) {
        menu->setID("right-side-menu");

        if (auto name = setIDSafe(menu, 0, "creator-name")) {
            auto menu = detachAndCreateMenu(
                this,
                "creator-info-menu",
                ColumnLayout::create()
                    ->setAxisReverse(true)
                    ->setAxisAlignment(AxisAlignment::End),
                name
            );
            menu->setPositionY(
                menu->getPositionY() - 40.f / 2 + 
                    name->getScaledContentSize().height / 2
            );
            menu->setContentSize({ 60.f, 40.f });
            menu->updateLayout();
        }

        auto leftSideMenu = CCMenu::create();
        leftSideMenu->setPosition(30.f, winSize.height / 2);
        leftSideMenu->setLayout(ColumnLayout::create());
        leftSideMenu->setID("left-side-menu");
        leftSideMenu->setContentSize({ 50.f, 225.f });
        this->addChild(leftSideMenu);

        menu->setPosition(winSize.width - 30.f, winSize.height / 2);

        for (auto child : CCArrayExt<CCNode>(menu->getChildren())) {
            if (child->getPositionX() < 0.f) {
                child->retain();
                child->removeFromParent();
                leftSideMenu->addChild(child);
                child->release();
            }
            child->setPositionX(0.f);
        }

        setIDSafe(menu, 0, "delete-button");
        setIDSafe(menu, 1, "refresh-button");
        setIDSafe(menu, 2, "info-button");
        setIDSafe(menu, 3, "leaderboards-button");
        setIDSafe(menu, 4, "like-button");
        setIDSafe(menu, 5, "rate-button");

        menu->setPosition(
            menu->getPositionX() + static_cast<CCNode*>(
                menu->getChildren()->firstObject()
            )->getPositionX(),
            winSize.height / 2
        );
        menu->setContentSize({ 60.f, winSize.height - 15.f });
        menu->setLayout(
            ColumnLayout::create()
                ->setGap(3.f)
                ->setAxisAlignment(AxisAlignment::End)
                ->setAxisReverse(true)
        );

        setIDSafe(leftSideMenu, 0, "copy-button");

        menu->updateLayout();
        leftSideMenu->updateLayout();
    }
}

struct LevelInfoLayerIDs : Modify<LevelInfoLayerIDs, LevelInfoLayer> {
    static void onModify(auto& self) {
        if (!self.setHookPriority("LevelInfoLayer::init", GEODE_ID_PRIORITY)) {
            log::warn("Failed to set LevelInfoLayer::init hook priority, node IDs may not work properly");
        }
    }

    bool init(GJGameLevel* level) {
        if (!LevelInfoLayer::init(level)) return false;

        NodeIDs::get()->provide(this);

        return true;
    }
};