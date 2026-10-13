#include <Geode/modify/MoreOptionsLayer.hpp>
#include <ui/mods/settings/KeybindsPopup.hpp>

using namespace geode::prelude;

#ifndef GEODE_IS_IOS
// This seems to also be merged with the editor keybindings button
class $modify(MoreOptionsLayer) {
	GEODE_FORWARD_COMPAT_DISABLE_HOOKS("MoreOptionsLayer keybindings button")

	$override
	void onKeybindings(CCObject*) {
		KeybindsPopup::create()->show();
	}
};
#endif
