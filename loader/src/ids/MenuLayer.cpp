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
	getChildOfType<CCSprite>(layer, 0)->setID("main-title");

	if (PlatformToolbox::isControllerConnected()) {
		getChildOfType<CCSprite>(layer, 1)->setID("play-gamepad-icon");
		getChildOfType<CCSprite>(layer, 2)->setID("editor-gamepad-icon");
		getChildOfType<CCSprite>(layer, 3)->setID("icon-kit-gamepad-icon");

		getChildOfType<CCSprite>(layer, 4)->setID("settings-gamepad-icon");
		getChildOfType<CCSprite>(layer, 5)->setID("mouse-gamepad-icon");
		getChildOfType<CCSprite>(layer, 6)->setID("click-gamepad-icon");

		getChildOfType<CCLabelBMFont>(layer, 0)->setID("mouse-gamepad-label");
		getChildOfType<CCLabelBMFont>(layer, 1)->setID("click-gamepad-label");

		getChildOfType<CCLabelBMFont>(layer, 2)->setID("player-username");
	} else {
		getChildOfType<CCLabelBMFont>(layer, 0)->setID("player-username");
	}
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
	if (auto menu = getChildOfType<CCMenu>(layer, 1)) {
		menu->setID("bottom-menu");
		auto ach = setIDSafe(menu, 0, "achievements-button");
		setIDSafe(menu, 1, "settings-button");
		setIDSafe(menu, 2, "stats-button");
		setIDSafe(menu, 3, "newgrounds-button");

		// move daily chest to its own menu

		if (auto dailyChest = setIDSafe(menu, -1, "daily-chest-button")) {
			dailyChest->retain();
			dailyChest->removeFromParent();

			auto rightSideMenu = CCMenu::create();
			rightSideMenu->setID("right-side-menu");
			rightSideMenu->setPosition(menu->convertToWorldSpace(dailyChest->getPosition()));
			dailyChest->setPosition(0, 0);
			rightSideMenu->addChild(dailyChest);
			rightSideMenu->setLayout(ColumnLayout::create(5.f, 0.f));
			layer->addChild(rightSideMenu);

			dailyChest->release();
		}

		menu->setLayout(RowLayout::create(5.f, ach->getPositionY()));
	}
	if (auto menu = getChildOfType<CCMenu>(layer, 2)) {
		menu->setID("social-media-menu");
		setIDSafe(menu, 0, "robtop-logo-button");
		setIDSafe(menu, 1, "facebook-button");
		setIDSafe(menu, 2, "twitter-button");
		setIDSafe(menu, 3, "youtube-button");
	}
	if (auto menu = getChildOfType<CCMenu>(layer, 3)) {
		menu->setID("more-games-menu");
		setIDSafe(menu, 0, "more-games-button");

		// move close button to its own menu

		if (auto closeBtn = setIDSafe(menu, 1, "close-button")) {
			closeBtn->retain();
			closeBtn->removeFromParent();

			auto closeMenu = CCMenu::create();
			closeMenu->setPosition(menu->convertToWorldSpace(closeBtn->getPosition()));
			closeMenu->setID("top-left-menu");
			closeBtn->setPosition(0, 0);
			closeMenu->addChild(closeBtn);
			closeMenu->setLayout(RowLayout::create(5.f, 0.f)->setAlignment(Alignment::Begin));
			layer->addChild(closeMenu);

			closeBtn->release();
		}
	}

	EnterLayerEvent("MenuLayer", layer).post();
}
