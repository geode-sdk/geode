#include "ModsLayer.hpp"
#include "SwelvyBG.hpp"
#include <Geode/ui/TextInput.hpp>
#include <Geode/utils/ColorProvider.hpp>
#include "GeodeStyle.hpp"

bool ModsLayer::init() {
    if (!CCLayer::init())
        return false;

    auto winSize = CCDirector::get()->getWinSize();
    
    this->addChild(SwelvyBG::create());
    
    auto backMenu = CCMenu::create();
    backMenu->setContentWidth(100.f);
    backMenu->setAnchorPoint({ .0f, .5f });
    
    auto backSpr = CCSprite::createWithSpriteFrameName("GJ_arrow_03_001.png");
    auto backBtn = CCMenuItemSpriteExtra::create(
        backSpr, this, menu_selector(ModsLayer::onBack)
    );
    backMenu->addChild(backBtn);

    backMenu->setLayout(
        RowLayout::create()
            ->setAxisAlignment(AxisAlignment::Start)
    );
    this->addChildAtPosition(backMenu, Anchor::TopLeft, ccp(12, -25), false);

    auto actionsMenu = CCMenu::create();
    actionsMenu->setContentHeight(200.f);
    actionsMenu->setAnchorPoint({ .5f, .0f });

    auto reloadSpr = CircleButtonSprite::create(
        CCSprite::createWithSpriteFrameName("reload.png"_spr),
        CircleBaseColor::DarkPurple,
        CircleBaseSize::Medium
    );
    reloadSpr->setScale(.8f);
    reloadSpr->setTopOffset(ccp(1, 0));
    auto reloadBtn = CCMenuItemSpriteExtra::create(
        reloadSpr, this, menu_selector(ModsLayer::onRefreshList)
    );
    actionsMenu->addChild(reloadBtn);

    actionsMenu->setLayout(
        ColumnLayout::create()
            ->setAxisAlignment(AxisAlignment::Start)
    );
    this->addChildAtPosition(actionsMenu, Anchor::BottomLeft, ccp(35, 12), false);

    m_frame = CCNode::create();
    m_frame->setAnchorPoint({ .5f, .5f });
    m_frame->setContentSize({ 380, 205 });

    auto frameBG = CCLayerColor::create(
        ColorProvider::get()->define("mod-list-bg"_spr, { 25, 17, 37, 255 })
    );
    frameBG->setContentSize(m_frame->getContentSize());
    frameBG->ignoreAnchorPointForPosition(false);
    m_frame->addChildAtPosition(frameBG, Anchor::Center);

    auto tabsTop = CCSprite::createWithSpriteFrameName("mods-list-top.png"_spr);
    tabsTop->setAnchorPoint({ .5f, .0f });
    m_frame->addChildAtPosition(tabsTop, Anchor::Top, ccp(0, -2));

    auto tabsLeft = CCSprite::createWithSpriteFrameName("mods-list-side.png"_spr);
    tabsLeft->setScaleY(m_frame->getContentHeight() / tabsLeft->getContentHeight());
    m_frame->addChildAtPosition(tabsLeft, Anchor::Left, ccp(6, 0));

    auto tabsRight = CCSprite::createWithSpriteFrameName("mods-list-side.png"_spr);
    tabsRight->setFlipX(true);
    tabsRight->setScaleY(m_frame->getContentHeight() / tabsRight->getContentHeight());
    m_frame->addChildAtPosition(tabsRight, Anchor::Right, ccp(-6, 0));

    auto tabsBottom = CCSprite::createWithSpriteFrameName("mods-list-bottom.png"_spr);
    tabsBottom->setAnchorPoint({ .5f, 1.f });
    m_frame->addChildAtPosition(tabsBottom, Anchor::Bottom, ccp(0, 2));

    this->addChildAtPosition(m_frame, Anchor::Center, ccp(0, -10), false);

    auto mainTabs = CCMenu::create();
    mainTabs->setContentWidth(tabsTop->getContentWidth() - 45);
    mainTabs->setAnchorPoint({ .5f, .0f });
    mainTabs->setPosition(m_frame->convertToWorldSpace(tabsTop->getPosition() + ccp(0, 10)));

    for (auto item : std::initializer_list<std::tuple<const char*, const char*, ModListSourceType>> {
        { "download.png"_spr, "Installed", ModListSourceType::Installed },
        { "GJ_bigStar_noShadow_001.png", "Featured", ModListSourceType::Featured },
        { "GJ_sTrendingIcon_001.png", "Trending", ModListSourceType::Trending },
        { "gj_folderBtn_001.png", "Mod Packs", ModListSourceType::ModPacks },
        { "globe.png"_spr, "All Mods", ModListSourceType::All },
    }) {
        const CCSize itemSize { 100, 35 };
        const CCSize iconSize { 18, 18 };

        auto spr = CCNode::create();
        spr->setContentSize(itemSize);
        spr->setAnchorPoint({ .5f, .5f });

        auto disabledBG = CCScale9Sprite::createWithSpriteFrameName("tab-bg.png"_spr);
        disabledBG->setScale(.8f);
        disabledBG->setContentSize(itemSize / .8f);
        disabledBG->setID("disabled-bg");
        disabledBG->setColor({ 26, 24, 29 });
        spr->addChildAtPosition(disabledBG, Anchor::Center);

        auto enabledBG = CCScale9Sprite::createWithSpriteFrameName("tab-bg.png"_spr);
        enabledBG->setScale(.8f);
        enabledBG->setContentSize(itemSize / .8f);
        enabledBG->setID("enabled-bg");
        enabledBG->setColor({ 168, 147, 185 });
        spr->addChildAtPosition(enabledBG, Anchor::Center);

        auto icon = CCSprite::createWithSpriteFrameName(std::get<0>(item));
        limitNodeSize(icon, iconSize, 3.f, .1f);
        spr->addChildAtPosition(icon, Anchor::Left, ccp(16, 0), false);

        auto title = CCLabelBMFont::create(std::get<1>(item), "bigFont.fnt");
        title->limitLabelWidth(spr->getContentWidth() - 34, .55f, .1f);
        title->setAnchorPoint({ .0f, .5f });
        spr->addChildAtPosition(title, Anchor::Left, ccp(28, 0), false);

        auto btn = CCMenuItemSpriteExtra::create(spr, this, menu_selector(ModsLayer::onTab));
        btn->setTag(static_cast<int>(std::get<2>(item)));
        mainTabs->addChild(btn);
        m_tabs.push_back(btn);
    }

    mainTabs->setLayout(RowLayout::create());
    this->addChild(mainTabs);

    // Actions

    auto listActionsMenu = CCMenu::create();
    listActionsMenu->setContentHeight(100);
    listActionsMenu->setAnchorPoint({ 1, 0 });
    listActionsMenu->setScale(.65f);

    auto bigSizeBtn = CCMenuItemSpriteExtra::create(
        GeodeButtonSprite::createWithSpriteFrameName("GJ_smallModeIcon_001.png", &m_bigView),
        this, menu_selector(ModsLayer::onBigView)
    );
    listActionsMenu->addChild(bigSizeBtn);

    auto searchBtn = CCMenuItemSpriteExtra::create(
        GeodeButtonSprite::createWithSpriteFrameName("search.png"_spr, &m_showSearch),
        this, menu_selector(ModsLayer::onSearch)
    );
    listActionsMenu->addChild(searchBtn);

    listActionsMenu->setLayout(ColumnLayout::create());
    m_frame->addChildAtPosition(listActionsMenu, Anchor::Left, ccp(-5, 25));

    m_pageMenu = CCMenu::create();
    m_pageMenu->setContentWidth(200.f);
    m_pageMenu->setAnchorPoint({ 1.f, 1.f });
    m_pageMenu->setScale(.65f);

    m_pageLabel = CCLabelBMFont::create("", "goldFont.fnt");
    m_pageLabel->setAnchorPoint({ .5f, 1.f });
    m_pageMenu->addChild(m_pageLabel);

    m_goToPageBtn = CCMenuItemSpriteExtra::create(
        CCSprite::createWithSpriteFrameName("gj_navDotBtn_on_001.png"),
        this, menu_selector(ModsLayer::onGoToPage)
    );
    m_pageMenu->addChild(m_goToPageBtn);

    m_pageMenu->setLayout(
        RowLayout::create()
            ->setAxisReverse(true)
            ->setAxisAlignment(AxisAlignment::End)
    );
    this->addChildAtPosition(m_pageMenu, Anchor::TopRight, ccp(-5, -5), false);

    this->gotoTab(ModListSourceType::Installed);

    this->setKeypadEnabled(true);
    cocos::handleTouchPriority(this, true);

    return true;
}

void ModsLayer::gotoTab(ModListSourceType type) {
    // Update selected tab
    for (auto tab : m_tabs) {
        auto selected = tab->getTag() == static_cast<int>(type);
        tab->getNormalImage()->getChildByID("disabled-bg")->setVisible(!selected);
        tab->getNormalImage()->getChildByID("enabled-bg")->setVisible(selected);
        tab->setEnabled(!selected);
    }

    auto src = ModListSource::get(type);

    // Remove current list from UI (it's Ref'd so it stays in memory)
    if (m_currentSource) {
        m_lists.at(m_currentSource)->removeFromParent();
    }
    // Update current source
    m_currentSource = src;

    // Lazily create new list and add it to UI
    if (!m_lists.contains(src)) {
        auto list = ModList::create(src, m_frame->getContentSize() - ccp(30, 0));
        list->onPageUpdated(std::bind(&ModsLayer::updatePageNumber, this));
        list->setPosition(m_frame->getPosition());
        this->addChild(list);
        m_lists.emplace(src, list);
    }
    // Add list to UI
    else {
        this->addChild(m_lists.at(src));
    }

    // Update the state of the current list
    m_lists.at(m_currentSource)->updateSize(m_bigView);
    m_lists.at(m_currentSource)->activateSearch(m_showSearch);
    m_lists.at(m_currentSource)->updatePageNumber();
}

void ModsLayer::keyBackClicked() {
    this->onBack(nullptr);
}

void ModsLayer::setTextPopupClosed(SetTextPopup* popup, gd::string value) {
    if (popup->getID() == "go-to-page"_spr) {
        if (auto res = numFromString<size_t>(value)) {
            size_t num = res.unwrap();
            // The page indices are 0-based but people think in 1-based
            if (num > 0) num -= 1;
            if (m_currentSource) {
                m_lists.at(m_currentSource)->gotoPage(num);
            }
        }
    }
}

void ModsLayer::updatePageNumber() {
    // Show current page number if the current source has total page count loaded
    if (m_currentSource && m_currentSource->getPageCount()) {
        auto page = m_lists.at(m_currentSource)->getPage() + 1;
        auto count = m_currentSource->getPageCount().value();
        auto total = m_currentSource->getItemCount().value();

        // Set the page count string
        auto fmt = fmt::format("Page {}/{} (Total {})", page, count, total);
        m_pageLabel->setString(fmt.c_str());

        // Make page menu visible
        m_pageMenu->setVisible(true);
        m_pageMenu->updateLayout();
    }
    // Hide page menu otherwise
    else {
        m_pageMenu->setVisible(false);
    }
}

void ModsLayer::onTab(CCObject* sender) {
    this->gotoTab(static_cast<ModListSourceType>(sender->getTag()));
}

void ModsLayer::onRefreshList(CCObject*) {
    m_lists.at(m_currentSource)->reloadPage();
}

void ModsLayer::onBack(CCObject*) {
    CCDirector::get()->replaceScene(CCTransitionFade::create(.5f, MenuLayer::scene(false)));
}

void ModsLayer::onGoToPage(CCObject*) {
    auto popup = SetTextPopup::create("", "Page", 5, "Go to Page", "OK", true, 60.f);
    popup->m_delegate = this;
    popup->m_input->m_allowedChars = getCommonFilterAllowedChars(CommonFilter::Uint);
    popup->setID("go-to-page"_spr);
    popup->show();
}

void ModsLayer::onBigView(CCObject*) {
    m_bigView = !m_bigView;
    // Make sure to avoid a crash
    if (m_currentSource) {
        m_lists.at(m_currentSource)->updateSize(m_bigView);
    }
}

void ModsLayer::onSearch(CCObject*) {
    m_showSearch = !m_showSearch;
    // Make sure to avoid a crash
    if (m_currentSource) {
        m_lists.at(m_currentSource)->activateSearch(m_showSearch);
    }
}

ModsLayer* ModsLayer::create() {
    auto ret = new ModsLayer();
    if (ret && ret->init()) {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

ModsLayer* ModsLayer::scene() {
    auto scene = CCScene::create();
    auto layer = ModsLayer::create();
    scene->addChild(layer);
    CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(.5f, scene));
    return layer;
}
