#include "GeodeSettingNode.hpp"

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

bool BoolSettingNode::setup(std::shared_ptr<BoolSetting> setting, float width) {
    m_toggle = CCMenuItemToggler::createWithStandardSprites(
        this, menu_selector(BoolSettingNode::onToggle), .6f
    );
    m_toggle->toggle(m_uncommittedValue);
    m_toggle->setPositionX(-10.f);
    m_menu->addChild(m_toggle);

    return true;
}

// IntSettingNode

float IntSettingNode::setupHeight(std::shared_ptr<IntSetting> setting) const {
    return setting->hasSlider() ? 55.f : 40.f;
}

bool IntSettingNode::setup(std::shared_ptr<IntSetting> setting, float width) {
    this->setupArrows(setting, width);
    this->setupInput(setting, width);
    this->setupSlider(setting, width);
    return true;
}

void IntSettingNode::valueChanged(bool updateText) {
    GeodeSettingNode::valueChanged(updateText);
    this->updateLabel(updateText);
    this->updateSlider();
}

// FloatSettingNode

float FloatSettingNode::setupHeight(std::shared_ptr<FloatSetting> setting) const {
    return setting->hasSlider() ? 55.f : 40.f;
}

bool FloatSettingNode::setup(std::shared_ptr<FloatSetting> setting, float width) {
    this->setupArrows(setting, width);
    this->setupInput(setting, width);
    this->setupSlider(setting, width);
    return true;
}

void FloatSettingNode::valueChanged(bool updateText) {
    GeodeSettingNode::valueChanged(updateText);
    this->updateLabel(updateText);
    this->updateSlider();
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

bool StringSettingNode::setup(std::shared_ptr<StringSetting> setting, float width) {
    m_input = InputNode::create(width / 2 - 10.f, "Text", "chatFont.fnt");
    m_input->setPosition({ -(width / 2 - 70.f) / 2, .0f });
    m_input->setScale(.65f);
    m_input->getInput()->setDelegate(this);
    m_menu->addChild(m_input);

    return true;
}

// ColorSettingNode

void ColorSettingNode::valueChanged(bool updateText) {
    GeodeSettingNode::valueChanged(updateText);
    m_colorSpr->setColor(m_uncommittedValue);
}

bool ColorSettingNode::setup(std::shared_ptr<ColorSetting> setting, float width) {
	m_colorSpr = ColorChannelSprite::create();
	m_colorSpr->setColor(m_uncommittedValue);
	m_colorSpr->setScale(.65f);
	
	auto button = CCMenuItemSpriteExtra::create(
		m_colorSpr, this, makeMenuSelector([this](CCObject*) {
            ColorPickPopup<ColorSettingNode>::create(this)->show();
        })
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

bool ColorAlphaSettingNode::setup(std::shared_ptr<ColorAlphaSetting> setting, float width) {
	m_colorSpr = ColorChannelSprite::create();
	m_colorSpr->setColor(to3B(m_uncommittedValue));
    m_colorSpr->updateOpacity(m_uncommittedValue.a / 255.f);
	m_colorSpr->setScale(.65f);
	
	auto button = CCMenuItemSpriteExtra::create(
		m_colorSpr, this, makeMenuSelector([this](CCObject*) {
            ColorPickPopup<ColorAlphaSettingNode>::create(this)->show();
        })
	);
    button->setPositionX(-10.f);
	m_menu->addChild(button);

    return true;
}
