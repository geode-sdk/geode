#include "SettingNodeV3.hpp"
#include <Geode/loader/SettingNode.hpp>

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
};

bool SettingNodeV3::init(std::shared_ptr<SettingV3> setting, float width) {
    if (!CCNode::init())
        return false;
    
    m_impl = std::make_shared<Impl>();
    m_impl->setting = setting;

    return true;
}

void SettingNodeV3::markChanged() {
    SettingNodeValueChangeEventV3(false).post();
}

void SettingNodeV3::commit() {
    this->onCommit();
    SettingNodeValueChangeEventV3(true).post();
}

void SettingNodeV3::setContentSize(CCSize const& size) {
    CCNode::setContentSize(size);
    SettingNodeSizeChangeEventV3(this).post();
}

std::shared_ptr<SettingV3> SettingNodeV3::getSetting() const {
    return m_impl->setting;
}

// TitleSettingNodeV3

bool TitleSettingNodeV3::init(std::shared_ptr<TitleSettingV3> setting, float width) {
    if (!SettingNodeV3::init(setting, width))
        return false;
    
    this->setContentHeight(20);
    
    auto label = CCLabelBMFont::create(setting->getTitle().c_str(), "goldFont.fnt");
    label->limitLabelWidth(width - m_obContentSize.height, .7f, .1f);
    this->addChildAtPosition(label, Anchor::Left, ccp(m_obContentSize.height / 2, 0));
    
    return true;
}

void TitleSettingNodeV3::onCommit() {}

TitleSettingNodeV3* TitleSettingNodeV3::create(std::shared_ptr<TitleSettingV3> setting, float width) {
    auto ret = new TitleSettingNodeV3();
    if (ret && ret->init(setting, width)) {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

bool TitleSettingNodeV3::hasUncommittedChanges() const {
    return false;
}
bool TitleSettingNodeV3::hasNonDefaultValue() const {
    return false;
}
void TitleSettingNodeV3::resetToDefault() {}

std::shared_ptr<TitleSettingV3> TitleSettingNodeV3::getSetting() const {
    return std::static_pointer_cast<TitleSettingV3>(SettingNodeV3::getSetting());
}

// BoolSettingNodeV3

bool BoolSettingNodeV3::init(std::shared_ptr<BoolSettingV3> setting, float width) {
    if (!SettingNodeV3::init(setting, width))
        return false;
    
    this->setContentHeight(30);

    auto label = CCLabelBMFont::create(setting->getName().c_str(), "bigFont.fnt");
    label->limitLabelWidth(width - m_obContentSize.height * 1.5f, .5f, .1f);
    this->addChildAtPosition(label, Anchor::Left, ccp(m_obContentSize.height / 2, 0));
    
    m_toggle = CCMenuItemToggler::createWithStandardSprites(
        this, nullptr, .8f
    );
    this->addChildAtPosition(m_toggle, Anchor::Right, ccp(-m_obContentSize.height / 2, 0));

    return true;
}

void BoolSettingNodeV3::onCommit() {
    this->getSetting()->setValue(m_toggle->isToggled());
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

bool BoolSettingNodeV3::hasUncommittedChanges() const {
    return m_toggle->isToggled() != this->getSetting()->getValue();
}
bool BoolSettingNodeV3::hasNonDefaultValue() const {
    return m_toggle->isToggled() != this->getSetting()->getDefaultValue();
}
void BoolSettingNodeV3::resetToDefault() {
    this->getSetting()->reset();
    m_toggle->toggle(this->getSetting()->getDefaultValue());
}

std::shared_ptr<BoolSettingV3> BoolSettingNodeV3::getSetting() const {
    return std::static_pointer_cast<BoolSettingV3>(SettingNodeV3::getSetting());
}

// IntSettingNodeV3

bool IntSettingNodeV3::init(std::shared_ptr<IntSettingV3> setting, float width) {
    if (!SettingNodeV3::init(setting, width))
        return false;
    
    // todo

    return true;
}

void IntSettingNodeV3::onCommit() {}

IntSettingNodeV3* IntSettingNodeV3::create(std::shared_ptr<IntSettingV3> setting, float width) {
    auto ret = new IntSettingNodeV3();
    if (ret && ret->init(setting, width)) {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

bool IntSettingNodeV3::hasUncommittedChanges() const {
    return false;
}
bool IntSettingNodeV3::hasNonDefaultValue() const {
    return false;
}
void IntSettingNodeV3::resetToDefault() {}

std::shared_ptr<IntSettingV3> IntSettingNodeV3::getSetting() const {
    return std::static_pointer_cast<IntSettingV3>(SettingNodeV3::getSetting());
}

// FloatSettingNodeV3

bool FloatSettingNodeV3::init(std::shared_ptr<FloatSettingV3> setting, float width) {
    if (!SettingNodeV3::init(setting, width))
        return false;
    
    // todo

    return true;
}

void FloatSettingNodeV3::onCommit() {}

FloatSettingNodeV3* FloatSettingNodeV3::create(std::shared_ptr<FloatSettingV3> setting, float width) {
    auto ret = new FloatSettingNodeV3();
    if (ret && ret->init(setting, width)) {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

bool FloatSettingNodeV3::hasUncommittedChanges() const {
    return false;
}
bool FloatSettingNodeV3::hasNonDefaultValue() const {
    return false;
}
void FloatSettingNodeV3::resetToDefault() {}

std::shared_ptr<FloatSettingV3> FloatSettingNodeV3::getSetting() const {
    return std::static_pointer_cast<FloatSettingV3>(SettingNodeV3::getSetting());
}

// StringSettingNodeV3

bool StringSettingNodeV3::init(std::shared_ptr<StringSettingV3> setting, float width) {
    if (!SettingNodeV3::init(setting, width))
        return false;
    
    // todo

    return true;
}

void StringSettingNodeV3::onCommit() {}

StringSettingNodeV3* StringSettingNodeV3::create(std::shared_ptr<StringSettingV3> setting, float width) {
    auto ret = new StringSettingNodeV3();
    if (ret && ret->init(setting, width)) {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

bool StringSettingNodeV3::hasUncommittedChanges() const {
    return false;
}
bool StringSettingNodeV3::hasNonDefaultValue() const {
    return false;
}
void StringSettingNodeV3::resetToDefault() {}

std::shared_ptr<StringSettingV3> StringSettingNodeV3::getSetting() const {
    return std::static_pointer_cast<StringSettingV3>(SettingNodeV3::getSetting());
}

// FileSettingNodeV3

bool FileSettingNodeV3::init(std::shared_ptr<FileSettingV3> setting, float width) {
    if (!SettingNodeV3::init(setting, width))
        return false;
    
    // todo

    return true;
}

void FileSettingNodeV3::onCommit() {}

FileSettingNodeV3* FileSettingNodeV3::create(std::shared_ptr<FileSettingV3> setting, float width) {
    auto ret = new FileSettingNodeV3();
    if (ret && ret->init(setting, width)) {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

bool FileSettingNodeV3::hasUncommittedChanges() const {
    return false;
}
bool FileSettingNodeV3::hasNonDefaultValue() const {
    return false;
}
void FileSettingNodeV3::resetToDefault() {}

std::shared_ptr<FileSettingV3> FileSettingNodeV3::getSetting() const {
    return std::static_pointer_cast<FileSettingV3>(SettingNodeV3::getSetting());
}

// Color3BSettingNodeV3

bool Color3BSettingNodeV3::init(std::shared_ptr<Color3BSettingV3> setting, float width) {
    if (!SettingNodeV3::init(setting, width))
        return false;
    
    // todo

    return true;
}

void Color3BSettingNodeV3::onCommit() {}

Color3BSettingNodeV3* Color3BSettingNodeV3::create(std::shared_ptr<Color3BSettingV3> setting, float width) {
    auto ret = new Color3BSettingNodeV3();
    if (ret && ret->init(setting, width)) {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

bool Color3BSettingNodeV3::hasUncommittedChanges() const {
    return false;
}
bool Color3BSettingNodeV3::hasNonDefaultValue() const {
    return false;
}
void Color3BSettingNodeV3::resetToDefault() {}

std::shared_ptr<Color3BSettingV3> Color3BSettingNodeV3::getSetting() const {
    return std::static_pointer_cast<Color3BSettingV3>(SettingNodeV3::getSetting());
}

// Color4BSettingNodeV3

bool Color4BSettingNodeV3::init(std::shared_ptr<Color4BSettingV3> setting, float width) {
    if (!SettingNodeV3::init(setting, width))
        return false;
    
    // todo

    return true;
}

void Color4BSettingNodeV3::onCommit() {}

Color4BSettingNodeV3* Color4BSettingNodeV3::create(std::shared_ptr<Color4BSettingV3> setting, float width) {
    auto ret = new Color4BSettingNodeV3();
    if (ret && ret->init(setting, width)) {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

bool Color4BSettingNodeV3::hasUncommittedChanges() const {
    return false;
}
bool Color4BSettingNodeV3::hasNonDefaultValue() const {
    return false;
}
void Color4BSettingNodeV3::resetToDefault() {}

std::shared_ptr<Color4BSettingV3> Color4BSettingNodeV3::getSetting() const {
    return std::static_pointer_cast<Color4BSettingV3>(SettingNodeV3::getSetting());
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

UnresolvedCustomSettingNodeV3* UnresolvedCustomSettingNodeV3::create(std::shared_ptr<LegacyCustomSettingV3> setting, float width) {
    auto ret = new UnresolvedCustomSettingNodeV3();
    if (ret && ret->init(setting, width)) {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

bool UnresolvedCustomSettingNodeV3::hasUncommittedChanges() const {
    return false;
}
bool UnresolvedCustomSettingNodeV3::hasNonDefaultValue() const {
    return false;
}
void UnresolvedCustomSettingNodeV3::resetToDefault() {}

std::shared_ptr<LegacyCustomSettingV3> UnresolvedCustomSettingNodeV3::getSetting() const {
    return std::static_pointer_cast<LegacyCustomSettingV3>(SettingNodeV3::getSetting());
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

LegacyCustomSettingToV3Node* LegacyCustomSettingToV3Node::create(std::shared_ptr<LegacyCustomSettingV3> original, float width) {
    auto ret = new LegacyCustomSettingToV3Node();
    if (ret && ret->init(original, width)) {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

bool LegacyCustomSettingToV3Node::hasUncommittedChanges() const {
    return m_original->hasUncommittedChanges();
}
bool LegacyCustomSettingToV3Node::hasNonDefaultValue() const {
    return m_original->hasNonDefaultValue();
}
void LegacyCustomSettingToV3Node::resetToDefault() {
    m_original->resetToDefault();
}
