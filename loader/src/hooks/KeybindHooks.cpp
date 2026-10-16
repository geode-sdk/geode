// TODO: 2.209
#if 0
#include <Geode/modify/MoreOptionsLayer.hpp>
#include <ui/mods/settings/KeybindsPopup.hpp>

using namespace geode::prelude;

#ifndef GEODE_IS_IOS
// This seems to also be merged with the editor keybindings button
class $modify(MoreOptionsLayer) {
	// TODO (v6) what do we do here
	$override
	// void onKeybindings(CCObject*) {
	// 	KeybindsPopup::create()->show();
	// }
};
#endif
#endif