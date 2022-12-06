
#include <Geode/utils/cocos.hpp>
#include "../ui/internal/list/ModListLayer.hpp"
#include <Geode/ui/BasedButtonSprite.hpp>
#include <Geode/ui/Notification.hpp>
#include <Geode/ui/GeodeUI.hpp>
#include <Geode/ui/Popup.hpp>
#include <Geode/utils/cocos.hpp>
#include <Geode/loader/Index.hpp>
#include <InternalLoader.hpp>
#include "../ids/AddIDs.hpp"
#include <InternalMod.hpp>
#include <Geode/modify/Modify.hpp>
#include <Geode/modify/MenuLayer.hpp>

USE_GEODE_NAMESPACE();

#pragma warning(disable : 4217)

class CustomMenuLayer;

static Ref<Notification> g_indexUpdateNotif = nullptr;
static Ref<CCSprite> g_geodeButton = nullptr;

struct CustomMenuLayer : Modify<CustomMenuLayer, MenuLayer> {
	void destructor() {
		g_geodeButton = nullptr;
		MenuLayer::~MenuLayer();
	}

	bool init() {
		if (!MenuLayer::init())
			return false;
		
		// make sure to add the string IDs for nodes (Geode has no manual 
		// hook order support yet so gotta do this to ensure)
		NodeIDs::provideFor(this);

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

		auto bottomMenu = static_cast<CCMenu*>(this->getChildByID("bottom-menu"));

		auto btn = CCMenuItemSpriteExtra::create(
			g_geodeButton.data(), this, menu_selector(CustomMenuLayer::onGeode)
		);
		btn->setID("geode-button"_spr);
		bottomMenu->addChild(btn);

		bottomMenu->updateLayout();

		if (auto node = this->getChildByID("settings-gamepad-icon")) {
			node->setPositionX(bottomMenu->getChildByID(
				"settings-button"
			)->getPositionX() + winSize.width / 2);
		}

		// show if some mods failed to load
		static bool shownFailedNotif = false;
		if (!shownFailedNotif) {
			shownFailedNotif = true;
			if (Loader::get()->getFailedMods().size()) {
				Notification::create(
					"Some mods failed to load",
					NotificationIcon::Error
				)->show();
			}
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
						geode::openIssueReportPopup(InternalMod::get());
					}
				},
				false
			);
			popup->m_scene = this;
			popup->m_noElasticity = true;
			popup->show();
		}

		// update mods index
		if (!g_indexUpdateNotif && !Index::get()->hasTriedToUpdate()) {
			g_indexUpdateNotif = Notification::create(
				"Updating Index", NotificationIcon::Loading, 0
			);
			g_indexUpdateNotif->show();

			Index::get()->update();
		}
	
		return true;
	}

	void onGeode(CCObject*) {
		ModListLayer::scene();
	}
};
