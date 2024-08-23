#pragma once

#include <Geode/loader/SettingV3.hpp>
#include <Geode/loader/SettingNode.hpp>
#include <Geode/binding/CCMenuItemToggler.hpp>

using namespace geode::prelude;

// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
// !! If these classes are ever exposed in a public header, make sure to pimpl EVERYTHING! !!
// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

class TitleSettingNodeV3 : public SettingNodeV3 {
protected:
    bool init(std::shared_ptr<TitleSettingV3> setting, float width);

    void onCommit() override;

public:
    static TitleSettingNodeV3* create(std::shared_ptr<TitleSettingV3> setting, float width);
    
    bool hasUncommittedChanges() const override;
    bool hasNonDefaultValue() const override;
    void onResetToDefault() override;

    std::shared_ptr<TitleSettingV3> getSetting() const;
};

class BoolSettingNodeV3 : public SettingNodeV3 {
protected:
    CCMenuItemToggler* m_toggle;

    bool init(std::shared_ptr<BoolSettingV3> setting, float width);

    void onCommit() override;
    void onToggle(CCObject*);

public:
    static BoolSettingNodeV3* create(std::shared_ptr<BoolSettingV3> setting, float width);
    
    bool hasUncommittedChanges() const override;
    bool hasNonDefaultValue() const override;
    void onResetToDefault() override;

    std::shared_ptr<BoolSettingV3> getSetting() const;
};

template <class S>
class NumberSettingNodeV3 : public SettingNodeV3 {
protected:
    using ValueType = typename S::ValueType;

    TextInput* m_input;
    Slider* m_slider;
    CCMenuItemSpriteExtra* m_arrowLeftBtn;
    CCMenuItemSpriteExtra* m_bigArrowLeftBtn;
    CCMenuItemSpriteExtra* m_arrowRightBtn;
    CCMenuItemSpriteExtra* m_bigArrowRightBtn;

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
        if (auto step = this->getSetting()->getSliderSnap()) {
            value = static_cast<ValueType>(round(value / *step) * (*step));
        }
        return value;
    }

    bool init(std::shared_ptr<S> setting, float width) {
        if (!SettingNodeV3::init(setting, width))
            return false;

        auto bigArrowLeftSpr = CCSprite::create();
        bigArrowLeftSpr->setCascadeColorEnabled(true);
        bigArrowLeftSpr->setCascadeOpacityEnabled(true);
        auto bigArrowLeftSpr1 = CCSprite::createWithSpriteFrameName("GJ_arrow_03_001.png");
        auto bigArrowLeftSpr2 = CCSprite::createWithSpriteFrameName("GJ_arrow_03_001.png");
        
        bigArrowLeftSpr->setContentSize(bigArrowLeftSpr1->getContentSize() + ccp(20, 0));
        bigArrowLeftSpr->addChildAtPosition(bigArrowLeftSpr2, Anchor::Center, ccp(10, 0));
        bigArrowLeftSpr->addChildAtPosition(bigArrowLeftSpr1, Anchor::Center, ccp(-10, 0));
        bigArrowLeftSpr->setScale(.3f);

        m_bigArrowLeftBtn = CCMenuItemSpriteExtra::create(
            bigArrowLeftSpr, this, menu_selector(NumberSettingNodeV3::onArrow)
        );
        m_bigArrowLeftBtn->setUserObject(ObjWrapper<ValueType>::create(-setting->getBigArrowStepSize()));
        m_bigArrowLeftBtn->setVisible(setting->isBigArrowsEnabled());
        this->getButtonMenu()->addChildAtPosition(m_bigArrowLeftBtn, Anchor::Left, ccp(5, 0));

        auto arrowLeftSpr = CCSprite::createWithSpriteFrameName("GJ_arrow_01_001.png");
        arrowLeftSpr->setScale(.5f);
        m_arrowLeftBtn = CCMenuItemSpriteExtra::create(
            arrowLeftSpr, this, menu_selector(NumberSettingNodeV3::onArrow)
        );
        m_arrowLeftBtn->setUserObject(ObjWrapper<ValueType>::create(-setting->getArrowStepSize()));
        m_arrowLeftBtn->setVisible(setting->isArrowsEnabled());
        this->getButtonMenu()->addChildAtPosition(m_arrowLeftBtn, Anchor::Left, ccp(22, 0));
        
        m_input = TextInput::create(this->getButtonMenu()->getContentWidth() - 40, "Num");
        m_input->setScale(.7f);
        m_input->setCallback([this](auto const&) {
            this->markChanged();
        });
        if (!setting->isInputEnabled()) {
            m_input->getBGSprite()->setVisible(false);
            m_input->setEnabled(false);
            m_input->getInputNode()->m_placeholderLabel->setOpacity(255);
            m_input->getInputNode()->m_placeholderLabel->setColor(ccWHITE);
        }
        this->getButtonMenu()->addChildAtPosition(m_input, Anchor::Center);

        auto arrowRightSpr = CCSprite::createWithSpriteFrameName("GJ_arrow_01_001.png");
        arrowRightSpr->setFlipX(true);
        arrowRightSpr->setScale(.5f);
        m_arrowRightBtn = CCMenuItemSpriteExtra::create(
            arrowRightSpr, this, menu_selector(NumberSettingNodeV3::onArrow)
        );
        m_arrowRightBtn->setUserObject(ObjWrapper<ValueType>::create(setting->getArrowStepSize()));
        m_arrowRightBtn->setVisible(setting->isArrowsEnabled());
        this->getButtonMenu()->addChildAtPosition(m_arrowRightBtn, Anchor::Right, ccp(-22, 0));

        auto bigArrowRightSpr = CCSprite::create();
        bigArrowRightSpr->setCascadeColorEnabled(true);
        bigArrowRightSpr->setCascadeOpacityEnabled(true);
        auto bigArrowRightSpr1 = CCSprite::createWithSpriteFrameName("GJ_arrow_03_001.png");
        bigArrowRightSpr1->setFlipX(true);
        auto bigArrowRightSpr2 = CCSprite::createWithSpriteFrameName("GJ_arrow_03_001.png");
        bigArrowRightSpr2->setFlipX(true);
        
        bigArrowRightSpr->setContentSize(bigArrowRightSpr1->getContentSize() + ccp(20, 0));
        bigArrowRightSpr->addChildAtPosition(bigArrowRightSpr1, Anchor::Center, ccp(-10, 0));
        bigArrowRightSpr->addChildAtPosition(bigArrowRightSpr2, Anchor::Center, ccp(10, 0));
        bigArrowRightSpr->setScale(.3f);

        m_bigArrowRightBtn = CCMenuItemSpriteExtra::create(
            bigArrowRightSpr, this, menu_selector(NumberSettingNodeV3::onArrow)
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

        this->setCurrentValue(setting->getValue());
        this->updateState();

        return true;
    }

    void updateState() override {
        SettingNodeV3::updateState();
        if (m_slider) {
            m_slider->m_touchLogic->m_thumb->setValue(this->valueToSlider(this->getCurrentValue()));
            m_slider->updateBar();
        }
        if (auto min = this->getSetting()->getMinValue()) {
            auto enable = this->getCurrentValue() > *min;
            m_arrowLeftBtn->setEnabled(enable);
            m_bigArrowLeftBtn->setEnabled(enable);
            static_cast<CCSprite*>(m_arrowLeftBtn->getNormalImage())->setOpacity(enable ? 255 : 155);
            static_cast<CCSprite*>(m_arrowLeftBtn->getNormalImage())->setColor(enable ? ccWHITE : ccGRAY);
            static_cast<CCSprite*>(m_bigArrowLeftBtn->getNormalImage())->setOpacity(enable ? 255 : 155);
            static_cast<CCSprite*>(m_bigArrowLeftBtn->getNormalImage())->setColor(enable ? ccWHITE : ccGRAY);
        }
        if (auto max = this->getSetting()->getMaxValue()) {
            auto enable = this->getCurrentValue() < *max;
            m_arrowRightBtn->setEnabled(enable);
            m_bigArrowRightBtn->setEnabled(enable);
            static_cast<CCSprite*>(m_arrowRightBtn->getNormalImage())->setOpacity(enable ? 255 : 155);
            static_cast<CCSprite*>(m_arrowRightBtn->getNormalImage())->setColor(enable ? ccWHITE : ccGRAY);
            static_cast<CCSprite*>(m_bigArrowRightBtn->getNormalImage())->setOpacity(enable ? 255 : 155);
            static_cast<CCSprite*>(m_bigArrowRightBtn->getNormalImage())->setColor(enable ? ccWHITE : ccGRAY);
        }
    }

    void onCommit() override {
        this->getSetting()->setValue(this->getCurrentValue());
    }
    void onArrow(CCObject* sender) {
        auto value = this->getCurrentValue() + static_cast<ObjWrapper<ValueType>*>(
            static_cast<CCNode*>(sender)->getUserObject()
        )->getValue();
        if (auto min = this->getSetting()->getMinValue()) {
            value = std::max(*min, value);
        }
        if (auto max = this->getSetting()->getMaxValue()) {
            value = std::min(*max, value);
        }
        this->setCurrentValue(value);
    }
    void onSlider(CCObject*) {
        this->setCurrentValue(this->valueFromSlider(m_slider->m_touchLogic->m_thumb->getValue()));
    }

    ValueType getCurrentValue() const {
        return numFromString<ValueType>(m_input->getString())
            .value_or(this->getSetting()->getDefaultValue());
    }
    void setCurrentValue(ValueType value) {
        m_input->setString(numToString(value));
        this->markChanged();
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
    
    bool hasUncommittedChanges() const override {
        return this->getSetting()->getValue() != this->getCurrentValue();
    }
    bool hasNonDefaultValue() const override {
        return this->getSetting()->getDefaultValue() != this->getCurrentValue();
    }
    void onResetToDefault() override {
        this->setCurrentValue(this->getSetting()->getDefaultValue());
    }

    std::shared_ptr<S> getSetting() const {
        return std::static_pointer_cast<S>(SettingNodeV3::getSetting());
    }
};

using IntSettingNodeV3 = NumberSettingNodeV3<IntSettingV3>;
using FloatSettingNodeV3 = NumberSettingNodeV3<FloatSettingV3>;

class StringSettingNodeV3 : public SettingNodeV3 {
protected:
    TextInput* m_input;

    bool init(std::shared_ptr<StringSettingV3> setting, float width);

    void onCommit() override;

    void onArrow(CCObject* sender);

public:
    static StringSettingNodeV3* create(std::shared_ptr<StringSettingV3> setting, float width);
    
    bool hasUncommittedChanges() const override;
    bool hasNonDefaultValue() const override;
    void onResetToDefault() override;

    std::shared_ptr<StringSettingV3> getSetting() const;
};

class FileSettingNodeV3 : public SettingNodeV3 {
protected:
    CCSprite* m_fileIcon;
    std::filesystem::path m_path;
    CCLabelBMFont* m_nameLabel;
    EventListener<Task<Result<std::filesystem::path>>> m_pickListener;

    bool init(std::shared_ptr<FileSettingV3> setting, float width);

    void updateState() override;

    void onCommit() override;
    void onPickFile(CCObject*);

public:
    static FileSettingNodeV3* create(std::shared_ptr<FileSettingV3> setting, float width);
    
    bool hasUncommittedChanges() const override;
    bool hasNonDefaultValue() const override;
    void onResetToDefault() override;

    std::shared_ptr<FileSettingV3> getSetting() const;
};

class Color3BSettingNodeV3 : public SettingNodeV3 {
protected:
    bool init(std::shared_ptr<Color3BSettingV3> setting, float width);

    void onCommit() override;

public:
    static Color3BSettingNodeV3* create(std::shared_ptr<Color3BSettingV3> setting, float width);
    
    bool hasUncommittedChanges() const override;
    bool hasNonDefaultValue() const override;
    void onResetToDefault() override;

    std::shared_ptr<Color3BSettingV3> getSetting() const;
};

class Color4BSettingNodeV3 : public SettingNodeV3 {
protected:
    bool init(std::shared_ptr<Color4BSettingV3> setting, float width);

    void onCommit() override;

public:
    static Color4BSettingNodeV3* create(std::shared_ptr<Color4BSettingV3> setting, float width);
    
    bool hasUncommittedChanges() const override;
    bool hasNonDefaultValue() const override;
    void onResetToDefault() override;

    std::shared_ptr<Color4BSettingV3> getSetting() const;
};

class UnresolvedCustomSettingNodeV3 : public SettingNodeV3 {
protected:
    bool init(std::shared_ptr<LegacyCustomSettingV3> setting, float width);

    void onCommit() override;

public:
    static UnresolvedCustomSettingNodeV3* create(std::shared_ptr<LegacyCustomSettingV3> setting, float width);
    
    bool hasUncommittedChanges() const override;
    bool hasNonDefaultValue() const override;
    void onResetToDefault() override;

    std::shared_ptr<LegacyCustomSettingV3> getSetting() const;
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
