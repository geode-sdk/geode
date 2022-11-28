#include "GeodeSettingNode.hpp"

#include <Geode/binding/ButtonSprite.hpp>
#include <Geode/binding/CCTextInputNode.hpp>
#include <Geode/binding/ColorChannelSprite.hpp>
#include <Geode/binding/Slider.hpp>

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
    auto setting = std::static_pointer_cast<FileSetting>(m_setting);
    if (auto path = file::pickFile(
            file::PickMode::OpenFile,
            {
                file::geodeRoot(),
                setting->getFileFilters().value_or(std::vector<file::FilePickOptions::Filter>())
            }
        )) {
        m_uncommittedValue = path.unwrap();
        this->valueChanged(true);
    }
}

bool FileSettingNode::setup(std::shared_ptr<FileSetting> setting, float width) {
    m_input = InputNode::create(width / 2 - 30.f, "Path to File", "chatFont.fnt");
    m_input->setPosition({ -(width / 2 - 80.f) / 2 - 15.f, .0f });
    m_input->setScale(.65f);
    m_input->getInput()->setDelegate(this);
    m_menu->addChild(m_input);

    auto fileBtnSpr = CCSprite::createWithSpriteFrameName("gj_folderBtn_001.png");
    fileBtnSpr->setScale(.5f);

    auto fileBtn =
        CCMenuItemSpriteExtra::create(fileBtnSpr, this, menu_selector(FileSettingNode::onPickFile));
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

bool ColorSettingNode::setup(std::shared_ptr<ColorSetting> setting, float width) {
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

bool ColorAlphaSettingNode::setup(std::shared_ptr<ColorAlphaSetting> setting, float width) {
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
