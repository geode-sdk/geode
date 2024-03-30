#include "ModList.hpp"
#include <Geode/utils/ColorProvider.hpp>
#include "../popups/FiltersPopup.hpp"
#include "../GeodeStyle.hpp"
#include "../ModsLayer.hpp"

bool ModList::init(ModListSource* src, CCSize const& size) {
    if (!CCNode::init())
        return false;
    
    this->setContentSize(size);
    this->setAnchorPoint({ .5f, .5f });

    m_source = src;
    m_source->reset();
    
    m_list = ScrollLayer::create(size);
    m_list->m_contentLayer->setLayout(
        ColumnLayout::create()
            ->setAxisReverse(true)
            ->setAxisAlignment(AxisAlignment::End)
            ->setAutoGrowAxis(size.height)
            ->setGap(2.5f)
    );
    this->addChildAtPosition(m_list, Anchor::Bottom, ccp(-m_list->getScaledContentWidth() / 2, 0));

    m_topContainer = CCNode::create();
    m_topContainer->ignoreAnchorPointForPosition(false);
    m_topContainer->setContentWidth(size.width);
    m_topContainer->setAnchorPoint({ .5f, 1.f });

    m_searchMenu = CCNode::create();
    m_searchMenu->ignoreAnchorPointForPosition(false);
    m_searchMenu->setContentSize({ size.width, 30 });

    auto searchBG = CCLayerColor::create(ColorProvider::get()->color("mod-list-search-bg"_spr));
    searchBG->setContentSize(m_searchMenu->getContentSize());
    searchBG->ignoreAnchorPointForPosition(false);
    m_searchMenu->addChildAtPosition(searchBG, Anchor::Center);

    m_searchInput = TextInput::create(size.width + 25, "Search Mods");
    m_searchInput->setScale(.75f);
    m_searchInput->setAnchorPoint({ 0, .5f });
    m_searchInput->setTextAlign(TextInputAlign::Left);
    m_searchInput->setCallback([this](auto const&) {
        // If the source is already in memory, we can immediately update the 
        // search query
        if (m_source->isInstalledMods()) {
            m_source->search(m_searchInput->getString());
            return;
        }
        // Otherwise buffer inputs by a bit
        // This avoids spamming servers for every character typed, 
        // instead waiting for input to stop to actually do the search
        std::thread([this] {
            m_searchInputThreads += 1;
            std::this_thread::sleep_for(std::chrono::milliseconds(400));
            m_searchInputThreads -= 1;
            if (m_searchInputThreads == 0) {
                Loader::get()->queueInMainThread([this] {
                    m_source->search(m_searchInput->getString());
                });
            }
        }).detach();
    });
    m_searchMenu->addChildAtPosition(m_searchInput, Anchor::Left, ccp(7.5f, 0));

    auto searchFiltersMenu = CCMenu::create();
    searchFiltersMenu->setContentWidth(size.width - m_searchInput->getScaledContentWidth() - 10);
    searchFiltersMenu->setAnchorPoint({ 1, .5f });
    searchFiltersMenu->setScale(.75f);

    // todo: sort button

    auto filterBtn = CCMenuItemSpriteExtra::create(
        GeodeSquareSprite::createWithSpriteFrameName("GJ_filterIcon_001.png"),
        this, menu_selector(ModList::onFilters)
    );
    searchFiltersMenu->addChild(filterBtn);

    auto clearFiltersBtn = CCMenuItemSpriteExtra::create(
        GeodeSquareSprite::createWithSpriteFrameName("GJ_deleteIcon_001.png"),
        this, menu_selector(ModList::onClearFilters)
    );
    searchFiltersMenu->addChild(clearFiltersBtn);

    searchFiltersMenu->setLayout(
        RowLayout::create()
            ->setAxisAlignment(AxisAlignment::End)
    );
    m_searchMenu->addChildAtPosition(searchFiltersMenu, Anchor::Right, ccp(-10, 0));

    m_topContainer->addChild(m_searchMenu);

    // Check for updates on installed mods, and show an update all button if there are some
    if (m_source->isInstalledMods()) {
        m_checkUpdatesListener.bind(this, &ModList::onCheckUpdates);
        m_checkUpdatesListener.setFilter(ModsLayer::checkInstalledModsForUpdates().listen());

        m_updateAllMenu = CCNode::create();
        m_updateAllMenu->ignoreAnchorPointForPosition(false);
        m_updateAllMenu->setContentSize({ size.width, 30 });
        m_updateAllMenu->setVisible(false);

        auto updateAllBG = CCLayerColor::create(ColorProvider::get()->color("mod-list-updates-available-bg"_spr));
        updateAllBG->setContentSize(m_updateAllMenu->getContentSize());
        updateAllBG->ignoreAnchorPointForPosition(false);
        m_updateAllMenu->addChildAtPosition(updateAllBG, Anchor::Center);
        
        m_updateCountLabel = TextArea::create("", "bigFont.fnt", .35f, size.width / 2 - 30, ccp(0, 1), 12.f, false);
        m_updateAllMenu->addChildAtPosition(m_updateCountLabel, Anchor::Left, ccp(10, 0), ccp(0, 0));
        
        auto updateAllMenu = CCMenu::create();
        updateAllMenu->setContentWidth(size.width / 2);
        updateAllMenu->setAnchorPoint({ 1, .5f });

        auto showUpdatesSpr = createGeodeButton(
            CCSprite::createWithSpriteFrameName("GJ_filterIcon_001.png"),
            "Show Updates", "GE_button_01.png"_spr
        );
        auto hideUpdatesSpr = createGeodeButton(
            CCSprite::createWithSpriteFrameName("GJ_filterIcon_001.png"),
            "Hide Updates", "GE_button_05.png"_spr
        );
        m_toggleUpdatesOnlyBtn = CCMenuItemToggler::create(
            showUpdatesSpr, hideUpdatesSpr, this, menu_selector(ModList::onToggleUpdates)
        );
        m_toggleUpdatesOnlyBtn->m_notClickable = true;
        updateAllMenu->addChild(m_toggleUpdatesOnlyBtn);

        auto updateAllSpr = createGeodeButton(
            CCSprite::createWithSpriteFrameName("update.png"_spr),
            "Update All", "GE_button_01.png"_spr
        );
        auto updateAllBtn = CCMenuItemSpriteExtra::create(
            updateAllSpr, this, menu_selector(ModList::onUpdateAll)
        );
        updateAllMenu->addChild(updateAllBtn);

        updateAllMenu->setLayout(
            RowLayout::create()
                ->setAxisAlignment(AxisAlignment::End)
                ->setDefaultScaleLimits(.1f, 1.f)
        );
        m_updateAllMenu->addChildAtPosition(updateAllMenu, Anchor::Right, ccp(-10, 0));

        m_topContainer->addChild(m_updateAllMenu);
    }

    m_topContainer->setLayout(
        ColumnLayout::create()
            ->setGap(0)
            ->setAxisReverse(true)
            ->setAutoGrowAxis(0.f)
    );
    m_topContainer->getLayout()->ignoreInvisibleChildren(true);

    this->addChildAtPosition(m_topContainer, Anchor::Top);

    // Paging

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

    // Status

    m_statusContainer = CCMenu::create();
    m_statusContainer->setScale(.5f);
    m_statusContainer->setContentHeight(size.height / m_statusContainer->getScale());
    m_statusContainer->setAnchorPoint({ .5f, .5f });
    m_statusContainer->ignoreAnchorPointForPosition(false);

    m_statusTitle = CCLabelBMFont::create("", "bigFont.fnt");
    m_statusTitle->setAlignment(kCCTextAlignmentCenter);
    m_statusContainer->addChild(m_statusTitle);

    m_statusDetailsBtn = CCMenuItemSpriteExtra::create(
        ButtonSprite::create("Details", "bigFont.fnt", "GJ_button_05.png", .75f),
        this, menu_selector(ModList::onShowStatusDetails)
    );
    m_statusContainer->addChild(m_statusDetailsBtn);

    m_statusDetails = SimpleTextArea::create("", "chatFont.fnt", .6f);
    m_statusDetails->setAlignment(kCCTextAlignmentCenter);
    m_statusContainer->addChild(m_statusDetails);

    m_statusLoadingCircle = createLoadingCircle(50);
    m_statusContainer->addChild(m_statusLoadingCircle);

    m_statusLoadingBar = Slider::create(this, nullptr);
    m_statusLoadingBar->m_touchLogic->m_thumb->setVisible(false);
    m_statusLoadingBar->setValue(0);
    m_statusLoadingBar->updateBar();
    m_statusLoadingBar->setAnchorPoint({ 0, 0 });
    m_statusContainer->addChild(m_statusLoadingBar);

    m_statusContainer->setLayout(
        ColumnLayout::create()
            ->setAxisReverse(true)
    );
    m_statusContainer->getLayout()->ignoreInvisibleChildren(true);
    this->addChildAtPosition(m_statusContainer, Anchor::Center);

    m_listener.bind(this, &ModList::onPromise);

    m_invalidateCacheListener.bind(this, &ModList::onInvalidateCache);
    m_invalidateCacheListener.setFilter(InvalidateCacheFilter(m_source));
    
    this->gotoPage(0);
    this->updateTopContainer();

    return true;
}

void ModList::onPromise(typename ModListSource::PageLoadEvent* event) {
    if (auto resolved = event->getResolve()) {
        // Hide status
        m_statusContainer->setVisible(false);

        // Create items
        bool first = true;
        for (auto item : *resolved) {
            // Add separators between items after the first one
            if (!first) {
                // auto separator = CCLayerColor::create(
                //     ColorProvider::get()->define("mod-list-separator"_spr, { 255, 255, 255, 45 })
                // );
                // separator->setContentSize({ m_obContentSize.width - 10, .5f });
                // m_list->m_contentLayer->addChild(separator);
            }
            first = false;
            m_list->m_contentLayer->addChild(item);
        }
        this->updateSize(m_bigSize);

        // Scroll list to top
        auto listTopScrollPos = -m_list->m_contentLayer->getContentHeight() + m_list->getContentHeight();
        m_list->m_contentLayer->setPositionY(listTopScrollPos);

        // Update page UI
        this->updateState();
    }
    else if (auto progress = event->getProgress()) {
        // todo: percentage in a loading bar
        if (progress->has_value()) {
            this->showStatus(ModListProgressStatus {
                .percentage = progress->value(),
            }, "Loading...");
        }
        else {
            this->showStatus(ModListUnkProgressStatus(), "Loading...");
        }
    }
    else if (auto rejected = event->getReject()) {
        this->showStatus(ModListErrorStatus(), rejected->message, rejected->details);
        this->updateState();
    }

    if (event->isFinally()) {
        // Clear listener
        m_listener.setFilter(ModListSource::PageLoadEventFilter());
    }
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

void ModList::onShowStatusDetails(CCObject*) {
    m_statusDetails->setVisible(!m_statusDetails->isVisible());
    m_statusContainer->updateLayout();
}

void ModList::onCheckUpdates(PromiseEvent<std::vector<std::string>, server::ServerError>* event) {
    if (auto mods = event->getResolve(); mods && mods->size() > 0) {
        std::string fmt;
        if (mods->size() == 1) {
            fmt = fmt::format("There is <cg>{}</c> update available!", mods->size());
        }
        else {
            fmt = fmt::format("There are <cg>{}</c> updates available!", mods->size());
        }
        m_updateCountLabel->setString(fmt.c_str());
        m_updateAllMenu->setVisible(true);
        this->updateTopContainer();
    }
}

void ModList::onInvalidateCache(InvalidateCacheEvent* event) {
    this->gotoPage(0);
}

void ModList::activateSearch(bool activate) {
    m_searchMenu->setVisible(activate);
    this->updateTopContainer();
}

void ModList::updateTopContainer() {
    m_topContainer->updateLayout();

    // Store old relative scroll position (ensuring no divide by zero happens)
    auto oldPositionArea = m_list->m_contentLayer->getContentHeight() - m_list->getContentHeight();
    auto oldPosition = oldPositionArea > 0.f ?
        m_list->m_contentLayer->getPositionY() / oldPositionArea : 
        -1.f;
        
    // Update list size to account for the top menu 
    // (giving a little bit of extra padding for it, the same size as gap)
    m_list->setContentHeight(
        m_topContainer->getContentHeight() > 0.f ?
            this->getContentHeight() - m_topContainer->getContentHeight() - 
                static_cast<AxisLayout*>(m_list->m_contentLayer->getLayout())->getGap() : 
            this->getContentHeight()
    );

    // Preserve relative scroll position
    m_list->m_contentLayer->setPositionY((
        m_list->m_contentLayer->getContentHeight() - m_list->getContentHeight()
    ) * oldPosition);

    // ModList uses an anchor layout, so this puts the list in the right place
    this->updateLayout();
}

void ModList::updateSize(bool big) {
    m_bigSize = big;

    // Update all BaseModItems that are children of the list
    // There may be non-BaseModItems there (like separators) so gotta be type-safe
    for (auto& node : CCArrayExt<CCNode*>(m_list->m_contentLayer->getChildren())) {
        if (auto item = typeinfo_cast<ModItem*>(node)) {
            item->updateSize(m_list->getContentWidth(), big);
        }
    }

    // Store old relative scroll position (ensuring no divide by zero happens)
    auto oldPositionArea = m_list->m_contentLayer->getContentHeight() - m_list->getContentHeight();
    auto oldPosition = oldPositionArea > 0.f ?
        m_list->m_contentLayer->getPositionY() / oldPositionArea : 
        -1.f;

    // Auto-grow the size of the list content
    m_list->m_contentLayer->updateLayout();

    // Preserve relative scroll position
    m_list->m_contentLayer->setPositionY((
        m_list->m_contentLayer->getContentHeight() - m_list->getContentHeight()
    ) * oldPosition);
}

void ModList::updateState() {
    // Update the "Show Updates" button on the updates available banner
    if (m_toggleUpdatesOnlyBtn) {
        auto src = typeinfo_cast<InstalledModListSource*>(m_source);
        if (src) {
            m_toggleUpdatesOnlyBtn->toggle(src->getQuery().onlyUpdates);
        }
    }

    auto pageCount = m_source->getPageCount();

    // Hide if page count hasn't been loaded
    m_pagePrevBtn->setVisible(pageCount && m_page > 0);
    m_pageNextBtn->setVisible(pageCount && m_page < pageCount.value() - 1);

    // Post the update page number event
    UpdateModListStateEvent(UpdatePageNumberState()).post();
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
    this->showStatus(ModListUnkProgressStatus(), "Loading...");
    m_listener.setFilter(m_source->loadPage(page, update).listen());

    // Do initial eager update on page UI (to prevent user spamming arrows 
    // to access invalid pages)
    this->updateState();
}

void ModList::showStatus(ModListStatus status, std::string const& message, std::optional<std::string> const& details) {
    // Clear list contents
    m_list->m_contentLayer->removeAllChildren();

    // Update status
    m_statusTitle->setString(message.c_str());
    m_statusDetails->setText(details.value_or(""));

    // Update status visibility
    m_statusContainer->setVisible(true);
    m_statusDetails->setVisible(false);
    m_statusDetailsBtn->setVisible(details.has_value());
    m_statusLoadingCircle->setVisible(std::holds_alternative<ModListUnkProgressStatus>(status));
    m_statusLoadingBar->setVisible(std::holds_alternative<ModListProgressStatus>(status));

    // Update progress bar
    if (auto per = std::get_if<ModListProgressStatus>(&status)) {
        m_statusLoadingBar->setValue(per->percentage / 100.f);
        m_statusLoadingBar->updateBar();
    }

    // Update layout to automatically rearrange everything neatly in the status
    m_statusContainer->updateLayout();
}

void ModList::onFilters(CCObject*) {
    FiltersPopup::create(m_source)->show();
}

void ModList::onClearFilters(CCObject*) {
    m_searchInput->setString("", true);
}

void ModList::onToggleUpdates(CCObject*) {
    if (auto src = typeinfo_cast<InstalledModListSource*>(m_source)) {
        src->getQueryMut()->onlyUpdates = !src->getQuery().onlyUpdates;
    }
}

void ModList::onUpdateAll(CCObject*) {}

size_t ModList::getPage() const {
    return m_page;
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
