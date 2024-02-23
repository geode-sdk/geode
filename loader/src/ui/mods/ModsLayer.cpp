#include "ModsLayer.hpp"
#include "SwelvyBG.hpp"

static bool BIG_VIEW = false;

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

    auto frame = CCNode::create();
    frame->setAnchorPoint({ .5f, .5f });
    frame->setContentSize({ 380, 205 });

    auto frameBG = CCLayerColor::create({ 25, 17, 37, 255 });
    frameBG->setContentSize(frame->getContentSize());
    frameBG->ignoreAnchorPointForPosition(false);
    frame->addChildAtPosition(frameBG, Anchor::Center);

    auto tabsTop = CCSprite::createWithSpriteFrameName("mods-list-top.png"_spr);
    tabsTop->setAnchorPoint({ .5f, .0f });
    frame->addChildAtPosition(tabsTop, Anchor::Top, ccp(0, -2));

    auto tabsLeft = CCSprite::createWithSpriteFrameName("mods-list-side.png"_spr);
    tabsLeft->setScaleY(frame->getContentHeight() / tabsLeft->getContentHeight());
    frame->addChildAtPosition(tabsLeft, Anchor::Left, ccp(6, 0));

    auto tabsRight = CCSprite::createWithSpriteFrameName("mods-list-side.png"_spr);
    tabsRight->setFlipX(true);
    tabsRight->setScaleY(frame->getContentHeight() / tabsRight->getContentHeight());
    frame->addChildAtPosition(tabsRight, Anchor::Right, ccp(-6, 0));

    auto tabsBottom = CCSprite::createWithSpriteFrameName("mods-list-bottom.png"_spr);
    tabsBottom->setAnchorPoint({ .5f, 1.f });
    frame->addChildAtPosition(tabsBottom, Anchor::Bottom, ccp(0, 2));

    this->addChildAtPosition(frame, Anchor::Center, ccp(0, -10), false);

    m_list = ScrollLayer::create(frame->getContentSize() - ccp(24, 0));
    m_list->m_contentLayer->setLayout(
        ColumnLayout::create()
            ->setAxisReverse(true)
            ->setAxisAlignment(AxisAlignment::End)
            ->setAutoGrowAxis(frame->getContentHeight())
    );
    this->addChildAtPosition(m_list, Anchor::Center, -m_list->getScaledContentSize() / 2 - ccp(0, 10), false);

    auto mainTabs = CCMenu::create();
    mainTabs->setContentWidth(tabsTop->getContentWidth() - 45);
    mainTabs->setAnchorPoint({ .5f, .0f });
    mainTabs->setPosition(frame->convertToWorldSpace(tabsTop->getPosition() + ccp(0, 10)));

    for (auto item : std::initializer_list<std::tuple<const char*, const char*, const char*>> {
        { "download.png"_spr, "Installed", "installed" },
        { "GJ_bigStar_noShadow_001.png", "Featured", "featured" },
        { "GJ_sTrendingIcon_001.png", "Trending", "trending" },
        { "gj_folderBtn_001.png", "Mod Packs", "mod-packs" },
        { "search.png"_spr, "Search", "search" },
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
        btn->setID(std::get<2>(item));
        mainTabs->addChild(btn);
        m_tabs.push_back(btn);
    }

    mainTabs->setLayout(RowLayout::create());
    this->addChild(mainTabs);

    this->gotoTab("installed");

    this->setKeypadEnabled(true);
    cocos::handleTouchPriority(this, true);

    return true;
}

void ModsLayer::loadList(std::string const& id) {
    if (!m_currentListID.empty()) {
        m_listPosCaches[m_currentListID].scrollPosition = m_list->m_contentLayer->getPositionY();
        m_listPosCaches[m_currentListID].page = m_currentPage;
    }
    if (!m_listPosCaches.contains(id)) {
        m_listPosCaches.insert_or_assign(id, ListPosCache {
            .page = 0,
            .scrollPosition = std::numeric_limits<float>::max(),
        });
    }
    m_currentListID = id;
    if (auto srcOpt = ModListSource::get(id)) {
        auto& src = srcOpt.value().get();
        src.loadTotalPageCount()
            .then([this](size_t count) {
                // todo: show count in UI
                this->loadPage(m_listPosCaches.at(id).page);
            });
    }
    else {

    }
}

void ModsLayer::loadPage(size_t page, bool update) {
    m_list->m_contentLayer->removeAllChildren();
    if (auto srcOpt = ModListSource::get(m_currentListID)) {
        auto& src = srcOpt.value().get();
        src.loadNewPage();
    }
    else {

    }
    for (auto item : cache.source.loadNewPage(m_currentPage, update)) {
        m_list->m_contentLayer->addChild(item);
        item->updateSize(m_list->getContentWidth(), BIG_VIEW);
    }
    m_list->m_contentLayer->updateLayout();
    auto listTopScrollPos = -m_list->m_contentLayer->getContentHeight() + m_list->getContentHeight();
    if (cache.scrollPosition > 0.f || cache.scrollPosition < listTopScrollPos) {
        cache.scrollPosition = listTopScrollPos;
    }
    m_list->m_contentLayer->setPositionY(cache.scrollPosition);
    m_currentList = &cache;
}

void ModsLayer::gotoTab(std::string const& id) {
    for (auto tab : m_tabs) {
        auto selected = tab->getID() == id;
        tab->getNormalImage()->getChildByID("disabled-bg")->setVisible(!selected);
        tab->getNormalImage()->getChildByID("enabled-bg")->setVisible(selected);
        tab->setEnabled(!selected);
    }
    this->loadList(id);
}

void ModsLayer::onTab(CCObject* sender) {
    this->gotoTab(static_cast<CCNode*>(sender)->getID());
}

void ModsLayer::keyBackClicked() {
    this->onBack(nullptr);
}

void ModsLayer::onRefreshList(CCObject*) {
    if (m_currentList) {
        this->loadList(m_currentList->id, true);
    }
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
