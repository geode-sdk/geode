#pragma once

#include <Geode/loader/Setting.hpp>
#include <Geode/loader/SettingNode.hpp>
#include <Geode/Bindings.hpp>
#include <Geode/ui/InputNode.hpp>
#include <Geode/ui/BasedButtonSprite.hpp>
#include <Geode/utils/convert.hpp>
#include <Geode/utils/string.hpp>
#include <Geode/utils/cocos.hpp>
#include <Geode/utils/operators.hpp>
#include <Geode/ui/Popup.hpp>
#include <Geode/ui/ColorPickPopup.hpp>

USE_GEODE_NAMESPACE();

namespace {
    template<class Num>
    Num parseNumForInput(std::string const& str) {
        try {
            if constexpr(std::is_same_v<Num, int64_t>) {
                return std::stoll(str);
            } else if constexpr(std::is_same_v<Num, double>) {
                return std::stod(str);
            }
        } catch(...) {
            return 0;
        }
    }

    template<class N, class T>
    class GeodeSettingNode : public SettingNode {
    public:
        using value_t = typename T::value_t;

    protected:
        float m_width;
        float m_height;
        value_t m_uncommittedValue;
        CCMenu* m_menu;
        CCLabelBMFont* m_nameLabel;
        CCLabelBMFont* m_errorLabel;
        CCMenuItemSpriteExtra* m_resetBtn;

        bool init(std::shared_ptr<T> setting, float width) {
            if (!SettingNode::init(std::static_pointer_cast<Setting>(setting)))
                return false;
            
            m_width = width;
            m_height = this->setupHeight(setting);
            this->setContentSize({ width, m_height });

            constexpr auto sidePad = 40.f;

            m_uncommittedValue = setting->getValue();

            auto name = setting->getDisplayName();
            
            m_nameLabel = CCLabelBMFont::create(name.c_str(), "bigFont.fnt");
            m_nameLabel->setAnchorPoint({ .0f, .5f });
            m_nameLabel->limitLabelWidth(width / 2 - 50.f, .5f, .1f);
            m_nameLabel->setPosition({
                sidePad / 2,
                m_obContentSize.height / 2
            });
            this->addChild(m_nameLabel);
            
            m_errorLabel = CCLabelBMFont::create("", "bigFont.fnt");
            m_errorLabel->setAnchorPoint({ .0f, .5f });
            m_errorLabel->limitLabelWidth(width / 2 - 50.f, .25f, .1f);
            m_errorLabel->setPosition({
                sidePad / 2,
                m_obContentSize.height / 2 - 14.f
            });
            m_errorLabel->setColor({ 255, 100, 100 });
            m_errorLabel->setZOrder(5);
            this->addChild(m_errorLabel);

            m_menu = CCMenu::create();
            m_menu->setPosition({
                m_obContentSize.width - sidePad / 2,
                m_obContentSize.height / 2
            });
            this->addChild(m_menu);

            float btnPos = 15.f;

            if (setting->getDescription()) {
                auto infoSpr = CCSprite::createWithSpriteFrameName(
                    "GJ_infoIcon_001.png"
                );
                infoSpr->setScale(.6f);

                auto infoBtn = CCMenuItemSpriteExtra::create(
                    infoSpr, this, menu_selector(GeodeSettingNode::onDescription)
                );
                infoBtn->setPosition(
                    -m_obContentSize.width + sidePad + 
                        m_nameLabel->getScaledContentSize().width + btnPos,
                    0.f
                );
                m_menu->addChild(infoBtn);

                btnPos += 20.f;
            }

            if (setting->canResetToDefault()) {
                auto resetBtnSpr = CCSprite::createWithSpriteFrameName(
                    "reset-gold.png"_spr
                );
                resetBtnSpr->setScale(.5f);

                m_resetBtn = CCMenuItemSpriteExtra::create(
                    resetBtnSpr, this, menu_selector(GeodeSettingNode::onReset)
                );
                m_resetBtn->setPosition(
                    -m_obContentSize.width + sidePad + 
                        m_nameLabel->getScaledContentSize().width + btnPos,
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

        void onDescription(CCObject*) {
            auto setting = std::static_pointer_cast<T>(m_setting);
            FLAlertLayer::create(
                setting->getDisplayName().c_str(),
                setting->getDescription().value(),
                "OK"
            )->show();
        }

        void onReset(CCObject*) {
            auto setting = std::static_pointer_cast<T>(m_setting);
            createQuickPopup(
                "Reset",
                "Are you sure you want to <cr>reset</c> <cl>" +
                setting->getDisplayName() + 
                "</c> to <cy>default</c>?",
                "Cancel", "Reset",
                [this](auto, bool btn2) {
                    if (btn2) this->resetToDefault();
                }
            );
        }

        virtual float setupHeight(std::shared_ptr<T> setting) const {
            return 40.f;
        }
        virtual bool setup(std::shared_ptr<T> setting, float width) = 0;

        virtual void valueChanged(bool updateText = true) {
            if (m_delegate) m_delegate->settingValueChanged(this);
            if (this->hasUncommittedChanges()) {
                m_nameLabel->setColor(cc3x(0x1d0));
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
            m_uncommittedValue = std::static_pointer_cast<T>(m_setting)->getValue();
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

    template<class C, class T>
    class ImplInput : public TextInputDelegate {
    protected:
        InputNode* m_input = nullptr;
        CCLabelBMFont* m_label = nullptr;

        C* self() {
            return static_cast<C*>(this);
        }

        void setupInput(std::shared_ptr<T> setting, float width) {
            if (setting->hasInput()) {
                m_input = InputNode::create(width / 2 - 70.f, "Text", "chatFont.fnt");
                m_input->setPosition({
                    -(width / 2 - 70.f) / 2,
                    setting->hasSlider() ? 5.f : 0.f
                });
                m_input->setScale(.65f);
                m_input->getInput()->setDelegate(this);
                self()->m_menu->addChild(m_input);
            } else {
                m_label = CCLabelBMFont::create("", "bigFont.fnt");
                m_label->setPosition({
                    -(width / 2 - 70.f) / 2,
                    setting->hasSlider() ? 5.f : 0.f
                });
                m_label->limitLabelWidth(width / 2 - 70.f, .5f, .1f);
                self()->m_menu->addChild(m_label);
            }
        }

        void updateLabel(bool updateText) {
            if (!updateText) return;
            if (m_input) {
                // hacky way to make setString not called textChanged
                m_input->getInput()->setDelegate(nullptr);
                m_input->setString(
                    numToString(self()->m_uncommittedValue)
                );
                m_input->getInput()->setDelegate(this);
            } else {
                m_label->setString(
                    numToString(self()->m_uncommittedValue).c_str()
                );
                m_label->limitLabelWidth(self()->m_width / 2 - 70.f, .5f, .1f);
            }
        }

        void textChanged(CCTextInputNode* input) override {
            try {
                self()->m_uncommittedValue = parseNumForInput<typename C::value_t>(
                    input->getString()
                );
                self()->valueChanged(false);
            } catch(...) {}
        }
    };

    template<class C, class T>
    class ImplArrows {
    protected:
        CCMenuItemSpriteExtra* m_decArrow = nullptr;
        CCMenuItemSpriteExtra* m_incArrow = nullptr;
        CCMenuItemSpriteExtra* m_bigDecArrow = nullptr;
        CCMenuItemSpriteExtra* m_bigIncArrow = nullptr;

        C* self() {
            return static_cast<C*>(this);
        }

        void setupArrows(std::shared_ptr<T> setting, float width) {
            auto yPos = setting->hasSlider() ? 5.f : 0.f;

            if (setting->hasArrows()) {
                auto decArrowSpr = CCSprite::createWithSpriteFrameName("navArrowBtn_001.png");
                decArrowSpr->setFlipX(true);
                decArrowSpr->setScale(.3f);

                m_decArrow = CCMenuItemSpriteExtra::create(
                    decArrowSpr, self(), menu_selector(ImplArrows::onDecrement)
                );
                m_decArrow->setPosition(-width / 2 + 80.f, yPos);
                self()->m_menu->addChild(m_decArrow);

                auto incArrowSpr = CCSprite::createWithSpriteFrameName("navArrowBtn_001.png");
                incArrowSpr->setScale(.3f);

                m_incArrow = CCMenuItemSpriteExtra::create(
                    incArrowSpr, self(), menu_selector(ImplArrows::onIncrement)
                );
                m_incArrow->setPosition(-10.f, yPos);
                self()->m_menu->addChild(m_incArrow);
            }

            if (setting->hasBigArrows()) {
                auto decArrowSpr = CCSprite::createWithSpriteFrameName("double-nav.png"_spr);
                decArrowSpr->setFlipX(true);
                decArrowSpr->setScale(.3f);

                m_bigDecArrow = CCMenuItemSpriteExtra::create(
                    decArrowSpr, self(), menu_selector(ImplArrows::onBigDecrement)
                );
                m_bigDecArrow->setPosition(-width / 2 + 65.f, yPos);
                self()->m_menu->addChild(m_bigDecArrow);

                auto incArrowSpr = CCSprite::createWithSpriteFrameName("double-nav.png"_spr);
                incArrowSpr->setScale(.3f);

                m_bigIncArrow = CCMenuItemSpriteExtra::create(
                    incArrowSpr, self(), menu_selector(ImplArrows::onBigIncrement)
                );
                m_bigIncArrow->setPosition(5.f, yPos);
                self()->m_menu->addChild(m_bigIncArrow);
            }
        }

        void onIncrement(CCObject*) {
            // intentionally refcast to prevent warnings on clang and 
            // not to offset this as it has already been offset to the 
            // correct vtable when it's passed to CCMenuItemSpriteExtra
            auto self = reference_cast<C*>(this);
            self->m_uncommittedValue += std::static_pointer_cast<T>(
                self->m_setting
            )->getArrowStepSize();
            self->valueChanged(true);
        }

        void onDecrement(CCObject*) {
            // intentional, see ImplArrows::onIncrement
            auto self = reference_cast<C*>(this);
            self->m_uncommittedValue -= std::static_pointer_cast<T>(
                self->m_setting
            )->getArrowStepSize();
            self->valueChanged(true);
        }

        void onBigIncrement(CCObject*) {
            // intentional, see ImplArrows::onIncrement
            auto self = reference_cast<C*>(this);
            self->m_uncommittedValue += std::static_pointer_cast<T>(
                self->m_setting
            )->getBigArrowStepSize();
            self->valueChanged(true);
        }

        void onBigDecrement(CCObject*) {
            // intentional, see ImplArrows::onIncrement
            auto self = reference_cast<C*>(this);
            self->m_uncommittedValue -= std::static_pointer_cast<T>(
                self->m_setting
            )->getBigArrowStepSize();
            self->valueChanged(true);
        }
    };

    template<class C, class T>
    class ImplSlider {
    protected:
        Slider* m_slider = nullptr;

        C* self() {
            return static_cast<C*>(this);
        }

        static float valueToSlider(
            std::shared_ptr<T> setting,
            typename T::value_t num
        ) {
            auto min = setting->getMin() ? setting->getMin().value() : -100;
            auto max = setting->getMax() ? setting->getMax().value() : +100;
            auto range = max - min;
            return static_cast<float>(clamp(
                static_cast<double>(num - min) / range, 0.0, 1.0
            ));
        }

        static typename T::value_t valueFromSlider(
            std::shared_ptr<T> setting,
            float num
        ) {
            auto min = setting->getMin() ? setting->getMin().value() : -100;
            auto max = setting->getMax() ? setting->getMax().value() : +100;
            auto range = max - min;
            auto value = static_cast<typename T::value_t>(num * range + min);
            if (auto step = setting->getSliderStepSize()) {
                value = static_cast<typename T::value_t>(
                    round(value / step.value()) * step.value()
                );
            }
            return value;
        }

        void setupSlider(std::shared_ptr<T> setting, float width) {
            if (setting->hasSlider()) {
                m_slider = Slider::create(
                    self(), menu_selector(ImplSlider::onSlider), .5f
                );
                m_slider->setPosition(-50.f, -15.f);
                self()->m_menu->addChild(m_slider);

                this->updateSlider();
            }
        }

        void updateSlider() {
            if (m_slider) {
                auto setting = std::static_pointer_cast<T>(self()->m_setting);
                m_slider->setValue(
                    valueToSlider(setting, self()->m_uncommittedValue)
                );
                m_slider->updateBar();
            }
        }

        void onSlider(CCObject* slider) {
            // intentional, see ImplArrows::onIncrement
            auto self = reference_cast<C*>(this);
            auto setting = std::static_pointer_cast<T>(self->m_setting);

            self->m_uncommittedValue = valueFromSlider(
                setting,
                static_cast<SliderThumb*>(slider)->getValue()
            );
            self->valueChanged(true);
        }
    };
}

class BoolSettingNode :
    public GeodeSettingNode<BoolSettingNode, BoolSetting>
{
protected:
    CCMenuItemToggler* m_toggle;

    void onToggle(CCObject*);
    void valueChanged(bool updateText) override;
    bool setup(std::shared_ptr<BoolSetting> setting, float width) override;
};

class IntSettingNode :
    public GeodeSettingNode<IntSettingNode, IntSetting>,
    public ImplArrows<IntSettingNode, IntSetting>,
    public ImplInput<IntSettingNode, IntSetting>,
    public ImplSlider<IntSettingNode, IntSetting>
{
protected:
    friend class ImplArrows<IntSettingNode, IntSetting>;
    friend class ImplInput<IntSettingNode, IntSetting>;
    friend class ImplSlider<IntSettingNode, IntSetting>;

    void valueChanged(bool updateText) override;

    float setupHeight(std::shared_ptr<IntSetting> setting) const override;
    bool setup(std::shared_ptr<IntSetting> setting, float width) override;
};

class FloatSettingNode :
    public GeodeSettingNode<FloatSettingNode, FloatSetting>,
    public ImplArrows<FloatSettingNode, FloatSetting>,
    public ImplInput<FloatSettingNode, FloatSetting>,
    public ImplSlider<FloatSettingNode, FloatSetting>
{
protected:
    friend class ImplArrows<FloatSettingNode, FloatSetting>;
    friend class ImplInput<FloatSettingNode, FloatSetting>;
    friend class ImplSlider<FloatSettingNode, FloatSetting>;

    void valueChanged(bool updateText) override;

    float setupHeight(std::shared_ptr<FloatSetting> setting) const override;
    bool setup(std::shared_ptr<FloatSetting> setting, float width) override;
};

class StringSettingNode :
    public GeodeSettingNode<StringSettingNode, StringSetting>,
    public TextInputDelegate
{
protected:
    InputNode* m_input;

    void textChanged(CCTextInputNode* input) override;
    void valueChanged(bool updateText) override;
    void updateLabel();
    
    bool setup(std::shared_ptr<StringSetting> setting, float width) override;
};

class FileSettingNode :
    public GeodeSettingNode<FileSettingNode, FileSetting>,
    public TextInputDelegate
{
protected:
    InputNode* m_input;

    void textChanged(CCTextInputNode* input) override;
    void valueChanged(bool updateText) override;
    void updateLabel();

    void onPickFile(CCObject*);
    
    bool setup(std::shared_ptr<FileSetting> setting, float width) override;
};

class ColorSettingNode : 
    public GeodeSettingNode<ColorSettingNode, ColorSetting>,
    public ColorPickPopupDelegate
{
protected:
    ColorChannelSprite* m_colorSpr;

    void valueChanged(bool updateText) override;
    void updateColor(ccColor4B const& color) override;

    void onSelectColor(CCObject*);

    bool setup(std::shared_ptr<ColorSetting> setting, float width) override;
};

class ColorAlphaSettingNode : 
    public GeodeSettingNode<ColorAlphaSettingNode, ColorAlphaSetting>,
    public ColorPickPopupDelegate
{
protected:
    ColorChannelSprite* m_colorSpr;

    void valueChanged(bool updateText) override;
    void updateColor(ccColor4B const& color) override;

    void onSelectColor(CCObject*);
    
    bool setup(std::shared_ptr<ColorAlphaSetting> setting, float width) override;
};
