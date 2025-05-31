#include "FiltersPopup.hpp"

bool FiltersPopup::setup(ModListSource* src) {
    m_noElasticity = true;
    m_source = src;
    m_selectedTags = src->getModTags();

    this->setTitle("Search Filters");

    auto tagsContainer = CCNode::create();
    tagsContainer->setContentSize(ccp(310, 80));
    tagsContainer->setAnchorPoint({ .5f, .5f });

    auto tagsBG = CCScale9Sprite::create("square02b_001.png");
    tagsBG->setColor({ 0, 0, 0 });
    tagsBG->setOpacity(75);
    tagsBG->setScale(.3f);
    tagsBG->setContentSize(tagsContainer->getContentSize() / tagsBG->getScale());
    tagsContainer->addChildAtPosition(tagsBG, Anchor::Center);

    m_tagsMenu = CCMenu::create();
    m_tagsMenu->setContentSize(tagsContainer->getContentSize() - ccp(10, 10));
    m_tagsMenu->addChild(createLoadingCircle(40));
    m_tagsMenu->setLayout(
        RowLayout::create()
            ->setDefaultScaleLimits(.1f, 1.f)
            ->setGrowCrossAxis(true)
            ->setCrossAxisOverflow(false)
            ->setAxisAlignment(AxisAlignment::Center)
            ->setCrossAxisAlignment(AxisAlignment::Center)
    );
    tagsContainer->addChildAtPosition(m_tagsMenu, Anchor::Center);

    auto tagsTitleMenu = CCMenu::create();
    tagsTitleMenu->setAnchorPoint({ .5f, 0 });
    tagsTitleMenu->setContentWidth(tagsContainer->getContentWidth());

    auto tagsTitle = CCLabelBMFont::create("Tags", "bigFont.fnt");
    tagsTitleMenu->addChild(tagsTitle);

    tagsTitleMenu->addChild(SpacerNode::create());

    auto resetSpr = CCSprite::createWithSpriteFrameName("GJ_trashBtn_001.png");
    auto resetBtn = CCMenuItemSpriteExtra::create(
        resetSpr, this, menu_selector(FiltersPopup::onResetTags)
    );
    tagsTitleMenu->addChild(resetBtn);

    tagsTitleMenu->setLayout(
        RowLayout::create()
            ->setDefaultScaleLimits(.1f, .4f)
    );
    tagsContainer->addChildAtPosition(tagsTitleMenu, Anchor::Top, ccp(0, 4));

    m_mainLayer->addChildAtPosition(tagsContainer, Anchor::Top, ccp(0, -85));

    if (auto src = typeinfo_cast<InstalledModListSource*>(m_source)) {
        auto optionsContainer = CCNode::create();
        optionsContainer->setContentSize(ccp(240, 35));
        optionsContainer->setAnchorPoint({ .5f, .5f });

        auto optionsBG = CCScale9Sprite::create("square02b_001.png");
        optionsBG->setColor({ 0, 0, 0 });
        optionsBG->setOpacity(75);
        optionsBG->setScale(.3f);
        optionsBG->setContentSize(optionsContainer->getContentSize() / optionsBG->getScale());
        optionsContainer->addChildAtPosition(optionsBG, Anchor::Center);

        auto optionsMenu = CCMenu::create();
        optionsMenu->setContentSize(optionsContainer->getContentSize() - ccp(10, 10));
        optionsMenu->setLayout(
            RowLayout::create()
                ->setAutoScale(false)
        );

        m_enabledModsOnly = CCMenuItemExt::createTogglerWithStandardSprites(.6f, [](auto) {});
        m_enabledModsOnly->toggle(src->getQuery().enabledOnly.value_or(false));
        optionsMenu->addChild(m_enabledModsOnly);

        auto enabledOnlyLabel = CCLabelBMFont::create("Enabled Only", "bigFont.fnt");
        enabledOnlyLabel->setScale(.35f);
        optionsMenu->addChild(enabledOnlyLabel);

        m_enabledModsFirst = CCMenuItemExt::createTogglerWithStandardSprites(.6f, [](auto) {});
        m_enabledModsFirst->toggle(src->getQuery().enabledFirst.value_or(false));
        m_enabledModsFirst->setLayoutOptions(AxisLayoutOptions::create()->setPrevGap(10.f));
        optionsMenu->addChild(m_enabledModsFirst);

        auto enabledFirstLabel = CCLabelBMFont::create("Enabled First", "bigFont.fnt");
        enabledFirstLabel->setScale(.35f);
        optionsMenu->addChild(enabledFirstLabel);

        optionsMenu->updateLayout();

        optionsContainer->addChildAtPosition(optionsMenu, Anchor::Center);

        auto optionsTitleMenu = CCMenu::create();
        optionsTitleMenu->setAnchorPoint({ .5f, 0 });
        optionsTitleMenu->setContentWidth(optionsContainer->getContentWidth());

        auto optionsTitle = CCLabelBMFont::create("Options", "bigFont.fnt");
        optionsTitleMenu->addChild(optionsTitle);

        optionsTitleMenu->addChild(SpacerNode::create());

        optionsTitleMenu->setLayout(
            RowLayout::create()
                ->setDefaultScaleLimits(.1f, .4f)
        );
        optionsContainer->addChildAtPosition(optionsTitleMenu, Anchor::Top, ccp(0, 4));

        m_mainLayer->addChildAtPosition(optionsContainer, Anchor::Bottom, ccp(0, 60), ccp(.5f, .5f));
    }
    else if (auto src = typeinfo_cast<ServerModListSource*>(m_source)) {
        auto inputContainer = CCNode::create();
        inputContainer->setContentSize(ccp(160, 35));
        inputContainer->setAnchorPoint({ .5f, .5f });

        m_developerNameInput = TextInput::create(inputContainer->getContentWidth(), "Developer Name");
        m_developerNameInput->setTextAlign(TextInputAlign::Left);
        m_developerNameInput->setFilter("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789-");
        m_developerNameInput->setString(src->getQuery().developer.value_or(""));
        inputContainer->addChildAtPosition(m_developerNameInput, Anchor::Center);

        auto inputTitleMenu = CCMenu::create();
        inputTitleMenu->setAnchorPoint({ .5f, 0 });
        inputTitleMenu->setContentWidth(inputContainer->getContentWidth());

        auto inputTitle = CCLabelBMFont::create("From Developer", "bigFont.fnt");
        inputTitleMenu->addChild(inputTitle);

        inputTitleMenu->addChild(SpacerNode::create());

        auto resetSpr = CCSprite::createWithSpriteFrameName("GJ_trashBtn_001.png");
        auto resetBtn = CCMenuItemSpriteExtra::create(
            resetSpr, this, menu_selector(FiltersPopup::onResetDevName)
        );
        inputTitleMenu->addChild(resetBtn);

        inputTitleMenu->setLayout(
            RowLayout::create()
                ->setDefaultScaleLimits(.1f, .4f)
        );
        inputContainer->addChildAtPosition(inputTitleMenu, Anchor::Top, ccp(0, 4));

        m_mainLayer->addChildAtPosition(inputContainer, Anchor::Bottom, ccp(0, 60), ccp(.5f, .5f));
    }

    auto okSpr = createGeodeButton("OK", false, GeodeButtonSprite::Default, m_forceDisableTheme);
    okSpr->setScale(.7f);
    auto okBtn = CCMenuItemSpriteExtra::create(
        okSpr, this, menu_selector(FiltersPopup::onClose)
    );
    m_buttonMenu->addChildAtPosition(okBtn, Anchor::Bottom, ccp(0, 20));

    m_tagsListener.bind(this, &FiltersPopup::onLoadTags);
    m_tagsListener.setFilter(server::getTags());

    return true;
}

void FiltersPopup::onLoadTags(typename server::ServerRequest<std::vector<server::ServerTag>>::Event* event) {
    if (event->getValue() && event->getValue()->isOk()) {
        auto tags = event->getValue()->unwrap();
        m_tagsMenu->removeAllChildren();
        for (auto& tag : tags) {
            auto offSpr = createGeodeTagLabel(tag);
            offSpr->m_BGSprite->setOpacity(105);
            offSpr->m_label->setOpacity(105);
            auto onSpr = createGeodeTagLabel(tag);
            auto btn = CCMenuItemToggler::create(
                offSpr, onSpr, this, menu_selector(FiltersPopup::onSelectTag)
            );
            btn->m_notClickable = true;
            btn->setUserObject("tag", CCString::create(tag.name));
            m_tagsMenu->addChild(btn);
        }
        m_tagsMenu->updateLayout();
        this->updateTags();
    }
    else if (event->isCancelled() || (event->getValue() && event->getValue()->isErr())) {
        m_tagsMenu->removeAllChildren();
        auto label = CCLabelBMFont::create("Unable to load tags", "bigFont.fnt");
        label->setOpacity(105);
        m_tagsMenu->addChild(label);
        m_tagsMenu->updateLayout();
    }
}

void FiltersPopup::updateTags() {
    for (auto node : CCArrayExt<CCNode*>(m_tagsMenu->getChildren())) {
        if (auto toggle = typeinfo_cast<CCMenuItemToggler*>(node)) {
            auto tag = static_cast<CCString*>(toggle->getUserObject("tag"))->getCString();
            toggle->toggle(m_selectedTags.contains(tag));
        }
    }
}
void FiltersPopup::onSelectTag(CCObject* sender) {
    auto toggle = static_cast<CCMenuItemToggler*>(sender);
    auto tag = static_cast<CCString*>(toggle->getUserObject("tag"))->getCString();
    if (m_selectedTags.contains(tag)) {
        m_selectedTags.erase(tag);
    }
    else {
        m_selectedTags.insert(tag);
    }
    this->updateTags();
}
void FiltersPopup::onResetTags(CCObject*) {
    m_selectedTags.clear();
    this->updateTags();
}
void FiltersPopup::onResetDevName(CCObject*) {
    if (m_developerNameInput) {
        m_developerNameInput->setString("");
    }
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
    float height = 170;
    if (typeinfo_cast<InstalledModListSource*>(src) || typeinfo_cast<ServerModListSource*>(src)) {
        height = 230;
    }
    if (ret->init(350, height, src)) {
        ret->autorelease();
        return ret;
    }
    delete ret;
    return nullptr;
}
