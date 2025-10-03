#include <Geode/loader/SettingV3.hpp>
#include <ui/mods/GeodeStyle.hpp>

using namespace geode::prelude;

class CopyButtonSetting : public SettingV3 {
public:
    static Result<std::shared_ptr<SettingV3>> parse(std::string const& key, std::string const& modID, matjson::Value const& json) {
        auto res = std::make_shared<CopyButtonSetting>();
        auto root = checkJson(json, "CopyButtonSetting");

        res->init(key, modID, root);
        res->parseNameAndDescription(root);

        return root.ok(std::static_pointer_cast<SettingV3>(res));
    }

    bool load(matjson::Value const& json) override {
        return true;
    }
    bool save(matjson::Value& json) const override {
        return true;
    }

    bool isDefaultValue() const override {
        return true;
    }
    void reset() override {}

    SettingNodeV3* createNode(float width) override;
};

class CopyButtonSettingNode : public SettingNodeV3 {
protected:
    bool init(std::shared_ptr<CopyButtonSetting> setting, float width) {
        if (!SettingNodeV3::init(setting, width))
            return false;

        auto buttonSprite = createGeodeButton("Copy Mods");
        buttonSprite->setScale(.5f);
        auto button = CCMenuItemSpriteExtra::create(
            buttonSprite, this, menu_selector(CopyButtonSettingNode::onCopy)
        );
        this->getButtonMenu()->addChildAtPosition(button, Anchor::Center);
        this->getButtonMenu()->setPosition(getContentSize() / 2);
        this->getButtonMenu()->setAnchorPoint({ .5f, .5f });
        this->getButtonMenu()->updateLayout();

        this->updateState(nullptr);

        return true;
    }

    void onCopy(CCObject*);

    void onCommit() override {}
    void onResetToDefault() override {}

public:
    static CopyButtonSettingNode* create(std::shared_ptr<CopyButtonSetting> setting, float width) {
        auto ret = new CopyButtonSettingNode();
        if (ret->init(setting, width)) {
            ret->autorelease();
            return ret;
        }
        delete ret;
        return nullptr;
    }

    bool hasUncommittedChanges() const override {
        return false;
    }
    bool hasNonDefaultValue() const override {
        return false;
    }
};
