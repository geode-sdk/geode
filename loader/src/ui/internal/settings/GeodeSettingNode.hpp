#pragma once

#include <Geode/loader/Setting.hpp>
#include <Geode/loader/SettingNode.hpp>
#include <Geode/Bindings.hpp>
#include <Geode/ui/InputNode.hpp>
#include <Geode/ui/BasedButtonSprite.hpp>
#include <Geode/utils/convert.hpp>

USE_GEODE_NAMESPACE();

namespace {
    template<class N, class T>
    class GeodeSettingNode : public SettingNode {
    public:
        using value_t = typename T::value_t;

    protected:
        value_t m_uncommittedValue;
        CCMenu* m_menu;
        CCLabelBMFont* m_nameLabel;
        CCLabelBMFont* m_errorLabel;
        CCMenuItemSpriteExtra* m_resetBtn;

        bool init(std::shared_ptr<T> setting, float width) {
            if (!SettingNode::init(std::static_pointer_cast<Setting>(setting)))
                return false;

            this->setContentSize({ width, 40.f });

            m_uncommittedValue = setting->getValue();
            
            m_nameLabel = CCLabelBMFont::create(
                (setting->getName().size() ?
                    setting->getName() :
                    setting->getKey()).c_str(),
                "bigFont.fnt"
            );
            m_nameLabel->setAnchorPoint({ .0f, .5f });
            m_nameLabel->limitLabelWidth(width / 2 - 50.f, .5f, .1f);
            m_nameLabel->setPosition({
                m_obContentSize.height / 2,
                m_obContentSize.height / 2
            });
            this->addChild(m_nameLabel);
            
            m_errorLabel = CCLabelBMFont::create("", "bigFont.fnt");
            m_errorLabel->setAnchorPoint({ .0f, .5f });
            m_errorLabel->limitLabelWidth(width / 2 - 50.f, .25f, .1f);
            m_errorLabel->setPosition({
                m_obContentSize.height / 2,
                m_obContentSize.height / 2 - 15.f
            });
            m_errorLabel->setColor({ 255, 100, 100 });
            m_errorLabel->setZOrder(5);
            this->addChild(m_errorLabel);

            m_menu = CCMenu::create();
            m_menu->setPosition({
                m_obContentSize.width - m_obContentSize.height / 2,
                m_obContentSize.height / 2
            });
            this->addChild(m_menu);

            if (setting->canResetToDefault()) {
                auto resetBtnSpr = ButtonSprite::create(
                    CCSprite::createWithSpriteFrameName(
                        "reset-gold.png"_spr
                    ),
                    0x20, true, 32.f, "black-square.png"_spr, 1.f
                );
                resetBtnSpr->setScale(.5f);

                m_resetBtn = CCMenuItemSpriteExtra::create(
                    resetBtnSpr, this,
                    (SEL_MenuHandler)(&GeodeSettingNode::onResetToDefault)
                );
                m_resetBtn->setPosition(
                    -m_obContentSize.width + m_obContentSize.height + 
                        m_nameLabel->getScaledContentSize().width + 15.f,
                    .0f
                );
                m_menu->addChild(m_resetBtn);
            }

            CCDirector::sharedDirector()->getTouchDispatcher()->incrementForcePrio(2);
            m_menu->registerWithTouchDispatcher();

            if (!this->setup(setting, width))
                return false;
            
            this->valueChanged();
            
            return true;
        }

        virtual bool setup(std::shared_ptr<T> setting, float width) = 0;

        virtual void valueChanged() {
            if (m_delegate) m_delegate->settingValueChanged(this);
            if (this->hasUncommittedChanges()) {
                m_nameLabel->setColor(cc3x(0xaaa));
            } else {
                m_nameLabel->setColor(cc3x(0xfff));
            }
            if (m_resetBtn) m_resetBtn->setVisible(
                this->hasNonDefaultValue()
            );
            auto isValid = std::static_pointer_cast<T>(m_setting)
                ->isValidValue(m_uncommittedValue);
            if (!isValid) {
                m_errorLabel->setVisible(true);
                m_errorLabel->setString(isValid.error().c_str());
            } else {
                m_errorLabel->setVisible(false);
            }
        }
    
        void onResetToDefault(CCObject*) {
            this->resetToDefault();
        }

    public:
        static N* create(
            std::shared_ptr<T> setting,
            float width
        ) {
            auto ret = new N();
            if (ret && ret->init(setting, width)) {
                ret->autorelease();
                return ret;
            }
            CC_SAFE_DELETE(ret);
            return nullptr;
        }

        void commit() override {
            std::static_pointer_cast<T>(m_setting)->setValue(m_uncommittedValue);
            this->valueChanged();
            if (m_delegate) m_delegate->settingValueCommitted(this);
        }

        bool hasUncommittedChanges() override {
            return m_uncommittedValue !=
                std::static_pointer_cast<T>(m_setting)->getValue();
        }
            
        bool hasNonDefaultValue() override {
            return m_uncommittedValue !=
                std::static_pointer_cast<T>(m_setting)->getDefault();
        }

        void resetToDefault() override {
            m_uncommittedValue =
                std::static_pointer_cast<T>(m_setting)->getDefault();
            this->valueChanged();
        }
    };
}

class BoolSettingNode :
    public GeodeSettingNode<BoolSettingNode, BoolSetting>
{
protected:
    CCMenuItemToggler* m_toggle;

    void onToggle(CCObject*);
    void valueChanged() override;
    bool setup(std::shared_ptr<BoolSetting> setting, float width) override;
};

class IntSettingNode :
    public GeodeSettingNode<IntSettingNode, IntSetting>
{
protected:
    bool setup(std::shared_ptr<IntSetting> setting, float width) override;
};

class FloatSettingNode :
    public GeodeSettingNode<FloatSettingNode, FloatSetting>
{
protected:
    bool setup(std::shared_ptr<FloatSetting> setting, float width) override;
};

class StringSettingNode :
    public GeodeSettingNode<StringSettingNode, StringSetting>,
    public TextInputDelegate
{
protected:
    InputNode* m_input;

    void textChanged(CCTextInputNode* input);
    void valueChanged() override;
    void updateLabel();
    
    bool setup(std::shared_ptr<StringSetting> setting, float width) override;
};
