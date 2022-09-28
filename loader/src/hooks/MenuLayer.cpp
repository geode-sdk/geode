#include <Geode/Geode.hpp>
#include <Geode/utils/WackyGeodeMacros.hpp>
#include <Geode/ui/BasedButtonSprite.hpp>
#include <Geode/ui/Notification.hpp>
#include <Index.hpp>
#include "../ui/internal/list/ModListLayer.hpp"
#include <Geode/ui/MDPopup.hpp>

USE_GEODE_NAMESPACE();

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

static void updateModsProgress(
	UpdateStatus status,
	std::string const& info,
	uint8_t progress
) {
	if (status == UpdateStatus::Failed) {
		g_indexUpdateNotif->hide();
		g_indexUpdateNotif = nullptr;
		NotificationBuilder()
			.title("Some Updates Failed")
			.text("Some mods failed to update, click for details")
			.icon("info-alert.png"_spr)
			.clicked([info](auto) -> void {
				FLAlertLayer::create("Info", info, "OK")->show();
			})
			.show();
		addUpdateIcon("updates-failed.png"_spr);
	}

	if (status == UpdateStatus::Finished) {
		g_indexUpdateNotif->hide();
		g_indexUpdateNotif = nullptr;
		NotificationBuilder()
			.title("Updates Installed")
			.text(
				"Mods have been updated, please "
				"restart to apply changes"
			)
			.icon("updates-available.png"_spr)
			.clicked([info](auto) -> void {
				FLAlertLayer::create("Info", info, "OK")->show();
			})
			.show();
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
			// todo: uncomment and fix crash
			// if (Mod::get()->getDataStore()["enable-auto-updates"]) {
			// 	auto ticket = Index::get()->installUpdates(updateModsProgress);
			// 	if (!ticket) {
			// 		NotificationBuilder()
			// 			.title("Unable to auto-update")
			// 			.text("Unable to update mods :(")
			// 			.icon("updates-failed.png"_spr)
			// 			.show();
			// 	} else {
			// 		g_indexUpdateNotif = NotificationBuilder()
			// 			.title("Installing updates")
			// 			.text("Installing updates...")
			// 			.clicked([ticket](auto) -> void {
			// 				createQuickPopup(
			// 					"Cancel Updates",
			// 					"Do you want to <cr>cancel</c> updates?",
			// 					"Don't Cancel", "Cancel Updates",
			// 					[ticket](auto, bool btn2) -> void {
			// 						if (g_indexUpdateNotif && btn2) {
			// 							ticket.value()->cancel();
			// 						}
			// 					}
			// 				);
			// 			}, false)
			// 			.loading()
			// 			.stay()
			// 			.show();
			// 	}
			// } else {
			// 	NotificationBuilder()
			// 		.title("Updates available")
			// 		.text("Some mods have updates available!")
			// 		.icon("updates-available.png"_spr)
			// 		.clicked([](auto) -> void {
			// 			ModListLayer::scene();
			// 		})
			// 		.show();
			// }
			// addUpdateIcon();
		}
	}
}

class $modify(CustomMenuLayer, MenuLayer) {
	void destructor() {
		g_geodeButton = nullptr;
		MenuLayer::~MenuLayer();
	}

	bool init() {
		if (!MenuLayer::init())
			return false;

		auto bottomMenu = nodeOrDefault(getChildOfType<CCMenu>(this, 1));

		auto chest = getChild(bottomMenu, -1);
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
		bottomMenu->addChild(btn);

		bottomMenu->alignItemsHorizontallyWithPadding(3.f);

		CCARRAY_FOREACH_B_TYPE(bottomMenu->getChildren(), node, CCNode) {
			node->setPositionY(y);
		}
		if (chest) {
			bottomMenu->addChild(chest);
			chest->release();
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
		if (Loader::get()->didLastLaunchCrash()) {
			auto popup = createQuickPopup(
				"Crashed",
				"It appears that the last session crashed. Would you like to "
				"send a <cy>crash report</c>?",
				"No", "Send",
				[](auto, bool btn2) {
					if (btn2) {
						MDPopup::create(
							"Crash Report",
							"Please send the latest crash report file from `" + 
							Loader::get()->getCrashLogDirectory().string() + "` to the "
							"[#support](https://discord.com/channels/911701438269386882/979352389985390603) "
							"channnel in the [Geode Discord Server](https://discord.gg/9e43WMKzhp)\n\n",
							"OK"
						)->show();
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
		*(int*)(0) = 5;
		ModListLayer::scene();
	}
};

