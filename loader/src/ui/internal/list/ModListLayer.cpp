#include "ModListLayer.hpp"
#include "ModListCell.hpp"
#include "SearchFilterPopup.hpp"
#include <Geode/binding/ButtonSprite.hpp>
#include <Geode/binding/CCTextInputNode.hpp>
#include <Geode/binding/GJListLayer.hpp>
#include <Geode/binding/LoadingCircle.hpp>
#include <Geode/binding/MenuLayer.hpp>
#include <Geode/binding/TableView.hpp>
#include <Geode/ui/BasedButton.hpp>
#include <Geode/ui/Notification.hpp>
#include <Geode/utils/casts.hpp>
#include <Geode/loader/Dirs.hpp>
#include <Geode/loader/Loader.hpp>
#include <Geode/ui/ListView.hpp>
#include <Geode/utils/string.hpp>
#include <Geode/utils/ranges.hpp>

#define FTS_FUZZY_MATCH_IMPLEMENTATION
#include <Geode/external/fts/fts_fuzzy_match.h>

static ModListType g_tab = ModListType::Installed;
static ModListLayer* g_instance = nullptr;

// Mods

static std::optional<int> fuzzyMatch(std::string const& kw, std::string const& str) {
    int score;
    if (fts::fuzzy_match(kw.c_str(), str.c_str(), score)) {
        return score;
    }
    return std::nullopt;
}

#define WEIGHTED_MATCH(str_, weight_) \
    if (auto match = fuzzyMatch(query.keywords.value(), str_)) {\
        weighted += match.value() * weight_;                    \
        someMatched = true;                                     \
    }

#define WEIGHTED_MATCH_ADD(str_, weight_) \
    if (auto match = fuzzyMatch(query.keywords.value(), str_)) {\
        weighted += match.value() * weight_;                    \
    }

static std::optional<int> queryMatchKeywords(
    ModListQuery const& query,
    ModInfo const& info
) {
    double weighted = 0;

    // fuzzy match keywords
    if (query.keywords) {
        bool someMatched = false;
        WEIGHTED_MATCH(info.name(), 2);
        WEIGHTED_MATCH(info.id(), 1.5);
        WEIGHTED_MATCH(info.developer(), 1);
        WEIGHTED_MATCH(info.details().value_or(""), 2);
        WEIGHTED_MATCH(info.description().value_or(""), 1);
        if (!someMatched) {
            return std::nullopt;
        }
    } else {
        // this is like the dumbest way you could possibly sort alphabetically 
        // but it does enough to make the mods list somewhat alphabetically 
        // sorted, at least enough so that if you're scrolling it based on 
        // alphabetical order you will find the part you're looking for easily 
        // so it's fine
        weighted = -tolower(info.name()[0]);
    }

    // empty keywords always match
    return static_cast<int>(weighted);
}

static std::optional<int> queryMatch(ModListQuery const& query, Mod* mod) {
    // Only checking keywords makes sense for mods since their 
    // platform always matches, they are always visible and they don't 
    // currently list their tags
    return queryMatchKeywords(query, mod->getModInfo());
}

static std::optional<int> queryMatch(ModListQuery const& query, IndexItemHandle item) {
    // if no force visibility was provided and item is already installed, don't 
    // show it
    if (!query.forceVisibility && Loader::get()->isModInstalled(item->info.id())) {
        return std::nullopt;
    }
    // make sure all tags match
    for (auto& tag : query.tags) {
        if (!item->tags.count(tag)) {
            return std::nullopt;
        }
    }
    // make sure at least some platform matches
    if (!ranges::contains(query.platforms, [item](PlatformID id) {
        return item->download.platforms.count(id);
    })) {
        return std::nullopt;
    }
    // otherwise match keywords
    if (auto match = queryMatchKeywords(query, item->info)) {
        auto weighted = match.value();
        // add extra weight on tag matches
        if (query.keywords) {
            WEIGHTED_MATCH_ADD(ranges::join(item->tags, " "), 1.4);
        }
        // add extra weight to featured items to keep power consolidated in the 
        // hands of the rich Geode bourgeoisie
        // the number 420 is a reference to the number one bourgeois of modern 
        // society, elon musk
        weighted += item->isFeatured ? 420 : 0;
        return static_cast<int>(weighted);
    }
    // keywords must match bruh
    else {
        return std::nullopt;
    }
}

static std::optional<int> queryMatch(ModListQuery const& query, InvalidGeodeFile const& info) {
    // if any explicit filters were provided, no match
    if (
        query.tags.size() ||
        query.keywords.has_value()
    ) {
        return std::nullopt;
    }
    return 0;
}

CCArray* ModListLayer::createModCells(ModListType type, ModListQuery const& query) {
    auto mods = CCArray::create();
    switch (type) {
        default:
        case ModListType::Installed: {
            // failed mods first
            for (auto const& mod : Loader::get()->getFailedMods()) {
                if (!queryMatch(query, mod)) continue;
                mods->addObject(InvalidGeodeFileCell::create(
                    mod, this, m_display, this->getCellSize()
                ));
            }

            // sort the mods by match score 
            std::multimap<int, Mod*> sorted;

            // then other mods
            for (auto const& mod : Loader::get()->getAllMods()) {
                // if the mod is no longer installed nor
                // loaded, it's as good as not existing
                // (because it doesn't)
                if (mod->isUninstalled() && !mod->isLoaded()) continue;
                // only show mods that match query in list
                if (auto match = queryMatch(query, mod)) {
                    sorted.insert({ match.value(), mod });
                }
            }

            // add the mods sorted
            for (auto& [score, mod] : ranges::reverse(sorted)) {
                mods->addObject(ModCell::create(
                    mod, this, m_display, this->getCellSize()
                ));
            }
        } break;

        case ModListType::Download: {
            // sort the mods by match score 
            std::multimap<int, IndexItemHandle> sorted;

            for (auto const& item : Index::get()->getItems()) {
                if (auto match = queryMatch(query, item)) {
                    sorted.insert({ match.value(), item });
                }
            }

            // add the mods sorted
            for (auto& [score, item] : ranges::reverse(sorted)) {
                mods->addObject(IndexItemCell::create(
                    item, this, m_display, this->getCellSize()
                ));
            }
        } break;

        case ModListType::Featured: {
            // sort the mods by match score 
            std::multimap<int, IndexItemHandle> sorted;

            for (auto const& item : Index::get()->getFeaturedItems()) {
                if (auto match = queryMatch(query, item)) {
                    sorted.insert({ match.value(), item });
                }
            }

            // add the mods sorted
            for (auto& [score, item] : ranges::reverse(sorted)) {
                mods->addObject(IndexItemCell::create(
                    item, this, m_display, this->getCellSize()
                ));
            }
        } break;
    }
    return mods;
}

// UI

bool ModListLayer::init() {
    if (!CCLayer::init()) return false;

    m_indexListener.bind(this, &ModListLayer::onIndexUpdate);

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

void ModListLayer::reloadList(std::optional<ModListQuery> const& query) {
    auto winSize = CCDirector::sharedDirector()->getWinSize();

    if (query) {
        m_query = query.value();
    }

    // set search query
    m_query.keywords =
        m_searchInput && 
        m_searchInput->getString() &&
        strlen(m_searchInput->getString()) ? 
            std::optional<std::string>(m_searchInput->getString()) : 
            std::nullopt;

    // remove old list
    if (m_list) {
        if (m_searchBG) m_searchBG->retain();
        m_list->removeFromParent();
    }

    auto items = this->createModCells(g_tab, m_query);

    // create new list
    auto list = ListView::create(
        items,
        this->getCellSize().height,
        this->getListSize().width,
        this->getListSize().height
    );

    // set list status
    if (!items->count()) {
        m_listLabel->setVisible(true);
        m_listLabel->setString("No mods found");
    } else {
        m_listLabel->setVisible(false);
    }

    // update index if needed
    if (g_tab == ModListType::Download && !Index::get()->hasTriedToUpdate()) {
        m_listLabel->setVisible(true);
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
    auto hasQuery =
        (m_searchInput->getString() &&
        strlen(m_searchInput->getString()));
    m_searchBtn->setVisible(!hasQuery);
    m_searchClearBtn->setVisible(hasQuery);

    // add/remove "Check for Updates" button
    if (
		// only show it on the install tab
		g_tab == ModListType::Installed &&
		// check if index is updated, and if not 
		// add button if it doesn't exist yet
		!Index::get()->isUpToDate()
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

void ModListLayer::updateAllStates(ModListCell* toggled) {
    for (auto cell : CCArrayExt<GenericListCell>(
        m_list->m_listView->m_tableView->m_cellArray
    )) {
        auto node = static_cast<ModListCell*>(cell->getChildByID("mod-list-cell"));
        if (toggled != node) {
            node->updateState();
        }
    }
}

// Getters

CCSize ModListLayer::getListSize() const {
    return { 358.f, 190.f };
}

CCSize ModListLayer::getCellSize() const {
    return {
        getListSize().width,
        m_display == ModListDisplay::Expanded ? 60.f : 40.f
    };
}

ModListDisplay ModListLayer::getDisplay() const {
    return m_display;
}

ModListQuery& ModListLayer::getQuery() {
    return m_query;
}

// Callbacks & Vtable impls

void ModListLayer::onCheckForUpdates(CCObject*) {
    // store instance in a global so the
    // layer stays in memory even if the
    // user leaves the layer and we don't
    // end up trying to update the UI of
    // a deleted layer
    g_instance = this;
    g_instance->retain();

    // update index
    Index::get()->update();
}

void ModListLayer::onIndexUpdate(IndexUpdateEvent* event) {
    std::visit(makeVisitor {
        [&](UpdateProgress const& prog) {},
        [&](UpdateFinished const&) {
            this->reloadList();
        },
        [&](UpdateFailed const& error) {
            this->reloadList();
        }
    }, event->status);
}

void ModListLayer::onExit(CCObject*) {
    CCDirector::sharedDirector()->replaceScene(
        CCTransitionFade::create(.5f, MenuLayer::scene(false))
    );
}

void ModListLayer::onReload(CCObject*) {
    Loader::get()->refreshModsList();
    this->reloadList();
}

void ModListLayer::onExpand(CCObject* sender) {
    m_display = static_cast<CCMenuItemToggler*>(sender)->isToggled() ?
        ModListDisplay::Concise :
        ModListDisplay::Expanded;
    this->reloadList();
}

void ModListLayer::onFilters(CCObject*) {
    SearchFilterPopup::create(this, g_tab)->show();
}

void ModListLayer::onOpenFolder(CCObject*) {
    file::openFolder(ghc::filesystem::canonical(dirs::getModsDir()));
}

void ModListLayer::onResetSearch(CCObject*) {
    m_searchInput->setString("");
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

void ModListLayer::keyDown(enumKeyCodes key) {
    if (key == KEY_Escape) {
        this->onExit(nullptr);
    }
}

void ModListLayer::textChanged(CCTextInputNode* input) {
    this->reloadList();
}

// Constructors etc.

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
    this->removeAllChildrenWithCleanup(true);
}
