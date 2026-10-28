#pragma once

#include <Geode/loader/Setting.hpp>
#include <Geode/binding/CCMenuItemToggler.hpp>
#include <Geode/binding/ColorChannelSprite.hpp>
#include <Geode/ui/SliderNode.hpp>
#include <Geode/ui/ColorPickPopup.hpp>
#include <Geode/ui/ScrollLayer.hpp>
#include <Geode/ui/Button.hpp>
#include <Geode/ui/Label.hpp>
#include <ui/mods/GeodeStyle.hpp>

using namespace geode::prelude;

// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
// !! If these classes are ever exposed in a public header, make sure to pimpl EVERYTHING! !!
// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

class TitleSettingNodeV3 : public SettingNodeV3 {
protected:
    CCMenuItemToggler* m_collapseToggle = nullptr;

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
    CCMenuItemToggler* m_toggle = nullptr;

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
    CCSprite* m_arrowLeftBtnSpr = nullptr;
    CCSprite* m_bigArrowLeftBtnSpr = nullptr;
    CCSprite* m_arrowRightBtnSpr = nullptr;
    CCSprite* m_bigArrowRightBtnSpr = nullptr;

    bool init(std::shared_ptr<S> setting, float width) {
        if (!SettingValueNodeV3<S>::init(setting, width)) return false;

        m_bigArrowLeftBtnSpr = CCSprite::create();
        m_bigArrowLeftBtnSpr->setCascadeColorEnabled(true);
        m_bigArrowLeftBtnSpr->setCascadeOpacityEnabled(true);

        auto bigArrowLeftSpr1 = CCSprite::createWithSpriteFrameName("GJ_arrow_03_001.png");
        auto bigArrowLeftSpr2 = CCSprite::createWithSpriteFrameName("GJ_arrow_03_001.png");
        m_bigArrowLeftBtnSpr->setContentSize(bigArrowLeftSpr1->getContentSize() + CCSize{20.f, .0f});
        m_bigArrowLeftBtnSpr->addChildAtPosition(bigArrowLeftSpr2, Anchor::Center, {10.f, .0f});
        m_bigArrowLeftBtnSpr->addChildAtPosition(bigArrowLeftSpr1, Anchor::Center, {-10.f, .0f});
        m_bigArrowLeftBtnSpr->setScale(.3f);

        m_bigArrowLeftBtn = Button::createWithNode(
            m_bigArrowLeftBtnSpr,
            [this](Button* sender) {
                this->onArrow(sender);
            }
        );
        m_bigArrowLeftBtn->setUserObject(ObjWrapper<ValueType>::create(-setting->getBigArrowStepSize()));
        m_bigArrowLeftBtn->setVisible(setting->isBigArrowsEnabled());
        this->getButtonMenu()->addChildAtPosition(m_bigArrowLeftBtn, Anchor::Left, {5.f, .0f});

        m_arrowLeftBtnSpr = CCSprite::createWithSpriteFrameName("GJ_arrow_01_001.png");
        m_arrowLeftBtnSpr->setScale(.5f);
        m_arrowLeftBtn = Button::createWithNode(
            m_arrowLeftBtnSpr,
            [this](Button* sender) {
                this->onArrow(sender);
            }
        );
        m_arrowLeftBtn->setUserObject(ObjWrapper<ValueType>::create(-setting->getArrowStepSize()));
        m_arrowLeftBtn->setVisible(setting->isArrowsEnabled());
        this->getButtonMenu()->addChildAtPosition(m_arrowLeftBtn, Anchor::Left, {22.f, .0f});

        m_input = TextInput::create(this->getButtonMenu()->getContentWidth() - 40, "Num");
        m_input->setScale(.7f);
        m_input->setCommonFilter(std::is_floating_point_v<typename S::ValueType> ? CommonFilter::Float : CommonFilter::Int);
        m_input->setCallback([this, setting](auto const& str) {
            this->setValue(numFromString<ValueType>(str).unwrapOr(setting->getDefaultValue()), m_input);
        });
        if (!setting->isInputEnabled()) {
            m_input->getBGSprite()->setVisible(false);
            m_input->setEnabled(false);
            m_input->getInputNode()->m_textLabel->setOpacity(255);
            m_input->getInputNode()->m_textLabel->setColor(ccWHITE);
        }
        this->getButtonMenu()->addChildAtPosition(m_input, Anchor::Center);

        m_arrowRightBtnSpr = CCSprite::createWithSpriteFrameName("GJ_arrow_01_001.png");
        m_arrowRightBtnSpr->setFlipX(true);
        m_arrowRightBtnSpr->setScale(.5f);
        m_arrowRightBtn = Button::createWithNode(
            m_arrowRightBtnSpr,
            [this](Button* sender) {
                this->onArrow(sender);
            }
        );
        m_arrowRightBtn->setUserObject(ObjWrapper<ValueType>::create(setting->getArrowStepSize()));
        m_arrowRightBtn->setVisible(setting->isArrowsEnabled());
        this->getButtonMenu()->addChildAtPosition(m_arrowRightBtn, Anchor::Right, {-22.f, .0f});

        m_bigArrowRightBtnSpr = CCSprite::create();
        m_bigArrowRightBtnSpr->setCascadeColorEnabled(true);
        m_bigArrowRightBtnSpr->setCascadeOpacityEnabled(true);
        auto bigArrowRightSpr1 = CCSprite::createWithSpriteFrameName("GJ_arrow_03_001.png");
        bigArrowRightSpr1->setFlipX(true);
        auto bigArrowRightSpr2 = CCSprite::createWithSpriteFrameName("GJ_arrow_03_001.png");
        bigArrowRightSpr2->setFlipX(true);

        m_bigArrowRightBtnSpr->setContentSize(bigArrowRightSpr1->getContentSize() + CCSize{20.f, .0f});
        m_bigArrowRightBtnSpr->addChildAtPosition(bigArrowRightSpr1, Anchor::Center, {-10.f, .0f});
        m_bigArrowRightBtnSpr->addChildAtPosition(bigArrowRightSpr2, Anchor::Center, {10.f, .0f});
        m_bigArrowRightBtnSpr->setScale(.3f);

        m_bigArrowRightBtn = Button::createWithNode(
            m_bigArrowRightBtnSpr,
            [this](Button* sender) {
                this->onArrow(sender);
            }
        );
        m_bigArrowRightBtn->setUserObject(ObjWrapper<ValueType>::create(setting->getBigArrowStepSize()));
        m_bigArrowRightBtn->setVisible(setting->isBigArrowsEnabled());
        this->getButtonMenu()->addChildAtPosition(m_bigArrowRightBtn, Anchor::Right, {-5.f, .0f});

        if (setting->isSliderEnabled()) {
            this->setContentHeight(45);
            this->getButtonMenu()->updateAnchoredPosition(Anchor::Right, {-10.f, 7.f});

            m_slider = SliderNode::create([this](auto, float v) {
                ValueType value = v;

                if (value != this->getValue()) {
                    this->setValue(value, m_slider);
                }
            });
            m_slider->setScale(.5f);
            m_slider->setMin(setting->getMinValue().value_or(-100.f));
            m_slider->setMax(setting->getMaxValue().value_or(+100.f));
            m_slider->setSnapStep(setting->getSliderSnap());

            // TODO: somehow hide zeroes at the end
            m_slider->linkTextInput(m_input, 3);

            m_slider->setValue(setting->getValue());

            this->getButtonMenu()->addChildAtPosition(m_slider, Anchor::Center, {.0f, -20.f});
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

        if (invoker != m_input && (invoker != nullptr || !m_input->getInputNode()->m_selected)) {
            // round to 5 decimal places to avoid floating point errors
            m_input->setString(numToString(round(this->getValue() * 100000.0) / 100000.0));
        }

        auto min = this->getSetting()->getMinValue();
        auto enableLeft = enable && (!min || this->getValue() > *min);
        m_arrowLeftBtn->setEnabled(enableLeft);
        m_bigArrowLeftBtn->setEnabled(enableLeft);
        m_arrowLeftBtnSpr->setOpacity(enableLeft ? 255 : 155);
        m_arrowLeftBtnSpr->setColor(enableLeft ? ccWHITE : ccGRAY);
        m_bigArrowLeftBtnSpr->setOpacity(enableLeft ? 255 : 155);
        m_bigArrowLeftBtnSpr->setColor(enableLeft ? ccWHITE : ccGRAY);

        auto max = this->getSetting()->getMaxValue();
        auto enableRight = enable && (!max || this->getValue() < *max);
        m_arrowRightBtn->setEnabled(enableRight);
        m_bigArrowRightBtn->setEnabled(enableRight);
        m_arrowRightBtnSpr->setOpacity(enableRight ? 255 : 155);
        m_arrowRightBtnSpr->setColor(enableRight ? ccWHITE : ccGRAY);
        m_bigArrowRightBtnSpr->setOpacity(enableRight ? 255 : 155);
        m_bigArrowRightBtnSpr->setColor(enableRight ? ccWHITE : ccGRAY);

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
    CCSprite* m_arrowLeftSpr = nullptr;
    CCSprite* m_arrowRightSpr = nullptr;

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
    CCSprite* m_selectBtnSpr = nullptr;

    bool init(std::shared_ptr<FileSettingV3> setting, float width);
    void updateState(CCNode* invoker) override;
    void onPickFile(CCObject*);

public:
    static FileSettingNodeV3* create(std::shared_ptr<FileSettingV3> setting, float width);
};

class Color3BSettingNodeV3 : public SettingValueNodeV3<Color3BSettingV3> {
protected:
    Button* m_colorBtn = nullptr;
    ColorChannelSprite* m_colorSprite = nullptr;

    bool init(std::shared_ptr<Color3BSettingV3> setting, float width);
    void updateState(CCNode* invoker) override;
    void onSelectColor(CCObject*);

public:
    static Color3BSettingNodeV3* create(std::shared_ptr<Color3BSettingV3> setting, float width);
};

class Color4BSettingNodeV3 : public SettingValueNodeV3<Color4BSettingV3> {
protected:
    Button* m_colorBtn = nullptr;
    ColorChannelSprite* m_colorSprite = nullptr;

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
    void onExtra(CCObject*);
    void onKeybind(int index = 0);
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
