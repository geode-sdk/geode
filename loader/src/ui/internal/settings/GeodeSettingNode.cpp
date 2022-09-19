#include "GeodeSettingNode.hpp"

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
    m_toggle->setPosition(-10.f, .0f);
    m_toggle->toggle(m_uncommittedValue);
    m_menu->addChild(m_toggle);

    return true;
}

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
