#include <Geode/DefaultInclude.hpp>

#if GEODE_COMP_GD_VERSION < 22090
#ifndef GEODE_IS_IOS
#include <Geode/modify/MoreOptionsLayer.hpp>
#include <ui/mods/settings/KeybindsPopup.hpp>

using namespace geode::prelude;

// This seems to also be merged with the editor keybindings button
class $modify(MoreOptionsLayer) {
	GEODE_FORWARD_COMPAT_DISABLE_HOOKS("MoreOptionsLayer keybindings button")

	$override
	void onKeybindings(CCObject*) {
		KeybindsPopup::create()->show();
	}
};
#endif
#else
#ifndef GEODE_IS_IOS
#include <Geode/modify/KeybindingsLayer.hpp>
#include <Geode/binding/ButtonSprite.hpp>
#include <Geode/binding/CCMenuItemSpriteExtra.hpp>
#include <Geode/ui/BasedButtonSprite.hpp>
#include <ui/mods/settings/KeybindsPopup.hpp>

using namespace geode::prelude;

// In GD 2.209+, RobTop introduced native KeybindingsLayer.
// We hook KeybindingsLayer to add a button for Geode's mod keybinds.
class $modify(GeodeKeybindingsLayer, KeybindingsLayer) {
	GEODE_FORWARD_COMPAT_DISABLE_HOOKS("KeybindingsLayer geode keybinds button")

	bool init(GameSettingsLayer* settings) {
		if (!KeybindingsLayer::init(settings)) return false;

		if (auto menu = m_buttonMenu) {
			auto spr = CircleButtonSprite::createWithSpriteFrameName(
				"geode.loader/geode-logo.png",
				.8f,
				CircleBaseColor::DarkAqua,
				CircleBaseSize::Small
			);
			if (!spr) {
				spr = CircleButtonSprite::createWithSpriteFrameName(
					"geode.loader/logo.png",
					.8f,
					CircleBaseColor::DarkAqua,
					CircleBaseSize::Small
				);
			}
			if (spr) {
				auto btn = CCMenuItemSpriteExtra::create(
					spr, this, menu_selector(GeodeKeybindingsLayer::onGeodeKeybinds)
				);
				btn->setID("geode-keybinds-button"_spr);
				menu->addChild(btn);
				menu->updateLayout();
			}
		}

		return true;
	}

	void onGeodeKeybinds(CCObject*) {
		KeybindsPopup::create()->show();
	}
};
#endif
#endif
