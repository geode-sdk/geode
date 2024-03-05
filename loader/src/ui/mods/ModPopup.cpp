#include "ModPopup.hpp"
#include <Geode/ui/MDTextArea.hpp>

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
    leftColumn->setContentSize({ 145, mainContainer->getContentHeight() - 30 });

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

    auto statsContainer = CCNode::create();
    statsContainer->setContentSize({ leftColumn->getContentWidth(), 80 });
    statsContainer->setAnchorPoint({ .5f, .5f });

    auto statsBG = CCScale9Sprite::create("square02b_001.png");
    statsBG->setColor({ 0, 0, 0 });
    statsBG->setOpacity(90);
    statsBG->setScale(.6f);
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

    leftColumn->addChild(SpacerNode::create());

    leftColumn->setLayout(
        ColumnLayout::create()
            ->setAxisReverse(true)
            ->setCrossAxisOverflow(false)
            ->setDefaultScaleLimits(.1f, 1)
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

    // Select details tab
    this->loadTab(Tab::Details);

    // Load stats from server (or just from the source if it already has them)
    m_statsListener.bind(this, &ModPopup::onLoadServerInfo);
    m_statsListener.setFilter(m_source.fetchServerInfo().listen());

    return true;
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
            this->setStatLabel(updatesStat, "Update Found", false, { 99, 250, 255 });
            this->setStatValue(updatesStat, data->latestVersion().getVersion().toString());
        }
        else {
            this->setStatIcon(updatesStat, "GJ_completesIcon_001.png");
            this->setStatLabel(updatesStat, "Up to Date!", true, { 78, 245, 0 });
        }
    }
    else if (auto err = event->getReject()) {
        for (auto child : CCArrayExt<CCNode*>(m_stats->getChildren())) {
            if (!child->getUserObject("client-side")) {
                this->setStatValue(child, "N/A");
            }
        }
        auto updatesStat = m_stats->getChildByID("update-check");
        this->setStatLabel(updatesStat, "No Updates Found", true, { 125, 125, 125 });
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

ModPopup* ModPopup::create(ModSource&& src) {
    auto ret = new ModPopup();
    if (ret && ret->initAnchored(440, 280, std::move(src), "GE_square01.png"_spr)) {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}
