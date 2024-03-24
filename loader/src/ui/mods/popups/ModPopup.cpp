#include "ModPopup.hpp"
#include <Geode/ui/MDTextArea.hpp>
#include <Geode/utils/web.hpp>
#include <Geode/ui/GeodeUI.hpp>
#include <Geode/utils/ColorProvider.hpp>

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
        const char*, const char*, const char*, std::optional<std::string>
    >> {
        { "GJ_downloadsIcon_001.png", "Downloads", "downloads", std::nullopt },
        { "GJ_timeIcon_001.png", "Released", "release-date", std::nullopt },
        { "GJ_timeIcon_001.png", "Updated", "update-date", std::nullopt },
        { "version.png"_spr, "Version", "version", m_source.getMetadata().getVersion().toString() },
        { nullptr, "Checking for updates", "update-check", std::nullopt },
    }) {
        auto container = CCNode::create();
        container->setContentSize({ m_stats->getContentWidth(), 10 });
        container->setID(std::get<2>(stat));
        if (std::get<3>(stat).has_value()) {
            container->setUserObject("client-side", CCBool::create(true));
        }

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

        // todo: refactor these spinners into a reusable class that's not the ass LoadingCircle is
        auto spinnerContainer = CCNode::create();
        spinnerContainer->setContentSize({
            container->getContentHeight() / labelContainer->getScale(),
            container->getContentHeight() / labelContainer->getScale()
        });
        spinnerContainer->setID("loading-spinner");

        auto spinner = CCSprite::create("loadingCircle.png");
        spinner->setBlendFunc({ GL_ONE, GL_ONE });
        spinner->runAction(CCRepeatForever::create(CCRotateBy::create(1.f, 360.f)));
        limitNodeSize(spinner, spinnerContainer->getContentSize(), 1.f, .1f);
        spinnerContainer->addChildAtPosition(spinner, Anchor::Center);

        labelContainer->addChild(spinnerContainer);

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

    // todo: refactor these spinners into a reusable class that's not the ass LoadingCircle is
    auto tagsSpinnerContainer = CCNode::create();
    tagsSpinnerContainer->setContentSize({ 50, 50 });
    tagsSpinnerContainer->setID("loading-spinner");

    auto tagsSpinner = CCSprite::create("loadingCircle.png");
    tagsSpinner->setBlendFunc({ GL_ONE, GL_ONE });
    tagsSpinner->runAction(CCRepeatForever::create(CCRotateBy::create(1.f, 360.f)));
    limitNodeSize(tagsSpinner, tagsSpinnerContainer->getContentSize(), 1.f, .1f);
    tagsSpinnerContainer->addChildAtPosition(tagsSpinner, Anchor::Center);

    m_tags->addChild(tagsSpinnerContainer);

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
        to3B(ColorProvider::get()->color("mod-list-restart-required-label"_spr)),
        to3B(ColorProvider::get()->color("mod-list-restart-required-label-bg"_spr))
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

    for (auto stat : std::initializer_list<std::tuple<
        CCMenuItemToggler**,
        const char*, const char*, const char*,
        const char*, const char*, const char*, SEL_MenuHandler
    >> {
        {
            &m_enableBtn,
            "GJ_completesIcon_001.png", "Enable", "GJ_button_01.png",
            "GJ_deleteIcon_001.png", "Disable", "GJ_button_06.png",
            menu_selector(ModPopup::onEnable)
        },
        {
            &m_installBtn,
            "GJ_downloadsIcon_001.png", "Install", "GE_button_01.png"_spr,
            "edit_delBtn_001.png", "Uninstall", "GE_button_05.png"_spr,
            nullptr
        },
    }) {
        auto onSpr = createGeodeButton(
            CCSprite::createWithSpriteFrameName(std::get<1>(stat)),
            std::get<2>(stat),
            std::get<3>(stat)
        );
        onSpr->setScale(.5f);
        auto offSpr = createGeodeButton(
            CCSprite::createWithSpriteFrameName(std::get<4>(stat)),
            std::get<5>(stat),
            std::get<6>(stat)
        );
        offSpr->setScale(.5f);
        auto toggle = CCMenuItemToggler::create(offSpr, onSpr, this, std::get<7>(stat));
        toggle->m_notClickable = true;
        m_installMenu->addChild(toggle);
        *std::get<0>(stat) = toggle;
    }

    m_installMenu->setLayout(
        RowLayout::create()
            ->setDefaultScaleLimits(.1f, 1)
            ->setAxisAlignment(AxisAlignment::Center)
    );
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
        auto spr = GeodeTabSprite::create(std::get<0>(mdTab), std::get<1>(mdTab), 140);
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

    return true;
}

void ModPopup::updateState() {
    auto shouldEnableEnableBtn = 
        m_source.asMod() && 
        !modRequestedActionIsUninstall(m_source.asMod()->getRequestedAction());
    
    auto shouldEnableInstallBtn = 
        !modRequestedActionIsToggle(m_source.asMod()->getRequestedAction());
    
    auto wantsRestart = m_source.wantsRestart();

    auto enableBtnOff = static_cast<IconButtonSprite*>(m_enableBtn->m_offButton->getNormalImage());
    auto enableBtnOn = static_cast<IconButtonSprite*>(m_enableBtn->m_onButton->getNormalImage());
    auto installBtnOff = static_cast<IconButtonSprite*>(m_installBtn->m_offButton->getNormalImage());
    auto installBtnOn = static_cast<IconButtonSprite*>(m_installBtn->m_onButton->getNormalImage());

    m_enableBtn->toggle(m_source.asMod() && m_source.asMod()->isOrWillBeEnabled());
    m_enableBtn->setEnabled(shouldEnableEnableBtn);
    enableBtnOff->setOpacity(shouldEnableEnableBtn ? 255 : 105);
    enableBtnOff->setColor(shouldEnableEnableBtn ? ccc3(255, 255, 255) : ccc3(155, 155, 155));
    enableBtnOn->setOpacity(shouldEnableEnableBtn ? 255 : 105);
    enableBtnOn->setColor(shouldEnableEnableBtn ? ccc3(255, 255, 255) : ccc3(155, 155, 155));

    // todo: uninstall just installed server mods
    m_installBtn->toggle(m_source.asMod() && modRequestedActionIsUninstall(m_source.asMod()->getRequestedAction()));
    m_installBtn->setEnabled(shouldEnableInstallBtn);
    installBtnOff->setOpacity(shouldEnableInstallBtn ? 255 : 105);
    installBtnOff->setColor(shouldEnableInstallBtn ? ccc3(255, 255, 255) : ccc3(155, 155, 155));
    installBtnOn->setOpacity(shouldEnableInstallBtn ? 255 : 105);
    installBtnOn->setColor(shouldEnableInstallBtn ? ccc3(255, 255, 255) : ccc3(155, 155, 155));

    m_installBG->setColor(wantsRestart ? to3B(ColorProvider::get()->color("mod-list-restart-required-label"_spr)) : ccc3(0, 0, 0));
    m_installBG->setOpacity(wantsRestart ? 40 : 75);
    m_restartRequiredLabel->setVisible(wantsRestart);

    // Propagate update up the chain
    if (m_updateParentState) {
        m_updateParentState();
    }
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

        // Check if this has updates for an installed mod
        auto updatesStat = m_stats->getChildByID("update-check");
        if (data->hasUpdateForInstalledMod()) {
            this->setStatIcon(updatesStat, "updates-available.png"_spr);
            this->setStatLabel(updatesStat, "Update Found", false, ccc3(99, 250, 255));
            this->setStatValue(updatesStat, data->latestVersion().getVersion().toString());
        }
        else {
            this->setStatIcon(updatesStat, "GJ_completesIcon_001.png");
            this->setStatLabel(updatesStat, "Up to Date!", true, ccc3(78, 245, 0));
        }
    }
    else if (auto err = event->getReject()) {
        for (auto child : CCArrayExt<CCNode*>(m_stats->getChildren())) {
            if (!child->getUserObject("client-side")) {
                this->setStatValue(child, "N/A");
            }
        }
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
            m_tags->addChild(createGeodeTagLabel(readable, colors.first, colors.second));
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
                    m_source.getMetadata().getDetails().value_or("No description provided"),
                    size / mdScale
                );
                m_currentTabPage->setScale(mdScale);
            } break;

            case Tab::Changelog: {
                m_currentTabPage = MDTextArea::create(
                    m_source.getMetadata().getChangelog().value_or("No changelog provided"),
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
    this->updateState();
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
    if (ret && ret->init(440, 280, std::move(src))) {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

void ModPopup::onUpdateParentState(MiniFunction<void()> listener) {
    m_updateParentState = listener;
}
