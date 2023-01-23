
#include "../ids/AddIDs.hpp"
#include "../ui/internal/list/ModListLayer.hpp"

#include <Geode/loader/Index.hpp>
#include <Geode/modify/MenuLayer.hpp>
#include <Geode/modify/Modify.hpp>
#include <Geode/ui/BasedButtonSprite.hpp>
#include <Geode/ui/GeodeUI.hpp>
#include <Geode/ui/Notification.hpp>
#include <Geode/ui/Popup.hpp>
#include <Geode/utils/cocos.hpp>
#include <loader/ModImpl.hpp>

USE_GEODE_NAMESPACE();

#pragma warning(disable : 4217)

class CustomMenuLayer;

static Ref<Notification> INDEX_UPDATE_NOTIF = nullptr;

$execute {
	new EventListener<IndexUpdateFilter>(+[](IndexUpdateEvent* event) {
		if (!INDEX_UPDATE_NOTIF) return;
		std::visit(makeVisitor {
			[](UpdateProgress const& prog) {},
			[](UpdateFinished const&) {
				INDEX_UPDATE_NOTIF->setIcon(NotificationIcon::Success);
				INDEX_UPDATE_NOTIF->setString("Index Up-to-Date");
				INDEX_UPDATE_NOTIF->waitAndHide();
				INDEX_UPDATE_NOTIF = nullptr;
			},
			[](UpdateFailed const& info) {
				INDEX_UPDATE_NOTIF->setIcon(NotificationIcon::Error);
				INDEX_UPDATE_NOTIF->setString(info);
				INDEX_UPDATE_NOTIF->setTime(NOTIFICATION_LONG_TIME);
				INDEX_UPDATE_NOTIF = nullptr;
			},
		}, event->status);
	});
};

struct CustomMenuLayer : Modify<CustomMenuLayer, MenuLayer> {
	CCSprite* m_geodeButton;

    bool init() {
        if (!MenuLayer::init()) return false;

        // make sure to add the string IDs for nodes (Geode has no manual
        // hook order support yet so gotta do this to ensure)
        NodeIDs::provideFor(this);

        auto winSize = CCDirector::sharedDirector()->getWinSize();

		// add geode button
		
		m_fields->m_geodeButton = CircleButtonSprite::createWithSpriteFrameName(
			"geode-logo-outline-gold.png"_spr,
			1.0f,
			CircleBaseColor::Green,
			CircleBaseSize::Medium2
		);
		if (!m_fields->m_geodeButton) {
			m_fields->m_geodeButton = ButtonSprite::create("!!");
		}

        auto bottomMenu = static_cast<CCMenu*>(this->getChildByID("bottom-menu"));

		auto btn = CCMenuItemSpriteExtra::create(
			m_fields->m_geodeButton, this, menu_selector(CustomMenuLayer::onGeode)
		);
		btn->setID("geode-button"_spr);
		bottomMenu->addChild(btn);

        bottomMenu->updateLayout();

        if (auto node = this->getChildByID("settings-gamepad-icon")) {
            node->setPositionX(
                bottomMenu->getChildByID("settings-button")->getPositionX() + winSize.width / 2
            );
        }

        // show if some mods failed to load
        static bool shownFailedNotif = false;
        if (!shownFailedNotif) {
            shownFailedNotif = true;
            if (Loader::get()->getFailedMods().size()) {
                Notification::create("Some mods failed to load", NotificationIcon::Error)->show();
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
                "No",
                "Send",
                [](auto, bool btn2) {
                    if (btn2) {
                        geode::openIssueReportPopup(Mod::get());
                    }
                },
                false
            );
            popup->m_scene = this;
            popup->m_noElasticity = true;
            popup->show();
        }

		// update mods index
		if (!INDEX_UPDATE_NOTIF && !Index::get()->hasTriedToUpdate()) {
			this->addChild(EventListenerNode<IndexUpdateFilter>::create(
				this, &CustomMenuLayer::onIndexUpdate
			));
			INDEX_UPDATE_NOTIF = Notification::create(
				"Updating Index", NotificationIcon::Loading, 0
			);
			INDEX_UPDATE_NOTIF->show();
			Index::get()->update();
		}

		this->addUpdateIndicator();
	
		return true;
	}

	void onIndexUpdate(IndexUpdateEvent* event) {
		if (
			std::holds_alternative<UpdateFinished>(event->status) ||
			std::holds_alternative<UpdateFailed>(event->status)
		) {
			this->addUpdateIndicator();
		}
	}

	void addUpdateIndicator() {
		if (Index::get()->areUpdatesAvailable()) {
			auto icon = CCSprite::createWithSpriteFrameName("updates-available.png"_spr);
			icon->setPosition(
				m_fields->m_geodeButton->getContentSize() - CCSize { 10.f, 10.f }
			);
			icon->setZOrder(99);
			icon->setScale(.5f);
			m_fields->m_geodeButton->addChild(icon);
		}
	}

    void onGeode(CCObject*) {
        ModListLayer::scene();
    }
};
