#include <Geode/Geode.hpp>

USE_GEODE_NAMESPACE();

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
