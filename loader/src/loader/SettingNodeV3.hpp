#pragma once

#include <Geode/loader/SettingV3.hpp>
#include <Geode/loader/SettingNode.hpp>
#include <Geode/binding/CCMenuItemToggler.hpp>
#include <Geode/binding/ColorChannelSprite.hpp>
#include <Geode/binding/Slider.hpp>
#include <Geode/ui/ColorPickPopup.hpp>

using namespace geode::prelude;

// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
// !! If these classes are ever exposed in a public header, make sure to pimpl EVERYTHING! !!
// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

class TitleSettingNodeV3 : public SettingNodeV3 {
protected:
    CCMenuItemToggler* m_collapseToggle;

    bool init(std::shared_ptr<TitleSettingV3> setting, float width);

    void onCommit() override;
    void onCollapse(CCObject*);

public:
    static TitleSettingNodeV3* create(std::shared_ptr<TitleSettingV3> setting, float width);

    bool isCollapsed() const;
    
    bool hasUncommittedChanges() const override;
    bool hasNonDefaultValue() const override;
    void onResetToDefault() override;

    std::shared_ptr<TitleSettingV3> getSetting() const;
};

class BoolSettingNodeV3 : public SettingValueNodeV3<BoolSettingV3> {
protected:
    CCMenuItemToggler* m_toggle;

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

    TextInput* m_input;
    Slider* m_slider;
    CCMenuItemSpriteExtra* m_arrowLeftBtn;
    CCMenuItemSpriteExtra* m_bigArrowLeftBtn;
    CCMenuItemSpriteExtra* m_arrowRightBtn;
    CCMenuItemSpriteExtra* m_bigArrowRightBtn;
    CCSprite* m_arrowLeftBtnSpr;
    CCSprite* m_bigArrowLeftBtnSpr;
    CCSprite* m_arrowRightBtnSpr;
    CCSprite* m_bigArrowRightBtnSpr;

    float valueToSlider(ValueType value) {
        auto min = this->getSetting()->getMinValue().value_or(-100);
        auto max = this->getSetting()->getMaxValue().value_or(+100);
        auto range = max - min;
        return static_cast<float>(clamp(static_cast<double>(value - min) / range, 0.0, 1.0));
    }
    ValueType valueFromSlider(float num) {
        auto min = this->getSetting()->getMinValue().value_or(-100);
        auto max = this->getSetting()->getMaxValue().value_or(+100);
        auto range = max - min;
        auto value = static_cast<ValueType>(num * range + min);
        auto step = this->getSetting()->getSliderSnap();
        if (step > 0) {
            value = static_cast<ValueType>(round(value / step) * step);
        }
        return value;
    }

    bool init(std::shared_ptr<S> setting, float width) {
        if (!SettingValueNodeV3<S>::init(setting, width))
            return false;

        m_bigArrowLeftBtnSpr = CCSprite::create();
        m_bigArrowLeftBtnSpr->setCascadeColorEnabled(true);
        m_bigArrowLeftBtnSpr->setCascadeOpacityEnabled(true);
        
        auto bigArrowLeftSpr1 = CCSprite::createWithSpriteFrameName("GJ_arrow_03_001.png");
        auto bigArrowLeftSpr2 = CCSprite::createWithSpriteFrameName("GJ_arrow_03_001.png");
        m_bigArrowLeftBtnSpr->setContentSize(bigArrowLeftSpr1->getContentSize() + ccp(20, 0));
        m_bigArrowLeftBtnSpr->addChildAtPosition(bigArrowLeftSpr2, Anchor::Center, ccp(10, 0));
        m_bigArrowLeftBtnSpr->addChildAtPosition(bigArrowLeftSpr1, Anchor::Center, ccp(-10, 0));
        m_bigArrowLeftBtnSpr->setScale(.3f);

        m_bigArrowLeftBtn = CCMenuItemSpriteExtra::create(
            m_bigArrowLeftBtnSpr, this, menu_selector(NumberSettingNodeV3::onArrow)
        );
        m_bigArrowLeftBtn->setUserObject(ObjWrapper<ValueType>::create(-setting->getBigArrowStepSize()));
        m_bigArrowLeftBtn->setVisible(setting->isBigArrowsEnabled());
        this->getButtonMenu()->addChildAtPosition(m_bigArrowLeftBtn, Anchor::Left, ccp(5, 0));

        m_arrowLeftBtnSpr = CCSprite::createWithSpriteFrameName("GJ_arrow_01_001.png");
        m_arrowLeftBtnSpr->setScale(.5f);
        m_arrowLeftBtn = CCMenuItemSpriteExtra::create(
            m_arrowLeftBtnSpr, this, menu_selector(NumberSettingNodeV3::onArrow)
        );
        m_arrowLeftBtn->setUserObject(ObjWrapper<ValueType>::create(-setting->getArrowStepSize()));
        m_arrowLeftBtn->setVisible(setting->isArrowsEnabled());
        this->getButtonMenu()->addChildAtPosition(m_arrowLeftBtn, Anchor::Left, ccp(22, 0));
        
        m_input = TextInput::create(this->getButtonMenu()->getContentWidth() - 40, "Num");
        m_input->setScale(.7f);
        m_input->setCallback([this, setting](auto const& str) {
            this->setValue(numFromString<ValueType>(str).unwrapOr(setting->getDefaultValue()), m_input);
        });
        if (!setting->isInputEnabled()) {
            m_input->getBGSprite()->setVisible(false);
            m_input->setEnabled(false);
            m_input->getInputNode()->m_placeholderLabel->setOpacity(255);
            m_input->getInputNode()->m_placeholderLabel->setColor(ccWHITE);
        }
        this->getButtonMenu()->addChildAtPosition(m_input, Anchor::Center);

        m_arrowRightBtnSpr = CCSprite::createWithSpriteFrameName("GJ_arrow_01_001.png");
        m_arrowRightBtnSpr->setFlipX(true);
        m_arrowRightBtnSpr->setScale(.5f);
        m_arrowRightBtn = CCMenuItemSpriteExtra::create(
            m_arrowRightBtnSpr, this, menu_selector(NumberSettingNodeV3::onArrow)
        );
        m_arrowRightBtn->setUserObject(ObjWrapper<ValueType>::create(setting->getArrowStepSize()));
        m_arrowRightBtn->setVisible(setting->isArrowsEnabled());
        this->getButtonMenu()->addChildAtPosition(m_arrowRightBtn, Anchor::Right, ccp(-22, 0));

        m_bigArrowRightBtnSpr = CCSprite::create();
        m_bigArrowRightBtnSpr->setCascadeColorEnabled(true);
        m_bigArrowRightBtnSpr->setCascadeOpacityEnabled(true);
        auto bigArrowRightSpr1 = CCSprite::createWithSpriteFrameName("GJ_arrow_03_001.png");
        bigArrowRightSpr1->setFlipX(true);
        auto bigArrowRightSpr2 = CCSprite::createWithSpriteFrameName("GJ_arrow_03_001.png");
        bigArrowRightSpr2->setFlipX(true);
        
        m_bigArrowRightBtnSpr->setContentSize(bigArrowRightSpr1->getContentSize() + ccp(20, 0));
        m_bigArrowRightBtnSpr->addChildAtPosition(bigArrowRightSpr1, Anchor::Center, ccp(-10, 0));
        m_bigArrowRightBtnSpr->addChildAtPosition(bigArrowRightSpr2, Anchor::Center, ccp(10, 0));
        m_bigArrowRightBtnSpr->setScale(.3f);

        m_bigArrowRightBtn = CCMenuItemSpriteExtra::create(
            m_bigArrowRightBtnSpr, this, menu_selector(NumberSettingNodeV3::onArrow)
        );
        m_bigArrowRightBtn->setUserObject(ObjWrapper<ValueType>::create(setting->getBigArrowStepSize()));
        m_bigArrowRightBtn->setVisible(setting->isBigArrowsEnabled());
        this->getButtonMenu()->addChildAtPosition(m_bigArrowRightBtn, Anchor::Right, ccp(-5, 0));

        if (setting->isSliderEnabled()) {
            this->setContentHeight(45);
            this->getButtonMenu()->updateAnchoredPosition(Anchor::Right, ccp(-10, 7));

            m_slider = Slider::create(this, menu_selector(NumberSettingNodeV3::onSlider));
            m_slider->setScale(.5f);
            this->getButtonMenu()->addChildAtPosition(m_slider, Anchor::Center, ccp(0, -20), ccp(0, 0));
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

        if (invoker != m_input) {
            m_input->setString(numToString(this->getValue()));
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
            m_slider->m_touchLogic->m_thumb->setValue(this->valueToSlider(this->getValue()));
            m_slider->updateBar();
            m_slider->m_sliderBar->setColor(enable ? ccWHITE : ccGRAY);
            m_slider->m_touchLogic->m_thumb->setColor(enable ? ccWHITE : ccGRAY);
            m_slider->m_touchLogic->m_thumb->setEnabled(enable);
        }
    }

    void onArrow(CCObject* sender) {
        auto value = this->getValue() + static_cast<ObjWrapper<ValueType>*>(
            static_cast<CCNode*>(sender)->getUserObject()
        )->getValue();
        if (auto min = this->getSetting()->getMinValue()) {
            value = std::max(*min, value);
        }
        if (auto max = this->getSetting()->getMaxValue()) {
            value = std::min(*max, value);
        }
        this->setValue(value, static_cast<CCNode*>(sender));
    }
    void onSlider(CCObject*) {
        this->setValue(this->valueFromSlider(m_slider->m_touchLogic->m_thumb->getValue()), m_slider);
    }

public:
    static NumberSettingNodeV3* create(std::shared_ptr<S> setting, float width) {
        auto ret = new NumberSettingNodeV3();
        if (ret && ret->init(setting, width)) {
            ret->autorelease();
            return ret;
        }
        CC_SAFE_DELETE(ret);
        return nullptr;
    }
};

using IntSettingNodeV3 = NumberSettingNodeV3<IntSettingV3>;
using FloatSettingNodeV3 = NumberSettingNodeV3<FloatSettingV3>;

class StringSettingNodeV3 : public SettingValueNodeV3<StringSettingV3> {
protected:
    TextInput* m_input;
    CCSprite* m_arrowLeftSpr = nullptr;
    CCSprite* m_arrowRightSpr = nullptr;

    bool init(std::shared_ptr<StringSettingV3> setting, float width);
    void updateState(CCNode* invoker) override;
    void onArrow(CCObject* sender);

public:
    static StringSettingNodeV3* create(std::shared_ptr<StringSettingV3> setting, float width);
};

class FileSettingNodeV3 : public SettingValueNodeV3<FileSettingV3> {
protected:
    CCSprite* m_fileIcon;
    CCLabelBMFont* m_nameLabel;
    EventListener<Task<Result<std::filesystem::path>>> m_pickListener;
    CCMenuItemSpriteExtra* m_selectBtn;
    CCSprite* m_selectBtnSpr;

    bool init(std::shared_ptr<FileSettingV3> setting, float width);
    void updateState(CCNode* invoker) override;
    void onPickFile(CCObject*);

public:
    static FileSettingNodeV3* create(std::shared_ptr<FileSettingV3> setting, float width);
};

class Color3BSettingNodeV3 : public SettingValueNodeV3<Color3BSettingV3>, public ColorPickPopupDelegate {
protected:
    CCMenuItemSpriteExtra* m_colorBtn;
    ColorChannelSprite* m_colorSprite;

    bool init(std::shared_ptr<Color3BSettingV3> setting, float width);
    void updateState(CCNode* invoker) override;
    void onSelectColor(CCObject*);
    void updateColor(ccColor4B const& color) override;

public:
    static Color3BSettingNodeV3* create(std::shared_ptr<Color3BSettingV3> setting, float width);
};

class Color4BSettingNodeV3 : public SettingValueNodeV3<Color4BSettingV3>, public ColorPickPopupDelegate {
protected:
    CCMenuItemSpriteExtra* m_colorBtn;
    ColorChannelSprite* m_colorSprite;

    bool init(std::shared_ptr<Color4BSettingV3> setting, float width);
    void updateState(CCNode* invoker) override;
    void onSelectColor(CCObject*);
    void updateColor(ccColor4B const& color) override;

public:
    static Color4BSettingNodeV3* create(std::shared_ptr<Color4BSettingV3> setting, float width);
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

// If these classes do get exposed in headers, 
// LegacyCustomSettingToV3Node SHOULD NOT BE EXPOSED!!!!!! DO NOT DO THAT!!!!

class LegacyCustomSettingToV3Node : public SettingNodeV3, public SettingNodeDelegate {
protected:
    SettingNode* m_original;

    bool init(std::shared_ptr<LegacyCustomSettingV3> original, float width);

    void onCommit() override;

    void settingValueChanged(SettingNode*) override;
    void settingValueCommitted(SettingNode*) override;

public:
    static LegacyCustomSettingToV3Node* create(std::shared_ptr<LegacyCustomSettingV3> original, float width);

    bool hasUncommittedChanges() const override;
    bool hasNonDefaultValue() const override;
    void onResetToDefault() override;
};
