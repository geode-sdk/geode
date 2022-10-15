#include <Geode/Modify.hpp>
#include <Geode/Bindings.hpp>
#include <Geode/utils/cocos.hpp>
#include <Geode/ui/EnterLayerEvent.hpp>
#include "AddIDs.hpp"

USE_GEODE_NAMESPACE();

void addIDsToMenuLayer(MenuLayer* layer) {
	// set IDs to everything
	layer->setID("MenuLayer");
	setIDSafe(layer, 0, "main-menu-bg");
	setIDSafe<CCSprite>(layer, 0, "main-title");

	// controller
	if (PlatformToolbox::isControllerConnected()) {
		setIDSafe<CCSprite>(layer, 1, "play-gamepad-icon");
		setIDSafe<CCSprite>(layer, 2, "editor-gamepad-icon");
		setIDSafe<CCSprite>(layer, 3, "icon-kit-gamepad-icon");

		setIDSafe<CCSprite>(layer, 4, "settings-gamepad-icon");
		setIDSafe<CCSprite>(layer, 5, "mouse-gamepad-icon");
		setIDSafe<CCSprite>(layer, 6, "click-gamepad-icon");

		setIDSafe<CCLabelBMFont>(layer, 0, "mouse-gamepad-label");
		setIDSafe<CCLabelBMFont>(layer, 1, "click-gamepad-label");

		setIDSafe<CCLabelBMFont>(layer, 2, "player-username");
	} else {
		setIDSafe<CCLabelBMFont>(layer, 0, "player-username");
	}
	// main menu
	if (auto menu = getChildOfType<CCMenu>(layer, 0)) {
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
	if (auto menu = getChildOfType<CCMenu>(layer, 1)) {
		menu->setID("bottom-menu");
		auto ach = setIDSafe(menu, 0, "achievements-button");
		setIDSafe(menu, 1, "settings-button");
		setIDSafe(menu, 2, "stats-button");
		setIDSafe(menu, 3, "newgrounds-button");

		// move daily chest to its own menu

		if (auto dailyChest = setIDSafe(menu, -1, "daily-chest-button")) {
			detachIntoOwnMenu(layer, dailyChest, "right-side-menu", 
				ColumnLayout::create(5.f, 0.f)
			);
		}

		menu->setLayout(RowLayout::create(5.f, ach->getPositionY()));
	}
	// social media menu
	if (auto menu = getChildOfType<CCMenu>(layer, 2)) {
		menu->setID("social-media-menu");
		setIDSafe(menu, 0, "robtop-logo-button");
		setIDSafe(menu, 1, "facebook-button");
		setIDSafe(menu, 2, "twitter-button");
		setIDSafe(menu, 3, "youtube-button");
	}
	// more games menu
	if (auto menu = getChildOfType<CCMenu>(layer, 3)) {
		menu->setID("more-games-menu");
		setIDSafe(menu, 0, "more-games-button");

		// move close button to its own menu

		if (auto closeBtn = setIDSafe(menu, 1, "close-button")) {
			detachIntoOwnMenu(layer, closeBtn, "close-menu", 
				RowLayout::create(5.f, 0.f)->setAlignment(Alignment::Begin)
			);
		}
	}

	EnterLayerEvent("MenuLayer", layer).post();
}
