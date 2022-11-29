#include "ModListLayer.hpp"

#include "SearchFilterPopup.hpp"

#include <Geode/binding/ButtonSprite.hpp>
#include <Geode/binding/CCTextInputNode.hpp>
#include <Geode/binding/GJListLayer.hpp>
#include <Geode/binding/GameToolbox.hpp>
#include <Geode/binding/LoadingCircle.hpp>
#include <Geode/binding/MenuLayer.hpp>
#include <Geode/binding/StatsCell.hpp>
#include <Geode/binding/TableView.hpp>
#include <Geode/ui/BasedButton.hpp>
#include <Geode/ui/Notification.hpp>
#include <Geode/utils/casts.hpp>
#include <optional>

static ModListType g_tab = ModListType::Installed;
static ModListLayer* g_instance = nullptr;

bool ModListLayer::init() {
    if (!CCLayer::init()) return false;

    auto winSize = CCDirector::sharedDirector()->getWinSize();

    // create background
    auto bg = CCSprite::create("GJ_gradientBG.png");
    auto bgSize = bg->getTextureRect().size;

    bg->setAnchorPoint({ 0.0f, 0.0f });
    bg->setScaleX((winSize.width + 10.0f) / bgSize.width);
    bg->setScaleY((winSize.height + 10.0f) / bgSize.height);
    bg->setPosition({ -5.0f, -5.0f });
    bg->setColor({ 0, 102, 255 });

    this->addChild(bg);

    // create menus
    m_menu = CCMenu::create();
    m_topMenu = CCMenu::create();

    // add back button
    auto backBtn = CCMenuItemSpriteExtra::create(
        CCSprite::createWithSpriteFrameName("GJ_arrow_01_001.png"), this,
        menu_selector(ModListLayer::onExit)
    );
    backBtn->setPosition(-winSize.width / 2 + 25.0f, winSize.height / 2 - 25.0f);
    m_menu->addChild(backBtn);

    // add refresh mods button
    auto reloadSpr = CCSprite::createWithSpriteFrameName("GJ_updateBtn_001.png");
    reloadSpr->setScale(.8f);
    auto reloadBtn =
        CCMenuItemSpriteExtra::create(reloadSpr, this, menu_selector(ModListLayer::onReload));
    reloadBtn->setPosition(-winSize.width / 2 + 30.0f, -winSize.height / 2 + 30.0f);
    m_menu->addChild(reloadBtn);

    // add open folder button
    auto openSpr = CircleButtonSprite::createWithSpriteFrameName(
        "gj_folderBtn_001.png", .7f, CircleBaseColor::Green, CircleBaseSize::Small
    );
    auto openBtn =
        CCMenuItemSpriteExtra::create(openSpr, this, menu_selector(ModListLayer::onOpenFolder));
    openBtn->setPosition(-winSize.width / 2 + 30.0f, -winSize.height / 2 + 80.0f);
    m_menu->addChild(openBtn);

    // add list display button
    // the button backgrounds are intentially the wrong way around,
    // to match the behaviour of the smallModeIcon button on comments
    // in level info
    auto unextendedIconSpr = CCSprite::create("GJ_button_02.png");
    unextendedIconSpr->setScale(.75f);

    auto unextendedIconTopSpr = CCSprite::createWithSpriteFrameName("GJ_smallModeIcon_001.png");
    unextendedIconTopSpr->setPosition(unextendedIconSpr->getContentSize() / 2);
    unextendedIconSpr->addChild(unextendedIconTopSpr);

    auto extendedIconSpr = CCSprite::create("GJ_button_01.png");
    extendedIconSpr->setScale(.75f);

    auto extendedIconTopSpr = CCSprite::createWithSpriteFrameName("GJ_smallModeIcon_001.png");
    extendedIconTopSpr->setPosition(extendedIconSpr->getContentSize() / 2);
    extendedIconSpr->addChild(extendedIconTopSpr);

    auto listDisplayType = CCMenuItemToggler::create(
        unextendedIconSpr, extendedIconSpr, this, menu_selector(ModListLayer::onExpand)
    );
    listDisplayType->setPosition(-210.f, .0f);
    m_topMenu->addChild(listDisplayType);

    // add list status label
    m_listLabel = CCLabelBMFont::create("", "bigFont.fnt");

    m_listLabel->setPosition(winSize / 2);
    m_listLabel->setScale(.6f);
    m_listLabel->setVisible(false);
    m_listLabel->setZOrder(1001);

    this->addChild(m_listLabel);

    // add index update status label
    m_indexUpdateLabel = CCLabelBMFont::create("", "goldFont.fnt");

    m_indexUpdateLabel->setPosition(winSize.width / 2, winSize.height / 2 - 80.f);
    m_indexUpdateLabel->setScale(.5f);
    m_indexUpdateLabel->setZOrder(1001);

    this->addChild(m_indexUpdateLabel);

    // tabs
    m_installedTabBtn = TabButton::create("Installed", this, menu_selector(ModListLayer::onTab));
    m_installedTabBtn->setPosition(-95.f, 138.5f);
    m_installedTabBtn->setTag(static_cast<int>(ModListType::Installed));
    m_menu->addChild(m_installedTabBtn);

    m_downloadTabBtn = TabButton::create("Download", this, menu_selector(ModListLayer::onTab));
    m_downloadTabBtn->setPosition(0.f, 138.5f);
    m_downloadTabBtn->setTag(static_cast<int>(ModListType::Download));
    m_menu->addChild(m_downloadTabBtn);

    m_featuredTabBtn = TabButton::create("Featured", this, menu_selector(ModListLayer::onTab));
    m_featuredTabBtn->setPosition(95.f, 138.5f);
    m_featuredTabBtn->setTag(static_cast<int>(ModListType::Featured));
    m_menu->addChild(m_featuredTabBtn);

    // add menus
    m_menu->setZOrder(0);
    m_topMenu->setZOrder(10);

    this->addChild(m_menu);
    this->addChild(m_topMenu);

    // select first tab
    this->onTab(nullptr);

    // enable keyboard
    this->setKeyboardEnabled(true);
    this->setKeypadEnabled(true);

    return true;
}

void ModListLayer::createSearchControl() {
    auto winSize = CCDirector::sharedDirector()->getWinSize();

    m_searchBG = CCLayerColor::create({ 194, 114, 62, 255 }, 358.f, 30.f);

    auto menu = CCMenu::create();
    menu->setPosition(340.f, 15.f);

    constexpr float buttonSpace = 50.f;

    // filters
    auto filterSpr = EditorButtonSprite::createWithSpriteFrameName(
        "filters.png"_spr, 1.0f, EditorBaseColor::Gray
    );
    filterSpr->setScale(.7f);

    m_filterBtn =
        CCMenuItemSpriteExtra::create(filterSpr, this, menu_selector(ModListLayer::onFilters));
    m_filterBtn->setPosition(-10.f, 0.f);
    menu->addChild(m_filterBtn);

    // search button
    auto searchSpr = CCSprite::createWithSpriteFrameName("gj_findBtn_001.png");
    searchSpr->setScale(.7f);

    m_searchBtn = CCMenuItemSpriteExtra::create(searchSpr, this, nullptr);
    m_searchBtn->setPosition(-35.f, 0.f);
    menu->addChild(m_searchBtn);

    auto searchClearSpr = CCSprite::createWithSpriteFrameName("gj_findBtnOff_001.png");
    searchClearSpr->setScale(.7f);

    m_searchClearBtn = CCMenuItemSpriteExtra::create(
        searchClearSpr, this, menu_selector(ModListLayer::onResetSearch)
    );
    m_searchClearBtn->setPosition(-35.f, 0.f);
    m_searchClearBtn->setVisible(false);
    menu->addChild(m_searchClearBtn);

    m_searchBG->addChild(menu);
    m_searchBG->setPosition(0.f, 190.f);
    m_list->addChild(m_searchBG);

    // search input
    auto inputBG = CCScale9Sprite::create("square02b_001.png", { 0.0f, 0.0f, 80.0f, 80.0f });
    inputBG->setColor({ 126, 59, 7 });
    inputBG->setContentSize({ 650.f - buttonSpace * 2, 40.f });
    inputBG->setPosition(175.f - buttonSpace / 2, 15.f);
    inputBG->setScale(.5f);
    m_searchBG->addChild(inputBG);

    m_searchInput =
        CCTextInputNode::create(310.f - buttonSpace, 20.f, "Search Mods...", "bigFont.fnt");
    m_searchInput->setLabelPlaceholderColor({ 150, 150, 150 });
    m_searchInput->setLabelPlaceholderScale(.4f);
    m_searchInput->setMaxLabelScale(.4f);
    m_searchInput->setDelegate(this);
    m_searchInput->m_textField->setAnchorPoint({ .0f, .5f });
    m_searchInput->m_placeholderLabel->setAnchorPoint({ .0f, .5f });
    m_searchInput->setPosition(winSize.width / 2 - 160.f, winSize.height / 2 + 95.f);
    m_searchInput->setZOrder(60);
    this->addChild(m_searchInput);
}

void ModListLayer::indexUpdateProgress(
    UpdateStatus status, std::string const& info, uint8_t percentage
) {
    // if we have a check for updates button
    // visible, disable it from being clicked
    // again
    if (m_checkForUpdatesBtn) {
        m_checkForUpdatesBtn->setEnabled(false);
        as<ButtonSprite*>(m_checkForUpdatesBtn->getNormalImage())->setString("Updating Index");
    }

    // if finished, refresh list
    if (status == UpdateStatus::Finished) {
        m_indexUpdateLabel->setVisible(false);
        this->reloadList();

        // make sure to release global instance
        // and set it back to null
        CC_SAFE_RELEASE_NULL(g_instance);
    }
    else {
        m_indexUpdateLabel->setVisible(true);
        m_indexUpdateLabel->setString(info.c_str());
    }

    if (status == UpdateStatus::Failed) {
        FLAlertLayer::create("Error Updating Index", info, "OK")->show();

        // make sure to release global instance
        // and set it back to null
        CC_SAFE_RELEASE(g_instance);
    }
}

void ModListLayer::reloadList() {
    auto winSize = CCDirector::sharedDirector()->getWinSize();

    // remove old list
    if (m_list) {
        if (m_searchBG) m_searchBG->retain();
        m_list->removeFromParent();
    }

    // create new list
    m_query.m_searchFilter =
        m_searchInput && m_searchInput->getString() && strlen(m_searchInput->getString())
        ? std::optional<std::string>(m_searchInput->getString())
        : std::nullopt;
    auto list = ModListView::create(g_tab, m_expandedList, 358.f, 190.f, m_query);
    list->setLayer(this);

    // set list status
    auto status = list->getStatusAsString();
    if (status.size()) {
        m_listLabel->setVisible(true);
        m_listLabel->setString(status.c_str());
    }
    else {
        m_listLabel->setVisible(false);
    }

    // update index if needed
    if (g_tab == ModListType::Download && !Index::get()->isIndexUpdated()) {
        m_listLabel->setString("Updating index...");
        if (!m_loadingCircle) {
            m_loadingCircle = LoadingCircle::create();

            m_loadingCircle->setPosition(.0f, -40.f);
            m_loadingCircle->setScale(.7f);
            m_loadingCircle->setZOrder(1001);

            m_loadingCircle->show();
        }
        this->onCheckForUpdates(nullptr);
    }
    else {
        if (m_loadingCircle) {
            m_loadingCircle->fadeAndRemove();
            m_loadingCircle = nullptr;
        }
    }

    // set list
    m_list = GJListLayer::create(list, nullptr, { 0, 0, 0, 180 }, 358.f, 220.f);
    m_list->setZOrder(2);
    m_list->setPosition(winSize / 2 - m_list->getScaledContentSize() / 2);
    this->addChild(m_list);

    // add search input to list
    if (!m_searchInput) {
        this->createSearchControl();
    }
    else {
        m_list->addChild(m_searchBG);
        m_searchBG->release();
    }

    // enable filter button
    m_filterBtn->setEnabled(g_tab != ModListType::Installed);
    auto filterSpr = static_cast<EditorButtonSprite*>(m_filterBtn->getNormalImage());
    filterSpr->setColor(g_tab != ModListType::Installed ? cc3x(0xff) : cc3x(0x88));
    static_cast<CCSprite*>(filterSpr->getTopNode())
        ->setColor(g_tab != ModListType::Installed ? cc3x(0xff) : cc3x(0x88));

    // check if the user has searched something,
    // and show visual indicator if so
    auto hasQuery = m_query.m_searchFilter.has_value();
    m_searchBtn->setVisible(!hasQuery);
    m_searchClearBtn->setVisible(hasQuery);

    // add/remove "Check for Updates" button
    if (
		// only show it on the install tab
		g_tab == ModListType::Installed &&
		// check if index is updated, and if not 
		// add button if it doesn't exist yet
		!Index::get()->isIndexUpdated()
	) {
        if (!m_checkForUpdatesBtn) {
            auto checkSpr = ButtonSprite::create("Check for Updates");
            checkSpr->setScale(.7f);
            m_checkForUpdatesBtn = CCMenuItemSpriteExtra::create(
                checkSpr, this, menu_selector(ModListLayer::onCheckForUpdates)
            );
            m_checkForUpdatesBtn->setPosition(0, -winSize.height / 2 + 40.f);
            m_topMenu->addChild(m_checkForUpdatesBtn);
        }
    }
    // otherwise remove the button if it
    // exists
    else if (m_checkForUpdatesBtn) {
        m_checkForUpdatesBtn->removeFromParent();
        m_checkForUpdatesBtn = nullptr;
    }
}

void ModListLayer::onCheckForUpdates(CCObject*) {
    // store instance in a global so the
    // layer stays in memory even if the
    // user leaves the layer and we don't
    // end up trying to update the UI of
    // a deleted layer
    g_instance = this;
    g_instance->retain();

    // update index
    Index::get()->updateIndex(
        [](UpdateStatus status, std::string const& info, uint8_t progress) -> void {
            g_instance->indexUpdateProgress(status, info, progress);
        }
    );
}

void ModListLayer::textChanged(CCTextInputNode* input) {
    this->reloadList();
}

void ModListLayer::onExit(CCObject*) {
    CCDirector::sharedDirector()->replaceScene(
        CCTransitionFade::create(.5f, MenuLayer::scene(false))
    );
}

void ModListLayer::onReload(CCObject*) {
    (void)Loader::get()->refreshModsList();
    this->reloadList();
}

void ModListLayer::onExpand(CCObject* sender) {
    m_expandedList = !static_cast<CCMenuItemToggler*>(sender)->isToggled();
    this->reloadList();
}

void ModListLayer::onFilters(CCObject*) {
    SearchFilterPopup::create(this, g_tab)->show();
}

void ModListLayer::onOpenFolder(CCObject*) {
    file::openFolder(ghc::filesystem::canonical(Loader::get()->getGeodeDirectory() / "mods"));
}

void ModListLayer::onResetSearch(CCObject*) {
    m_searchInput->setString("");
}

void ModListLayer::keyDown(enumKeyCodes key) {
    if (key == KEY_Escape) {
        this->onExit(nullptr);
    }
}

void ModListLayer::onTab(CCObject* pSender) {
    if (pSender) {
        g_tab = static_cast<ModListType>(pSender->getTag());
    }
    this->reloadList();

    auto toggleTab = [this](CCMenuItemToggler* member) -> void {
        auto isSelected = member->getTag() == static_cast<int>(g_tab);
        auto targetMenu = isSelected ? m_topMenu : m_menu;
        member->toggle(isSelected);
        if (member->getParent() != targetMenu) {
            member->retain();
            member->removeFromParent();
            targetMenu->addChild(member);
            member->release();
        }
    };

    toggleTab(m_downloadTabBtn);
    toggleTab(m_installedTabBtn);
    toggleTab(m_featuredTabBtn);
}

ModListLayer* ModListLayer::create() {
    // return global instance if one exists
    if (g_instance) return g_instance;

    // otherwise create new instance like a
    // normal person
    auto ret = new ModListLayer();
    if (ret && ret->init()) {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

ModListLayer* ModListLayer::scene() {
    auto scene = CCScene::create();
    auto layer = ModListLayer::create();
    scene->addChild(layer);
    CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(.5f, scene));
    return layer;
}

ModListLayer::~ModListLayer() {
    removeAllChildrenWithCleanup(true);
}
