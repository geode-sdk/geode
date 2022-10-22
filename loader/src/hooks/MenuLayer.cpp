
#include <Geode/utils/WackyGeodeMacros.hpp>
#include <Geode/utils/cocos.hpp>
#include <Geode/utils/Ref.hpp>
#include <Geode/ui/BasedButtonSprite.hpp>
#include <Geode/ui/Notification.hpp>
#include <Index.hpp>
#include "../ui/internal/list/ModListLayer.hpp"
#include <Geode/ui/MDPopup.hpp>
#include <InternalMod.hpp>
#include "../ui/internal/info/ModInfoLayer.hpp"
#include <InternalLoader.hpp>
#include <Geode/Modify.hpp>
#include "../ids/AddIDs.hpp"

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
		
		// make sure to add the string IDs for nodes (Geode has no manual 
		// hook order support yet so gotta do this to ensure)
		NodeStringIDManager::get()->provide(this);

		auto winSize = CCDirector::sharedDirector()->getWinSize();

		// add geode button
		
		g_geodeButton = SafeCreate<CCSprite>()
			.with(CircleButtonSprite::createWithSpriteFrameName(
				"geode-logo-outline-gold.png"_spr,
				1.0f,
				CircleBaseColor::Green,
				CircleBaseSize::Medium2
			))
			.orMake<ButtonSprite>("!!");

		addUpdateIcon();

		auto bottomMenu = static_cast<CCMenu*>(this->getChildByID("bottom-menu"));

		auto btn = CCMenuItemSpriteExtra::create(
			g_geodeButton.data(), this, menu_selector(CustomMenuLayer::onGeode)
		);
		btn->setID("geode-button");
		bottomMenu->addChild(btn);

		bottomMenu->updateLayout();

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

