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
    title->limitLabelWidth(titleContainer->getContentWidth() - devAndTitlePos, .6f, .1f);
    title->setAnchorPoint({ .0f, .5f });
    titleContainer->addChildAtPosition(title, Anchor::TopLeft, ccp(devAndTitlePos, -titleContainer->getContentHeight() * .25f));

    auto by = "By " + ModMetadata::formatDeveloperDisplayString(m_source.getMetadata().getDevelopers());
    auto dev = CCLabelBMFont::create(by.c_str(), "goldFont.fnt");
    dev->limitLabelWidth(titleContainer->getContentWidth() - devAndTitlePos, .45f, .05f);
    dev->setAnchorPoint({ .0f, .5f });
    titleContainer->addChildAtPosition(dev, Anchor::BottomLeft, ccp(devAndTitlePos, titleContainer->getContentHeight() * .25f));

    leftColumn->addChild(titleContainer);

    auto statsContainer = CCNode::create();
    statsContainer->setContentSize({ leftColumn->getContentWidth(), 70 });
    statsContainer->setAnchorPoint({ .5f, .5f });

    auto statsBG = CCScale9Sprite::create("square02b_001.png");
    statsBG->setColor({ 0, 0, 0 });
    statsBG->setOpacity(90);
    statsBG->setScale(.6f);
    statsBG->setContentSize(statsContainer->getContentSize() / statsBG->getScale());
    statsContainer->addChildAtPosition(statsBG, Anchor::Center);

    auto statsLayout = CCNode::create();
    statsLayout->setContentSize(statsContainer->getContentSize() - ccp(10, 10));
    statsLayout->setAnchorPoint({ .5f, .5f });

    for (auto stat : std::initializer_list<std::tuple<const char*, const char*, std::string, ccColor3B>> {
        { "GJ_downloadsIcon_001.png", "Downloads", "TODO", { 255, 255, 255 } },
        { "GJ_timeIcon_001.png", "Released", "TODO", { 105, 155, 255 } },
        { "GJ_timeIcon_001.png", "Updated", "TODO", { 105, 155, 255 } },
        { "version.png"_spr, "Version", src.getMetadata().getVersion().toString(), { 105, 255, 155 } },
    }) {
        auto container = CCNode::create();
        container->setContentSize({ statsLayout->getContentWidth(), 10 });

        auto iconSize = container->getContentHeight();
        auto icon = CCSprite::createWithSpriteFrameName(std::get<0>(stat));
        limitNodeSize(icon, { iconSize, iconSize }, 1.f, .1f);
        container->addChildAtPosition(icon, Anchor::Left, ccp(container->getContentHeight() / 2, 0));

        auto title = CCLabelBMFont::create(std::get<1>(stat), "bigFont.fnt");
        title->setAnchorPoint({ .0f, .5f });
        limitNodeSize(
            title,
            {
                container->getContentWidth() / 2 - container->getContentHeight() - 5,
                container->getContentHeight()
            },
            .3f, .1f
        );
        container->addChildAtPosition(title, Anchor::Left, ccp(container->getContentHeight() + 5, 0));

        auto value = CCLabelBMFont::create(std::get<2>(stat).c_str(), "bigFont.fnt");
        value->setAnchorPoint({ 1.f, .5f });
        value->setColor(std::get<3>(stat));
        limitNodeSize(
            value,
            {
                container->getContentWidth() / 2.2f,
                container->getContentHeight()
            },
            .3f, .1f
        );
        container->addChildAtPosition(value, Anchor::Right);

        statsLayout->addChild(container);
    }

    statsLayout->setLayout(
        ColumnLayout::create()
            ->setAxisReverse(true)
            ->setDefaultScaleLimits(.1f, 1)
            ->setAxisAlignment(AxisAlignment::Even)
    );
    statsContainer->addChildAtPosition(statsLayout, Anchor::Center);

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

    return true;
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
