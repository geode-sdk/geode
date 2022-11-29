#include <Geode/modify/MenuLayer.hpp>
#include <Geode/utils/cocos.hpp>
#include "AddIDs.hpp"

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
	} else {
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
			pfp->setPositionHint(PositionHint::Absolute);
		}

		menu->setLayout(RowLayout::create(18.f, 0.f));
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
			detachIntoOwnMenu(this, dailyChest, "right-side-menu", 
				ColumnLayout::create(0.f, 0.f)
			);
		}

		menu->setLayout(RowLayout::create(5.f, ach->getPositionY()));
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
			detachIntoOwnMenu(this, closeBtn, "close-menu", 
				RowLayout::create(5.f, 0.f)->setAlignment(Alignment::Begin)
			);
		}
	}
}

// MenuLayer::init is hooked in ../hooks/MenuLayer.cpp
