#include "FiltersPopup.hpp"
#include <GUI/CCControlExtension/CCScale9Sprite.h>

#include <Geode/ui/Label.hpp>

bool FiltersPopup::init(ModListSource* src) {
    float height = 170.0f;
    if (typeinfo_cast<InstalledModListSource*>(src) || typeinfo_cast<ServerModListSource*>(src)) {
        height = 230.0f;
    }

    if (!GeodePopup::init(350.0f, height)) return false;

    m_noElasticity = true;
    m_source = src;
    m_selectedTags = src->getModTags();

    this->setTitle("Search Filters");

    auto tagsContainer = CCNode::create();
    tagsContainer->setContentSize({310.0f, 80.0f});
    tagsContainer->setAnchorPoint({ 0.5f, 0.5f });

    auto tagsBG = NineSlice::create("square02b_001.png");
    tagsBG->setColor({ 0, 0, 0 });
    tagsBG->setOpacity(75);
    tagsBG->setScaleMultiplier(0.3f);
    tagsBG->setContentSize(tagsContainer->getContentSize());
    tagsContainer->addChildAtPosition(tagsBG, Anchor::Center);

    m_tagsMenu = CCNode::create();
    m_tagsMenu->setID("tags-menu");
    m_tagsMenu->setAnchorPoint({0.5f, 0.5f});
    m_tagsMenu->setContentSize(tagsContainer->getContentSize() - CCSize{10.0f, 10.0f});
    m_tagsMenu->addChild(createLoadingCircle(40.0f));
    m_tagsMenu->setLayout(
        RowLayout::create()
            ->setDefaultScaleLimits(0.1f, 1.0f)
            ->setGrowCrossAxis(true)
            ->setCrossAxisOverflow(false)
            ->setAxisAlignment(AxisAlignment::Center)
            ->setCrossAxisAlignment(AxisAlignment::Center)
    );
    tagsContainer->addChildAtPosition(m_tagsMenu, Anchor::Center);

    auto tagsTitleMenu = CCNode::create();
    tagsTitleMenu->setAnchorPoint({ 0.5f, 0.0f });
    tagsTitleMenu->setContentWidth(tagsContainer->getContentWidth());

    auto tagsTitle = Label::create("Tags", "bigFont.fnt");
    tagsTitleMenu->addChild(tagsTitle);

    tagsTitleMenu->addChild(SpacerNode::create());

    auto resetBtn = Button::createWithSpriteFrameName("GJ_trashBtn_001.png", [this](auto) {
        m_selectedTags.clear();
        this->updateTags();
    });
    resetBtn->setID("reset-tags-button");
    tagsTitleMenu->addChild(resetBtn);

    tagsTitleMenu->setLayout(
        RowLayout::create()
            ->setDefaultScaleLimits(0.1f, .4f)
    );
    tagsContainer->addChildAtPosition(tagsTitleMenu, Anchor::Top, {0.0f, 4.0f});

    m_mainLayer->addChildAtPosition(tagsContainer, Anchor::Top, {0.0f, -85.0f});

    if (auto src = typeinfo_cast<InstalledModListSource*>(m_source)) {
        auto optionsContainer = CCNode::create();
        optionsContainer->setContentSize({240.0f, 35.0f});
        optionsContainer->setAnchorPoint({ 0.5f, 00.5f });

        auto optionsBG = NineSlice::create("square02b_001.png");
        optionsBG->setColor({ 0, 0, 0 });
        optionsBG->setOpacity(75);
        optionsBG->setScaleMultiplier(0.3f);
        optionsBG->setContentSize(optionsContainer->getContentSize());
        optionsContainer->addChildAtPosition(optionsBG, Anchor::Center);

        auto optionsMenu = CCNode::create();
        optionsMenu->setContentSize(optionsContainer->getContentSize() - CCSize{10.0f, 10.0f});
        optionsMenu->setLayout(
            RowLayout::create()
                ->setAutoScale(false)
        );

        m_enabledModsOnly = Toggler::createWithStandardSprites(0.6f);
        m_enabledModsOnly->toggle(src->getQuery().enabledOnly.value_or(false));
        optionsMenu->addChild(m_enabledModsOnly);

        auto enabledOnlyLabel = Label::create("Enabled Only", "bigFont.fnt");
        enabledOnlyLabel->setScale(0.35f);
        optionsMenu->addChild(enabledOnlyLabel);

        m_enabledModsFirst = Toggler::createWithStandardSprites(0.6f);
        m_enabledModsFirst->toggle(src->getQuery().enabledFirst.value_or(false));
        m_enabledModsFirst->setLayoutOptions(AxisLayoutOptions::create()->setPrevGap(10.0f));
        optionsMenu->addChild(m_enabledModsFirst);

        auto enabledFirstLabel = Label::create("Enabled First", "bigFont.fnt");
        enabledFirstLabel->setScale(0.35f);
        optionsMenu->addChild(enabledFirstLabel);

        optionsMenu->updateLayout();

        optionsContainer->addChildAtPosition(optionsMenu, Anchor::Center);

        auto optionsTitleMenu = CCNode::create();
        optionsTitleMenu->setAnchorPoint({ 0.5f, 0.0f });
        optionsTitleMenu->setContentWidth(optionsContainer->getContentWidth());

        auto optionsTitle = Label::create("Options", "bigFont.fnt");
        optionsTitleMenu->addChild(optionsTitle);

        optionsTitleMenu->addChild(SpacerNode::create());

        optionsTitleMenu->setLayout(
            RowLayout::create()
                ->setDefaultScaleLimits(0.1f, 0.4f)
        );
        optionsContainer->addChildAtPosition(optionsTitleMenu, Anchor::Top, {0.0f, 4.0f});

        m_mainLayer->addChildAtPosition(optionsContainer, Anchor::Bottom, {0.0f, 60.0f}, {0.5f, 0.5f});
    }
    else if (auto src = typeinfo_cast<ServerModListSource*>(m_source)) {
        auto inputContainer = CCNode::create();
        inputContainer->setContentSize({160.0f, 35.0f});
        inputContainer->setAnchorPoint({ 0.5f, 0.5f });

        m_developerNameInput = TextInput::create(inputContainer->getContentWidth(), "Developer Name");
        m_developerNameInput->setTextAlign(TextInputAlign::Left);
        m_developerNameInput->setFilter("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789-");
        m_developerNameInput->setString(src->getQuery().developer.value_or(""));
        inputContainer->addChildAtPosition(m_developerNameInput, Anchor::Center);

        auto inputTitleMenu = CCNode::create();
        inputTitleMenu->setAnchorPoint({ 0.5f, 0.0f });
        inputTitleMenu->setContentWidth(inputContainer->getContentWidth());

        auto inputTitle = Label::create("From Developer", "bigFont.fnt");
        inputTitleMenu->addChild(inputTitle);

        inputTitleMenu->addChild(SpacerNode::create());

        auto resetBtn = Button::createWithSpriteFrameName("GJ_trashBtn_001.png", [this](auto) {
            if (m_developerNameInput) m_developerNameInput->setString("");
        });
        resetBtn->setID("reset-developer-input-button");
        inputTitleMenu->addChild(resetBtn);

        inputTitleMenu->setLayout(
            RowLayout::create()
                ->setDefaultScaleLimits(0.1f, 0.4f)
        );
        inputContainer->addChildAtPosition(inputTitleMenu, Anchor::Top, {0.0f, 4.0f});

        m_mainLayer->addChildAtPosition(inputContainer, Anchor::Bottom, {0.0f, 60.0f}, {0.5f, 0.5f});
    }

    auto okBtn = Button::createWithNode(
        createGeodeButton("OK", false, GeodeButtonSprite::Default, m_forceDisableTheme),
        [this](Button* sender) {
            this->onClose(sender);
        }
    );
    okBtn->setScale(0.7f);
    m_buttonMenu->addChildAtPosition(okBtn, Anchor::Bottom, {0.0f, 20.0f});

    m_tagsListener.spawn(
        server::getTags(),
        [this](auto result) {
            this->onLoadTags(std::move(result));
        }
    );

    return true;
}

void FiltersPopup::onLoadTags(server::ServerResult<std::vector<server::ServerTag>> result) {
    if (result.isOk()) {
        auto tags = std::move(result).unwrap();
        m_tagsMenu->removeAllChildren();
        for (auto& tag : tags) {
            auto offSpr = createGeodeTagLabel(tag);
            offSpr->m_BGSprite->setOpacity(105);
            offSpr->m_label->setOpacity(105);
            auto onSpr = createGeodeTagLabel(tag);
            auto btn = Toggler::create(
                offSpr, onSpr,
                [this, name = std::move(tag.name)](Toggler* sender, bool toggled) {
                    sender->setToggled(!toggled);
                    this->onSelectTag(name);
                }
            );
            btn->setUserObject("tag", CCString::create(tag.name));
            m_tagsMenu->addChild(btn);
        }
        m_tagsMenu->updateLayout();
        this->updateTags();
    }
    else {
        m_tagsMenu->removeAllChildren();
        auto label = Label::create("Unable to load tags", "bigFont.fnt");
        label->setOpacity(105);
        m_tagsMenu->addChild(label);
        m_tagsMenu->updateLayout();
    }
}

void FiltersPopup::updateTags() {
    for (auto node : CCArrayExt<CCNode*>(m_tagsMenu->getChildren())) {
        if (auto toggle = typeinfo_cast<Toggler*>(node)) {
            auto tag = static_cast<CCString*>(toggle->getUserObject("tag"))->getCString();
            toggle->toggle(m_selectedTags.contains(tag));
        }
    }
}

void FiltersPopup::onSelectTag(ZStringView tag) {
    if (m_selectedTags.contains(tag)) {
        m_selectedTags.erase(tag);
    }
    else {
        m_selectedTags.insert(tag);
    }
    this->updateTags();
}

void FiltersPopup::onClose(CCObject* sender) {
    m_source->setModTags(m_selectedTags);

    if (auto src = typeinfo_cast<InstalledModListSource*>(m_source)) {
        src->getQueryMut()->enabledOnly = m_enabledModsOnly->isToggled() ? std::optional(true) : std::nullopt;
        src->getQueryMut()->enabledFirst = m_enabledModsFirst->isToggled() ? std::optional(true) : std::nullopt;

        auto filters = src->getQuery().dumpFilters();
        Mod::get()->setSavedValue("mod-list-installed-filters", filters);
    }
    else if (auto src = typeinfo_cast<ServerModListSource*>(m_source)) {
        src->getQueryMut()->developer = m_developerNameInput->getString();
        if (src->getQueryMut()->developer->empty()) {
            src->getQueryMut()->developer.reset();
        }
    }

    Popup::onClose(sender);
}

FiltersPopup* FiltersPopup::create(ModListSource* src) {
    auto ret = new FiltersPopup();
    if (ret->init(src)) {
        ret->autorelease();
        return ret;
    }
    delete ret;
    return nullptr;
}