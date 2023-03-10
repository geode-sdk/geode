#pragma once

#include <Geode/binding/CCMenuItemSpriteExtra.hpp>
#include <Geode/binding/CCTextInputNode.hpp>
#include <Geode/binding/Slider.hpp>
#include <Geode/binding/SliderThumb.hpp>
#include <Geode/loader/Setting.hpp>
#include <Geode/loader/SettingNode.hpp>
#include <Geode/ui/BasedButtonSprite.hpp>
#include <Geode/ui/ColorPickPopup.hpp>
#include <Geode/ui/InputNode.hpp>
#include <Geode/ui/Popup.hpp>
#include <Geode/utils/cocos.hpp>
#include <Geode/utils/string.hpp>

using namespace geode::prelude;

#define IMPL_SETT_CREATE(type_) \
    static type_##SettingNode* create(              \
        type_##SettingValue* value, float width     \
    ) {                                             \
        auto ret = new type_##SettingNode;          \
        if (ret && ret->init(value, width)) {       \
            ret->autorelease();                     \
            return ret;                             \
        }                                           \
        CC_SAFE_DELETE(ret);                        \
        return nullptr;                             \
    }

template <class T>
class GeodeSettingNode : public SettingNode {
public:
    using ValueType = typename T::ValueType;

protected:
    float m_width;
    float m_height;
    ValueType m_uncommittedValue;
    CCMenu* m_menu;
    CCLabelBMFont* m_nameLabel;
    CCLabelBMFont* m_errorLabel;
    CCMenuItemSpriteExtra* m_resetBtn;

    bool init(GeodeSettingValue<T>* setting, float width) {
        if (!SettingNode::init(setting))
            return false;

        m_width = width;
        m_height = this->setupHeight(setting);
        this->setContentSize({ width, m_height });

        constexpr auto sidePad = 40.f;

        m_uncommittedValue = setting->getValue();

        auto name = setting->getDefinition().getDisplayName();

        m_nameLabel = CCLabelBMFont::create(name.c_str(), "bigFont.fnt");
        m_nameLabel->setAnchorPoint({ .0f, .5f });
        m_nameLabel->limitLabelWidth(width / 2 - 50.f, .5f, .1f);
        m_nameLabel->setPosition({ sidePad / 2, m_obContentSize.height / 2 });
        this->addChild(m_nameLabel);

        m_errorLabel = CCLabelBMFont::create("", "bigFont.fnt");
        m_errorLabel->setAnchorPoint({ .0f, .5f });
        m_errorLabel->limitLabelWidth(width / 2 - 50.f, .25f, .1f);
        m_errorLabel->setPosition({ sidePad / 2, m_obContentSize.height / 2 - 14.f });
        m_errorLabel->setColor({ 255, 100, 100 });
        m_errorLabel->setZOrder(5);
        this->addChild(m_errorLabel);

        m_menu = CCMenu::create();
        m_menu->setPosition({ m_obContentSize.width - sidePad / 2, m_obContentSize.height / 2 }
        );
        this->addChild(m_menu);

        float btnPos = 15.f;

        if (setting->castDefinition().description) {
            auto infoSpr = CCSprite::createWithSpriteFrameName("GJ_infoIcon_001.png");
            infoSpr->setScale(.6f);

            auto infoBtn = CCMenuItemSpriteExtra::create(
                infoSpr, this, menu_selector(GeodeSettingNode::onDescription)
            );
            infoBtn->setPosition(
                -m_obContentSize.width + sidePad + m_nameLabel->getScaledContentSize().width +
                    btnPos,
                0.f
            );
            m_menu->addChild(infoBtn);

            btnPos += 20.f;
        }

        auto resetBtnSpr = CCSprite::createWithSpriteFrameName("reset-gold.png"_spr);
        resetBtnSpr->setScale(.5f);

        m_resetBtn = CCMenuItemSpriteExtra::create(
            resetBtnSpr, this, menu_selector(GeodeSettingNode::onReset)
        );
        m_resetBtn->setPosition(
            -m_obContentSize.width + sidePad + m_nameLabel->getScaledContentSize().width +
                btnPos,
            .0f
        );
        m_menu->addChild(m_resetBtn);

        CCDirector::sharedDirector()->getTouchDispatcher()->incrementForcePrio(2);
        m_menu->registerWithTouchDispatcher();

        if (!this->setup(setting, width)) return false;

        this->valueChanged();

        return true;
    }

    void onDescription(CCObject*) {
        FLAlertLayer::create(
            setting()->getDefinition().getDisplayName().c_str(),
            setting()->castDefinition().description.value(),
            "OK"
        )->show();
    }

    void onReset(CCObject*) {
        createQuickPopup(
            "Reset",
            "Are you sure you want to <cr>reset</c> <cl>" + 
                setting()->getDefinition().getDisplayName() +
                "</c> to <cy>default</c>?",
            "Cancel", "Reset",
            [this](auto, bool btn2) {
                if (btn2) {
                    this->resetToDefault();
                }
            }
        );
    }

    virtual float setupHeight(GeodeSettingValue<T>* setting) const {
        return 40.f;
    }

    virtual bool setup(GeodeSettingValue<T>* setting, float width) = 0;

    virtual void valueChanged(bool updateText = true) {
        if (this->hasUncommittedChanges()) {
            m_nameLabel->setColor(cc3x(0x1d0));
        }
        else {
            m_nameLabel->setColor(cc3x(0xfff));
        }
        if (m_resetBtn) m_resetBtn->setVisible(this->hasNonDefaultValue());
        auto isValid = setting()->validate(m_uncommittedValue);
        if (!isValid) {
            m_errorLabel->setVisible(true);
            m_errorLabel->setString(isValid.unwrapErr().c_str());
        }
        else {
            m_errorLabel->setVisible(false);
        }
        this->dispatchChanged();
    }

    GeodeSettingValue<T>* setting() {
        return static_cast<GeodeSettingValue<T>*>(m_value);
    }

public:
    void commit() override {
        setting()->setValue(m_uncommittedValue);
        m_uncommittedValue = setting()->getValue();
        this->valueChanged();
        this->dispatchCommitted();
    }

    bool hasUncommittedChanges() override {
        return m_uncommittedValue != setting()->getValue();
    }

    bool hasNonDefaultValue() override {
        return m_uncommittedValue != setting()->castDefinition().defaultValue;
    }

    void resetToDefault() override {
        m_uncommittedValue = setting()->castDefinition().defaultValue;
        this->valueChanged();
    }
};

class BoolSettingNode : public GeodeSettingNode<BoolSetting> {
protected:
    CCMenuItemToggler* m_toggle;

    void onToggle(CCObject*);
    void valueChanged(bool updateText) override;
    bool setup(BoolSettingValue* setting, float width) override;

public:
    IMPL_SETT_CREATE(Bool);
};

class IntSettingNode :
    public GeodeSettingNode<IntSetting>,
    public TextInputDelegate
{
protected:
    InputNode* m_input = nullptr;
    CCLabelBMFont* m_label = nullptr;
    Slider* m_slider = nullptr;
    CCMenuItemSpriteExtra* m_decArrow = nullptr;
    CCMenuItemSpriteExtra* m_incArrow = nullptr;
    CCMenuItemSpriteExtra* m_bigDecArrow = nullptr;
    CCMenuItemSpriteExtra* m_bigIncArrow = nullptr;

    void valueChanged(bool updateText) override;
    void textChanged(CCTextInputNode* input) override;

    float setupHeight(IntSettingValue* setting) const override;
    bool setup(IntSettingValue* setting, float width) override;

public:
    void updateSlider();
    void updateLabel();

    void onSlider(CCObject* slider);
    void onArrow(CCObject* sender);

    IMPL_SETT_CREATE(Int);
};

class FloatSettingNode :
    public GeodeSettingNode<FloatSetting>,
    public TextInputDelegate
{
protected:
    InputNode* m_input = nullptr;
    CCLabelBMFont* m_label = nullptr;
    Slider* m_slider = nullptr;
    CCMenuItemSpriteExtra* m_decArrow = nullptr;
    CCMenuItemSpriteExtra* m_incArrow = nullptr;
    CCMenuItemSpriteExtra* m_bigDecArrow = nullptr;
    CCMenuItemSpriteExtra* m_bigIncArrow = nullptr;

    void valueChanged(bool updateText) override;
    void textChanged(CCTextInputNode* input) override;

    float setupHeight(FloatSettingValue* setting) const override;
    bool setup(FloatSettingValue* setting, float width) override;

public:
    void updateSlider();
    void updateLabel();
    
    void onSlider(CCObject* slider);
    void onArrow(CCObject* sender);

    IMPL_SETT_CREATE(Float);
};

class StringSettingNode :
    public GeodeSettingNode<StringSetting>,
    public TextInputDelegate
{
protected:
    InputNode* m_input;

    void textChanged(CCTextInputNode* input) override;
    void valueChanged(bool updateText) override;
    void updateLabel();

    bool setup(StringSettingValue* setting, float width) override;

public:
    IMPL_SETT_CREATE(String);
};

class FileSettingNode :
    public GeodeSettingNode<FileSetting>,
    public TextInputDelegate
{
protected:
    InputNode* m_input;

    void textChanged(CCTextInputNode* input) override;
    void valueChanged(bool updateText) override;
    void updateLabel();

    void onPickFile(CCObject*);

    bool setup(FileSettingValue* setting, float width) override;

public:
    IMPL_SETT_CREATE(File);
};

class ColorSettingNode :
    public GeodeSettingNode<ColorSetting>,
    public ColorPickPopupDelegate {
protected:
    ColorChannelSprite* m_colorSpr;

    void valueChanged(bool updateText) override;
    void updateColor(ccColor4B const& color) override;

    void onSelectColor(CCObject*);

    bool setup(ColorSettingValue* setting, float width) override;

public:
    IMPL_SETT_CREATE(Color);
};

class ColorAlphaSettingNode :
    public GeodeSettingNode<ColorAlphaSetting>,
    public ColorPickPopupDelegate {
protected:
    ColorChannelSprite* m_colorSpr;

    void valueChanged(bool updateText) override;
    void updateColor(ccColor4B const& color) override;

    void onSelectColor(CCObject*);

    bool setup(ColorAlphaSettingValue* setting, float width) override;

public:
    IMPL_SETT_CREATE(ColorAlpha);
};

class CustomSettingPlaceholderNode : public SettingNode {
protected:
    void commit() override;
    bool hasUncommittedChanges() override;
    bool hasNonDefaultValue() override;
    void resetToDefault() override;

    bool init(std::string const& key, float width);

public:
    static CustomSettingPlaceholderNode* create(
        std::string const& key, float width
    );
};
