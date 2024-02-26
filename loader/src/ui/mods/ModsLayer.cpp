#include "ModsLayer.hpp"
#include "SwelvyBG.hpp"
#include <Geode/ui/TextInput.hpp>

static bool BIG_VIEW = false;

bool ModList::init(ModListSource* src, CCSize const& size) {
    if (!CCNode::init())
        return false;
    
    this->setContentSize(size);
    this->setAnchorPoint({ .5f, .5f });

    m_source = src;
    
    m_list = ScrollLayer::create(size);
    m_list->m_contentLayer->setLayout(
        ColumnLayout::create()
            ->setAxisReverse(true)
            ->setAxisAlignment(AxisAlignment::End)
            ->setAutoGrowAxis(size.height)
    );
    this->addChildAtPosition(m_list, Anchor::Center, -m_list->getScaledContentSize() / 2);

    auto pageLeftMenu = CCMenu::create();
    pageLeftMenu->setContentWidth(30.f);
    pageLeftMenu->setAnchorPoint({ 1.f, .5f });

    m_pagePrevBtn = CCMenuItemSpriteExtra::create(
        CCSprite::createWithSpriteFrameName("GJ_arrow_02_001.png"),
        this, menu_selector(ModList::onPage)
    );
    m_pagePrevBtn->setTag(-1);
    pageLeftMenu->addChild(m_pagePrevBtn);

    pageLeftMenu->setLayout(
        RowLayout::create()
            ->setAxisAlignment(AxisAlignment::End)
            ->setAxisReverse(true)
    );
    this->addChildAtPosition(pageLeftMenu, Anchor::Left, ccp(-5, 0));

    auto pageRightMenu = CCMenu::create();
    pageRightMenu->setContentWidth(30.f);
    pageRightMenu->setAnchorPoint({ 0.f, .5f });

    auto pageNextSpr = CCSprite::createWithSpriteFrameName("GJ_arrow_02_001.png");
    pageNextSpr->setFlipX(true);
    m_pageNextBtn = CCMenuItemSpriteExtra::create(
        pageNextSpr,
        this, menu_selector(ModList::onPage)
    );
    m_pageNextBtn->setTag(1);
    pageRightMenu->addChild(m_pageNextBtn);

    pageRightMenu->setLayout(
        RowLayout::create()
            ->setAxisAlignment(AxisAlignment::Start)
    );
    this->addChildAtPosition(pageRightMenu, Anchor::Right, ccp(5, 0));

    auto pageLabelMenu = CCMenu::create();
    pageLabelMenu->setContentWidth(200.f);
    pageLabelMenu->setAnchorPoint({ .5f, 1.f });

    // Default text is so that the button gets a proper hitbox, since it's 
    // based on sprite content size
    m_pageLabel = CCLabelBMFont::create("Page XX/XX", "bigFont.fnt");
    m_pageLabel->setAnchorPoint({ .5f, 1.f });
    m_pageLabel->setScale(.45f);

    m_pageLabelBtn = CCMenuItemSpriteExtra::create(
        m_pageLabel, this, menu_selector(ModList::onGoToPage)
    );
    pageLabelMenu->addChild(m_pageLabelBtn);

    pageLabelMenu->setLayout(RowLayout::create());
    this->addChildAtPosition(pageLabelMenu, Anchor::Bottom, ccp(0, -5));

    m_statusText = SimpleTextArea::create("", "bigFont.fnt", .6f);
    m_statusText->setAlignment(kCCTextAlignmentCenter);
    this->addChildAtPosition(m_statusText, Anchor::Center, ccp(0, 40));

    m_statusLoadingCircle = CCSprite::create("loadingCircle.png");
    m_statusLoadingCircle->runAction(CCRepeatForever::create(CCRotateBy::create(1.f, 360.f)));
    m_statusLoadingCircle->setScale(.6f);
    this->addChildAtPosition(m_statusLoadingCircle, Anchor::Center, ccp(0, -40));

    m_listener.bind(this, &ModList::onPromise);

    this->gotoPage(0);

    return true;
}

void ModList::onPromise(typename ModListSource::PageLoadEvent* event) {
    if (auto resolved = event->getResolve()) {
        // Hide status
        m_statusText->setVisible(false);
        m_statusLoadingCircle->setVisible(false);

        // Create items
        for (auto item : *resolved) {
            m_list->m_contentLayer->addChild(item);
            item->updateSize(m_list->getContentWidth(), BIG_VIEW);
        }
        // Auto-grow the size of the list content
        m_list->m_contentLayer->updateLayout();

        // Scroll list to top
        auto listTopScrollPos = -m_list->m_contentLayer->getContentHeight() + m_list->getContentHeight();
        m_list->m_contentLayer->setPositionY(listTopScrollPos);

        // Update page UI
        this->updatePageUI();
    }
    else if (auto progress = event->getProgress()) {
        // todo: percentage in a loading bar
        this->showStatus("Loading...", true);
    }
    else if (auto rejected = event->getReject()) {
        this->showStatus(rejected->message, false);
        // todo: details
        this->updatePageUI(true);
    }

    if (event->isFinally()) {
        // Clear listener
        m_listener.setFilter(ModListSource::PageLoadEventFilter());
    }
}

void ModList::onGoToPage(CCObject*) {
    auto popup = SetTextPopup::create("", "Page", 5, "Go to Page", "OK", true, 60.f);
    popup->m_delegate = this;
    popup->m_input->m_allowedChars = getCommonFilterAllowedChars(CommonFilter::Uint);
    popup->setID("go-to-page"_spr);
    popup->show();
}

void ModList::onPage(CCObject* sender) {
    // If no page count has been loaded yet, we can't do anything
    if (!m_source->getPageCount()) return;
    auto pageCount = m_source->getPageCount().value();

    // Make sure you can't go beyond the limits
    if (sender->getTag() < 0 && m_page >= -sender->getTag()) {
        m_page += sender->getTag();
    }
    // Ig this can technically overflow, but why would there be over 4 billion pages 
    // (and why would someone manually scroll that far)
    else if (sender->getTag() > 0 && m_page + sender->getTag() < m_source->getPageCount()) {
        m_page += sender->getTag();
    }

    // Load new page
    this->gotoPage(m_page);
}

void ModList::updatePageUI(bool hide) {
    auto pageCount = m_source->getPageCount();

    // Hide if page count hasn't been loaded
    if (!pageCount) {
        hide = true;
    }
    m_pagePrevBtn->setVisible(!hide && m_page > 0);
    m_pageNextBtn->setVisible(!hide && m_page < pageCount.value() - 1);
    m_pageLabelBtn->setVisible(!hide);
    if (pageCount > 0u) {
        auto fmt = fmt::format("Page {}/{}", m_page + 1, pageCount.value());
        m_pageLabel->setString(fmt.c_str());
    }
}

void ModList::setTextPopupClosed(SetTextPopup* popup, gd::string value) {
    if (popup->getID() == "go-to-page"_spr) {
        if (auto res = numFromString<size_t>(value)) {
            size_t num = res.unwrap();
            // The page indices are 0-based but people think in 1-based
            if (num > 0) num -= 1;
            this->gotoPage(num);
        }
    }
}

void ModList::reloadPage() {
    // Just force an update on the current page
    this->gotoPage(m_page, true);
}

void ModList::gotoPage(size_t page, bool update) {
    // Clear list contents
    m_list->m_contentLayer->removeAllChildren();
    m_page = page;
    
    // Start loading new page with generic loading message
    this->showStatus("Loading...", true);
    m_listener.setFilter(m_source->loadPage(page, update).listen());

    // Do initial eager update on page UI (to prevent user spamming arrows 
    // to access invalid pages)
    this->updatePageUI();
}

void ModList::showStatus(std::string const& status, bool loading) {
    // Clear list contents
    m_list->m_contentLayer->removeAllChildren();

    // Update status
    m_statusText->setText(status);
    m_statusText->updateAnchoredPosition(Anchor::Center, (loading ? ccp(0, 40) : ccp(0, 0)));

    // Make status visible
    m_statusText->setVisible(true);
    m_statusLoadingCircle->setVisible(loading);
}

ModList* ModList::create(ModListSource* src, CCSize const& size) {
    auto ret = new ModList();
    if (ret && ret->init(src, size)) {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

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

    auto frameBG = CCLayerColor::create({ 25, 17, 37, 255 });
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

    for (auto item : std::initializer_list<std::tuple<const char*, const char*, ModListSource*>> {
        { "download.png"_spr, "Installed", InstalledModsList::get() },
        { "GJ_bigStar_noShadow_001.png", "Featured", FeaturedModsList::get() },
        { "GJ_sTrendingIcon_001.png", "Trending", nullptr },
        { "gj_folderBtn_001.png", "Mod Packs", ModPacksModsList::get() },
        { "search.png"_spr, "Search", nullptr },
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
        btn->setUserData(std::get<2>(item));
        mainTabs->addChild(btn);
        m_tabs.push_back(btn);
    }

    mainTabs->setLayout(RowLayout::create());
    this->addChild(mainTabs);

    this->gotoTab();

    this->setKeypadEnabled(true);
    cocos::handleTouchPriority(this, true);

    return true;
}

void ModsLayer::gotoTab(ModListSource* src) {
    // Default to installed mods
    if (!src) {
        src = InstalledModsList::get();
    }

    // Update selected tab
    for (auto tab : m_tabs) {
        auto selected = tab->getUserData() == src;
        tab->getNormalImage()->getChildByID("disabled-bg")->setVisible(!selected);
        tab->getNormalImage()->getChildByID("enabled-bg")->setVisible(selected);
        tab->setEnabled(!selected);
    }
    // Remove current list from UI (it's Ref'd so it stays in memory)
    if (m_currentSource) {
        m_lists.at(m_currentSource)->removeFromParent();
    }
    // Update current source
    m_currentSource = src;

    // Lazily create new list and add it to UI
    if (!m_lists.contains(src)) {
        auto list = ModList::create(src, m_frame->getContentSize() - ccp(24, 0));
        list->setPosition(m_frame->getPosition());
        this->addChild(list);
        m_lists.emplace(src, list);
    }
    // Add list to UI
    else {
        this->addChild(m_lists.at(src));
    }
}

void ModsLayer::onTab(CCObject* sender) {
    this->gotoTab(static_cast<ModListSource*>(static_cast<CCNode*>(sender)->getUserData()));
}

void ModsLayer::keyBackClicked() {
    this->onBack(nullptr);
}

void ModsLayer::onRefreshList(CCObject*) {
    m_lists.at(m_currentSource)->reloadPage();
}

void ModsLayer::onBack(CCObject*) {
    CCDirector::get()->replaceScene(CCTransitionFade::create(.5f, MenuLayer::scene(false)));
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
