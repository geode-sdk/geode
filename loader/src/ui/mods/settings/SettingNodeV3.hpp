#pragma once

#include <Geode/loader/Setting.hpp>
#include <Geode/binding/ColorChannelSprite.hpp>
#include <Geode/ui/SliderNode.hpp>
#include <Geode/ui/ColorPickPopup.hpp>
#include <Geode/ui/ScrollLayer.hpp>
#include <Geode/ui/Toggler.hpp>
#include <Geode/ui/Button.hpp>
#include <Geode/ui/Label.hpp>
#include <ui/GeodeStyle.hpp>

using namespace geode::prelude;

// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
// !! If these classes are ever exposed in a public header, make sure to pimpl EVERYTHING! !!
// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

class TitleSettingNodeV3 : public SettingNodeV3 {
protected:
    Toggler* m_collapseToggle = nullptr;

    bool init(std::shared_ptr<TitleSettingV3> setting, float width);

    void onCommit() override;
    void onCollapse(CCObject*);

public:
    // `setting` may be null here
    static TitleSettingNodeV3* create(std::shared_ptr<TitleSettingV3> setting, float width);
    static TitleSettingNodeV3* create(std::string title, float width);

    bool isCollapsed() const;
    void setCollapsed(bool collapsed);

    bool hasUncommittedChanges() const override;
    bool hasNonDefaultValue() const override;
    void onResetToDefault() override;

    std::shared_ptr<TitleSettingV3> getSetting() const;
};

class InfoSettingNodeV3 : public SettingNodeV3 {
protected:
    bool init(std::shared_ptr<InfoSettingV3> setting, float width);

    void onCommit() override;

public:
    static InfoSettingNodeV3* create(std::shared_ptr<InfoSettingV3> setting, float width);

    bool hasUncommittedChanges() const override;
    bool hasNonDefaultValue() const override;
    void onResetToDefault() override;
};

class ButtonSettingNodeV3 : public SettingNodeV3 {
protected:
    std::vector<Ref<Button>> buttons;

    bool init(std::shared_ptr<ButtonSettingV3> setting, float width);

    void onCommit() override;
    void updateState(CCNode* invoker) override;
    void enableButtons(bool enabled);

public:
    static ButtonSettingNodeV3* create(std::shared_ptr<ButtonSettingV3> setting, float width);

    bool hasUncommittedChanges() const override;
    bool hasNonDefaultValue() const override;
    void onResetToDefault() override;
};

class BoolSettingNodeV3 : public SettingValueNodeV3<BoolSettingV3> {
protected:
    Toggler* m_toggle = nullptr;

    bool init(std::shared_ptr<BoolSettingV3> setting, float width);
    void updateState(CCNode* invoker) override;
    void onToggle(CCObject*);

public:
    static BoolSettingNodeV3* create(std::shared_ptr<BoolSettingV3> setting, float width);
};

template <class S>
class NumberSettingNodeV3 : public SettingValueNodeV3<S> {
protected:
    using ValueType = typename S::ValueType;
    using ValueAssignType = typename S::ValueAssignType;

    TextInput* m_input = nullptr;
    SliderNode* m_slider = nullptr;
    Button* m_arrowLeftBtn = nullptr;
    Button* m_bigArrowLeftBtn = nullptr;
    Button* m_arrowRightBtn = nullptr;
    Button* m_bigArrowRightBtn = nullptr;

    bool init(std::shared_ptr<S> setting, float width) {
        if (!SettingValueNodeV3<S>::init(setting, width)) return false;

        auto bigArrowLeftBtnSpr = CCSprite::create();
        bigArrowLeftBtnSpr->setCascadeOpacityEnabled(true);

        auto bigArrowLeftSpr1 = CCSprite::createWithSpriteFrameName("GJ_arrow_03_001.png");
        auto bigArrowLeftSpr2 = CCSprite::createWithSpriteFrameName("GJ_arrow_03_001.png");

        bigArrowLeftBtnSpr->setContentSize(bigArrowLeftSpr1->getContentSize() + CCSize{20.0f, 0.0f});
        bigArrowLeftBtnSpr->addChildAtPosition(bigArrowLeftSpr2, Anchor::Center, {10.0f, 0.0f});
        bigArrowLeftBtnSpr->addChildAtPosition(bigArrowLeftSpr1, Anchor::Center, {-10.0f, 0.0f});

        m_bigArrowLeftBtn = Button::createWithNode(
            bigArrowLeftBtnSpr,
            [this](Button* sender) {
                this->onArrow(sender);
            }
        );
        m_bigArrowLeftBtn->setScale(.3f);
        m_bigArrowLeftBtn->setUserObject(ObjWrapper<ValueType>::create(-setting->getBigArrowStepSize()));
        m_bigArrowLeftBtn->setVisible(setting->isBigArrowsEnabled());
        this->getButtonMenu()->addChildAtPosition(m_bigArrowLeftBtn, Anchor::Left, {5.0f, 0.0f});

        m_arrowLeftBtn = Button::createWithSpriteFrameName(
            "GJ_arrow_01_001.png",
            [this](Button* sender) {
                this->onArrow(sender);
            }
        );
        m_arrowLeftBtn->setScale(.5f);
        m_arrowLeftBtn->setUserObject(ObjWrapper<ValueType>::create(-setting->getArrowStepSize()));
        m_arrowLeftBtn->setVisible(setting->isArrowsEnabled());
        this->getButtonMenu()->addChildAtPosition(m_arrowLeftBtn, Anchor::Left, {22.0f, 0.0f});

        m_input = TextInput::create(this->getButtonMenu()->getContentWidth() - 40.0f, "Num");
        m_input->setNumericMode(true);
        m_input->setScale(.7f);
        m_input->setCommonFilter(std::is_floating_point_v<typename S::ValueType> ? CommonFilter::Float : CommonFilter::Int);
        m_input->setCallback([this, setting](auto const& str) {
            this->setValue(numFromString<ValueType>(str).unwrapOr(setting->getDefaultValue()), m_input);
        });
        if (!setting->isInputEnabled()) {
            m_input->getBGSprite()->setVisible(false);
            m_input->setEnabled(false);
            m_input->setTextColor({255, 255, 255, 255});
        }
        this->getButtonMenu()->addChildAtPosition(m_input, Anchor::Center);

        m_arrowRightBtn = Button::createWithSpriteFrameName(
            "GJ_arrow_01_001.png",
            [this](Button* sender) {
                this->onArrow(sender);
            }
        );
        m_arrowRightBtn->setScale(.5f);
        m_arrowRightBtn->setUserObject(ObjWrapper<ValueType>::create(setting->getArrowStepSize()));
        m_arrowRightBtn->setVisible(setting->isArrowsEnabled());
        static_cast<CCSprite*>(m_arrowRightBtn->getDisplayNode())->setFlipX(true);
        this->getButtonMenu()->addChildAtPosition(m_arrowRightBtn, Anchor::Right, {-22.0f, 0.0f});

        auto bigArrowRightBtnSpr = CCSprite::create();
        bigArrowRightBtnSpr->setCascadeOpacityEnabled(true);
        auto bigArrowRightSpr1 = CCSprite::createWithSpriteFrameName("GJ_arrow_03_001.png");
        bigArrowRightSpr1->setFlipX(true);
        auto bigArrowRightSpr2 = CCSprite::createWithSpriteFrameName("GJ_arrow_03_001.png");
        bigArrowRightSpr2->setFlipX(true);

        bigArrowRightBtnSpr->setContentSize(bigArrowRightSpr1->getContentSize() + CCSize{20.0f, 0.0f});
        bigArrowRightBtnSpr->addChildAtPosition(bigArrowRightSpr1, Anchor::Center, {-10.0f, 0.0f});
        bigArrowRightBtnSpr->addChildAtPosition(bigArrowRightSpr2, Anchor::Center, {10.0f, 0.0f});

        m_bigArrowRightBtn = Button::createWithNode(
            bigArrowRightBtnSpr,
            [this](Button* sender) {
                this->onArrow(sender);
            }
        );
        m_bigArrowRightBtn->setScale(.3f);
        m_bigArrowRightBtn->setUserObject(ObjWrapper<ValueType>::create(setting->getBigArrowStepSize()));
        m_bigArrowRightBtn->setVisible(setting->isBigArrowsEnabled());
        this->getButtonMenu()->addChildAtPosition(m_bigArrowRightBtn, Anchor::Right, {-5.0f, 0.0f});

        if (setting->isSliderEnabled()) {
            this->setContentHeight(45);
            this->getButtonMenu()->updateAnchoredPosition(Anchor::Right, {-10.0f, 7.0f});

            m_slider = SliderNode::create([this](auto, float v) {
                ValueType value = v;

                if (value != this->getValue()) {
                    this->setValue(value, m_slider);
                }
            });
            m_slider->setScale(.5f);
            m_slider->setMin(setting->getMinValue().value_or(-100.0f));
            m_slider->setMax(setting->getMaxValue().value_or(+100.0f));
            m_slider->setSnapStep(setting->getSliderSnap());

            unsigned int precision = 0; // only way i thought of removing the decimals when linking to slidernode
            if constexpr (std::is_same_v<FloatSettingV3, S>) precision = 3;

            m_slider->linkTextInput(m_input, precision);

            m_slider->setValue(setting->getValue());

            this->getButtonMenu()->addChildAtPosition(m_slider, Anchor::Center, {0.0f, -20.0f});
        }

        this->setValue(setting->getValue(), nullptr);
        this->updateState(nullptr);

        return true;
    }

    void updateState(CCNode* invoker) override {
        SettingValueNodeV3<S>::updateState(invoker);
        auto enable = this->getSetting()->shouldEnable();
        if (this->getSetting()->isInputEnabled()) {
            m_input->setEnabled(enable);
        }

        if (invoker != m_input && (invoker != nullptr || !m_input->isFocused())) {
            // round to 5 decimal places to avoid floating point errors
            m_input->setString(numToString(round(this->getValue() * 100000.0) / 100000.0));
        }

        auto min = this->getSetting()->getMinValue();
        auto enableLeft = enable && (!min || this->getValue() > *min);

        m_arrowLeftBtn->setEnabled(enableLeft);
        m_bigArrowLeftBtn->setEnabled(enableLeft);

        m_arrowLeftBtn->setOpacity(enableLeft ? 255 : 155);
        m_arrowLeftBtn->setColor(enableLeft ? ccWHITE : ccGRAY);
        m_bigArrowLeftBtn->setOpacity(enableLeft ? 255 : 155);
        m_bigArrowLeftBtn->setColor(enableLeft ? ccWHITE : ccGRAY);

        auto max = this->getSetting()->getMaxValue();
        auto enableRight = enable && (!max || this->getValue() < *max);

        m_arrowRightBtn->setEnabled(enableRight);
        m_bigArrowRightBtn->setEnabled(enableRight);

        m_arrowRightBtn->setOpacity(enableRight ? 255 : 155);
        m_arrowRightBtn->setColor(enableRight ? ccWHITE : ccGRAY);
        m_bigArrowRightBtn->setOpacity(enableRight ? 255 : 155);
        m_bigArrowRightBtn->setColor(enableRight ? ccWHITE : ccGRAY);

        if (m_slider) {
            m_slider->setValue(this->getValue());
            m_slider->getBar()->setColor(enable ? ccWHITE : ccGRAY);
            m_slider->getThumb()->setColor(enable ? ccWHITE : ccGRAY);
            m_slider->setEnabled(enable);
        }
    }

    void onArrow(CCNode* sender) {
        auto value = this->getValue() + static_cast<ObjWrapper<ValueType>*>(sender->getUserObject())->getValue();
        if (auto min = this->getSetting()->getMinValue()) {
            value = std::max(*min, value);
        }
        if (auto max = this->getSetting()->getMaxValue()) {
            value = std::min(*max, value);
        }
        this->setValue(value, sender);
    }

public:
    static NumberSettingNodeV3* create(std::shared_ptr<S> setting, float width) {
        auto ret = new NumberSettingNodeV3();
        if (ret->init(setting, width)) {
            ret->autorelease();
            return ret;
        }
        delete ret;
        return nullptr;
    }
};

using IntSettingNodeV3 = NumberSettingNodeV3<IntSettingV3>;
using FloatSettingNodeV3 = NumberSettingNodeV3<FloatSettingV3>;

class StringSettingNodeV3 : public SettingValueNodeV3<StringSettingV3> {
protected:
    TextInput* m_input = nullptr;
    Button* m_arrowLeftBtn = nullptr;
    Button* m_arrowRightBtn = nullptr;

    bool init(std::shared_ptr<StringSettingV3> setting, float width);
    void updateState(CCNode* invoker) override;
    void onArrow(CCNode* sender, int by);

public:
    static StringSettingNodeV3* create(std::shared_ptr<StringSettingV3> setting, float width);
};

class FileSettingNodeV3 : public SettingValueNodeV3<FileSettingV3> {
protected:
    CCSprite* m_fileIcon = nullptr;
    Label* m_nameLabel = nullptr;
    ListenerHandle m_pickHandle;
    async::TaskHolder<Result<std::optional<asp::fs::Path>>> m_pickListener;
    Button* m_selectBtn = nullptr;

    bool init(std::shared_ptr<FileSettingV3> setting, float width);
    void updateState(CCNode* invoker) override;
    void onPickFile(CCObject*);

public:
    static FileSettingNodeV3* create(std::shared_ptr<FileSettingV3> setting, float width);
};

class Color3BSettingNodeV3 : public SettingValueNodeV3<Color3BSettingV3> {
protected:
    Button* m_colorBtn = nullptr;

    bool init(std::shared_ptr<Color3BSettingV3> setting, float width);
    void updateState(CCNode* invoker) override;
    void onSelectColor(CCObject*);

public:
    static Color3BSettingNodeV3* create(std::shared_ptr<Color3BSettingV3> setting, float width);
};

class Color4BSettingNodeV3 : public SettingValueNodeV3<Color4BSettingV3> {
protected:
    Button* m_colorBtn = nullptr;

    bool init(std::shared_ptr<Color4BSettingV3> setting, float width);
    void updateState(CCNode* invoker) override;
    void onSelectColor(CCObject*);

public:
    static Color4BSettingNodeV3* create(std::shared_ptr<Color4BSettingV3> setting, float width);
};

class KeybindSettingNodeV3 : public SettingNodeV3 {
protected:
    std::vector<Keybind> m_currentValue;

    bool init(std::shared_ptr<KeybindSettingV3> setting, float width);
    void updateState(CCNode* invoker) override;
    void onExtra();
    void onKeybind(int index = -1);
    void onCommit() override;
    bool hasUncommittedChanges() const override;
    bool hasNonDefaultValue() const override;
    void onResetToDefault() override;
public:
    static KeybindSettingNodeV3* create(std::shared_ptr<KeybindSettingV3> setting, float width);

    std::shared_ptr<KeybindSettingV3> getSetting() const;
};

class UnresolvedCustomSettingNodeV3 : public SettingNodeV3 {
protected:
    Mod* m_mod;

    bool init(std::string_view key, Mod* mod, float width);

    void updateState(CCNode* invoker) override;

    void onCommit() override;

public:
    static UnresolvedCustomSettingNodeV3* create(std::string_view key, Mod* mod, float width);

    bool hasUncommittedChanges() const override;
    bool hasNonDefaultValue() const override;
    void onResetToDefault() override;
};
