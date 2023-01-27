#include <Geode/Loader.hpp>

USE_GEODE_NAMESPACE();

#include <Geode/modify/MenuLayer.hpp>
#include <Geode/loader/SettingNode.hpp>
#include <Geode/loader/ModJsonTest.hpp>


enum class Icon {
    Steve,
    Mike,
    LazarithTheDestroyerOfForsakenSouls,
    Geoff,
};
constexpr Icon DEFAULT_ICON = Icon::Steve;

class MySettingValue;

class MySettingValue : public SettingValue {
protected:
    Icon m_icon;

public:
    MySettingValue(std::string const& key, Icon icon)
      : SettingValue(key), m_icon(icon) {}

    bool load(json::Value const& json) override {
        try {
            m_icon = static_cast<Icon>(json.as<int>());
            return true;
        } catch(...) {
            return false;
        }
    }
    bool save(json::Value& json) const override {
        json = static_cast<int>(m_icon);
        return true;
    }
    SettingNode* createNode(float width) override;

    void setIcon(Icon icon) {
        m_icon = icon;
    }
    Icon getIcon() const {
        return m_icon;
    }
};

class MySettingNode : public SettingNode {
protected:
    Icon m_currentIcon;
    std::vector<CCSprite*> m_sprites;

    bool init(MySettingValue* value, float width) {
        if (!SettingNode::init(value))
            return false;
        
        m_currentIcon = value->getIcon();
        this->setContentSize({ width, 40.f });

        auto menu = CCMenu::create();
        menu->setPosition(width / 2, 20.f);

        float x = -75.f;

        for (auto& [spr, icon] : {
            std::pair { "player_01_001.png", Icon::Steve, },
            std::pair { "player_02_001.png", Icon::Mike, },
            std::pair { "player_03_001.png", Icon::LazarithTheDestroyerOfForsakenSouls, },
            std::pair { "player_04_001.png", Icon::Geoff, },
        }) {
            auto btnSpr = CCSprite::createWithSpriteFrameName(spr);
            btnSpr->setScale(.7f);
            m_sprites.push_back(btnSpr);
            if (icon == m_currentIcon) {
                btnSpr->setColor({ 0, 255, 0 });
            } else {
                btnSpr->setColor({ 200, 200, 200 });
            }
            auto btn = CCMenuItemSpriteExtra::create(
                btnSpr, this, menu_selector(MySettingNode::onSelect)
            );
            btn->setTag(static_cast<int>(icon));
            btn->setPosition(x, 0);
            menu->addChild(btn);

            x += 50.f;
        }

        this->addChild(menu);

        return true;
    }

    void onSelect(CCObject* sender) {
        for (auto& spr : m_sprites) {
            spr->setColor({ 200, 200, 200 });
        }
        m_currentIcon = static_cast<Icon>(sender->getTag());
        static_cast<CCSprite*>(
            static_cast<CCMenuItemSpriteExtra*>(sender)->getNormalImage()
        )->setColor({ 0, 255, 0 });
        this->dispatchChanged();
    }

public:
    void commit() override {
        static_cast<MySettingValue*>(m_value)->setIcon(m_currentIcon);
        this->dispatchCommitted();
    }
    bool hasUncommittedChanges() override {
        return m_currentIcon != static_cast<MySettingValue*>(m_value)->getIcon();
    }
    bool hasNonDefaultValue() override {
        return m_currentIcon != DEFAULT_ICON;
    }
    void resetToDefault() override {
        m_currentIcon = DEFAULT_ICON;
    }

    static MySettingNode* create(MySettingValue* value, float width) {
        auto ret = new MySettingNode;
        if (ret && ret->init(value, width)) {
            ret->autorelease();
            return ret;
        }
        CC_SAFE_DELETE(ret);
        return nullptr;
    }
};

SettingNode* MySettingValue::createNode(float width) {
    return MySettingNode::create(this, width);
}

struct MyMenuLayer : Modify<MyMenuLayer, MenuLayer> {
    void onMoreGames(CCObject*) {
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

$on_mod(Loaded) {
    Mod::get()->registerCustomSetting(
        "overcast-skies",
        std::make_unique<MySettingValue>("overcast-skies", DEFAULT_ICON)
    );

    // Dispatcher::get()->addFunction<void(GJGarageLayer*)>("test-garage-open", [](GJGarageLayer*
    // gl) { 	auto label = CCLabelBMFont::create("Dispatcher works!", "bigFont.fnt");
    // 	label->setPosition(100, 80);
    // 	label->setScale(.4f);
    // 	label->setZOrder(99999);
    // 	gl->addChild(label);
    // });
}
