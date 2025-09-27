#include <Geode/Loader.hpp>
#include <Geode/modify/MenuLayer.hpp>
#define GEODE_DEFINE_EVENT_EXPORTS
#include <Geode/loader/Dispatch.hpp>
#include <Geode/Bindings.hpp>
#include "main.hpp"
#include <string_view>

using namespace geode::prelude;

std::string_view TestEvent::getData() const {
    return data;
}

TestEvent::TestEvent(std::string_view data) : data(data) {}

ListenerResult TestEventFilter::handle(std::function<Callback> fn, TestEvent* event) {
    fn(event);
    return ListenerResult::Propagate;
}

TestEventFilter::TestEventFilter() = default;

struct MyMenuLayer : Modify<MyMenuLayer, MenuLayer> {
    void onMoreGames(CCObject*) {
        TestEvent("Event system works!"s).post();

        static auto* mod = Mod::get();
        if (mod->getSettingValue<bool>("its-raining-after-all")) {
            FLAlertLayer::create("Damn", ":(", "OK")->show();
        }
        else {
            FLAlertLayer::create(
                "Yay",
                "The weather report said it wouldn't rain today :)",
                "OK"
            )->show();
        }
    }
};

struct AfterMenuLayer : Modify<AfterMenuLayer, MenuLayer> {
    static void onModify(auto& self) {
        if (self.setHookPriority("MenuLayer::init", "geode.test", -1)) {
            log::debug("priority set after test");
        }
    }

    bool init() {
        if (!MenuLayer::init()) return false;
        log::debug("should run third!");
        return true;
    }
};

struct BeforeMenuLayer : Modify<BeforeMenuLayer, MenuLayer> {
    static void onModify(auto& self) {
        if (self.setHookPriority("MenuLayer::init", "geode.test", 1)) {
            log::debug("priority set before test");
        }
    }

    bool init() {
        if (!MenuLayer::init()) return false;
        log::debug("should run first!");
        return true;
    }
};

$on_mod(Loaded) {
    (void)new EventListener(+[](GJGarageLayer* gl) {
        auto label = CCLabelBMFont::create("Dispatcher works!", "bigFont.fnt");
        label->setPosition(100, 80);
        label->setScale(.4f);
        label->setZOrder(99999);
        gl->addChild(label);
        return ListenerResult::Propagate;
    }, MyDispatchFilter("geode.test/test-garage-open"));
}

Result<int> api::addNumbers(int a, int b) {
    return Ok(a + b);
}

Result<int> api::Test::addNumbers(int a, int b) {
    return Ok(a + b);
}
