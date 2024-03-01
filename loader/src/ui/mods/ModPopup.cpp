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

    const CCSize titleSize { 230, 50 };
    const float titlePad = 10;

    auto titleContainer = CCNode::create();
    titleContainer->setContentSize(titleSize);
    titleContainer->setAnchorPoint({ 0, 1 });
    titleContainer->setScale(.6f);

    auto logo = m_source.createModLogo();
    limitNodeSize(logo, { titleSize.height, titleSize.height }, 5.f, .1f);
    titleContainer->addChildAtPosition(logo, Anchor::Left, ccp(titleSize.height / 2, 0));

    auto title = CCLabelBMFont::create(m_source.getMetadata().getName().c_str(), "bigFont.fnt");
    title->limitLabelWidth(titleSize.width - titleSize.height - titlePad, 1.f, .1f);
    title->setAnchorPoint({ .0f, .5f });
    titleContainer->addChildAtPosition(title, Anchor::TopLeft, ccp(titleSize.height + titlePad, -titleSize.height * .25f));

    auto by = "By " + ModMetadata::formatDeveloperDisplayString(m_source.getMetadata().getDevelopers());
    auto dev = CCLabelBMFont::create(by.c_str(), "goldFont.fnt");
    dev->limitLabelWidth(titleSize.width - titleSize.height - titlePad, .75f, .1f);
    dev->setAnchorPoint({ .0f, .5f });
    titleContainer->addChildAtPosition(dev, Anchor::BottomLeft, ccp(titleSize.height + titlePad, titleSize.height * .25f));

    m_mainLayer->addChildAtPosition(titleContainer, Anchor::TopLeft, ccp(20, -35));

    auto statsBG = CCScale9Sprite::create("square02b_001.png");
    statsBG->setColor({ 0, 0, 0 });
    statsBG->setOpacity(90);
    statsBG->setAnchorPoint({ 0, 0 });
    statsBG->setContentSize({ 120, 130 });
    m_mainLayer->addChildAtPosition(statsBG, Anchor::BottomLeft, ccp(30, 30));

    auto tabsSize = CCSize { 250, 250 };
    auto tabsContainer = CCNode::create();
    tabsContainer->setContentSize(tabsSize);
    tabsContainer->setAnchorPoint({ 1.f, .5f });

    m_mdArea = MDTextArea::create("", (tabsSize - ccp(0, 30)));
    m_mdArea->setAnchorPoint({ .5f, .0f });
    tabsContainer->addChildAtPosition(m_mdArea, Anchor::Bottom);

    m_tabsMenu = CCMenu::create();
    m_tabsMenu->ignoreAnchorPointForPosition(false);
    m_tabsMenu->setScale(.65f);
    m_tabsMenu->setContentWidth(m_mdArea->getScaledContentSize().width / m_tabsMenu->getScale());
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

    m_mainLayer->addChildAtPosition(tabsContainer, Anchor::Right, ccp(-20, 0));

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
