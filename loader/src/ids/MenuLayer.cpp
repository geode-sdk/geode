#include "AddIDs.hpp"

#include <Geode/modify/MenuLayer.hpp>
#include <Geode/utils/cocos.hpp>

USE_GEODE_NAMESPACE();

$register_ids(MenuLayer) {
    // set IDs to everything
    setIDSafe(this, 0, "main-menu-bg");
    setIDSafe<CCSprite>(this, 0, "main-title");

    // controller
    if (PlatformToolbox::isControllerConnected()) {
        setIDSafe<CCSprite>(this, 1, "play-gamepad-icon");
        setIDSafe<CCSprite>(this, 2, "editor-gamepad-icon");
        setIDSafe<CCSprite>(this, 3, "icon-kit-gamepad-icon");

        setIDSafe<CCSprite>(this, 4, "settings-gamepad-icon");
        setIDSafe<CCSprite>(this, 5, "mouse-gamepad-icon");
        setIDSafe<CCSprite>(this, 6, "click-gamepad-icon");

        setIDSafe<CCLabelBMFont>(this, 0, "mouse-gamepad-label");
        setIDSafe<CCLabelBMFont>(this, 1, "click-gamepad-label");

        setIDSafe<CCLabelBMFont>(this, 2, "player-username");
    }
    else {
        setIDSafe<CCLabelBMFont>(this, 0, "player-username");
    }
    // main menu
    if (auto menu = getChildOfType<CCMenu>(this, 0)) {
        menu->setID("main-menu");
        auto playBtn = setIDSafe(menu, 0, "play-button");
        auto iconBtn = setIDSafe(menu, 1, "icon-kit-button");

        // the buttons are added in order play, icon, editor which doesn't work
        // well with setLayout that deals with children in order
        menu->swapChildIndices(playBtn, iconBtn);

        setIDSafe(menu, 2, "editor-button");

        if (auto pfp = setIDSafe(menu, 3, "profile-button")) {
            auto profileMenu = detachAndCreateMenu(
                this, "profile-menu",
                ColumnLayout::create()
                    ->setAxisAlignment(AxisAlignment::Start)
                    ->setAxisReverse(true),
                pfp
            );
            profileMenu->setContentSize({ 50.f, 200.f });
            profileMenu->setPositionY(
                profileMenu->getPositionY() + 200.f / 2 - 
                    pfp->getScaledContentSize().height / 2
            );
            profileMenu->updateLayout();
        }

        menu->setContentSize({ 400.f, 65.f });
        menu->setLayout(
            RowLayout::create()
                ->setGap(18.f)
                ->setCrossAxisOverflow(true)
        );
    }
    // bottom menu
    if (auto menu = getChildOfType<CCMenu>(this, 1)) {
        menu->setID("bottom-menu");
        auto ach = setIDSafe(menu, 0, "achievements-button");
        setIDSafe(menu, 1, "settings-button");
        setIDSafe(menu, 2, "stats-button");
        setIDSafe(menu, 3, "newgrounds-button");

        // move daily chest to its own menu

        if (auto dailyChest = setIDSafe(menu, -1, "daily-chest-button")) {
            auto menu = detachAndCreateMenu(
                this,
                "right-side-menu",
                ColumnLayout::create(),
                dailyChest
            );
            menu->setContentSize({ 65.f, 180.f });
            menu->updateLayout();
        }

        menu->setContentSize({ 360.f, 65.f });
        menu->setLayout(RowLayout::create());
    }
    // social media menu
    if (auto menu = getChildOfType<CCMenu>(this, 2)) {
        menu->setID("social-media-menu");
        setIDSafe(menu, 0, "robtop-logo-button");
        setIDSafe(menu, 1, "facebook-button");
        setIDSafe(menu, 2, "twitter-button");
        setIDSafe(menu, 3, "youtube-button");
    }
    // more games menu
    if (auto menu = getChildOfType<CCMenu>(this, 3)) {
        menu->setID("more-games-menu");
        setIDSafe(menu, 0, "more-games-button");

        // move close button to its own menu

        if (auto closeBtn = setIDSafe(menu, 1, "close-button")) {
            auto closeMenu = detachAndCreateMenu(
                this,
                "close-menu",
                RowLayout::create()
                    ->setAxisAlignment(AxisAlignment::Start),
                closeBtn
            );
            closeMenu->setContentSize({ 200.f, 50.f });
            closeMenu->setPositionX(
                closeMenu->getPositionX() + 200.f / 2 - 
                    closeBtn->getScaledContentSize().width / 2
            );
            closeMenu->updateLayout();
        }
    }
}

// MenuLayer::init is hooked in ../hooks/MenuLayer.cpp
