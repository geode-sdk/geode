#include <Geode/Geode.hpp>

USE_GEODE_NAMESPACE();

class $modify(MenuLayer) {
	void onMoreGames(CCObject*) {
		if (Mod::get()->getSettingValue<bool>("enable-gay-dreams")) {
			FLAlertLayer::create(
				"Warning",
				"You are homosexual",
				"OK"
			)->show();
		} else {
			FLAlertLayer::create(
				"Warning",
				"There is no evidence for nor against you being homosexual",
				"OK"
			)->show();
		}
	}
};

GEODE_API bool GEODE_CALL geode_load(Mod*) {
	// Dispatcher::get()->addFunction<void(GJGarageLayer*)>("test-garage-open", [](GJGarageLayer* gl) {
	// 	auto label = CCLabelBMFont::create("Dispatcher works!", "bigFont.fnt");
	// 	label->setPosition(100, 80);
	// 	label->setScale(.4f);
	// 	label->setZOrder(99999);
	// 	gl->addChild(label);
	// });
	return true;
}
