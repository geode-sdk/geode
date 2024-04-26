#include <Geode/Loader.hpp>
#include <Geode/loader/ModEvent.hpp>
#include <Geode/utils/cocos.hpp>
#include "../dependency/main.hpp"

using namespace geode::prelude;

auto test = []() {
    log::info("Static logged");
    return 0;
};

// Exported functions
$on_mod(Loaded) {
    log::info("Loaded");
}

static std::string s_recievedEvent;

// Events
$execute {
    new EventListener<TestEventFilter>(+[](TestEvent* event) {
        log::info("Received event: {}", event->getData());
        s_recievedEvent = event->getData();
    });
}

#include <Geode/modify/MenuLayer.hpp>
struct $modify(MenuLayer) {
    bool init() {
        if (!MenuLayer::init())
            return false;
        
        auto node = CCNode::create();
        auto ref = WeakRef(node);
        log::info("ref: {}", ref.lock().data());
        node->release();
        log::info("ref: {}", ref.lock().data());

        // Launch arguments
        log::info("Testing launch args...");
        log::pushNest();
        log::info("For global context:");
        log::pushNest();
        for (const auto& arg : Loader::get()->getLaunchArgumentNames()) {
            log::info("{}", arg);
        }
        log::popNest();
        log::info("For this mod:");
        log::pushNest();
        for (const auto& arg : Mod::get()->getLaunchArgumentNames()) {
            log::info("{}", arg);
        }
        log::popNest();
        log::info("Mod has launch arg 'mod-arg': {}", Mod::get()->hasLaunchArgument("mod-arg"));
        log::info("Loader flag 'bool-arg': {}", Loader::get()->getLaunchFlag("bool-arg"));
        log::info("Loader int 'int-arg': {}", Loader::get()->parseLaunchArgument<int>("int-arg"));
        log::popNest();

        return true;
    }
};

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
        MyDispatchEvent("geode.test/test-garage-open", this).post();

        if (s_recievedEvent.size() > 0) {
            auto label = CCLabelBMFont::create("Event works!", "bigFont.fnt");
            label->setPosition(100, 70);
            label->setScale(.4f);
            label->setZOrder(99999);
            addChild(label);
        }

        return true;
    }
};


#include <Geode/modify/GJGarageLayer.hpp>

struct GJGarageLayerTest2 : Modify<GJGarageLayerTest2, GJGarageLayer> {
    struct Fields {
        int myOtherValue = 80085;
    };
    
    bool init() {
        if (!GJGarageLayer::init()) return false;

        if (m_fields->myOtherValue == 80085) {
            auto label = CCLabelBMFont::create("Alternate Fields works!", "bigFont.fnt");
            label->setPosition(100, 60);
            label->setScale(.4f);
            label->setZOrder(99999);
            this->addChild(label);
        }

        return true;
    }
};
