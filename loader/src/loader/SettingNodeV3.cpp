#include "SettingNodeV3.hpp"
#include <Geode/loader/SettingNode.hpp>

template<class T>
static float valueToSlider(std::shared_ptr<T> setting, typename T::ValueType value) {
    auto min = setting->getMinValue().value_or(-100);
    auto max = setting->getMaxValue().value_or(+100);
    auto range = max - min;
    return static_cast<float>(clamp(static_cast<double>(value - min) / range, 0.0, 1.0));
}
template<class T>
static typename T::ValueType valueFromSlider(std::shared_ptr<T> setting, float num) {
    auto min = setting->getMinValue().value_or(-100);
    auto max = setting->getMaxValue().value_or(+100);
    auto range = max - min;
    auto value = static_cast<typename T::ValueType>(num * range + min);
    if (auto step = setting->getSliderSnap()) {
        value = static_cast<typename T::ValueType>(
            round(value / *step) * (*step)
        );
    }
    return value;
}

class SettingNodeSizeChangeEventV3::Impl final {
public:
    SettingNodeV3* node;
};

SettingNodeSizeChangeEventV3::SettingNodeSizeChangeEventV3(SettingNodeV3* node)
  : m_impl(std::make_shared<Impl>())
{
    m_impl->node = node;
}
SettingNodeSizeChangeEventV3::~SettingNodeSizeChangeEventV3() = default;

SettingNodeV3* SettingNodeSizeChangeEventV3::getTargetNode() const {
    return m_impl->node;
}

class SettingNodeValueChangeEventV3::Impl final {
public:
    bool commit = false;
};

SettingNodeValueChangeEventV3::SettingNodeValueChangeEventV3(bool commit)
  : m_impl(std::make_shared<Impl>())
{
    m_impl->commit = commit;
}
SettingNodeValueChangeEventV3::~SettingNodeValueChangeEventV3() = default;

bool SettingNodeValueChangeEventV3::isCommit() const {
    return m_impl->commit;
}

class SettingNodeV3::Impl final {
public:
    std::shared_ptr<SettingV3> setting;
    CCLabelBMFont* nameLabel;
    CCMenu* nameMenu;
    CCMenu* buttonMenu;
    CCMenuItemSpriteExtra* resetButton;
};

bool SettingNodeV3::init(std::shared_ptr<SettingV3> setting, float width) {
    if (!CCNode::init())
        return false;
    
    m_impl = std::make_shared<Impl>();
    m_impl->setting = setting;

    m_impl->nameMenu = CCMenu::create();
    m_impl->nameMenu->setContentWidth(width / 2 - 20);

    m_impl->nameLabel = CCLabelBMFont::create(setting->getDisplayName().c_str(), "bigFont.fnt");
    m_impl->nameLabel->setLayoutOptions(AxisLayoutOptions::create()->setScaleLimits(.1f, .4f)->setScalePriority(1));
    m_impl->nameMenu->addChild(m_impl->nameLabel);

    if (setting->getDescription()) {
        auto descSpr = CCSprite::createWithSpriteFrameName("GJ_infoIcon_001.png");
        descSpr->setScale(.5f);
        auto descBtn = CCMenuItemSpriteExtra::create(
            descSpr, this, menu_selector(SettingNodeV3::onDescription)
        );
        m_impl->nameMenu->addChild(descBtn);
    }

    auto resetSpr = CCSprite::createWithSpriteFrameName("reset-gold.png"_spr);
    resetSpr->setScale(.5f);
    m_impl->resetButton = CCMenuItemSpriteExtra::create(
        resetSpr, this, menu_selector(SettingNodeV3::onReset)
    );
    m_impl->nameMenu->addChild(m_impl->resetButton);

    m_impl->nameMenu->setLayout(RowLayout::create()->setAxisAlignment(AxisAlignment::Start));
    this->addChildAtPosition(m_impl->nameMenu, Anchor::Left, ccp(10, 0), ccp(0, .5f));

    m_impl->buttonMenu = CCMenu::create();
    m_impl->buttonMenu->setContentWidth(width / 2 - 20);
    m_impl->buttonMenu->setLayout(RowLayout::create()->setAxisAlignment(AxisAlignment::End));
    this->addChildAtPosition(m_impl->buttonMenu, Anchor::Right, ccp(-10, 0), ccp(1, .5f));

    this->setAnchorPoint({ .5f, .5f });
    this->setContentSize({ width, 30 });

    return true;
}

void SettingNodeV3::updateState() {
    this->getNameLabel()->setColor(this->hasUncommittedChanges() ? ccc3(17, 221, 0) : ccWHITE);
    m_impl->resetButton->setVisible(this->hasNonDefaultValue());
    m_impl->nameMenu->updateLayout();
}

void SettingNodeV3::onDescription(CCObject*) {
    auto title = m_impl->setting->getDisplayName();
    FLAlertLayer::create(
        nullptr,
        title.c_str(),
        m_impl->setting->getDescription().value_or("No description provided"),
        "OK", nullptr,
        clamp(title.size() * 16, 240, 400)
    )->show();
}
void SettingNodeV3::onReset(CCObject*) {
    createQuickPopup(
        "Reset",
        fmt::format(
            "Are you sure you want to <cr>reset</c> <cl>{}</c> to <cy>default</c>?",
            this->getSetting()->getDisplayName()
        ),
        "Cancel", "Reset",
        [this](auto, bool btn2) {
            if (btn2) {
                this->resetToDefault();
            }
        }
    );
}

void SettingNodeV3::markChanged() {
    this->updateState();
    SettingNodeValueChangeEventV3(false).post();
}
void SettingNodeV3::commit() {
    this->onCommit();
    this->updateState();
    SettingNodeValueChangeEventV3(true).post();
}
void SettingNodeV3::resetToDefault() {
    m_impl->setting->reset();
    this->onResetToDefault();
    this->updateState();
    SettingNodeValueChangeEventV3(false).post();
}

void SettingNodeV3::setContentSize(CCSize const& size) {
    CCNode::setContentSize(size);
    this->updateLayout();
    SettingNodeSizeChangeEventV3(this).post();
}

CCLabelBMFont* SettingNodeV3::getNameLabel() const {
    return m_impl->nameLabel;
}
CCMenu* SettingNodeV3::getNameMenu() const {
    return m_impl->nameMenu;
}
CCMenu* SettingNodeV3::getButtonMenu() const {
    return m_impl->buttonMenu;
}

std::shared_ptr<SettingV3> SettingNodeV3::getSetting() const {
    return m_impl->setting;
}

// TitleSettingNodeV3

bool TitleSettingNodeV3::init(std::shared_ptr<TitleSettingV3> setting, float width) {
    if (!SettingNodeV3::init(setting, width))
        return false;
    
    this->getNameLabel()->setFntFile("goldFont.fnt");
    this->getNameMenu()->updateLayout();
    this->setContentHeight(20);
    this->updateState();
    
    return true;
}

void TitleSettingNodeV3::onCommit() {}

bool TitleSettingNodeV3::hasUncommittedChanges() const {
    return false;
}
bool TitleSettingNodeV3::hasNonDefaultValue() const {
    return false;
}
void TitleSettingNodeV3::onResetToDefault() {}

std::shared_ptr<TitleSettingV3> TitleSettingNodeV3::getSetting() const {
    return std::static_pointer_cast<TitleSettingV3>(SettingNodeV3::getSetting());
}

TitleSettingNodeV3* TitleSettingNodeV3::create(std::shared_ptr<TitleSettingV3> setting, float width) {
    auto ret = new TitleSettingNodeV3();
    if (ret && ret->init(setting, width)) {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

// BoolSettingNodeV3

bool BoolSettingNodeV3::init(std::shared_ptr<BoolSettingV3> setting, float width) {
    if (!SettingNodeV3::init(setting, width))
        return false;
    
    m_toggle = CCMenuItemToggler::createWithStandardSprites(
        this, menu_selector(BoolSettingNodeV3::onToggle), .55f
    );
    m_toggle->m_onButton->setContentSize({ 25, 25 });
    m_toggle->m_onButton->getNormalImage()->setPosition(ccp(25, 25) / 2);
    m_toggle->m_offButton->setContentSize({ 25, 25 });
    m_toggle->m_offButton->getNormalImage()->setPosition(ccp(25, 25) / 2);
    m_toggle->m_notClickable = true;
    m_toggle->toggle(setting->getValue());
    this->getButtonMenu()->addChild(m_toggle);
    this->getButtonMenu()->updateLayout();

    this->updateState();

    return true;
}

void BoolSettingNodeV3::onCommit() {
    this->getSetting()->setValue(m_toggle->isToggled());
}
void BoolSettingNodeV3::onToggle(CCObject*) {
    m_toggle->toggle(!m_toggle->isToggled());
    this->markChanged();
}

bool BoolSettingNodeV3::hasUncommittedChanges() const {
    return m_toggle->isToggled() != this->getSetting()->getValue();
}
bool BoolSettingNodeV3::hasNonDefaultValue() const {
    return m_toggle->isToggled() != this->getSetting()->getDefaultValue();
}
void BoolSettingNodeV3::onResetToDefault() {
    m_toggle->toggle(this->getSetting()->getDefaultValue());
}

std::shared_ptr<BoolSettingV3> BoolSettingNodeV3::getSetting() const {
    return std::static_pointer_cast<BoolSettingV3>(SettingNodeV3::getSetting());
}

BoolSettingNodeV3* BoolSettingNodeV3::create(std::shared_ptr<BoolSettingV3> setting, float width) {
    auto ret = new BoolSettingNodeV3();
    if (ret && ret->init(setting, width)) {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

// IntSettingNodeV3

bool IntSettingNodeV3::init(std::shared_ptr<IntSettingV3> setting, float width) {
    if (!SettingNodeV3::init(setting, width))
        return false;

    auto bigArrowLeftSpr = CCSprite::create();
    auto bigArrowLeftSpr1 = CCSprite::createWithSpriteFrameName("GJ_arrow_03_001.png");
    auto bigArrowLeftSpr2 = CCSprite::createWithSpriteFrameName("GJ_arrow_03_001.png");
    
    bigArrowLeftSpr->setContentSize(bigArrowLeftSpr1->getContentSize() + ccp(20, 0));
    bigArrowLeftSpr->addChildAtPosition(bigArrowLeftSpr2, Anchor::Center, ccp(10, 0));
    bigArrowLeftSpr->addChildAtPosition(bigArrowLeftSpr1, Anchor::Center, ccp(-10, 0));
    bigArrowLeftSpr->setScale(.45f);

    auto bigArrowLeftBtn = CCMenuItemSpriteExtra::create(
        bigArrowLeftSpr, this, menu_selector(IntSettingNodeV3::onArrow)
    );
    bigArrowLeftBtn->setTag(-setting->getBigArrowStepSize());
    bigArrowLeftBtn->setVisible(setting->isBigArrowsEnabled());
    this->getButtonMenu()->addChild(bigArrowLeftBtn);

    auto arrowLeftSpr = CCSprite::createWithSpriteFrameName("GJ_arrow_01_001.png");
    arrowLeftSpr->setScale(.65f);
    auto arrowLeftBtn = CCMenuItemSpriteExtra::create(
        arrowLeftSpr, this, menu_selector(IntSettingNodeV3::onArrow)
    );
    arrowLeftBtn->setTag(-setting->getArrowStepSize());
    arrowLeftBtn->setVisible(setting->isArrowsEnabled());
    this->getButtonMenu()->addChild(arrowLeftBtn);
    
    m_input = TextInput::create(width / 2 - 70, "Num");
    m_input->setCallback([this](auto const&) {
        this->markChanged();
    });
    if (!setting->isInputEnabled()) {
        m_input->getBGSprite()->setVisible(false);
        m_input->setEnabled(false);
    }
    this->getButtonMenu()->addChild(m_input);

    auto arrowRightSpr = CCSprite::createWithSpriteFrameName("GJ_arrow_01_001.png");
    arrowRightSpr->setFlipX(true);
    arrowRightSpr->setScale(.65f);
    auto arrowRightBtn = CCMenuItemSpriteExtra::create(
        arrowRightSpr, this, menu_selector(IntSettingNodeV3::onArrow)
    );
    arrowRightBtn->setTag(setting->getArrowStepSize());
    arrowRightBtn->setVisible(setting->isArrowsEnabled());
    this->getButtonMenu()->addChild(arrowRightBtn);

    auto bigArrowRightSpr = CCSprite::create();
    auto bigArrowRightSpr1 = CCSprite::createWithSpriteFrameName("GJ_arrow_03_001.png");
    bigArrowRightSpr1->setFlipX(true);
    auto bigArrowRightSpr2 = CCSprite::createWithSpriteFrameName("GJ_arrow_03_001.png");
    bigArrowRightSpr2->setFlipX(true);
    
    bigArrowRightSpr->setContentSize(bigArrowRightSpr1->getContentSize() + ccp(20, 0));
    bigArrowRightSpr->addChildAtPosition(bigArrowRightSpr1, Anchor::Center, ccp(-10, 0));
    bigArrowRightSpr->addChildAtPosition(bigArrowRightSpr2, Anchor::Center, ccp(10, 0));
    bigArrowRightSpr->setScale(.45f);

    auto bigArrowRightBtn = CCMenuItemSpriteExtra::create(
        bigArrowRightSpr, this, menu_selector(IntSettingNodeV3::onArrow)
    );
    bigArrowRightBtn->setTag(setting->getBigArrowStepSize());
    bigArrowRightBtn->setVisible(setting->isBigArrowsEnabled());
    this->getButtonMenu()->addChild(bigArrowRightBtn);

    if (setting->isSliderEnabled()) {
        this->setContentHeight(45);
        this->getButtonMenu()->updateAnchoredPosition(Anchor::Right, ccp(0, 7));

        m_slider = Slider::create(this, menu_selector(IntSettingNodeV3::onSlider));
        m_slider->setScale(.5f);
        this->addChildAtPosition(m_slider, Anchor::Right, ccp(-75, -12), ccp(0, 0));
    }

    this->setCurrentValue(setting->getValue());
    this->getButtonMenu()->updateLayout();
    this->updateState();

    return true;
}

void IntSettingNodeV3::updateState() {
    SettingNodeV3::updateState();
    m_slider->m_touchLogic->m_thumb->setValue(valueToSlider(
        this->getSetting(), this->getCurrentValue()
    ));
    m_slider->updateBar();
}

int64_t IntSettingNodeV3::getCurrentValue() const {
    return numFromString<int64_t>(m_input->getString()).value_or(this->getSetting()->getDefaultValue());
}
void IntSettingNodeV3::setCurrentValue(int64_t value) {
    m_input->setString(std::to_string(value));
    this->markChanged();
}

void IntSettingNodeV3::onCommit() {
    this->getSetting()->setValue(this->getCurrentValue());
}
void IntSettingNodeV3::onArrow(CCObject* sender) {
    this->setCurrentValue(this->getCurrentValue() + sender->getTag());
    this->updateState();
}
void IntSettingNodeV3::onSlider(CCObject* sender) {
    this->setCurrentValue(valueFromSlider(
        this->getSetting(),
        m_slider->m_touchLogic->m_thumb->getValue()
    ));
}

bool IntSettingNodeV3::hasUncommittedChanges() const {
    return this->getSetting()->getValue() != this->getCurrentValue();
}
bool IntSettingNodeV3::hasNonDefaultValue() const {
    return this->getSetting()->getDefaultValue() != this->getCurrentValue();
}
void IntSettingNodeV3::onResetToDefault() {
    this->setCurrentValue(this->getSetting()->getDefaultValue());
}

std::shared_ptr<IntSettingV3> IntSettingNodeV3::getSetting() const {
    return std::static_pointer_cast<IntSettingV3>(SettingNodeV3::getSetting());
}

IntSettingNodeV3* IntSettingNodeV3::create(std::shared_ptr<IntSettingV3> setting, float width) {
    auto ret = new IntSettingNodeV3();
    if (ret && ret->init(setting, width)) {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

// FloatSettingNodeV3

bool FloatSettingNodeV3::init(std::shared_ptr<FloatSettingV3> setting, float width) {
    if (!SettingNodeV3::init(setting, width))
        return false;
    
    auto bigArrowLeftSpr = CCSprite::create();
    auto bigArrowLeftSpr1 = CCSprite::createWithSpriteFrameName("GJ_arrow_03_001.png");
    auto bigArrowLeftSpr2 = CCSprite::createWithSpriteFrameName("GJ_arrow_03_001.png");
    
    bigArrowLeftSpr->setContentSize(bigArrowLeftSpr1->getContentSize() + ccp(20, 0));
    bigArrowLeftSpr->addChildAtPosition(bigArrowLeftSpr2, Anchor::Center, ccp(10, 0));
    bigArrowLeftSpr->addChildAtPosition(bigArrowLeftSpr1, Anchor::Center, ccp(-10, 0));
    bigArrowLeftSpr->setScale(.45f);

    auto bigArrowLeftBtn = CCMenuItemSpriteExtra::create(
        bigArrowLeftSpr, this, menu_selector(FloatSettingNodeV3::onArrow)
    );
    bigArrowLeftBtn->setTag(-setting->getBigArrowStepSize());
    bigArrowLeftBtn->setVisible(setting->isBigArrowsEnabled());
    this->getButtonMenu()->addChild(bigArrowLeftBtn);

    auto arrowLeftSpr = CCSprite::createWithSpriteFrameName("GJ_arrow_01_001.png");
    arrowLeftSpr->setScale(.65f);
    auto arrowLeftBtn = CCMenuItemSpriteExtra::create(
        arrowLeftSpr, this, menu_selector(FloatSettingNodeV3::onArrow)
    );
    arrowLeftBtn->setTag(-setting->getArrowStepSize());
    arrowLeftBtn->setVisible(setting->isArrowsEnabled());
    this->getButtonMenu()->addChild(arrowLeftBtn);
    
    m_input = TextInput::create(width / 2 - 70, "Num");
    m_input->setCallback([this](auto const&) {
        this->markChanged();
    });
    if (!setting->isInputEnabled()) {
        m_input->getBGSprite()->setVisible(false);
        m_input->setEnabled(false);
    }
    this->getButtonMenu()->addChild(m_input);

    auto arrowRightSpr = CCSprite::createWithSpriteFrameName("GJ_arrow_01_001.png");
    arrowRightSpr->setFlipX(true);
    arrowRightSpr->setScale(.65f);
    auto arrowRightBtn = CCMenuItemSpriteExtra::create(
        arrowRightSpr, this, menu_selector(FloatSettingNodeV3::onArrow)
    );
    arrowRightBtn->setTag(setting->getArrowStepSize());
    arrowRightBtn->setVisible(setting->isArrowsEnabled());
    this->getButtonMenu()->addChild(arrowRightBtn);

    auto bigArrowRightSpr = CCSprite::create();
    auto bigArrowRightSpr1 = CCSprite::createWithSpriteFrameName("GJ_arrow_03_001.png");
    bigArrowRightSpr1->setFlipX(true);
    auto bigArrowRightSpr2 = CCSprite::createWithSpriteFrameName("GJ_arrow_03_001.png");
    bigArrowRightSpr2->setFlipX(true);
    
    bigArrowRightSpr->setContentSize(bigArrowRightSpr1->getContentSize() + ccp(20, 0));
    bigArrowRightSpr->addChildAtPosition(bigArrowRightSpr1, Anchor::Center, ccp(-10, 0));
    bigArrowRightSpr->addChildAtPosition(bigArrowRightSpr2, Anchor::Center, ccp(10, 0));
    bigArrowRightSpr->setScale(.45f);

    auto bigArrowRightBtn = CCMenuItemSpriteExtra::create(
        bigArrowRightSpr, this, menu_selector(FloatSettingNodeV3::onArrow)
    );
    bigArrowRightBtn->setTag(setting->getBigArrowStepSize());
    bigArrowRightBtn->setVisible(setting->isBigArrowsEnabled());
    this->getButtonMenu()->addChild(bigArrowRightBtn);

    if (setting->isSliderEnabled()) {
        this->setContentHeight(45);
        this->getButtonMenu()->updateAnchoredPosition(Anchor::Right, ccp(0, 7));

        m_slider = Slider::create(this, menu_selector(FloatSettingNodeV3::onSlider));
        m_slider->setScale(.5f);
        this->addChildAtPosition(m_slider, Anchor::Right, ccp(-75, -12), ccp(0, 0));
    }

    this->setCurrentValue(setting->getValue());
    this->getButtonMenu()->updateLayout();
    this->updateState();

    return true;
}

void FloatSettingNodeV3::updateState() {
    SettingNodeV3::updateState();
    m_slider->m_touchLogic->m_thumb->setValue(valueToSlider(
        this->getSetting(), this->getCurrentValue()
    ));
    m_slider->updateBar();
}

double FloatSettingNodeV3::getCurrentValue() const {
    return numFromString<double>(m_input->getString()).value_or(this->getSetting()->getDefaultValue());
}
void FloatSettingNodeV3::setCurrentValue(double value) {
    m_input->setString(numToString(value));
}

void FloatSettingNodeV3::onCommit() {
    this->getSetting()->setValue(this->getCurrentValue());
}
void FloatSettingNodeV3::onArrow(CCObject* sender) {
    this->setCurrentValue(this->getCurrentValue() + sender->getTag());
    this->updateState();
}
void FloatSettingNodeV3::onSlider(CCObject* sender) {
    this->setCurrentValue(valueFromSlider(
        this->getSetting(),
        m_slider->m_touchLogic->m_thumb->getValue()
    ));
}

bool FloatSettingNodeV3::hasUncommittedChanges() const {
    return this->getSetting()->getValue() != this->getCurrentValue();
}
bool FloatSettingNodeV3::hasNonDefaultValue() const {
    return this->getSetting()->getDefaultValue() != this->getCurrentValue();
}
void FloatSettingNodeV3::onResetToDefault() {
    this->setCurrentValue(this->getSetting()->getDefaultValue());
}

std::shared_ptr<FloatSettingV3> FloatSettingNodeV3::getSetting() const {
    return std::static_pointer_cast<FloatSettingV3>(SettingNodeV3::getSetting());
}

FloatSettingNodeV3* FloatSettingNodeV3::create(std::shared_ptr<FloatSettingV3> setting, float width) {
    auto ret = new FloatSettingNodeV3();
    if (ret && ret->init(setting, width)) {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

// StringSettingNodeV3

bool StringSettingNodeV3::init(std::shared_ptr<StringSettingV3> setting, float width) {
    if (!SettingNodeV3::init(setting, width))
        return false;
    
    // todo

    return true;
}

void StringSettingNodeV3::onCommit() {}

bool StringSettingNodeV3::hasUncommittedChanges() const {
    return false;
}
bool StringSettingNodeV3::hasNonDefaultValue() const {
    return false;
}
void StringSettingNodeV3::onResetToDefault() {}

std::shared_ptr<StringSettingV3> StringSettingNodeV3::getSetting() const {
    return std::static_pointer_cast<StringSettingV3>(SettingNodeV3::getSetting());
}

StringSettingNodeV3* StringSettingNodeV3::create(std::shared_ptr<StringSettingV3> setting, float width) {
    auto ret = new StringSettingNodeV3();
    if (ret && ret->init(setting, width)) {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

// FileSettingNodeV3

bool FileSettingNodeV3::init(std::shared_ptr<FileSettingV3> setting, float width) {
    if (!SettingNodeV3::init(setting, width))
        return false;
    
    // todo

    return true;
}

void FileSettingNodeV3::onCommit() {}

bool FileSettingNodeV3::hasUncommittedChanges() const {
    return false;
}
bool FileSettingNodeV3::hasNonDefaultValue() const {
    return false;
}
void FileSettingNodeV3::onResetToDefault() {}

std::shared_ptr<FileSettingV3> FileSettingNodeV3::getSetting() const {
    return std::static_pointer_cast<FileSettingV3>(SettingNodeV3::getSetting());
}

FileSettingNodeV3* FileSettingNodeV3::create(std::shared_ptr<FileSettingV3> setting, float width) {
    auto ret = new FileSettingNodeV3();
    if (ret && ret->init(setting, width)) {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

// Color3BSettingNodeV3

bool Color3BSettingNodeV3::init(std::shared_ptr<Color3BSettingV3> setting, float width) {
    if (!SettingNodeV3::init(setting, width))
        return false;
    
    // todo

    return true;
}

void Color3BSettingNodeV3::onCommit() {}

bool Color3BSettingNodeV3::hasUncommittedChanges() const {
    return false;
}
bool Color3BSettingNodeV3::hasNonDefaultValue() const {
    return false;
}
void Color3BSettingNodeV3::onResetToDefault() {}

std::shared_ptr<Color3BSettingV3> Color3BSettingNodeV3::getSetting() const {
    return std::static_pointer_cast<Color3BSettingV3>(SettingNodeV3::getSetting());
}

Color3BSettingNodeV3* Color3BSettingNodeV3::create(std::shared_ptr<Color3BSettingV3> setting, float width) {
    auto ret = new Color3BSettingNodeV3();
    if (ret && ret->init(setting, width)) {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

// Color4BSettingNodeV3

bool Color4BSettingNodeV3::init(std::shared_ptr<Color4BSettingV3> setting, float width) {
    if (!SettingNodeV3::init(setting, width))
        return false;
    
    // todo

    return true;
}

void Color4BSettingNodeV3::onCommit() {}

bool Color4BSettingNodeV3::hasUncommittedChanges() const {
    return false;
}
bool Color4BSettingNodeV3::hasNonDefaultValue() const {
    return false;
}
void Color4BSettingNodeV3::onResetToDefault() {}

std::shared_ptr<Color4BSettingV3> Color4BSettingNodeV3::getSetting() const {
    return std::static_pointer_cast<Color4BSettingV3>(SettingNodeV3::getSetting());
}

Color4BSettingNodeV3* Color4BSettingNodeV3::create(std::shared_ptr<Color4BSettingV3> setting, float width) {
    auto ret = new Color4BSettingNodeV3();
    if (ret && ret->init(setting, width)) {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

// UnresolvedCustomSettingNodeV3

bool UnresolvedCustomSettingNodeV3::init(std::shared_ptr<LegacyCustomSettingV3> setting, float width) {
    if (!SettingNodeV3::init(setting, width))
        return false;
    
    this->setContentHeight(30);
    
    auto label = CCLabelBMFont::create(
        fmt::format("Missing setting '{}'", setting->getKey()).c_str(),
        "bigFont.fnt"
    );
    label->limitLabelWidth(width - m_obContentSize.height, .5f, .1f);
    this->addChildAtPosition(label, Anchor::Left, ccp(m_obContentSize.height / 2, 0));
    
    return true;
}

void UnresolvedCustomSettingNodeV3::onCommit() {}

bool UnresolvedCustomSettingNodeV3::hasUncommittedChanges() const {
    return false;
}
bool UnresolvedCustomSettingNodeV3::hasNonDefaultValue() const {
    return false;
}
void UnresolvedCustomSettingNodeV3::onResetToDefault() {}

std::shared_ptr<LegacyCustomSettingV3> UnresolvedCustomSettingNodeV3::getSetting() const {
    return std::static_pointer_cast<LegacyCustomSettingV3>(SettingNodeV3::getSetting());
}

UnresolvedCustomSettingNodeV3* UnresolvedCustomSettingNodeV3::create(std::shared_ptr<LegacyCustomSettingV3> setting, float width) {
    auto ret = new UnresolvedCustomSettingNodeV3();
    if (ret && ret->init(setting, width)) {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

// LegacyCustomSettingToV3Node

bool LegacyCustomSettingToV3Node::init(std::shared_ptr<LegacyCustomSettingV3> original, float width) {
    if (!SettingNodeV3::init(original, width))
        return false;

    m_original = original->getValue()->createNode(width);
    this->setContentSize({ width, m_original->getContentHeight() });
    this->addChildAtPosition(m_original, Anchor::Center);
    
    return true;
}

void LegacyCustomSettingToV3Node::onCommit() {
    m_original->commit();
}

bool LegacyCustomSettingToV3Node::hasUncommittedChanges() const {
    return m_original->hasUncommittedChanges();
}
bool LegacyCustomSettingToV3Node::hasNonDefaultValue() const {
    return m_original->hasNonDefaultValue();
}
void LegacyCustomSettingToV3Node::onResetToDefault() {
    m_original->resetToDefault();
}

LegacyCustomSettingToV3Node* LegacyCustomSettingToV3Node::create(std::shared_ptr<LegacyCustomSettingV3> original, float width) {
    auto ret = new LegacyCustomSettingToV3Node();
    if (ret && ret->init(original, width)) {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}
