#include <Geode/Loader.hpp>
#include <Geode/loader/ModJsonTest.hpp>
#include <Geode/loader/ModEvent.hpp>
#include "../dependency/main.hpp"

using namespace geode::prelude;

auto test = []() {
    log::info("Static logged");
    return 0;
};

// Exported functions
$on_mod(Enabled) {
    log::info("Enabled");
}
$on_mod(Disabled) {
    log::info("Disabled");
}
$on_mod(Loaded) {
    log::info("Loaded");
}
$on_mod(Unloaded) {
    log::info("Unloaded");
}

// Events
$execute {
    new EventListener<TestEventFilter>(+[](TestEvent* event) {
        log::info("Received event: {}", event->getData());
    });
}

// Modify
#include <Geode/modify/GJGarageLayer.hpp>

struct GJGarageLayerTest : Modify<GJGarageLayerTest, GJGarageLayer> {
    GJGarageLayerTest() : myValue(1907) {}

    int myValue;
    std::string myString = "yeah have fun finding a better thing for this";

    bool init() {
        if (!GJGarageLayer::init()) return false;

        auto label = CCLabelBMFont::create("Modify works!", "bigFont.fnt");
        label->setPosition(100, 110);
        label->setScale(.4f);
        label->setZOrder(99999);
        addChild(label);

        if (m_fields->myValue == 1907 && m_fields->myString != "") {
            auto label = CCLabelBMFont::create("Field default works!", "bigFont.fnt");
            label->setPosition(100, 100);
            label->setScale(.4f);
            label->setZOrder(99999);
            addChild(label);
        }

        // Saved Values
        auto timesOpened = Mod::get()->getSavedValue<int64_t>("times-opened", 0);
        Mod::get()->setSavedValue("times-opened", timesOpened + 1);

        auto label2 = CCLabelBMFont::create(
            fmt::format("Times opened: {}", timesOpened).c_str(),
            "bigFont.fnt"
        );
        label2->setPosition(100, 90);
        label2->setScale(.4f);
        label2->setZOrder(99999);
        addChild(label2);

        // Dispatch system pt. 1
        // auto fn = Dispatcher::get()->getFunction<void(GJGarageLayer*)>("test-garage-open");
        // fn(this);

        return true;
    }
};

/*// Event system pt. 2
int a = (0, []() {

        Dispatcher::get()->addSelector("test-garage-open", [](GJGarageLayer* gl) {
                auto label = CCLabelBMFont::create("EventCenter works!", "bigFont.fnt");
                label->setPosition(100, 80);
                label->setScale(.4f);
                label->setZOrder(99999);
                gl->addChild(label);

                TestDependency::depTest(gl);
        });

// Event system pt. 2
// $observe("test-garage-open", GJGarageLayer*, evt) {
// 	auto gl = evt.object();
// 	auto label = CCLabelBMFont::create("EventCenter works!", "bigFont.fnt");
// 	label->setPosition(100, 80);
// 	label->setScale(.4f);
// 	label->setZOrder(99999);
// 	gl->addChild(label);

// 	// API pt. 2
// 	TestDependency::depTest(gl);
// }
        return 0;
}());*/
