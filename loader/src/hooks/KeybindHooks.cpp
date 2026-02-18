#include <Geode/modify/MoreOptionsLayer.hpp>
#include <ui/mods/settings/KeybindsPopup.hpp>

using namespace geode::prelude;

// This seems to also be merged with the editor keybindings button
class $modify(MoreOptionsLayer) {
	$override
	void onKeybindings(CCObject*) {
		KeybindsPopup::create()->show();
	}
};
