#include <Geode/utils/cocos.hpp>
#include <Geode/ui/BasedButtonSprite.hpp>
#include <Geode/ui/Notification.hpp>
#include <Index.hpp>
#include "../ui/internal/list/ModListLayer.hpp"
#include <Geode/ui/MDPopup.hpp>
#include <InternalMod.hpp>
#include "../ui/internal/info/ModInfoLayer.hpp"
#include <InternalLoader.hpp>

USE_GEODE_NAMESPACE();

#pragma warning(disable: 4217)

class CustomMenuLayer;

static Ref<Notification> g_indexUpdateNotif = nullptr;
static Ref<CCSprite> g_geodeButton = nullptr;

static void addUpdateIcon(const char* icon = "updates-available.png"_spr) {
	if (g_geodeButton && Index::get()->areUpdatesAvailable()) {
		auto updateIcon = CCSprite::createWithSpriteFrameName(icon);
		updateIcon->setPosition(
			g_geodeButton->getContentSize() - CCSize { 10.f, 10.f }
		);
		updateIcon->setZOrder(99);
		updateIcon->setScale(.5f);
		g_geodeButton->addChild(updateIcon);
	}
}

static void updateIndexProgress(
	UpdateStatus status,
	std::string const& info,
	uint8_t progress
) {
	if (status == UpdateStatus::Failed) {
		g_indexUpdateNotif->hide();
		g_indexUpdateNotif = nullptr;
		NotificationBuilder()
			.title("Index Update")
			.text("Index update failed :(")
			.icon("info-alert.png"_spr)
			.show();
		addUpdateIcon("updates-failed.png"_spr);
	}

	if (status == UpdateStatus::Finished) {
		g_indexUpdateNotif->hide();
		g_indexUpdateNotif = nullptr;
		if (Index::get()->areUpdatesAvailable()) {
			NotificationBuilder()
				.title("Updates available")
				.text("Some mods have updates available!")
				.icon("updates-available.png"_spr)
				.clicked([](auto) -> void {
					ModListLayer::scene();
				})
				.show();
			addUpdateIcon();
		}
	}
}

#include <Geode/modify/MenuLayer.hpp>
class $modify(CustomMenuLayer, MenuLayer) {
	void destructor() {
		g_geodeButton = nullptr;
		MenuLayer::~MenuLayer();
	}

	bool init() {
		if (!MenuLayer::init())
			return false;
		
		Loader::get()->updateResourcePaths();

		auto setIDSafe = +[](CCNode* node, int index, const char* id) {
			if (auto child = getChild(node, index)) {
				child->setID(id);
			}
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
			setIDSafe(menu, 1, "icon-kit-button");
			setIDSafe(menu, 2, "editor-button");
			setIDSafe(menu, 3, "profile-button");
		}
		if (auto menu = getChildOfType<CCMenu>(this, 1)) {
			menu->setID("bottom-menu");
			setIDSafe(menu, 0, "achievements-button");
			setIDSafe(menu, 1, "settings-button");
			setIDSafe(menu, 2, "stats-button");
			setIDSafe(menu, 3, "newgrounds-button");
			setIDSafe(menu, -1,"daily-chest-button");
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

		auto winSize = CCDirector::sharedDirector()->getWinSize();

		// add geode button
		auto bottomMenu = static_cast<CCMenu*>(this->getChildByID("bottom-menu"));

		// keep chest in the same position
		auto chest = bottomMenu->getChildByID("daily-chest-button");
		if (chest) {
			chest->retain();
			chest->removeFromParent();
		}
		
		auto y = getChild(bottomMenu, 0)->getPositionY();
		
		g_geodeButton = SafeCreate<CCSprite>()
			.with(CircleButtonSprite::createWithSpriteFrameName(
				"geode-logo-outline-gold.png"_spr,
				1.0f,
				CircleBaseColor::Green,
				CircleBaseSize::Medium2
			))
			.orMake<ButtonSprite>("!!");

		addUpdateIcon();
		auto btn = CCMenuItemSpriteExtra::create(
			g_geodeButton.data(), this, menu_selector(CustomMenuLayer::onGeode)
		);
		btn->setID("geode-button");
		bottomMenu->addChild(btn);

		bottomMenu->alignItemsHorizontallyWithPadding(3.f);

		for (auto node : CCArrayExt<CCNode>(bottomMenu->getChildren())) {
			node->setPositionY(y);
		}
		if (chest) {
			bottomMenu->addChild(chest);
			chest->release();
		}

		if (auto node = this->getChildByID("settings-gamepad-icon")) {
			node->setPositionX(bottomMenu->getChildByID(
				"settings-button"
			)->getPositionX() + winSize.width / 2);
		}

		// show if some mods failed to load
		auto failed = Loader::get()->getFailedMods();
		if (failed.size()) {
			NotificationBuilder()
				.title("Failed to load")
				.text("Some mods failed to load")
				.show();
        }

		// show crash info
		static bool shownLastCrash = false;
		if (Loader::get()->didLastLaunchCrash() && !shownLastCrash) {
			shownLastCrash = true;
			auto popup = createQuickPopup(
				"Crashed",
				"It appears that the last session crashed. Would you like to "
				"send a <cy>crash report</c>?",
				"No", "Send",
				[](auto, bool btn2) {
					if (btn2) {
						ModInfoLayer::showIssueReportPopup(
							InternalMod::get()->getModInfo()
						);
					}
				},
				false
			);
			popup->m_scene = this;
			popup->m_noElasticity = true;
			popup->show();
		}

		// update mods index
		if (!g_indexUpdateNotif && !Index::get()->isIndexUpdated()) {
			g_indexUpdateNotif = NotificationBuilder()
				.title("Index Update")
				.text("Updating index...")
				.loading()
				.stay()
				.show();

			Index::get()->updateIndex(updateIndexProgress);
		}

		return true;
	}

	void onGeode(CCObject*) {
		ModListLayer::scene();
	}
};

