#include <Geode/modify/IDManager.hpp>
#include <Geode/modify/MenuLayer.hpp>
#include <Geode/utils/cocos.hpp>
#include <Geode/utils/NodeIDs.hpp>
#include <Geode/ui/BasedButtonSprite.hpp>
#include <Geode/binding/GameManager.hpp>
#include <Geode/binding/PlatformToolbox.hpp>

using namespace geode::prelude;
using namespace geode::node_ids;

$register_ids(MenuLayer) {
    // set IDs to everything
    size_t spriteOffset = 0;
    size_t labelOffset = 0;

    setIDSafe(this, 0, "main-menu-bg");
    setIDSafe<CCSprite>(this, spriteOffset++, "main-title");

    auto winSize = CCDirector::get()->getWinSize();
    auto GM = GameManager::sharedState();

    if(!GM->m_clickedGarage) {
        setIDSafe<CCSprite>(this, spriteOffset++, "character-select-hint");
    }

    if(!GM->m_clickedEditor) {
        setIDSafe<CCSprite>(this, spriteOffset++, "level-editor-hint");
    }

    // controller
    if (PlatformToolbox::isControllerConnected()) {
        setIDSafe<CCSprite>(this, spriteOffset++, "play-gamepad-icon");
        setIDSafe<CCSprite>(this, spriteOffset++, "editor-gamepad-icon");
        setIDSafe<CCSprite>(this, spriteOffset++, "icon-kit-gamepad-icon");

        setIDSafe<CCSprite>(this, spriteOffset++, "settings-gamepad-icon");

        if(!GM->getGameVariable("0028")) {
            setIDSafe<CCSprite>(this, spriteOffset++, "mouse-gamepad-icon");
            setIDSafe<CCSprite>(this, spriteOffset++, "click-gamepad-icon");

            setIDSafe<CCLabelBMFont>(this, labelOffset++, "mouse-gamepad-label");
            setIDSafe<CCLabelBMFont>(this, labelOffset++, "click-gamepad-label");
        }
    }
    
    setIDSafe<CCLabelBMFont>(this, labelOffset++, "player-username");
    
    // main menu
    if (auto menu = getChildOfType<CCMenu>(this, 0)) {
        menu->setID("main-menu");
        auto playBtn = setIDSafe(menu, 0, "play-button");
        auto iconBtn = setIDSafe(menu, 1, "icon-kit-button");

        setIDSafe(menu, 2, "editor-button");

        if (auto pfp = setIDSafe(menu, 3, "profile-button")) {
            auto profileMenu = detachAndCreateMenu(
                this, "profile-menu",
                RowLayout::create()
                    ->setAxisAlignment(AxisAlignment::Start),
                pfp
            );
            profileMenu->setContentSize({ 150.f, 50.f });
            profileMenu->setPositionX(
                profileMenu->getPositionX() + 150.f / 2 - 
                    pfp->getScaledContentSize().height / 2
            );
            profileMenu->updateLayout();
        }

        // the buttons are added in order play, icon, editor which doesn't work
        // well with setLayout that deals with children in order
        menu->swapChildIndices(playBtn, iconBtn);

        menu->setContentSize({ winSize.width - 140.f, 65.f });
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

        menu->setContentSize({ winSize.width - 220.f, 65.f });
        menu->setLayout(
            RowLayout::create()
        );
    }
    
    // social media menu
    if (auto menu = getChildOfType<CCMenu>(this, 2)) {
        menu->setID("social-media-menu");
        setIDSafe(menu, 0, "robtop-logo-button");
        setIDSafe(menu, 1, "facebook-button");
        setIDSafe(menu, 2, "twitter-button");
        setIDSafe(menu, 3, "youtube-button");
        setIDSafe(menu, 4, "twitch-button");
        setIDSafe(menu, 5, "discord-button");
    }
    
    // more games menu
    if (auto menu = getChildOfType<CCMenu>(this, 3)) {
        menu->setID("more-games-menu");
        auto moreGamesBtn = setIDSafe(menu, 0, "more-games-button");

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
    
        menu->setContentSize({ 100.f, 50.f });
        menu->setPositionX(
            menu->getPositionX() - 100.f / 2 + 
                getSizeSafe(moreGamesBtn).width / 2
        );
        menu->setLayout(
            RowLayout::create()
                ->setAxisAlignment(AxisAlignment::End)
                ->setAxisReverse(true)
        );
    }

    // add a menu to the top right corner and middle left that are empty 
    // but prolly a place mods want to add stuff

    auto topRightMenu = CCMenu::create();
    topRightMenu->setPosition(winSize.width - 210.f / 2, winSize.height - 50.f / 2);
    topRightMenu->setID("top-right-menu");
    topRightMenu->setContentSize({ 200.f, 50.f });
    topRightMenu->setLayout(
        RowLayout::create()
            ->setAxisReverse(true)
            ->setAxisAlignment(AxisAlignment::End)
    );
    this->addChild(topRightMenu);

    auto middleLeftMenu = CCMenu::create();
    middleLeftMenu->setPosition(25.f, 215.f);
    middleLeftMenu->setID("side-menu");
    middleLeftMenu->setContentSize({ 50.f, 120.f });
    middleLeftMenu->setLayout(ColumnLayout::create());
    this->addChild(middleLeftMenu);
}

// MenuLayer::init is hooked in ../hooks/MenuLayer.cpp
