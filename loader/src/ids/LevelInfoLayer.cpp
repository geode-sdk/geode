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
        menu->setID("exit-menu");
		setIDSafe(menu, 0, "exit-button");
    }

    if (auto menu = getChildOfType<CCMenu>(this, 1)) {
        menu->setID("right-side-menu");

        if (auto name = setIDSafe(menu, 0, "creator-name")) {
            detachIntoOwnMenu(
                this, name, "creator-info-menu",
                ColumnLayout::create()->setAlignment(Alignment::Begin)
            );
        }

        auto leftSideMenu = CCMenu::create();
        leftSideMenu->setPosition(winSize / 2 + ccp(-254.f, 30.f));
        leftSideMenu->setLayout(ColumnLayout::create());
        leftSideMenu->setID("left-side-menu");
        this->addChild(leftSideMenu);

        menu->setPosition(winSize / 2 + ccp(254.f, 0.f));
        
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

        setIDSafe(leftSideMenu, 0, "copy-button");

        menu->updateLayout();
        leftSideMenu->updateLayout();
    }
}

class $modify(LevelInfoLayer) {
    bool init(GJGameLevel* level) {
        if (!LevelInfoLayer::init(level))
            return false;
        
        NodeIDs::get()->provide(this);

        return true;
    }
};
