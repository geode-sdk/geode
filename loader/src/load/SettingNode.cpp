#include <Geode/loader/SettingNode.hpp>
#include <Geode/utils/cocos.hpp>

USE_GEODE_NAMESPACE();

bool SettingNode::init(std::shared_ptr<Setting> setting) {
    m_setting = setting;
    return true;
}

bool BoolSettingNode::setup(std::shared_ptr<BoolSetting> setting, float width) {
    auto toggle = CCMenuItemToggler::createWithStandardSprites(
        this, makeMenuSelector([this](CCMenuItemToggler* toggle) {
            m_uncommittedValue = !toggle->isToggled();
            this->valueChanged();
        }), .5f
    );
    toggle->setPosition(.0f, .0f);
    toggle->toggle(m_uncommittedValue);
    m_menu->addChild(toggle);

    return true;
}

bool IntSettingNode::setup(std::shared_ptr<IntSetting> setting, float width) {
    return true;
}

bool FloatSettingNode::setup(std::shared_ptr<FloatSetting> setting, float width) {
    return true;
}

void StringSettingNode::updateLabel() {
    m_input->setString(m_uncommittedValue);
}

bool StringSettingNode::setup(std::shared_ptr<StringSetting> setting, float width) {
    m_input = InputNode::create(180.f, "Text", "chatFont.fnt");
    m_input->setPosition({ -50.f, .0f });
    m_input->setScale(.65f);
    m_menu->addChild(m_input);
    
    this->updateLabel();

    return true;
}
