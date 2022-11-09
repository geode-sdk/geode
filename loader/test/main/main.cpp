#include <Geode/Loader.hpp>

USE_GEODE_NAMESPACE();

auto test = []() {
    log::info("Static logged");
    return 0;
};

// Exported functions
GEODE_API bool GEODE_CALL geode_enable() {
    log::info("Enabled");
    return true;
}

GEODE_API bool GEODE_CALL geode_disable() {
    log::info("Disabled");
    return true;
}

GEODE_API bool GEODE_CALL geode_load(Mod*) {
    log::info("Loaded");
    return true;
}

GEODE_API bool GEODE_CALL geode_unload() {
    log::info("Unloaded");
    return true;
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

        // Data Store
        auto ds = Mod::get()->getDataStore();
        int out = ds["times-opened"];
        ds["times-opened"] = out + 1;

        std::string text = std::string("Times opened: ") + std::to_string(out);

        auto label2 = CCLabelBMFont::create(text.c_str(), "bigFont.fnt");
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
