#include <Geode/Loader.hpp>
#include <Geode/modify/MenuLayer.hpp>
#define GEODE_DEFINE_EVENT_EXPORTS
#include <Geode/loader/Dispatch.hpp>
#include <Geode/Bindings.hpp>
#include "main.hpp"

using namespace geode::prelude;

std::string TestEvent::getData() const {
    return data;
}

TestEvent::TestEvent(std::string const& data) : data(data) {}

ListenerResult TestEventFilter::handle(std::function<Callback> fn, TestEvent* event) {
    fn(event);
    return ListenerResult::Propagate;
}

TestEventFilter::TestEventFilter() {}

enum class Icon {
    Steve,
    Mike,
    LazarithTheDestroyerOfForsakenSouls,
    Geoff,
};
constexpr Icon DEFAULT_ICON = Icon::Steve;

class MySettingValue;

// TODO: v4 (port to new settings)
// class MySettingValue : public SettingValue {
// protected:
//     Icon m_icon;

// public:
//     MySettingValue(std::string const& key, std::string const& modID, Icon icon)
//       : SettingValue(key, modID), m_icon(icon) {}

//     bool load(matjson::Value const& json) override {
//         if (!json.is<int>()) return false;
//         m_icon = static_cast<Icon>(json.as<int>());
//         return true;
//     }
//     bool save(matjson::Value& json) const override {
//         json = static_cast<int>(m_icon);
//         return true;
//     }
//     SettingNode* createNode(float width) override;

//     void setIcon(Icon icon) {
//         m_icon = icon;
//     }
//     Icon getIcon() const {
//         return m_icon;
//     }
// };

// class MySettingNode : public SettingNode {
// protected:
//     Icon m_currentIcon;
//     std::vector<CCSprite*> m_sprites;

//     bool init(MySettingValue* value, float width) {
//         if (!SettingNode::init(value))
//             return false;

//         m_currentIcon = value->getIcon();
//         this->setContentSize({ width, 40.f });

//         auto menu = CCMenu::create();
//         menu->setPosition(width / 2, 20.f);

//         float x = -75.f;

//         for (auto& [spr, icon] : {
//             std::pair { "GJ_square01.png", Icon::Steve, },
//             std::pair { "GJ_square02.png", Icon::Mike, },
//             std::pair { "GJ_square03.png", Icon::LazarithTheDestroyerOfForsakenSouls, },
//             std::pair { "GJ_square04.png", Icon::Geoff, },
//         }) {
//             auto btnSpr = CCSprite::create(spr);
//             btnSpr->setScale(.7f);
//             m_sprites.push_back(btnSpr);
//             if (icon == m_currentIcon) {
//                 btnSpr->setColor({ 0, 255, 0 });
//             } else {
//                 btnSpr->setColor({ 200, 200, 200 });
//             }
//             auto btn = CCMenuItemSpriteExtra::create(
//                 btnSpr, this, menu_selector(MySettingNode::onSelect)
//             );
//             btn->setTag(static_cast<int>(icon));
//             btn->setPosition(x, 0);
//             menu->addChild(btn);

//             x += 50.f;
//         }

//         this->addChild(menu);

//         return true;
//     }

//     void onSelect(CCObject* sender) {
//         for (auto& spr : m_sprites) {
//             spr->setColor({ 200, 200, 200 });
//         }
//         m_currentIcon = static_cast<Icon>(sender->getTag());
//         static_cast<CCSprite*>(
//             static_cast<CCMenuItemSpriteExtra*>(sender)->getNormalImage()
//         )->setColor({ 0, 255, 0 });
//         this->dispatchChanged();
//     }

// public:
//     void commit() override {
//         static_cast<MySettingValue*>(m_value)->setIcon(m_currentIcon);
//         this->dispatchCommitted();
//     }
//     bool hasUncommittedChanges() override {
//         return m_currentIcon != static_cast<MySettingValue*>(m_value)->getIcon();
//     }
//     bool hasNonDefaultValue() override {
//         return m_currentIcon != DEFAULT_ICON;
//     }
//     void resetToDefault() override {
//         m_currentIcon = DEFAULT_ICON;
//     }

//     static MySettingNode* create(MySettingValue* value, float width) {
//         auto ret = new MySettingNode;
//         if (ret->init(value, width)) {
//             ret->autorelease();
//             return ret;
//         }
//         delete ret;
//         return nullptr;
//     }
// };

// SettingNode* MySettingValue::createNode(float width) {
//     return MySettingNode::create(this, width);
// }

struct MyMenuLayer : Modify<MyMenuLayer, MenuLayer> {
    void onMoreGames(CCObject*) {
        TestEvent("Event system works!").post();
        if (Mod::get()->getSettingValue<bool>("its-raining-after-all")) {
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
        if (self.setHookPriorityAfterPost("MenuLayer::init", "geode.test")) {
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
        if (self.setHookPriorityBeforePost("MenuLayer::init", "geode.test")) {
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
    // Mod::get()->addCustomSetting<MySettingValue>("overcast-skies", DEFAULT_ICON);

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