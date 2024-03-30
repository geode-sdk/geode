#include "ModPopup.hpp"
#include <Geode/ui/MDTextArea.hpp>
#include <Geode/utils/web.hpp>
#include <Geode/ui/GeodeUI.hpp>
#include <Geode/utils/ColorProvider.hpp>
#include "ConfirmUninstallPopup.hpp"

bool ModPopup::setup(ModSource&& src) {
    m_source = std::move(src);
    m_noElasticity = true;

    auto mainContainer = CCNode::create();
    mainContainer->setContentSize(m_mainLayer->getContentSize() - ccp(20, 20));
    mainContainer->setAnchorPoint({ .5f, .5f });
    mainContainer->setLayout(
        RowLayout::create()
            ->setGap(7.5f)
            ->setCrossAxisLineAlignment(AxisAlignment::Start)
    );

    auto leftColumn = CCNode::create();
    leftColumn->setContentSize({ 145, mainContainer->getContentHeight() });

    auto titleContainer = CCNode::create();
    titleContainer->setContentSize({ leftColumn->getContentWidth(), 25 });
    titleContainer->setAnchorPoint({ .5f, .5f });

    auto logo = m_source.createModLogo();
    limitNodeSize(
        logo,
        ccp(titleContainer->getContentHeight(), titleContainer->getContentHeight()),
        5.f, .1f
    );
    titleContainer->addChildAtPosition(
        logo, Anchor::Left, ccp(titleContainer->getContentHeight() / 2, 0)
    );

    // Lil padding
    auto devAndTitlePos = titleContainer->getContentHeight() + 5;

    auto title = CCLabelBMFont::create(m_source.getMetadata().getName().c_str(), "bigFont.fnt");
    title->limitLabelWidth(titleContainer->getContentWidth() - devAndTitlePos, .45f, .1f);
    title->setAnchorPoint({ .0f, .5f });
    titleContainer->addChildAtPosition(title, Anchor::TopLeft, ccp(devAndTitlePos, -titleContainer->getContentHeight() * .25f));

    auto by = "By " + ModMetadata::formatDeveloperDisplayString(m_source.getMetadata().getDevelopers());
    auto dev = CCLabelBMFont::create(by.c_str(), "goldFont.fnt");
    dev->limitLabelWidth(titleContainer->getContentWidth() - devAndTitlePos, .35f, .05f);
    dev->setAnchorPoint({ .0f, .5f });
    titleContainer->addChildAtPosition(dev, Anchor::BottomLeft, ccp(devAndTitlePos, titleContainer->getContentHeight() * .25f));

    leftColumn->addChild(titleContainer);

    auto idStr = "(ID: " + m_source.getMetadata().getID() + ")";
    auto idLabel = CCLabelBMFont::create(idStr.c_str(), "bigFont.fnt");
    idLabel->limitLabelWidth(leftColumn->getContentWidth(), .25f, .05f);
    idLabel->setColor({ 150, 150, 150 });
    idLabel->setOpacity(140);
    leftColumn->addChild(idLabel);

    auto statsContainer = CCNode::create();
    statsContainer->setContentSize({ leftColumn->getContentWidth(), 80 });
    statsContainer->setAnchorPoint({ .5f, .5f });

    auto statsBG = CCScale9Sprite::create("square02b_001.png");
    statsBG->setColor({ 0, 0, 0 });
    statsBG->setOpacity(75);
    statsBG->setScale(.3f);
    statsBG->setContentSize(statsContainer->getContentSize() / statsBG->getScale());
    statsContainer->addChildAtPosition(statsBG, Anchor::Center);

    m_stats = CCNode::create();
    m_stats->setContentSize(statsContainer->getContentSize() - ccp(10, 10));
    m_stats->setAnchorPoint({ .5f, .5f });

    for (auto stat : std::initializer_list<std::tuple<
        const char*, const char*, const char*, std::optional<std::string>, const char*
    >> {
        { "GJ_downloadsIcon_001.png", "Downloads", "downloads", std::nullopt, "stats" },
        { "GJ_timeIcon_001.png", "Released", "release-date", std::nullopt, "stats" },
        { "GJ_timeIcon_001.png", "Updated", "update-date", std::nullopt, "stats" },
        { "version.png"_spr, "Version", "version", m_source.getMetadata().getVersion().toString(), "client" },
        { nullptr, "Checking for updates", "update-check", std::nullopt, "updates" },
    }) {
        auto container = CCNode::create();
        container->setContentSize({ m_stats->getContentWidth(), 10 });
        container->setID(std::get<2>(stat));
        container->setUserObject(std::get<4>(stat), CCBool::create(true));

        auto labelContainer = CCNode::create();
        labelContainer->setID("labels");
        labelContainer->setLayout(RowLayout::create());
        labelContainer->getLayout()->ignoreInvisibleChildren(true);
        labelContainer->setAnchorPoint({ 1.f, .5f });
        labelContainer->setScale(.3f);
        labelContainer->setContentWidth(
            (container->getContentWidth() - container->getContentHeight() - 5) / 
                labelContainer->getScale()
        );
        container->addChildAtPosition(labelContainer, Anchor::Right);

        auto label = CCLabelBMFont::create("", "bigFont.fnt");
        label->setID("label");
        labelContainer->addChild(label);

        labelContainer->addChild(SpacerNode::create());

        auto valueLabel = CCLabelBMFont::create("", "bigFont.fnt");
        valueLabel->setID("value-label");
        labelContainer->addChild(valueLabel);

        labelContainer->addChild(createLoadingCircle(container->getContentHeight() / labelContainer->getScale()));

        this->setStatIcon(container, std::get<0>(stat));
        this->setStatLabel(container, std::get<1>(stat));
        this->setStatValue(container, std::get<3>(stat));

        m_stats->addChild(container);
    }

    m_stats->setLayout(
        ColumnLayout::create()
            ->setAxisReverse(true)
            ->setDefaultScaleLimits(.1f, 1)
            ->setAxisAlignment(AxisAlignment::Even)
    );
    statsContainer->addChildAtPosition(m_stats, Anchor::Center);

    leftColumn->addChild(statsContainer);

    // Tags

    auto tagsTitle = CCLabelBMFont::create("Tags", "bigFont.fnt");
    tagsTitle->limitLabelWidth(leftColumn->getContentWidth(), .25f, .05f);
    tagsTitle->setOpacity(195);
    leftColumn->addChild(tagsTitle);

    auto tagsContainer = CCNode::create();
    tagsContainer->setContentSize({ leftColumn->getContentWidth(), 35 });
    tagsContainer->setAnchorPoint({ .5f, .5f });

    auto tagsBG = CCScale9Sprite::create("square02b_001.png");
    tagsBG->setColor({ 0, 0, 0 });
    tagsBG->setOpacity(75);
    tagsBG->setScale(.3f);
    tagsBG->setContentSize(tagsContainer->getContentSize() / tagsBG->getScale());
    tagsContainer->addChildAtPosition(tagsBG, Anchor::Center);

    m_tags = CCNode::create();
    m_tags->ignoreAnchorPointForPosition(false);
    m_tags->setContentSize(tagsContainer->getContentSize() - ccp(10, 10));
    m_tags->setAnchorPoint({ .5f, .5f });

    m_tags->addChild(createLoadingCircle(50));

    m_tags->setLayout(
        RowLayout::create()
            ->setDefaultScaleLimits(.1f, .3f)
            ->setGrowCrossAxis(true)
            ->setCrossAxisOverflow(false)
            ->setAxisAlignment(AxisAlignment::Start)
            ->setCrossAxisAlignment(AxisAlignment::End)
    );
    tagsContainer->addChildAtPosition(m_tags, Anchor::Center);

    leftColumn->addChild(tagsContainer);

    // Installing

    auto manageContainer = CCNode::create();
    manageContainer->setContentSize({ leftColumn->getContentWidth(), 10 });

    auto manageTitle = CCLabelBMFont::create("Manage", "bigFont.fnt");
    manageTitle->setScale(.25f);
    manageTitle->setOpacity(195);
    manageContainer->addChildAtPosition(manageTitle, Anchor::Left, ccp(0, 0), ccp(0, .5f));

    m_restartRequiredLabel = createGeodeTagLabel(
        "Restart Required",
        {{
            to3B(ColorProvider::get()->color("mod-list-restart-required-label"_spr)),
            to3B(ColorProvider::get()->color("mod-list-restart-required-label-bg"_spr))
        }}
    );
    m_restartRequiredLabel->setLayoutOptions(AxisLayoutOptions::create()->setMaxScale(.75f));
    m_restartRequiredLabel->setScale(.3f);
    manageContainer->addChildAtPosition(m_restartRequiredLabel, Anchor::Right, ccp(0, 0), ccp(1, .5f));

    leftColumn->addChild(manageContainer);

    auto installContainer = CCNode::create();
    installContainer->setContentSize({ leftColumn->getContentWidth(), 25 });
    installContainer->setAnchorPoint({ .5f, .5f });

    m_installBG = CCScale9Sprite::create("square02b_001.png");
    m_installBG->setScale(.3f);
    m_installBG->setContentSize(installContainer->getContentSize() / m_installBG->getScale());
    installContainer->addChildAtPosition(m_installBG, Anchor::Center);

    m_installMenu = CCMenu::create();
    m_installMenu->ignoreAnchorPointForPosition(false);
    m_installMenu->setContentSize(installContainer->getContentSize() - ccp(10, 10));
    m_installMenu->setAnchorPoint({ .5f, .5f });

    auto enableModOffSpr = createGeodeButton(
        CCSprite::createWithSpriteFrameName("GJ_completesIcon_001.png"),
        "Enable",
        "GJ_button_01.png"
    );
    enableModOffSpr->setScale(.5f);
    auto enableModOnSpr = createGeodeButton(
        CCSprite::createWithSpriteFrameName("GJ_deleteIcon_001.png"),
        "Disable",
        "GJ_button_06.png"
    );
    enableModOnSpr->setScale(.5f);
    m_enableBtn = CCMenuItemToggler::create(
        enableModOffSpr, enableModOnSpr,
        this, menu_selector(ModPopup::onEnable)
    );
    m_enableBtn->m_notClickable = true;
    m_installMenu->addChild(m_enableBtn);

    auto reenableModOffSpr = createGeodeButton(
        CCSprite::createWithSpriteFrameName("reset.png"_spr),
        "Re-Enable",
        "GE_button_05.png"_spr
    );
    reenableModOffSpr->setScale(.5f);
    auto reenableModOnSpr = createGeodeButton(
        CCSprite::createWithSpriteFrameName("reset.png"_spr),
        "Re-Disable",
        "GE_button_05.png"_spr
    );
    reenableModOnSpr->setScale(.5f);
    m_reenableBtn = CCMenuItemToggler::create(
        reenableModOffSpr, reenableModOnSpr,
        this, menu_selector(ModPopup::onEnable)
    );
    m_reenableBtn->m_notClickable = true;
    m_installMenu->addChild(m_reenableBtn);

    auto installModSpr = createGeodeButton(
        CCSprite::createWithSpriteFrameName("GJ_downloadsIcon_001.png"),
        "Install",
        "GE_button_01.png"_spr
    );
    installModSpr->setScale(.5f);
    m_installBtn = CCMenuItemSpriteExtra::create(
        installModSpr, this, nullptr
    );
    m_installMenu->addChild(m_installBtn);

    auto uninstallModSpr = createGeodeButton(
        CCSprite::createWithSpriteFrameName("delete-white.png"_spr),
        "Uninstall",
        "GE_button_05.png"_spr
    );
    uninstallModSpr->setScale(.5f);
    m_uninstallBtn = CCMenuItemSpriteExtra::create(
        uninstallModSpr, this, menu_selector(ModPopup::onUninstall)
    );
    m_installMenu->addChild(m_uninstallBtn);

    m_installStatusLabel = CCLabelBMFont::create("", "bigFont.fnt");
    m_installStatusLabel->setOpacity(120);
    m_installMenu->addChild(m_installStatusLabel);

    m_installMenu->setLayout(
        RowLayout::create()
            ->setDefaultScaleLimits(.1f, 1)
            ->setAxisAlignment(AxisAlignment::Center)
    );
    m_installMenu->getLayout()->ignoreInvisibleChildren(true);
    installContainer->addChildAtPosition(m_installMenu, Anchor::Center);

    leftColumn->addChild(installContainer);

    // Links

    auto linksContainer = CCNode::create();
    linksContainer->setContentSize({ leftColumn->getContentWidth() - 40, 30 });
    linksContainer->setAnchorPoint({ .5f, .5f });
    linksContainer->setLayoutOptions(
        AxisLayoutOptions::create()
            ->setCrossAxisAlignment(AxisAlignment::End)
    );

    auto linksBG = CCScale9Sprite::create("square02b_001.png");
    linksBG->setColor({ 0, 0, 0 });
    linksBG->setOpacity(75);
    linksBG->setScale(.3f);
    linksBG->setContentSize(linksContainer->getContentSize() / linksBG->getScale());
    linksContainer->addChildAtPosition(linksBG, Anchor::Center);

    auto linksMenu = CCMenu::create();
    linksMenu->ignoreAnchorPointForPosition(false);
    linksMenu->setContentSize(linksContainer->getContentSize() - ccp(10, 10));
    linksMenu->setAnchorPoint({ .5f, .5f });

    // auto linksLabel = CCLabelBMFont::create("Links", "bigFont.fnt");
    // linksLabel->setLayoutOptions(
    //     AxisLayoutOptions::create()
    //         ->setRelativeScale(.35f)
    // );
    // linksMenu->addChild(linksLabel);

    for (auto stat : std::initializer_list<std::tuple<
        const char*, std::optional<std::string>, SEL_MenuHandler
    >> {
        { "homepage.png"_spr, m_source.getMetadata().getLinks().getHomepageURL(), nullptr },
        { "github.png"_spr, m_source.getMetadata().getLinks().getSourceURL(), nullptr },
        { "gj_discordIcon_001.png", m_source.getMetadata().getLinks().getCommunityURL(), nullptr },
        { "gift.png"_spr, m_source.getMetadata().getSupportInfo(), menu_selector(ModPopup::onSupport) },
    }) {
        auto spr = CCSprite::createWithSpriteFrameName(std::get<0>(stat));
        spr->setScale(.75f);
        if (!std::get<1>(stat).has_value()) {
            spr->setColor({ 155, 155, 155 });
            spr->setOpacity(155);
        }
        auto btn = CCMenuItemSpriteExtra::create(
            spr, this, (
                std::get<1>(stat).has_value() ?
                    (std::get<2>(stat) ? std::get<2>(stat) : menu_selector(ModPopup::onLink)) : 
                    nullptr
            )
        );
        if (!std::get<2>(stat) && std::get<1>(stat)) {
            btn->setUserObject("url", CCString::create(*std::get<1>(stat)));
        }
        linksMenu->addChild(btn);
    }

    linksMenu->setLayout(
        RowLayout::create()
            ->setDefaultScaleLimits(.1f, 1)
            ->setAxisAlignment(AxisAlignment::Center)
    );
    linksContainer->addChildAtPosition(linksMenu, Anchor::Center);

    leftColumn->addChild(linksContainer);

    // auto bottomPadding = CCNode::create();
    // bottomPadding->setContentHeight(13);
    // leftColumn->addChild(bottomPadding);

    leftColumn->setLayout(
        ColumnLayout::create()
            ->setAxisReverse(true)
            ->setCrossAxisOverflow(false)
            ->setAutoScale(false)
            ->setAxisAlignment(AxisAlignment::Start)
            ->setCrossAxisLineAlignment(AxisAlignment::Start)
            ->setGap(4)
    );
    mainContainer->addChild(leftColumn);

    m_rightColumn = CCNode::create();
    m_rightColumn->setContentSize({
        // The -5 is to give a little bit of padding
        mainContainer->getContentWidth() - leftColumn->getContentWidth() - 
            static_cast<AxisLayout*>(mainContainer->getLayout())->getGap(),
        mainContainer->getContentHeight()
    });

    auto tabsMenu = CCMenu::create();
    tabsMenu->ignoreAnchorPointForPosition(false);
    tabsMenu->setScale(.65f);
    tabsMenu->setContentWidth(m_rightColumn->getContentWidth() / tabsMenu->getScale());
    tabsMenu->setAnchorPoint({ .5f, 1.f });

    for (auto mdTab : std::initializer_list<std::tuple<const char*, const char*, Tab>> {
        { "message.png"_spr,   "Description", Tab::Details },
        { "changelog.png"_spr, "Changelog",   Tab::Changelog },
        { "version.png"_spr,   "Versions",    Tab::Versions },
    }) {
        auto spr = GeodeTabSprite::create(std::get<0>(mdTab), std::get<1>(mdTab), 140, m_source.asServer());
        auto btn = CCMenuItemSpriteExtra::create(spr, this, menu_selector(ModPopup::onTab));
        btn->setTag(static_cast<int>(std::get<2>(mdTab)));
        tabsMenu->addChild(btn);
        m_tabs.insert({ std::get<2>(mdTab), { spr, nullptr } });
    }

    tabsMenu->setLayout(RowLayout::create());
    m_rightColumn->addChildAtPosition(tabsMenu, Anchor::Top);

    mainContainer->addChildAtPosition(m_rightColumn, Anchor::Right, ccp(-20, 0));

    mainContainer->updateLayout();
    m_mainLayer->addChildAtPosition(mainContainer, Anchor::Center);

    auto settingsSpr = createGeodeCircleButton("settings.png"_spr);
    settingsSpr->setScale(.6f);
    auto settingsBtn = CCMenuItemSpriteExtra::create(
        settingsSpr, this, nullptr
    );
    m_buttonMenu->addChildAtPosition(settingsBtn, Anchor::BottomLeft, ccp(28, 25));

    // Select details tab
    this->loadTab(Tab::Details);

    // Update enabling and installing buttons
    this->updateState();

    // Load stats from server (or just from the source if it already has them)
    m_statsListener.bind(this, &ModPopup::onLoadServerInfo);
    m_statsListener.setFilter(m_source.fetchServerInfo().listen());
    m_tagsListener.bind(this, &ModPopup::onLoadTags);
    m_tagsListener.setFilter(m_source.fetchValidTags().listen());

    if (m_source.asMod()) {
        m_checkUpdateListener.bind(this, &ModPopup::onCheckUpdates);
        m_checkUpdateListener.setFilter(m_source.checkUpdates().listen());
    }

    // Only listen for updates on this mod specifically
    m_updateStateListener.setFilter(UpdateModListStateFilter(UpdateModState(m_source.getID())));
    m_updateStateListener.bind([this](auto) { this->updateState(); });

    return true;
}

void ModPopup::updateState() {
    auto asMod = m_source.asMod();
    auto asServer = m_source.asServer();
    auto wantsRestart = m_source.wantsRestart();

    m_enableBtn->toggle(asMod && asMod->isOrWillBeEnabled());
    m_enableBtn->setVisible(asMod && asMod->getRequestedAction() == ModRequestedAction::None);

    m_reenableBtn->toggle(m_enableBtn->isToggled());
    m_reenableBtn->setVisible(asMod && modRequestedActionIsToggle(asMod->getRequestedAction()));

    m_installBtn->setVisible(asServer);
    m_uninstallBtn->setVisible(asMod && asMod->getRequestedAction() == ModRequestedAction::None);

    m_installBG->setColor(wantsRestart ? to3B(ColorProvider::get()->color("mod-list-restart-required-label"_spr)) : ccc3(0, 0, 0));
    m_installBG->setOpacity(wantsRestart ? 40 : 75);
    m_restartRequiredLabel->setVisible(wantsRestart);

    if (asMod && modRequestedActionIsUninstall(asMod->getRequestedAction())) {
        m_installStatusLabel->setString("Mod has been uninstalled");
    }
    else {
        m_installStatusLabel->setString("");
    }

    m_installMenu->updateLayout();
}

void ModPopup::setStatIcon(CCNode* stat, const char* spr) {
    // Remove old icon
    stat->removeChildByID("icon");
    
    // Create new icon
    if (spr) {
        auto iconSize = stat->getContentHeight();
        auto icon = CCSprite::createWithSpriteFrameName(spr);
        limitNodeSize(icon, { iconSize, iconSize }, 1.f, .1f);
        icon->setID("icon");
        stat->addChildAtPosition(icon, Anchor::Left, ccp(stat->getContentHeight() / 2, 0));
    }
}

void ModPopup::setStatLabel(CCNode* stat, std::string const& value, bool noValue, ccColor3B color) {
    auto container = stat->getChildByID("labels");

    // Update label
    auto label = static_cast<CCLabelBMFont*>(container->getChildByID("label"));
    label->setString(value.c_str());
    label->setColor(color);

    // Remove value if requested
    if (noValue) {
        container->getChildByID("value-label")->setVisible(false);
        container->getChildByID("loading-spinner")->setVisible(false);
    }

    container->updateLayout();
}

void ModPopup::setStatValue(CCNode* stat, std::optional<std::string> const& value) {
    auto container = stat->getChildByID("labels");
    auto valueLabel = static_cast<CCLabelBMFont*>(container->getChildByID("value-label"));
    auto spinner = container->getChildByID("loading-spinner");

    // Show loading if no value provided
    valueLabel->setVisible(value.has_value());
    spinner->setVisible(!value.has_value());
    
    // Update value
    if (value) {
        valueLabel->setString(value.value().c_str());
    }

    // Update layout
    container->updateLayout();
}

void ModPopup::onLoadServerInfo(PromiseEvent<server::ServerModMetadata, server::ServerError>* event) {
    if (auto data = event->getResolve()) {
        auto timeToString = [](auto const& time) {
            if (time.has_value()) {
                return time.value().toAgoString();
            }
            return std::string("N/A");
        };

        // Update server stats
        for (auto id : std::initializer_list<std::pair<const char*, std::string>> {
            { "downloads", std::to_string(data->downloadCount) },
            { "release-date", timeToString(data->createdAt) },
            { "update-date", timeToString(data->updatedAt) },
        }) {
            if (auto stat = m_stats->getChildByID(id.first)) {
                this->setStatValue(stat, id.second);
            }
        }
    }
    else if (auto err = event->getReject()) {
        for (auto child : CCArrayExt<CCNode*>(m_stats->getChildren())) {
            if (child->getUserObject("stats")) {
                this->setStatValue(child, "N/A");
            }
        }
    }
}

void ModPopup::onCheckUpdates(PromiseEvent<std::optional<server::ServerModUpdate>, server::ServerError>* event) {
    if (auto resolved = event->getResolve()) {
        // Check if this has updates for an installed mod
        auto updatesStat = m_stats->getChildByID("update-check");
        if (resolved->has_value()) {
            this->setStatIcon(updatesStat, "updates-available.png"_spr);
            this->setStatLabel(updatesStat, "Update Found", false, ccc3(99, 250, 255));
            this->setStatValue(updatesStat, resolved->value().version.toString());
        }
        else {
            this->setStatIcon(updatesStat, "GJ_completesIcon_001.png");
            this->setStatLabel(updatesStat, "Up to Date!", true, ccc3(78, 245, 0));
        }
    }
    else if (auto err = event->getReject()) {
        auto updatesStat = m_stats->getChildByID("update-check");
        this->setStatLabel(updatesStat, "No Updates Found", true, ccc3(125, 125, 125));
    }
}

void ModPopup::onLoadTags(PromiseEvent<std::unordered_set<std::string>, server::ServerError>* event) {
    if (auto data = event->getResolve()) {
        m_tags->removeAllChildren();
        
        for (auto& tag : *data) {
            auto readable = tag;
            readable[0] = std::toupper(readable[0]);
            auto colors = geodeTagColor(tag);
            m_tags->addChild(createGeodeTagLabel(readable));
        }
        
        if (data->empty()) {
            auto label = CCLabelBMFont::create("No tags found", "bigFont.fnt");
            label->setOpacity(120);
            m_tags->addChild(label);
        }

        m_tags->updateLayout();
    }
    else if (auto err = event->getReject()) {
        m_tags->removeAllChildren();

        auto label = CCLabelBMFont::create("No tags found", "bigFont.fnt");
        label->setOpacity(120);
        m_tags->addChild(label);

        m_tags->updateLayout();
    }
}

void ModPopup::loadTab(ModPopup::Tab tab) {
    // Remove current page
    if (m_currentTabPage) {
        m_currentTabPage->removeFromParent();
    }

    // Highlight selected tab
    for (auto [value, btn] : m_tabs) {
        btn.first->select(value == tab);
    }

    if (auto existing = m_tabs.at(tab).second) {
        m_currentTabPage = existing;
        m_rightColumn->addChildAtPosition(existing, Anchor::Bottom);
    }
    else {
        const auto size = (m_rightColumn->getContentSize() - ccp(0, 30));
        const float mdScale = .85f;
        switch (tab) {
            case Tab::Details: {
                m_currentTabPage = MDTextArea::create(
                    m_source.getAbout().value_or("No description provided"),
                    size / mdScale
                );
                m_currentTabPage->setScale(mdScale);
            } break;

            case Tab::Changelog: {
                m_currentTabPage = MDTextArea::create(
                    m_source.getChangelog().value_or("No changelog provided"),
                    size / mdScale
                );
                m_currentTabPage->setScale(mdScale);
            } break;

            case Tab::Versions: {
                m_currentTabPage = CCNode::create();
                m_currentTabPage->setContentSize(size);
            } break;
        }
        m_currentTabPage->setAnchorPoint({ .5f, .0f });
        m_rightColumn->addChildAtPosition(m_currentTabPage, Anchor::Bottom);
        m_tabs.at(tab).second = m_currentTabPage;
    }
}

void ModPopup::onTab(CCObject* sender) {
    this->loadTab(static_cast<Tab>(sender->getTag()));
}

void ModPopup::onEnable(CCObject*) {
    if (auto mod = m_source.asMod()) {
        // Toggle the mod state
        auto res = mod->isOrWillBeEnabled() ? mod->disable() : mod->enable();
        if (!res) {
            FLAlertLayer::create("Error Toggling Mod", res.unwrapErr(), "OK")->show();
        }
    }
    else {
        FLAlertLayer::create("Error Toggling Mod", "This mod can not be toggled!", "OK")->show();
    }
    UpdateModListStateEvent(UpdateModState(m_source.getID())).post();
}

void ModPopup::onUninstall(CCObject*) {
    if (auto mod = m_source.asMod()) {
        ConfirmUninstallPopup::create(mod)->show();
    }
    else {
        FLAlertLayer::create(
            "Error Uninstalling Mod",
            "This mod can not be uninstalled! (It is not installed at all)", 
            "OK"
        )->show();
    }
}

void ModPopup::onLink(CCObject* sender) {
    auto url = static_cast<CCString*>(static_cast<CCNode*>(sender)->getUserObject("url"));
    web::openLinkInBrowser(url->getCString());
}

void ModPopup::onSupport(CCObject*) {
    openSupportPopup(m_source.getMetadata());
}

ModPopup* ModPopup::create(ModSource&& src) {
    auto ret = new ModPopup();
    bool isServer = src.asServer();
    if (ret && ret->init(440, 280, std::move(src), isServer)) {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}
