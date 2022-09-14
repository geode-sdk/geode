#include <Geode/loader/SettingNode.hpp>
#include <Geode/utils/cocos.hpp>

USE_GEODE_NAMESPACE();

bool SettingNode::init(std::shared_ptr<Setting> setting) {
    m_setting = setting;
    return true;
}

void BoolSettingNode::valueChanged() {
    GeodeSettingNode::valueChanged();
    m_toggle->toggle(m_uncommittedValue);
}

void BoolSettingNode::onToggle(CCObject*) {
    m_uncommittedValue = !m_toggle->isToggled();
    this->valueChanged();
    m_toggle->toggle(!m_uncommittedValue);
}

bool BoolSettingNode::setup(std::shared_ptr<BoolSetting> setting, float width) {
    m_toggle = CCMenuItemToggler::createWithStandardSprites(
        this, menu_selector(BoolSettingNode::onToggle), .6f
    );
    m_toggle->setPosition(.0f, .0f);
    m_toggle->toggle(m_uncommittedValue);
    m_menu->addChild(m_toggle);

    return true;
}

bool IntSettingNode::setup(std::shared_ptr<IntSetting> setting, float width) {
    return true;
}

bool FloatSettingNode::setup(std::shared_ptr<FloatSetting> setting, float width) {
    return true;
}

void StringSettingNode::updateLabel() {
    // hacky way to make setString not called textChanged
    m_input->getInput()->setDelegate(nullptr);
    m_input->setString(m_uncommittedValue);
    m_input->getInput()->setDelegate(this);
}

void StringSettingNode::textChanged(CCTextInputNode* input) {
    m_uncommittedValue = input->getString();
    this->valueChanged();
}

void StringSettingNode::valueChanged() {
    GeodeSettingNode::valueChanged();
    this->updateLabel();
}

bool StringSettingNode::setup(std::shared_ptr<StringSetting> setting, float width) {
    m_input = InputNode::create(180.f, "Text", "chatFont.fnt");
    m_input->setPosition({ -50.f, .0f });
    m_input->setScale(.65f);
    m_input->getInput()->setDelegate(this);
    m_menu->addChild(m_input);

    return true;
}
