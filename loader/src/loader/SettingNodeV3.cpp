#include "SettingNodeV3.hpp"
#include <Geode/loader/SettingNode.hpp>
#include <Geode/utils/ColorProvider.hpp>
#include <Geode/utils/ranges.hpp>
#include <Geode/loader/Dirs.hpp>
#include <ui/mods/GeodeStyle.hpp>

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

SettingNodeV3* SettingNodeSizeChangeEventV3::getNode() const {
    return m_impl->node;
}

class SettingNodeValueChangeEventV3::Impl final {
public:
    SettingNodeV3* node;
    bool commit = false;
};

SettingNodeValueChangeEventV3::SettingNodeValueChangeEventV3(SettingNodeV3* node, bool commit)
  : m_impl(std::make_shared<Impl>())
{
    m_impl->node = node;
    m_impl->commit = commit;
}
SettingNodeValueChangeEventV3::~SettingNodeValueChangeEventV3() = default;

SettingNodeV3* SettingNodeValueChangeEventV3::getNode() const {
    return m_impl->node;
}
bool SettingNodeValueChangeEventV3::isCommit() const {
    return m_impl->commit;
}

class SettingNodeV3::Impl final {
public:
    std::shared_ptr<SettingV3> setting;
    CCLayerColor* bg;
    CCLabelBMFont* nameLabel;
    CCMenu* nameMenu;
    CCMenu* buttonMenu;
    CCMenuItemSpriteExtra* resetButton;
    CCLabelBMFont* statusLabel;
    ccColor4B bgColor = ccc4(0, 0, 0, 0);
    bool committed = false;
};

bool SettingNodeV3::init(std::shared_ptr<SettingV3> setting, float width) {
    if (!CCNode::init())
        return false;

    // note: setting may be null due to UnresolvedCustomSettingNodeV3
    
    m_impl = std::make_shared<Impl>();
    m_impl->setting = setting;

    m_impl->bg = CCLayerColor::create({ 0, 0, 0, 0 });
    m_impl->bg->setContentSize({ width, 0 });
    m_impl->bg->ignoreAnchorPointForPosition(false);
    m_impl->bg->setAnchorPoint(ccp(.5f, .5f));
    this->addChildAtPosition(m_impl->bg, Anchor::Center);
    
    m_impl->nameMenu = CCMenu::create();
    m_impl->nameMenu->setContentWidth(width / 2 + 25);

    m_impl->nameLabel = CCLabelBMFont::create(setting ? setting->getDisplayName().c_str() : "", "bigFont.fnt");
    m_impl->nameLabel->setLayoutOptions(AxisLayoutOptions::create()->setScaleLimits(.1f, .4f)->setScalePriority(1));
    m_impl->nameMenu->addChild(m_impl->nameLabel);

    m_impl->statusLabel = CCLabelBMFont::create("", "bigFont.fnt");
    m_impl->statusLabel->setScale(.25f);
    this->addChildAtPosition(m_impl->statusLabel, Anchor::Left, ccp(10, -10), ccp(0, .5f));

    if (setting && setting->getDescription()) {
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
    m_impl->nameMenu->getLayout()->ignoreInvisibleChildren(true);
    this->addChildAtPosition(m_impl->nameMenu, Anchor::Left, ccp(10, 0), ccp(0, .5f));

    m_impl->buttonMenu = CCMenu::create();
    m_impl->buttonMenu->setContentSize({ width / 2 - 55, 30 });
    m_impl->buttonMenu->setLayout(AnchorLayout::create());
    this->addChildAtPosition(m_impl->buttonMenu, Anchor::Right, ccp(-10, 0), ccp(1, .5f));

    this->setAnchorPoint({ .5f, .5f });
    this->setContentSize({ width, 30 });

    return true;
}

void SettingNodeV3::updateState(CCNode* invoker) {
    m_impl->statusLabel->setVisible(false);

    m_impl->nameLabel->setColor(this->hasUncommittedChanges() ? ccc3(17, 221, 0) : ccWHITE);
    m_impl->resetButton->setVisible(this->hasNonDefaultValue());

    m_impl->bg->setColor(to3B(m_impl->bgColor));
    m_impl->bg->setOpacity(m_impl->bgColor.a);

    if (m_impl->setting && !m_impl->setting->shouldEnable()) {
        if (auto desc = m_impl->setting->getEnableIfDescription()) {
            m_impl->nameLabel->setColor(ccGRAY);
            m_impl->statusLabel->setVisible(true);
            m_impl->statusLabel->setColor("mod-list-errors-found"_cc3b);
            m_impl->statusLabel->setString(desc->c_str());
        }
    }
    if (m_impl->setting && m_impl->setting->requiresRestart() && m_impl->committed) {
        m_impl->statusLabel->setVisible(true);
        m_impl->statusLabel->setColor("mod-list-restart-required-label"_cc3b);
        m_impl->statusLabel->setString("Restart Required");
        m_impl->bg->setColor("mod-list-restart-required-label-bg"_cc3b);
        m_impl->bg->setOpacity(75);
    }

    m_impl->nameMenu->setContentWidth(this->getContentWidth() - m_impl->buttonMenu->getContentWidth() - 20);
    m_impl->nameMenu->updateLayout();
}

void SettingNodeV3::onDescription(CCObject*) {
    auto title = m_impl->setting->getDisplayName();
    FLAlertLayer::create(
        nullptr,
        title.c_str(),
        m_impl->setting->getDescription().value_or("No description provided"),
        "OK", nullptr,
        clamp(title.size() * 16, 300, 400)
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

void SettingNodeV3::setDefaultBGColor(ccColor4B color) {
    m_impl->bgColor = color;
    this->updateState(nullptr);
}

void SettingNodeV3::markChanged(CCNode* invoker) {
    this->updateState(invoker);
    SettingNodeValueChangeEventV3(this, false).post();
}
void SettingNodeV3::commit() {
    this->onCommit();
    m_impl->committed = true;
    this->updateState(nullptr);
    SettingNodeValueChangeEventV3(this, true).post();
}
void SettingNodeV3::resetToDefault() {
    if (!m_impl->setting) return;
    m_impl->setting->reset();
    m_impl->committed = true;
    this->onResetToDefault();
    this->updateState(nullptr);
    SettingNodeValueChangeEventV3(this, false).post();
}

void SettingNodeV3::setContentSize(CCSize const& size) {
    CCNode::setContentSize(size);
    m_impl->bg->setContentSize(size);
    this->updateLayout();
    SettingNodeSizeChangeEventV3(this).post();
}

CCLabelBMFont* SettingNodeV3::getNameLabel() const {
    return m_impl->nameLabel;
}
CCLabelBMFont* SettingNodeV3::getStatusLabel() const {
    return m_impl->statusLabel;
}
CCMenu* SettingNodeV3::getNameMenu() const {
    return m_impl->nameMenu;
}
CCMenu* SettingNodeV3::getButtonMenu() const {
    return m_impl->buttonMenu;
}
CCLayerColor* SettingNodeV3::getBG() const {
    return m_impl->bg;
}

std::shared_ptr<SettingV3> SettingNodeV3::getSetting() const {
    return m_impl->setting;
}

// TitleSettingNodeV3

bool TitleSettingNodeV3::init(std::shared_ptr<TitleSettingV3> setting, float width) {
    if (!SettingNodeV3::init(setting, width))
        return false;
    
    auto collapseSprBG = CCSprite::create("square02c_001.png");
    collapseSprBG->setColor(ccc3(25, 25, 25));
    collapseSprBG->setOpacity(105);
    auto collapseSpr = CCSprite::createWithSpriteFrameName("edit_downBtn_001.png");
    collapseSpr->setScale(1.9f);
    collapseSprBG->addChildAtPosition(collapseSpr, Anchor::Center);
    collapseSprBG->setScale(.2f);

    auto uncollapseSprBG = CCSprite::create("square02c_001.png");
    uncollapseSprBG->setColor(ccc3(25, 25, 25));
    uncollapseSprBG->setOpacity(105);
    auto uncollapseSpr = CCSprite::createWithSpriteFrameName("edit_delCBtn_001.png");
    uncollapseSpr->setScale(1.5f);
    uncollapseSprBG->addChildAtPosition(uncollapseSpr, Anchor::Center);
    uncollapseSprBG->setScale(.2f);
    
    m_collapseToggle = CCMenuItemToggler::create(
        collapseSprBG, uncollapseSprBG,
        this, menu_selector(TitleSettingNodeV3::onCollapse)
    );
    m_collapseToggle->m_notClickable = true;
    this->getButtonMenu()->setContentWidth(20);
    this->getButtonMenu()->addChildAtPosition(m_collapseToggle, Anchor::Center);
    
    this->getNameLabel()->setFntFile("goldFont.fnt");
    this->getNameMenu()->updateLayout();
    this->setContentHeight(20);
    this->updateState(nullptr);
    
    return true;
}

void TitleSettingNodeV3::onCollapse(CCObject* sender) {
    m_collapseToggle->toggle(!m_collapseToggle->isToggled());
    // This triggers popup state to update due to SettingNodeValueChangeEventV3 being posted
    this->markChanged(static_cast<CCNode*>(sender));
}
void TitleSettingNodeV3::onCommit() {}

bool TitleSettingNodeV3::isCollapsed() const {
    return m_collapseToggle->isToggled();
}

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
    if (!SettingValueNodeV3::init(setting, width))
        return false;

    this->getButtonMenu()->setContentWidth(20);
    
    m_toggle = CCMenuItemToggler::createWithStandardSprites(
        this, menu_selector(BoolSettingNodeV3::onToggle), .55f
    );
    m_toggle->m_onButton->setContentSize({ 25, 25 });
    m_toggle->m_onButton->getNormalImage()->setPosition(ccp(25, 25) / 2);
    m_toggle->m_offButton->setContentSize({ 25, 25 });
    m_toggle->m_offButton->getNormalImage()->setPosition(ccp(25, 25) / 2);
    m_toggle->m_notClickable = true;
    m_toggle->toggle(setting->getValue());
    this->getButtonMenu()->addChildAtPosition(m_toggle, Anchor::Right, ccp(-10, 0));

    this->updateState(nullptr);

    return true;
}

void BoolSettingNodeV3::updateState(CCNode* invoker) {
    SettingValueNodeV3::updateState(invoker);
    auto enable = this->getSetting()->shouldEnable();
    m_toggle->toggle(this->getValue());
    m_toggle->setCascadeColorEnabled(true);
    m_toggle->setCascadeOpacityEnabled(true);
    m_toggle->setEnabled(enable);
    m_toggle->setColor(enable ? ccWHITE : ccGRAY);
    m_toggle->setOpacity(enable ? 255 : 155);
}

void BoolSettingNodeV3::onToggle(CCObject*) {
    this->setValue(!m_toggle->isToggled(), m_toggle);
    this->markChanged(m_toggle);
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

// StringSettingNodeV3

bool StringSettingNodeV3::init(std::shared_ptr<StringSettingV3> setting, float width) {
    if (!SettingValueNodeV3::init(setting, width))
        return false;

    m_input = TextInput::create(setting->getEnumOptions() ? width / 2 - 50 : width / 2, "Text");
    m_input->setCallback([this](auto const& str) {
        this->setValue(str, m_input);
    });
    m_input->setScale(.7f);
    m_input->setString(this->getSetting()->getValue());
    this->getButtonMenu()->addChildAtPosition(m_input, Anchor::Center);
    
    if (setting->getEnumOptions()) {
        m_input->getBGSprite()->setVisible(false);
        m_input->setEnabled(false);
        m_input->getInputNode()->m_placeholderLabel->setOpacity(255);
        m_input->getInputNode()->m_placeholderLabel->setColor(ccWHITE);
        
        m_arrowLeftSpr = CCSprite::createWithSpriteFrameName("navArrowBtn_001.png");
        m_arrowLeftSpr->setFlipX(true);
        m_arrowLeftSpr->setScale(.4f);
        auto arrowLeftBtn = CCMenuItemSpriteExtra::create(
            m_arrowLeftSpr, this, menu_selector(StringSettingNodeV3::onArrow)
        );
        arrowLeftBtn->setTag(-1);
        this->getButtonMenu()->addChildAtPosition(arrowLeftBtn, Anchor::Left, ccp(5, 0));

        m_arrowRightSpr = CCSprite::createWithSpriteFrameName("navArrowBtn_001.png");
        m_arrowRightSpr->setScale(.4f);
        auto arrowRightBtn = CCMenuItemSpriteExtra::create(
            m_arrowRightSpr, this, menu_selector(StringSettingNodeV3::onArrow)
        );
        arrowRightBtn->setTag(1);
        this->getButtonMenu()->addChildAtPosition(arrowRightBtn, Anchor::Right, ccp(-5, 0));
    }

    this->updateState(nullptr);

    return true;
}

void StringSettingNodeV3::updateState(CCNode* invoker) {
    SettingValueNodeV3::updateState(invoker);

    if (invoker != m_input) {
        m_input->setString(this->getValue());
    }

    auto enable = this->getSetting()->shouldEnable();
    if (!this->getSetting()->getEnumOptions()) {
        m_input->setEnabled(enable);
    }
    else {
        m_arrowRightSpr->setOpacity(enable ? 255 : 155);
        m_arrowRightSpr->setColor(enable ? ccWHITE : ccGRAY);
        m_arrowLeftSpr->setOpacity(enable ? 255 : 155);
        m_arrowLeftSpr->setColor(enable ? ccWHITE : ccGRAY);
    }
}

void StringSettingNodeV3::onArrow(CCObject* sender) {
    auto options = *this->getSetting()->getEnumOptions();
    auto index = ranges::indexOf(options, this->getValue()).value_or(0);
    if (sender->getTag() > 0) {
        index = index < options.size() - 1 ? index + 1 : 0;
    }
    else {
        index = index > 0 ? index - 1 : options.size() - 1;
    }
    this->setValue(options.at(index), static_cast<CCNode*>(sender));
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
    if (!SettingValueNodeV3::init(setting, width))
        return false;
    
    auto labelBG = extension::CCScale9Sprite::create("square02b_001.png", { 0, 0, 80, 80 });
    labelBG->setScale(.25f);
    labelBG->setColor({ 0, 0, 0 });
    labelBG->setOpacity(90);
    labelBG->setContentSize({ 420, 80 });
    this->getButtonMenu()->addChildAtPosition(labelBG, Anchor::Center, ccp(-10, 0));

    m_fileIcon = CCSprite::create();
    this->getButtonMenu()->addChildAtPosition(m_fileIcon, Anchor::Left, ccp(5, 0));

    m_nameLabel = CCLabelBMFont::create("", "bigFont.fnt");
    this->getButtonMenu()->addChildAtPosition(m_nameLabel, Anchor::Left, ccp(13, 0), ccp(0, .5f));

    m_selectBtnSpr = CCSprite::createWithSpriteFrameName("GJ_plus2Btn_001.png");
    m_selectBtnSpr->setScale(.7f);
    m_selectBtn = CCMenuItemSpriteExtra::create(
        m_selectBtnSpr, this, menu_selector(FileSettingNodeV3::onPickFile)
    );
    this->getButtonMenu()->addChildAtPosition(m_selectBtn, Anchor::Right, ccp(-5, 0));

    this->updateState(nullptr);

    return true;
}

void FileSettingNodeV3::updateState(CCNode* invoker) {
    // This is because people tend to put `"default": "Please pick a good file"` 
    // which is clever and good UX but also a hack so I also need to hack to support that
    const auto isTextualDefaultValue = [this, setting = this->getSetting()]() {
        if (this->hasNonDefaultValue()) return false;
        std::error_code ec;
        return setting->isFolder() ? 
            !std::filesystem::is_directory(setting->getDefaultValue(), ec) :
            !std::filesystem::is_regular_file(setting->getDefaultValue(), ec);
    }();

    SettingValueNodeV3::updateState(invoker);
    m_fileIcon->setDisplayFrame(CCSpriteFrameCache::get()->spriteFrameByName(
        this->getSetting()->isFolder() ? "folderIcon_001.png" : "file.png"_spr
    ));
    limitNodeSize(m_fileIcon, ccp(10, 10), 1.f, .1f);
    if (this->getValue().empty() || isTextualDefaultValue) {
        if (isTextualDefaultValue) {
            m_nameLabel->setString(this->getSetting()->getDefaultValue().string().c_str());
        }
        else {
            m_nameLabel->setString(this->getSetting()->isFolder() ? "No Folder Selected" : "No File Selected");
        }
        m_nameLabel->setColor(ccGRAY);
        m_nameLabel->setOpacity(155);
    }
    else {
        m_nameLabel->setString(this->getValue().filename().string().c_str());
        m_nameLabel->setColor(ccWHITE);
        m_nameLabel->setOpacity(255);
    }
    m_nameLabel->limitLabelWidth(75, .35f, .1f);

    auto enable = this->getSetting()->shouldEnable();
    m_selectBtnSpr->setOpacity(enable ? 255 : 155);
    m_selectBtnSpr->setColor(enable ? ccWHITE : ccGRAY);
    m_selectBtn->setEnabled(enable);
}

void FileSettingNodeV3::onPickFile(CCObject*) {
    m_pickListener.bind([this](auto* event) {
        auto value = event->getValue();
        if (!value) {
            return;
        }
        if (value->isOk()) {
            this->setValue(value->unwrap(), nullptr);
        }
        else {
            FLAlertLayer::create(
                "Failed",
                fmt::format("Failed to pick file: {}", value->unwrapErr()),
                "Ok"
            )->show();
        }
    });
    std::error_code ec;
    m_pickListener.setFilter(file::pick(
        this->getSetting()->isFolder() ? 
            file::PickMode::OpenFolder : 
            (this->getSetting()->useSaveDialog() ? file::PickMode::SaveFile : file::PickMode::OpenFile), 
        {
            // Prefer opening the current path directly if possible
            this->getValue().empty() || !std::filesystem::exists(this->getValue().parent_path(), ec) ? 
                dirs::getGameDir() : 
                this->getValue(),
            this->getSetting()->getFilters().value_or(std::vector<file::FilePickOptions::Filter>())
        }
    ));
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
    if (!SettingValueNodeV3::init(setting, width))
        return false;
    
    m_colorSprite = ColorChannelSprite::create();
    m_colorSprite->setScale(.65f);

    m_colorBtn = CCMenuItemSpriteExtra::create(
        m_colorSprite, this, menu_selector(Color3BSettingNodeV3::onSelectColor)
    );
    this->getButtonMenu()->addChildAtPosition(m_colorBtn, Anchor::Right, ccp(-10, 0));

    this->updateState(nullptr);

    return true;
}

void Color3BSettingNodeV3::updateState(CCNode* invoker) {
    SettingValueNodeV3::updateState(invoker);
    m_colorSprite->setColor(this->getValue());
    
    auto enable = this->getSetting()->shouldEnable();
    m_colorSprite->setOpacity(enable ? 255 : 155);
    m_colorBtn->setEnabled(enable);
}

void Color3BSettingNodeV3::onSelectColor(CCObject*) {
    auto popup = ColorPickPopup::create(this->getValue());
    popup->setDelegate(this);
    popup->show();
}
void Color3BSettingNodeV3::updateColor(ccColor4B const& color) {
    this->setValue(to3B(color), nullptr);
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
    if (!SettingValueNodeV3::init(setting, width))
        return false;
    
    m_colorSprite = ColorChannelSprite::create();
    m_colorSprite->setScale(.65f);

    m_colorBtn = CCMenuItemSpriteExtra::create(
        m_colorSprite, this, menu_selector(Color4BSettingNodeV3::onSelectColor)
    );
    this->getButtonMenu()->addChildAtPosition(m_colorBtn, Anchor::Right, ccp(-10, 0));

    this->updateState(nullptr);

    return true;
}

void Color4BSettingNodeV3::updateState(CCNode* invoker) {
    SettingValueNodeV3::updateState(invoker);
    m_colorSprite->setColor(to3B(this->getValue()));
    m_colorSprite->updateOpacity(this->getValue().a / 255.f);
    
    auto enable = this->getSetting()->shouldEnable();
    m_colorSprite->setOpacity(enable ? 255 : 155);
    m_colorBtn->setEnabled(enable);
}

void Color4BSettingNodeV3::onSelectColor(CCObject*) {
    auto popup = ColorPickPopup::create(this->getValue());
    popup->setDelegate(this);
    popup->show();
}
void Color4BSettingNodeV3::updateColor(ccColor4B const& color) {
    this->setValue(color, nullptr);
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

bool UnresolvedCustomSettingNodeV3::init(std::string_view key, Mod* mod, float width) {
    if (!SettingNodeV3::init(nullptr, width))
        return false;
    
    m_mod = mod;
    
    this->setContentHeight(30);

    auto label = CCLabelBMFont::create(
        (mod && mod->isEnabled() ?
            fmt::format("Missing setting '{}'", key) : 
            fmt::format("Enable the Mod to Edit '{}'", key)
        ).c_str(),
        "bigFont.fnt"
    );
    label->setColor(mod && mod->isEnabled() ? "mod-list-errors-found-2"_cc3b : "mod-list-gray"_cc3b);
    label->limitLabelWidth(width - m_obContentSize.height, .3f, .1f);
    this->addChildAtPosition(label, Anchor::Left, ccp(m_obContentSize.height / 2, 0), ccp(0, .5f));

    return true;
}

void UnresolvedCustomSettingNodeV3::updateState(CCNode* invoker) {
    SettingNodeV3::updateState(invoker);
    this->getBG()->setColor(m_mod && m_mod->isEnabled() ? "mod-list-errors-found-2"_cc3b : "mod-list-gray"_cc3b);
    this->getBG()->setOpacity(75);
}

void UnresolvedCustomSettingNodeV3::onCommit() {}

bool UnresolvedCustomSettingNodeV3::hasUncommittedChanges() const {
    return false;
}
bool UnresolvedCustomSettingNodeV3::hasNonDefaultValue() const {
    return false;
}
void UnresolvedCustomSettingNodeV3::onResetToDefault() {}

UnresolvedCustomSettingNodeV3* UnresolvedCustomSettingNodeV3::create(std::string_view key, Mod* mod, float width) {
    auto ret = new UnresolvedCustomSettingNodeV3();
    if (ret && ret->init(key, mod, width)) {
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
    
    this->getNameMenu()->setVisible(false);
    this->getButtonMenu()->setVisible(false);

    m_original = original->getValue()->createNode(width);
    m_original->setDelegate(this);
    this->setContentSize({ width, m_original->getContentHeight() });
    this->addChildAtPosition(m_original, Anchor::BottomLeft, ccp(0, 0), ccp(0, 0));
    
    return true;
}

void LegacyCustomSettingToV3Node::settingValueChanged(SettingNode*) {
    SettingNodeValueChangeEventV3(this, false).post();
}
void LegacyCustomSettingToV3Node::settingValueCommitted(SettingNode*) {
    SettingNodeValueChangeEventV3(this, true).post();
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
