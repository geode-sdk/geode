#include "ModPopup.hpp"
#include <Geode/ui/MDTextArea.hpp>
#include "GeodeStyle.hpp"

std::optional<std::string> getModMarkdownData(ModSource const& src, ModMarkdownData data) {
    switch (data) {
        default: return std::nullopt;
        case ModMarkdownData::Details: return src.getMetadata().getDetails().value_or("No description provided");
        case ModMarkdownData::Changelog: return src.getMetadata().getChangelog();
    }
}

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
    leftColumn->setContentSize({ 125, mainContainer->getContentHeight() - 30 });

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

    for (auto stat : std::initializer_list<std::tuple<const char*, const char*, std::string>> {
        { "GJ_downloadsIcon_001.png", "Downloads", "TODO" },
        { "GJ_timeIcon_001.png", "Released", "TODO" },
        { "GJ_timeIcon_001.png", "Updated", "TODO" },
        { "version.png"_spr, "Version", src.getMetadata().getVersion().toString() },
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
        limitNodeSize(
            value,
            {
                container->getContentWidth() / 2.5f,
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

    auto tabsContainer = CCNode::create();
    tabsContainer->setContentSize({
        // The -5 is to give a little bit of padding
        mainContainer->getContentWidth() - leftColumn->getContentWidth() - 
            static_cast<AxisLayout*>(mainContainer->getLayout())->getGap(),
        mainContainer->getContentHeight()
    });

    m_mdArea = MDTextArea::create("", (tabsContainer->getContentSize() - ccp(0, 30)));
    m_mdArea->setAnchorPoint({ .5f, .0f });
    tabsContainer->addChildAtPosition(m_mdArea, Anchor::Bottom);

    m_tabsMenu = CCMenu::create();
    m_tabsMenu->ignoreAnchorPointForPosition(false);
    m_tabsMenu->setScale(.65f);
    m_tabsMenu->setContentWidth(m_mdArea->getScaledContentWidth() / m_tabsMenu->getScale());
    m_tabsMenu->setAnchorPoint({ .5f, 1.f });

    for (auto mdTab : std::initializer_list<std::tuple<const char*, const char*, ModMarkdownData>> {
        { "GJ_chatBtn_02_001.png", "Description", ModMarkdownData::Details },
        { "changelog.png"_spr, "Changelog", ModMarkdownData::Changelog },
    }) {
        auto btn = CCMenuItemSpriteExtra::create(
            GeodeTabSprite::create(std::get<0>(mdTab), std::get<1>(mdTab), 140),
            this, menu_selector(ModPopup::onMDTab)
        );
        if (auto data = getModMarkdownData(m_source, std::get<2>(mdTab))) {
            btn->setTag(static_cast<int>(std::get<2>(mdTab)));
        }
        else {
            btn->setEnabled(false);
            static_cast<GeodeTabSprite*>(btn->getNormalImage())->disable(true);
        }
        m_tabsMenu->addChild(btn);
    }

    m_tabsMenu->setLayout(RowLayout::create());
    tabsContainer->addChildAtPosition(m_tabsMenu, Anchor::Top);

    mainContainer->addChildAtPosition(tabsContainer, Anchor::Right, ccp(-20, 0));

    mainContainer->updateLayout();
    m_mainLayer->addChildAtPosition(mainContainer, Anchor::Center);

    // Select details tab
    this->onMDTab(m_tabsMenu->getChildren()->firstObject());

    return true;
}

void ModPopup::onMDTab(CCObject* sender) {
    if (auto data = getModMarkdownData(m_source, static_cast<ModMarkdownData>(sender->getTag()))) {
        m_mdArea->setString(data.value().c_str());
    }

    // Highlight selected tab
    for (auto tab : CCArrayExt<CCMenuItemSpriteExtra*>(m_tabsMenu->getChildren())) {
        static_cast<GeodeTabSprite*>(tab->getNormalImage())->select(tab->getTag() == sender->getTag());
    }
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
