#include "GeodeSettingNode.hpp"

#include <Geode/binding/ButtonSprite.hpp>
#include <Geode/binding/CCTextInputNode.hpp>
#include <Geode/binding/ColorChannelSprite.hpp>
#include <Geode/binding/Slider.hpp>
#include <Geode/binding/CCMenuItemToggler.hpp>
#include <Geode/loader/Loader.hpp>
#include <Geode/loader/Dirs.hpp>

// Helpers

template <class Num>
Num parseNumForInput(std::string const& str) {
    try {
        if constexpr (std::is_same_v<Num, int64_t>) {
            return std::stoll(str);
        }
        else if constexpr (std::is_same_v<Num, double>) {
            return std::stod(str);
        }
        else {
            static_assert(!std::is_same_v<Num, Num>, "Impl Num for parseNumForInput");
        }
    }
    catch (...) {
        return 0;
    }
}

template<class T>
static float valueToSlider(T const& setting, typename T::ValueType value) {
    auto min = setting.min ? setting.min.value() : -100;
    auto max = setting.max ? setting.max.value() : +100;
    auto range = max - min;
    return static_cast<float>(clamp(static_cast<double>(value - min) / range, 0.0, 1.0));
}

template<class T>
static typename T::ValueType valueFromSlider(T const& setting, float num) {
    auto min = setting.min ? setting.min.value() : -100;
    auto max = setting.max ? setting.max.value() : +100;
    auto range = max - min;
    auto value = static_cast<typename T::ValueType>(num * range + min);
    if (auto step = setting.controls.sliderStep) {
        value = static_cast<typename T::ValueType>(
            round(value / step.value()) * step.value()
        );
    }
    return value;
}

template<class C, class T>
InputNode* createInput(C* node, GeodeSettingValue<T>* setting, float width) {
    auto input = InputNode::create(width / 2 - 70.f, "Text", "chatFont.fnt");
    input->setPosition({
        -(width / 2 - 70.f) / 2,
        setting->castDefinition().controls.slider ? 5.f : 0.f
    });
    input->setScale(.65f);
    input->getInput()->setDelegate(node);
    return input;
}

template<class C, class T>
Slider* createSlider(C* node, GeodeSettingValue<T>* setting, float width) {
    auto slider = Slider::create(
        node, menu_selector(C::onSlider), .5f
    );
    slider->setPosition(-50.f, -15.f);
    node->updateSlider();
    return slider;
}

template<class C, class T>
std::pair<
    CCMenuItemSpriteExtra*, CCMenuItemSpriteExtra*
> createArrows(C* node, GeodeSettingValue<T>* setting, float width, bool big) {
    auto yPos = setting->castDefinition().controls.slider ? 5.f : 0.f;
    auto decArrowSpr = CCSprite::createWithSpriteFrameName(
        big ? "double-nav.png"_spr : "navArrowBtn_001.png"
    );
    decArrowSpr->setFlipX(true);
    decArrowSpr->setScale(.3f);

    auto decArrow = CCMenuItemSpriteExtra::create(
        decArrowSpr, node, menu_selector(C::onArrow)
    );
    decArrow->setPosition(-width / 2 + (big ? 65.f : 80.f), yPos);
    decArrow->setTag(big ? 
        -setting->castDefinition().controls.bigArrowStep :
        -setting->castDefinition().controls.arrowStep
    );

    auto incArrowSpr = CCSprite::createWithSpriteFrameName(
        big ? "double-nav.png"_spr : "navArrowBtn_001.png"
    );
    incArrowSpr->setScale(.3f);

    auto incArrow = CCMenuItemSpriteExtra::create(
        incArrowSpr, node, menu_selector(C::onArrow)
    );
    incArrow->setTag(big ? 
        setting->castDefinition().controls.bigArrowStep :
        setting->castDefinition().controls.arrowStep
    );
    incArrow->setPosition(big ? 5.f : -10.f, yPos);

    return { decArrow, incArrow };
}

// BoolSettingNode

void BoolSettingNode::valueChanged(bool updateText) {
    GeodeSettingNode::valueChanged(updateText);
    m_toggle->toggle(m_uncommittedValue);
}

void BoolSettingNode::onToggle(CCObject*) {
    m_uncommittedValue = !m_toggle->isToggled();
    this->valueChanged(true);
    m_toggle->toggle(!m_uncommittedValue);
}

bool BoolSettingNode::setup(BoolSettingValue* setting, float width) {
    m_toggle = CCMenuItemToggler::createWithStandardSprites(
        this, menu_selector(BoolSettingNode::onToggle), .6f
    );
    m_toggle->toggle(m_uncommittedValue);
    m_toggle->setPositionX(-10.f);
    m_menu->addChild(m_toggle);

    return true;
}

// IntSettingNode

float IntSettingNode::setupHeight(IntSettingValue* setting) const {
    return setting->castDefinition().controls.slider ? 55.f : 40.f;
}

void IntSettingNode::onSlider(CCObject* slider) {
    m_uncommittedValue = valueFromSlider(
        setting()->castDefinition(),
        static_cast<SliderThumb*>(slider)->getValue()
    );
    this->valueChanged(true);
}

void IntSettingNode::valueChanged(bool updateText) {
    GeodeSettingNode::valueChanged(updateText);
    if (updateText) {
        this->updateLabel();
    }
    this->updateSlider();
}

void IntSettingNode::updateSlider() {
    if (m_slider) {
        m_slider->setValue(valueToSlider(
            setting()->castDefinition(),
            m_uncommittedValue
        ));
        m_slider->updateBar();
    }
}

void IntSettingNode::updateLabel() {
    if (m_input) {
        // hacky way to make setString not called textChanged
        m_input->getInput()->setDelegate(nullptr);
        m_input->setString(numToString(m_uncommittedValue));
        m_input->getInput()->setDelegate(this);
    }
    else {
        m_label->setString(numToString(m_uncommittedValue).c_str());
        m_label->limitLabelWidth(m_width / 2 - 70.f, .5f, .1f);
    }
}

void IntSettingNode::onArrow(CCObject* sender) {
    m_uncommittedValue += sender->getTag();
    this->valueChanged(true);
}

void IntSettingNode::textChanged(CCTextInputNode* input) {
    m_uncommittedValue = parseNumForInput<ValueType>(input->getString());
    this->valueChanged(false);
}

bool IntSettingNode::setup(IntSettingValue* setting, float width) {
    if (setting->castDefinition().controls.input) {
        m_menu->addChild(m_input = createInput(this, setting, width));
    }
    else {
        m_label = CCLabelBMFont::create("", "bigFont.fnt");
        m_label->setPosition({
            -(width / 2 - 70.f) / 2,
            setting->castDefinition().controls.slider ? 5.f : 0.f 
        });
        m_label->limitLabelWidth(width / 2 - 70.f, .5f, .1f);
        m_menu->addChild(m_label);
    }
    if (setting->castDefinition().controls.slider) {
        m_menu->addChild(m_slider = createSlider(this, setting, width));
    }
    if (setting->castDefinition().controls.arrows) {
        auto [dec, inc] = createArrows(this, setting, width, false);
        m_menu->addChild(m_decArrow = dec);
        m_menu->addChild(m_incArrow = inc);
    }
    if (setting->castDefinition().controls.bigArrows) {
        auto [dec, inc] = createArrows(this, setting, width, true);
        m_menu->addChild(m_bigDecArrow = dec);
        m_menu->addChild(m_bigIncArrow = inc);
    }
    return true;
}

// FloatSettingNode

float FloatSettingNode::setupHeight(FloatSettingValue* setting) const {
    return setting->castDefinition().controls.slider ? 55.f : 40.f;
}

void FloatSettingNode::onSlider(CCObject* slider) {
    m_uncommittedValue = valueFromSlider(
        setting()->castDefinition(),
        static_cast<SliderThumb*>(slider)->getValue()
    );
    this->valueChanged(true);
}

void FloatSettingNode::valueChanged(bool updateText) {
    GeodeSettingNode::valueChanged(updateText);
    if (updateText) {
        this->updateLabel();
    }
    this->updateSlider();
}

void FloatSettingNode::updateSlider() {
    if (m_slider) {
        m_slider->setValue(valueToSlider(
            setting()->castDefinition(),
            m_uncommittedValue
        ));
        m_slider->updateBar();
    }
}

void FloatSettingNode::updateLabel() {
    if (m_input) {
        // hacky way to make setString not called textChanged
        m_input->getInput()->setDelegate(nullptr);
        m_input->setString(numToString(m_uncommittedValue));
        m_input->getInput()->setDelegate(this);
    }
    else {
        m_label->setString(numToString(m_uncommittedValue).c_str());
        m_label->limitLabelWidth(m_width / 2 - 70.f, .5f, .1f);
    }
}

void FloatSettingNode::onArrow(CCObject* sender) {
    m_uncommittedValue += sender->getTag();
    this->valueChanged(true);
}

void FloatSettingNode::textChanged(CCTextInputNode* input) {
    m_uncommittedValue = parseNumForInput<ValueType>(input->getString());
    this->valueChanged(false);
}

bool FloatSettingNode::setup(FloatSettingValue* setting, float width) {
    if (setting->castDefinition().controls.input) {
        m_menu->addChild(m_input = createInput(this, setting, width));
    }
    else {
        m_label = CCLabelBMFont::create("", "bigFont.fnt");
        m_label->setPosition({
            -(width / 2 - 70.f) / 2,
            setting->castDefinition().controls.slider ? 5.f : 0.f 
        });
        m_label->limitLabelWidth(width / 2 - 70.f, .5f, .1f);
        m_menu->addChild(m_label);
    }
    if (setting->castDefinition().controls.slider) {
        m_menu->addChild(m_slider = createSlider(this, setting, width));
    }
    if (setting->castDefinition().controls.arrows) {
        auto [dec, inc] = createArrows(this, setting, width, false);
        m_menu->addChild(m_decArrow = dec);
        m_menu->addChild(m_incArrow = inc);
    }
    if (setting->castDefinition().controls.bigArrows) {
        auto [dec, inc] = createArrows(this, setting, width, true);
        m_menu->addChild(m_bigDecArrow = dec);
        m_menu->addChild(m_bigIncArrow = inc);
    }
    return true;
}

// StringSettingNode

void StringSettingNode::updateLabel() {
    // hacky way to make setString not called textChanged
    m_input->getInput()->setDelegate(nullptr);
    m_input->setString(m_uncommittedValue);
    m_input->getInput()->setDelegate(this);
}

void StringSettingNode::textChanged(CCTextInputNode* input) {
    m_uncommittedValue = input->getString();
    this->valueChanged(false);
}

void StringSettingNode::valueChanged(bool updateText) {
    GeodeSettingNode::valueChanged(updateText);
    this->updateLabel();
}

bool StringSettingNode::setup(StringSettingValue* setting, float width) {
    m_input = InputNode::create(width / 2 - 10.f, "Text", "chatFont.fnt");
    m_input->setPosition({ -(width / 2 - 70.f) / 2, .0f });
    m_input->setScale(.65f);

    if (setting->castDefinition().filter.has_value()) {
        m_input->getInput()->setAllowedChars(setting->castDefinition().filter.value());
    }

    m_input->getInput()->setDelegate(this);
    m_menu->addChild(m_input);

    return true;
}

// FileSettingNode

void FileSettingNode::updateLabel() {
    // hacky way to make setString not called textChanged
    m_input->getInput()->setDelegate(nullptr);
    m_input->setString(m_uncommittedValue.string());
    m_input->getInput()->setDelegate(this);
}

void FileSettingNode::textChanged(CCTextInputNode* input) {
    m_uncommittedValue = input->getString();
    this->valueChanged(false);
}

void FileSettingNode::valueChanged(bool updateText) {
    GeodeSettingNode::valueChanged(updateText);
    this->updateLabel();
}

void FileSettingNode::onPickFile(CCObject*) {
    if (auto path = file::pickFile(
        file::PickMode::OpenFile,
        {
            dirs::getGameDir(),
            setting()->castDefinition().controls.filters
        }
    )) {
        m_uncommittedValue = path.unwrap();
        this->valueChanged(true);
    }
}

bool FileSettingNode::setup(FileSettingValue* setting, float width) {
    m_input = InputNode::create(width / 2 - 30.f, "Path to File", "chatFont.fnt");
    m_input->setPosition({ -(width / 2 - 80.f) / 2 - 15.f, .0f });
    m_input->setScale(.65f);
    m_input->getInput()->setDelegate(this);
    m_menu->addChild(m_input);

    auto fileBtnSpr = CCSprite::createWithSpriteFrameName("gj_folderBtn_001.png");
    fileBtnSpr->setScale(.5f);

    auto fileBtn = CCMenuItemSpriteExtra::create(
        fileBtnSpr, this, menu_selector(FileSettingNode::onPickFile)
    );
    fileBtn->setPosition(.0f, .0f);
    m_menu->addChild(fileBtn);

    return true;
}

// ColorSettingNode

void ColorSettingNode::valueChanged(bool updateText) {
    GeodeSettingNode::valueChanged(updateText);
    m_colorSpr->setColor(m_uncommittedValue);
}

void ColorSettingNode::updateColor(ccColor4B const& color) {
    m_uncommittedValue = to3B(color);
    this->valueChanged(true);
}

void ColorSettingNode::onSelectColor(CCObject*) {
    auto popup = ColorPickPopup::create(m_uncommittedValue);
    popup->setDelegate(this);
    popup->setColorTarget(m_colorSpr);
    popup->show();
}

bool ColorSettingNode::setup(ColorSettingValue* setting, float width) {
    m_colorSpr = ColorChannelSprite::create();
    m_colorSpr->setColor(m_uncommittedValue);
    m_colorSpr->setScale(.65f);

    auto button = CCMenuItemSpriteExtra::create(
        m_colorSpr, this, menu_selector(ColorSettingNode::onSelectColor)
    );
    button->setPositionX(-10.f);
    m_menu->addChild(button);

    return true;
}

// ColorAlphaSettingNode

void ColorAlphaSettingNode::valueChanged(bool updateText) {
    GeodeSettingNode::valueChanged(updateText);
    m_colorSpr->setColor(to3B(m_uncommittedValue));
    m_colorSpr->updateOpacity(m_uncommittedValue.a / 255.f);
}

void ColorAlphaSettingNode::updateColor(ccColor4B const& color) {
    m_uncommittedValue = color;
    this->valueChanged(true);
}

void ColorAlphaSettingNode::onSelectColor(CCObject*) {
    auto popup = ColorPickPopup::create(m_uncommittedValue);
    popup->setDelegate(this);
    popup->setColorTarget(m_colorSpr);
    popup->show();
}

bool ColorAlphaSettingNode::setup(ColorAlphaSettingValue* setting, float width) {
    m_colorSpr = ColorChannelSprite::create();
    m_colorSpr->setColor(to3B(m_uncommittedValue));
    m_colorSpr->updateOpacity(m_uncommittedValue.a / 255.f);
    m_colorSpr->setScale(.65f);

    auto button = CCMenuItemSpriteExtra::create(
        m_colorSpr, this, menu_selector(ColorAlphaSettingNode::onSelectColor)
    );
    button->setPositionX(-10.f);
    m_menu->addChild(button);

    return true;
}

// CustomSettingPlaceholderNode

void CustomSettingPlaceholderNode::commit() {

}

bool CustomSettingPlaceholderNode::hasUncommittedChanges() {
    return false;
}

bool CustomSettingPlaceholderNode::hasNonDefaultValue() {
    return false;
}

void CustomSettingPlaceholderNode::resetToDefault() {

}

bool CustomSettingPlaceholderNode::init(std::string const& key, float width) {
    if (!SettingNode::init(nullptr))
        return false;

    constexpr auto sidePad = 40.f;

    this->setContentSize({ width, 40.f });

    auto info = CCLabelBMFont::create(
        "You need to enable the mod to modify this setting.",
        "bigFont.fnt"
    );
    info->setAnchorPoint({ .0f, .5f });
    info->limitLabelWidth(width - sidePad, .5f, .1f);
    info->setPosition({ sidePad / 2, m_obContentSize.height / 2 });
    this->addChild(info);

    return true;
}

CustomSettingPlaceholderNode* CustomSettingPlaceholderNode::create(
    std::string const& key, float width
) {
    auto ret = new CustomSettingPlaceholderNode;
    if (ret && ret->init(key, width)) {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}
