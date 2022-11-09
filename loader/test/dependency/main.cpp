#include <Geode/Loader.hpp>

USE_GEODE_NAMESPACE();

#include <Geode/modify/MenuLayer.hpp>

struct MyMenuLayer : Modify<MyMenuLayer, MenuLayer> {
    void onMoreGames(CCObject*) {
        if (Mod::get()->getSettingValue<bool>("its-raining-after-all")) {
            FLAlertLayer::create("Damn", ":(", "OK")->show();
        }
        else {
            FLAlertLayer::create("Yay", "The weather report said it wouldn't rain today :)", "OK")
                ->show();
        }
    }
};

GEODE_API bool GEODE_CALL geode_load(Mod*) {
    // Dispatcher::get()->addFunction<void(GJGarageLayer*)>("test-garage-open", [](GJGarageLayer*
    // gl) { 	auto label = CCLabelBMFont::create("Dispatcher works!", "bigFont.fnt");
    // 	label->setPosition(100, 80);
    // 	label->setScale(.4f);
    // 	label->setZOrder(99999);
    // 	gl->addChild(label);
    // });
    return true;
}
