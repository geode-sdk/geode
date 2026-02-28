#include "SettingNodeV3.hpp"
#include <Geode/utils/ColorProvider.hpp>
#include <Geode/utils/ranges.hpp>
#include <Geode/loader/Dirs.hpp>
#include <Geode/ui/MDPopup.hpp>
#include <Geode/ui/Scrollbar.hpp>
#include "KeybindEditPopup.hpp"

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
    // This is because you can create `TitleSettingNodeV3`s without having an
    // actual `TitleSettingV3`
    std::optional<std::string> customDescription;
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

    m_impl->nameMenu->setContentWidth(this->getContentWidth() - m_impl->buttonMenu->getContentWidth() - 25);
    m_impl->nameMenu->updateLayout();
}
void SettingNodeV3::updateState2(CCNode* invoker) {
    return this->updateState(invoker);
}

void SettingNodeV3::onDescription(CCObject*) {
    if (!m_impl->setting) return;
    auto title = m_impl->setting->getDisplayName();
    MDPopup::create(true,
        title.c_str(),
        m_impl->setting->getDescription().value_or("No description provided"),
        "OK"
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
    SettingNodeValueChangeEventV3(
        m_impl->setting ? m_impl->setting->getModID() : "",
        m_impl->setting ? m_impl->setting->getKey() : ""
    ).send(this, false);
}
void SettingNodeV3::commit() {
    if (!m_impl->setting) return;
    this->onCommit();
    m_impl->committed = true;
    this->updateState(nullptr);
    SettingNodeValueChangeEventV3(m_impl->setting->getModID(), m_impl->setting->getKey()).send(this, true);
}
void SettingNodeV3::resetToDefault() {
    if (!m_impl->setting || m_impl->setting->isDefaultValue()) return;
    m_impl->setting->reset();
    m_impl->committed = true;
    this->onResetToDefault();
    this->updateState(nullptr);
    SettingNodeValueChangeEventV3(m_impl->setting->getModID(), m_impl->setting->getKey()).send(this, false);
}

void SettingNodeV3::overrideDescription(std::optional<ZStringView> description) {
    m_impl->customDescription = description ? std::optional(std::string(*description)) : std::nullopt;
}

void SettingNodeV3::setContentSize(CCSize const& size) {
    CCNode::setContentSize(size);
    m_impl->bg->setContentSize(size);
    this->updateLayout();
    if (!m_impl->setting) return;
    SettingNodeSizeChangeEventV3(m_impl->setting->getModID(), m_impl->setting->getKey()).send(this);
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

    // note: setting may be null

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

void TitleSettingNodeV3::setCollapsed(bool collapsed) {
    m_collapseToggle->toggle(collapsed);
    this->markChanged(m_collapseToggle);
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
    if (ret->init(setting, width)) {
        ret->autorelease();
        return ret;
    }
    delete ret;
    return nullptr;
}
TitleSettingNodeV3* TitleSettingNodeV3::create(ZStringView title, std::optional<ZStringView> description, float width) {
    auto ret = TitleSettingNodeV3::create(nullptr, width);
    ret->getNameLabel()->setString(title.c_str());
    ret->overrideDescription(description);
    ret->updateState(nullptr);
    return ret;
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
    if (ret->init(setting, width)) {
        ret->autorelease();
        return ret;
    }
    delete ret;
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
    if (auto filter = this->getSetting()->getAllowedCharacters()) {
        m_input->setFilter(*filter);
    }

    this->getButtonMenu()->addChildAtPosition(m_input, Anchor::Center);

    if (setting->getEnumOptions()) {
        m_input->getBGSprite()->setVisible(false);
        m_input->setEnabled(false);
        m_input->getInputNode()->m_textLabel->setOpacity(255);
        m_input->getInputNode()->m_textLabel->setColor(ccWHITE);

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
    if (ret->init(setting, width)) {
        ret->autorelease();
        return ret;
    }
    delete ret;
    return nullptr;
}

// FileSettingNodeV3

bool FileSettingNodeV3::init(std::shared_ptr<FileSettingV3> setting, float width) {
    if (!SettingValueNodeV3::init(setting, width))
        return false;

    auto labelBG = NineSlice::create("square02b_001.png", { 0, 0, 80, 80 });
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
        if (utils::string::pathToString(setting->getDefaultValue()).size() > 20) return false;
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
            m_nameLabel->setString(utils::string::pathToString(this->getSetting()->getDefaultValue()).c_str());
        }
        else {
            m_nameLabel->setString(this->getSetting()->isFolder() ? "No Folder Selected" : "No File Selected");
        }
        m_nameLabel->setColor(ccGRAY);
        m_nameLabel->setOpacity(155);
    }
    else {
        m_nameLabel->setString(utils::string::pathToString(this->getValue().filename()).c_str());
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
    std::error_code ec;

    m_pickListener.spawn(
        file::pick(
            this->getSetting()->isFolder() ?
            file::PickMode::OpenFolder :
            this->getSetting()->useSaveDialog() ? file::PickMode::SaveFile : file::PickMode::OpenFile,
            {
                // Prefer opening the current path directly if possible
                this->getValue().empty() || !std::filesystem::exists(this->getValue().parent_path(), ec)
                    ? dirs::getGameDir() : this->getValue(),
                this->getSetting()->getFilters().value_or(std::vector<file::FilePickOptions::Filter>())
            }
        ),
        [this](Result<std::optional<std::filesystem::path>> path) {
            if (path.isOk() && path.unwrap().has_value()) {
                this->setValue(std::move(path).unwrap().value(), nullptr);
            }
            else if (path.isErr()) {
                FLAlertLayer::create(
                    "Failed",
                    fmt::format("Failed to pick file: {}", path.unwrapErr()),
                    "Ok"
                )->show();
            }
        }
    );
}

FileSettingNodeV3* FileSettingNodeV3::create(std::shared_ptr<FileSettingV3> setting, float width) {
    auto ret = new FileSettingNodeV3();
    if (ret->init(setting, width)) {
        ret->autorelease();
        return ret;
    }
    delete ret;
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
    popup->setCallback([this](ccColor4B const& color) { this->setValue(to3B(color), nullptr); });
    popup->show();
}
Color3BSettingNodeV3* Color3BSettingNodeV3::create(std::shared_ptr<Color3BSettingV3> setting, float width) {
    auto ret = new Color3BSettingNodeV3();
    if (ret->init(setting, width)) {
        ret->autorelease();
        return ret;
    }
    delete ret;
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
    popup->setCallback([this](ccColor4B const& color) { this->setValue(color, nullptr); });
    popup->show();
}

Color4BSettingNodeV3* Color4BSettingNodeV3::create(std::shared_ptr<Color4BSettingV3> setting, float width) {
    auto ret = new Color4BSettingNodeV3();
    if (ret->init(setting, width)) {
        ret->autorelease();
        return ret;
    }
    delete ret;
    return nullptr;
}

// KeybindSettingNodeV3

KeybindSettingNodeV3* KeybindSettingNodeV3::create(std::shared_ptr<KeybindSettingV3> setting, float width) {
    auto ret = new KeybindSettingNodeV3();
    if (ret->init(setting, width)) {
        ret->autorelease();
        return ret;
    }
    delete ret;
    return nullptr;
}

bool KeybindSettingNodeV3::init(std::shared_ptr<KeybindSettingV3> setting, float width) {
    if (!SettingNodeV3::init(setting, width))
        return false;

    m_currentValue = setting->getValue();
    this->getButtonMenu()->setLayout(RowLayout::create()->setAxisAlignment(AxisAlignment::End));

    if (auto category = setting->getCategory()) {
        const char* catSpr;
        switch (*category) {
            default:
            case KeybindCategory::Editor: {
                catSpr = "GJ_hammerIcon_001.png";
            } break;

            case KeybindCategory::Gameplay: {
                catSpr = "controllerBtn_DPad_Right_001.png";
            } break;

            case KeybindCategory::Universal: {
                catSpr = "GJ_sMagicIcon_001.png";
            } break;
        }
        auto categoryLabel = createTagLabelWithIcon(
            CCSprite::createWithSpriteFrameName(catSpr), "",
            std::make_pair(ccWHITE, "keybinds-list-category-label"_cc3b)
        );
        categoryLabel->setLayoutOptions(
            AxisLayoutOptions::create()->setScaleLimits(.1f, .35f)
        );
        this->getNameMenu()->addChild(categoryLabel);
    }

    this->updateState(nullptr);

    return true;
}

std::shared_ptr<KeybindSettingV3> KeybindSettingNodeV3::getSetting() const {
    return std::static_pointer_cast<KeybindSettingV3>(SettingNodeV3::getSetting());
}

void KeybindSettingNodeV3::updateState(CCNode* invoker) {
    SettingNodeV3::updateState(invoker);

    auto buttonMenu = getButtonMenu();

    buttonMenu->removeAllChildren();
    size_t index = 0;
    for (auto& keybind : m_currentValue) {
        auto bspr = createKeybindButton(keybind);
        bspr->setScale(.5f);
        auto button = CCMenuItemSpriteExtra::create(bspr, this, menu_selector(KeybindSettingNodeV3::onKeybind));
        button->setTag(index);
        buttonMenu->addChild(button);
        index += 1;
    }
    auto plusSprite = createGeodeButton("+", true);
    plusSprite->setScale(.5f);
    auto plusButton = CCMenuItemSpriteExtra::create(plusSprite, this, menu_selector(KeybindSettingNodeV3::onKeybind));
    buttonMenu->addChild(plusButton);
    buttonMenu->updateLayout();

    if (buttonMenu->getChildByIndex(0)->getScale() < 1.f) {
        auto children = buttonMenu->getChildrenExt();
        for (auto it = children.end() - 1; it != children.begin(); --it) {
            buttonMenu->removeChild(*it, false);
        }

        auto moreSprite = createGeodeButton("...", true);
        moreSprite->setScale(.5f);
        auto moreButton = CCMenuItemSpriteExtra::create(moreSprite, this, menu_selector(KeybindSettingNodeV3::onExtra));
        buttonMenu->addChild(moreButton);

        buttonMenu->updateLayout();
    }
}

void KeybindSettingNodeV3::onExtra(CCObject* sender) {
    KeybindListPopup::create(
        getSetting(),
        m_currentValue,
        [this](std::vector<Keybind> newKeybinds) {
            if (m_currentValue == newKeybinds) return;
            m_currentValue = std::move(newKeybinds);
            this->markChanged(nullptr);
        }
    )->show();
}

void KeybindSettingNodeV3::onKeybind(CCObject* sender) {
    auto index = sender->getTag();
    KeybindEditPopup::create(
        getSetting(),
        index >= 0 ? m_currentValue[index] : Keybind(),
        [this, index](Keybind const& newKeybind) {
            if (index >= 0) {
                if (newKeybind.key == KEY_None || std::ranges::contains(m_currentValue, newKeybind)) {
                    m_currentValue.erase(m_currentValue.begin() + index);
                }
                else {
                    m_currentValue[index] = newKeybind;
                }
            }
            else {
                if (std::ranges::contains(m_currentValue, newKeybind)) return;
                m_currentValue.push_back(newKeybind);
            }
            this->markChanged(nullptr);
        }
    )->show();
}

void KeybindSettingNodeV3::onCommit() {
    getSetting()->setValue(m_currentValue);
    this->markChanged(nullptr);
}

bool KeybindSettingNodeV3::hasUncommittedChanges() const {
    return m_currentValue != getSetting()->getValue();
}

bool KeybindSettingNodeV3::hasNonDefaultValue() const {
    return m_currentValue != getSetting()->getDefaultValue();
}

void KeybindSettingNodeV3::onResetToDefault() {
    m_currentValue = getSetting()->getDefaultValue();
    this->markChanged(nullptr);
}

// UnresolvedCustomSettingNodeV3

bool UnresolvedCustomSettingNodeV3::init(std::string_view key, Mod* mod, float width) {
    if (!SettingNodeV3::init(nullptr, width))
        return false;

    m_mod = mod;

    this->setContentHeight(30);

    auto label = CCLabelBMFont::create(
        (mod && mod->isLoaded() ?
            fmt::format("Missing setting '{}'", key) :
            fmt::format("Enable the Mod to Edit '{}'", key)
        ).c_str(),
        "bigFont.fnt"
    );
    label->setColor(mod && mod->isLoaded() ? "mod-list-errors-found-2"_cc3b : "mod-list-gray"_cc3b);
    label->limitLabelWidth(width - m_obContentSize.height, .3f, .1f);
    this->addChildAtPosition(label, Anchor::Left, ccp(m_obContentSize.height / 2, 0), ccp(0, .5f));

    return true;
}

void UnresolvedCustomSettingNodeV3::updateState(CCNode* invoker) {
    SettingNodeV3::updateState(invoker);
    this->getBG()->setColor(m_mod && m_mod->isLoaded() ? "mod-list-errors-found-2"_cc3b : "mod-list-gray"_cc3b);
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
    if (ret->init(key, mod, width)) {
        ret->autorelease();
        return ret;
    }
    delete ret;
    return nullptr;
}
