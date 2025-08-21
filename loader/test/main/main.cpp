#include <Geode/Loader.hpp>
#include <Geode/loader/ModEvent.hpp>
#include <Geode/utils/cocos.hpp>
#include <chrono>
#include "../dependency/main.hpp"
#include "Geode/utils/general.hpp"
#include <Geode/utils/VMTHookManager.hpp>

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

// Coroutines
#include <Geode/utils/async.hpp>
auto advanceFrame() {
    auto [task, finish, progress, cancelled] = Task<void>::spawn();
    queueInMainThread(std::bind(finish, true));

    return task;
}

$execute {
    $async() {
        auto start = std::chrono::steady_clock::now();
        log::info("Waiting for 10 frames...");
        for (int i = 0; i < 10; ++i)
            co_await advanceFrame();

        log::info("Finished waiting! Took {} seconds", std::chrono::duration_cast<std::chrono::seconds>(
            std::chrono::steady_clock::now() - start
        ).count());
    };

    auto output = $try<VersionInfo> {
        log::info("Parsing number 123: {}", co_await utils::numFromString<int>("123"));
        log::info("Parsing number 12.3: {}", co_await utils::numFromString<int>("12.3"));

        co_return VersionInfo::parse("1.2.3-alpha.4");
    };

    if (!output) {
        log::info("$try successfully caught error");
    }
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
        log::NestScope nest;
        log::info("For global context:");
        {
            log::NestScope nest;
            for (const auto& arg : Loader::get()->getLaunchArgumentNames()) {
                log::info("{}", arg);
            }
        }
        log::info("For this mod:");
        {
            log::NestScope nest;
            for (const auto& arg : Mod::get()->getLaunchArgumentNames()) {
                log::info("{}", arg);
            }
        }
        log::info("Mod has launch arg 'mod-arg': {}", Mod::get()->hasLaunchArgument("mod-arg"));
        log::info("Loader flag 'bool-arg': {}", Loader::get()->getLaunchFlag("bool-arg"));
        log::info("Loader int 'int-arg': {}", Loader::get()->parseLaunchArgument<int>("int-arg").unwrapOr(0));

        log::debug("should run second!");

        if (GEODE_UNWRAP_IF_OK(val, api::addNumbers(5, 6))) {
            log::info("5 + 6 = {}", val);
        }
        else {
            log::error("Failed to API (function)");
        }

        api::Test test;
        if (GEODE_UNWRAP_IF_OK(val, test.addNumbers(5, 6))) {
            log::info("5 + 6 = {}", val);
        }
        else {
            log::error("Failed to API (method)");
        }


        return true;
    }
};

// Modify
#include <Geode/modify/GJGarageLayer.hpp>

struct GJGarageLayerTest : Modify<GJGarageLayerTest, GJGarageLayer> {
    struct Fields {
        int myValue = 1907;
        std::string myString = "yeah have fun finding a better thing for this";
    };

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
        int counter = 0;
    };

    bool init() override {
        if (!GJGarageLayer::init()) return false;

        if (m_fields->myOtherValue == 80085) {
            auto label = CCLabelBMFont::create("Alternate Fields works!", "bigFont.fnt");
            label->setPosition(100, 60);
            label->setScale(.4f);
            label->setZOrder(99999);
            this->addChild(label);
        }

        this->setTouchMode(kCCTouchesOneByOne);
        this->setTouchEnabled(true);

        auto hook = VMTHookManager::get().addHook<
            ResolveC<GJGarageLayerTest2>::func(&GJGarageLayerTest2::ccTouchBegan)
        >(this, "GJGarageLayer::ccTouchBegan");
        auto hook2 = VMTHookManager::get().addHook<
            ResolveC<CCTouchDelegate>::func(&GJGarageLayerTest2::ccTouchBegan)
        >(this, "GJGarageLayer::ccTouchBegan");

        auto hook3 = VMTHookManager::get().addHook<
            ResolveC<CCTouchDelegate>::func(&GJGarageLayerTest2::ccTouchEnded)
        >(this, "GJGarageLayer::ccTouchEnded");
        auto hook4 = VMTHookManager::get().addHook<
            ResolveC<GJGarageLayerTest2>::func(&GJGarageLayerTest2::ccTouchEnded)
        >(this, "GJGarageLayer::ccTouchEnded");

        return true;
    }

    bool ccTouchBegan(CCTouch* touch, CCEvent* event) override {
        this->ccTouchBegan(touch, event);
        log::debug("Touch began on GJGarageLayer");

        if (m_fields->counter % 2) {
            VMTHookManager::get().forceDisableFunction<
                ResolveC<GJGarageLayerTest2>::func(&GJGarageLayerTest2::ccTouchEnded)
            >(this);
        }
        else {
            VMTHookManager::get().forceEnableFunction<
                ResolveC<GJGarageLayerTest2>::func(&GJGarageLayerTest2::ccTouchEnded)
            >(this);
        }
        m_fields->counter++;
        return true;
    }

    void ccTouchEnded(CCTouch* touch, CCEvent* event) override {
        this->ccTouchEnded(touch, event);
        log::debug("Touch ended on GJGarageLayer");
    }
};
