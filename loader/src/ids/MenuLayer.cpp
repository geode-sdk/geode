#include <Geode/Modify.hpp>
#include <Geode/Bindings.hpp>
#include <Geode/utils/cocos.hpp>
#include <Geode/ui/EnterLayerEvent.hpp>

USE_GEODE_NAMESPACE();

class $modify(MenuLayer) {
    bool init() {
		auto setIDSafe = +[](CCNode* node, int index, const char* id) -> CCNode* {
			if (auto child = getChild(node, index)) {
				child->setID(id);
				return child;
			}
			return nullptr;
		};

		// set IDs to everything
		this->setID("main-menu-layer");
		setIDSafe(this, 0, "main-menu-bg");
		getChildOfType<CCSprite>(this, 0)->setID("main-title");

		if (PlatformToolbox::isControllerConnected()) {
			getChildOfType<CCSprite>(this, 1)->setID("play-gamepad-icon");
			getChildOfType<CCSprite>(this, 2)->setID("editor-gamepad-icon");
			getChildOfType<CCSprite>(this, 3)->setID("icon-kit-gamepad-icon");

			getChildOfType<CCSprite>(this, 4)->setID("settings-gamepad-icon");
			getChildOfType<CCSprite>(this, 5)->setID("mouse-gamepad-icon");
			getChildOfType<CCSprite>(this, 6)->setID("click-gamepad-icon");

			getChildOfType<CCLabelBMFont>(this, 0)->setID("mouse-gamepad-label");
			getChildOfType<CCLabelBMFont>(this, 1)->setID("click-gamepad-label");

			getChildOfType<CCLabelBMFont>(this, 2)->setID("player-username");
		} else {
			getChildOfType<CCLabelBMFont>(this, 0)->setID("player-username");
		}
		if (auto menu = getChildOfType<CCMenu>(this, 0)) {
			menu->setID("main-menu");
			setIDSafe(menu, 0, "play-button");
			auto iconKit = setIDSafe(menu, 1, "icon-kit-button");
			setIDSafe(menu, 2, "editor-button");
			
			if (auto pfp = setIDSafe(menu, 3, "profile-button")) {
				pfp->setPositionHint(PositionHint::Absolute);
			}

			menu->setLayout(RowLayout::create(5.f, 0.f));
		}
		if (auto menu = getChildOfType<CCMenu>(this, 1)) {
			menu->setID("bottom-menu");
			auto ach = setIDSafe(menu, 0, "achievements-button");
			setIDSafe(menu, 1, "settings-button");
			setIDSafe(menu, 2, "stats-button");
			setIDSafe(menu, 3, "newgrounds-button");

			if (auto dailyChest = setIDSafe(menu, -1, "daily-chest-button")) {
				dailyChest->setPositionHint(PositionHint::Absolute);
			}

			menu->setLayout(RowLayout::create(5.f, ach->getPositionY()));
		}
		if (auto menu = getChildOfType<CCMenu>(this, 2)) {
			menu->setID("social-media-menu");
			setIDSafe(menu, 0, "robtop-logo-button");
			setIDSafe(menu, 1, "facebook-button");
			setIDSafe(menu, 2, "twitter-button");
			setIDSafe(menu, 3, "youtube-button");
		}
		if (auto menu = getChildOfType<CCMenu>(this, 3)) {
			menu->setID("more-games-menu");
			setIDSafe(menu, 0, "more-games-button");
			setIDSafe(menu, 1, "close-button");
		}

        EnterLayerEvent("MenuLayer", this).post();

        return true;
    }
};
