#include "ModList.hpp"
#include <Geode/utils/ColorProvider.hpp>
#include "../popups/FiltersPopup.hpp"
#include "../popups/SortPopup.hpp"
#include "../GeodeStyle.hpp"
#include "../ModsLayer.hpp"

bool ModList::init(ModListSource* src, CCSize const& size) {
    if (!CCNode::init())
        return false;
    
    this->setContentSize(size);
    this->setAnchorPoint({ .5f, .5f });
    this->setID("ModList");

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
    m_topContainer->setID("top-container");
    m_topContainer->ignoreAnchorPointForPosition(false);
    m_topContainer->setContentWidth(size.width);
    m_topContainer->setAnchorPoint({ .5f, 1.f });

    // Check for updates on installed mods, and show an update all button if there are some
    if (typeinfo_cast<InstalledModListSource*>(m_source)) {
        m_checkUpdatesListener.bind(this, &ModList::onCheckUpdates);
        m_checkUpdatesListener.setFilter(ModsLayer::checkInstalledModsForUpdates());

        m_updateAllContainer = CCNode::create();
        m_updateAllContainer->setID("update-all-container");
        m_updateAllContainer->ignoreAnchorPointForPosition(false);
        m_updateAllContainer->setContentSize({ size.width, 30 });
        m_updateAllContainer->setVisible(false);

        auto updateAllBG = CCLayerGradient::create(
            "mod-list-updates-available-bg"_cc4b,
            "mod-list-updates-available-bg-2"_cc4b,
            ccp(1, -.5f)
        );
        updateAllBG->setID("update-all-bg");
        updateAllBG->setContentSize(m_updateAllContainer->getContentSize());
        updateAllBG->ignoreAnchorPointForPosition(false);

        updateAllBG->addChildAtPosition(
            CCLayerColor::create("mod-list-bg"_cc4b, m_updateAllContainer->getContentWidth(), 1),
            Anchor::TopLeft
        );
        updateAllBG->addChildAtPosition(
            CCLayerColor::create("mod-list-bg"_cc4b, m_updateAllContainer->getContentWidth(), 1),
            Anchor::BottomLeft, ccp(0, -1)
        );

        m_updateAllContainer->addChildAtPosition(updateAllBG, Anchor::Center);
        
        m_updateCountLabel = TextArea::create("", "bigFont.fnt", .35f, size.width / 2 - 30, ccp(0, 1), 12.f, false);
        m_updateCountLabel->setID("update-count-label");
        m_updateAllContainer->addChildAtPosition(m_updateCountLabel, Anchor::Left, ccp(10, 0), ccp(0, 0));
        
        m_updateAllMenu = CCMenu::create();
        m_updateAllMenu->setID("update-all-menu");
        m_updateAllMenu->setContentWidth(size.width / 2);
        m_updateAllMenu->setAnchorPoint({ 1, .5f });

        m_showUpdatesSpr = createGeodeButton(
            CCSprite::createWithSpriteFrameName("GJ_filterIcon_001.png"),
            "Show Updates", GeodeButtonSprite::Install
        );
        m_hideUpdatesSpr = createGeodeButton(
            CCSprite::createWithSpriteFrameName("GJ_filterIcon_001.png"),
            "Hide Updates", GeodeButtonSprite::Default
        );
        m_toggleUpdatesOnlyBtn = CCMenuItemToggler::create(
            m_showUpdatesSpr, m_hideUpdatesSpr, this, menu_selector(ModList::onToggleUpdates)
        );
        m_toggleUpdatesOnlyBtn->setID("toggle-updates-only-button");
        m_toggleUpdatesOnlyBtn->m_notClickable = true;
        m_updateAllMenu->addChild(m_toggleUpdatesOnlyBtn);

        m_updateAllSpr = createGeodeButton(
            CCSprite::createWithSpriteFrameName("update.png"_spr),
            "Update All", GeodeButtonSprite::Install
        );
        m_updateAllBtn = CCMenuItemSpriteExtra::create(
            m_updateAllSpr, this, menu_selector(ModList::onUpdateAll)
        );
        m_updateAllBtn->setID("update-all-button");
        m_updateAllMenu->addChild(m_updateAllBtn);

        m_updateAllLoadingCircle = createLoadingCircle(32);
        m_updateAllMenu->addChild(m_updateAllLoadingCircle);

        m_updateAllMenu->setLayout(
            RowLayout::create()
                ->setAxisAlignment(AxisAlignment::End)
                ->setDefaultScaleLimits(.1f, .6f)
        );
        m_updateAllMenu->getLayout()->ignoreInvisibleChildren(true);
        m_updateAllContainer->addChildAtPosition(m_updateAllMenu, Anchor::Right, ccp(-10, 0));

        m_topContainer->addChild(m_updateAllContainer);

        if (Loader::get()->getProblems().size()) {
            m_errorsContainer = CCNode::create();
            m_errorsContainer->setID("errors-container");
            m_errorsContainer->ignoreAnchorPointForPosition(false);
            m_errorsContainer->setContentSize({ size.width, 30 });
            m_errorsContainer->setVisible(false);

            auto errorsBG = CCLayerGradient::create(
                "mod-list-errors-found"_cc4b,
                "mod-list-errors-found-2"_cc4b,
                ccp(1, -.5f)
            );
            errorsBG->setID("errors-bg");
            errorsBG->setContentSize(m_errorsContainer->getContentSize());
            errorsBG->ignoreAnchorPointForPosition(false);

            m_errorsContainer->addChildAtPosition(errorsBG, Anchor::Center);
            
            auto errorsLabel = TextArea::create(
                "There were <cy>errors</c> loading some mods",
                "bigFont.fnt", .35f, size.width / 2 - 30, ccp(0, 1), 12.f, false
            );
            errorsLabel->setID("errors-label");
            m_errorsContainer->addChildAtPosition(errorsLabel, Anchor::Left, ccp(10, 0), ccp(0, 0));
            
            auto errorsMenu = CCMenu::create();
            errorsMenu->setID("errors-menu");
            errorsMenu->setContentWidth(size.width / 2);
            errorsMenu->setAnchorPoint({ 1, .5f });

            auto showErrorsSpr = createGeodeButton(
                CCSprite::createWithSpriteFrameName("GJ_filterIcon_001.png"),
                "Show Errors Only", GeodeButtonSprite::Delete
            );
            auto hideErrorsSpr = createGeodeButton(
                CCSprite::createWithSpriteFrameName("GJ_filterIcon_001.png"),
                "Hide Errors Only", GeodeButtonSprite::Default
            );
            m_toggleErrorsOnlyBtn = CCMenuItemToggler::create(
                showErrorsSpr, hideErrorsSpr, this, menu_selector(ModList::onToggleErrors)
            );
            m_toggleErrorsOnlyBtn->setID("toggle-errors-only-button");
            m_toggleErrorsOnlyBtn->m_notClickable = true;
            errorsMenu->addChild(m_toggleErrorsOnlyBtn);

            errorsMenu->setLayout(
                RowLayout::create()
                    ->setAxisAlignment(AxisAlignment::End)
                    ->setDefaultScaleLimits(.1f, .6f)
            );
            m_errorsContainer->addChildAtPosition(errorsMenu, Anchor::Right, ccp(-10, 0));

            m_topContainer->addChild(m_errorsContainer);
        }
    }

    m_searchMenu = CCNode::create();
    m_searchMenu->setID("search-menu");
    m_searchMenu->ignoreAnchorPointForPosition(false);
    m_searchMenu->setContentSize({ size.width, 30 });

    auto searchBG = CCLayerColor::create(ColorProvider::get()->color("mod-list-search-bg"_spr));
    searchBG->setContentSize(m_searchMenu->getContentSize());
    searchBG->ignoreAnchorPointForPosition(false);
    searchBG->setID("search-id");
    m_searchMenu->addChildAtPosition(searchBG, Anchor::Center);

    m_searchInput = TextInput::create(size.width - 5, "Search Mods");
    m_searchInput->setID("search-input");
    m_searchInput->setScale(.75f);
    m_searchInput->setAnchorPoint({ 0, .5f });
    m_searchInput->setTextAlign(TextInputAlign::Left);
    m_searchInput->setCallback([this](auto const&) {
        // If the source is already in memory, we can immediately update the 
        // search query
        if (typeinfo_cast<InstalledModListSource*>(m_source)) {
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
    searchFiltersMenu->setID("search-filters-menu");
    searchFiltersMenu->setContentWidth(size.width - m_searchInput->getScaledContentWidth() - 5);
    searchFiltersMenu->setAnchorPoint({ 1, .5f });
    searchFiltersMenu->setScale(.75f);
    // Set higher prio to not let list items override touch
    searchFiltersMenu->setTouchPriority(-150);

    auto sortSpr = GeodeSquareSprite::createWithSpriteFrameName("GJ_sortIcon_001.png");
    auto sortBtn = CCMenuItemSpriteExtra::create(
        sortSpr, this, menu_selector(ModList::onSort)
    );
    sortBtn->setID("sort-button");
    if (!typeinfo_cast<ServerModListSource*>(m_source)) {
        sortBtn->setEnabled(false);
        sortSpr->setColor(ccGRAY);
        sortSpr->setOpacity(105);
        sortSpr->getTopSprite()->setColor(ccGRAY);
        sortSpr->getTopSprite()->setOpacity(105);
    }
    searchFiltersMenu->addChild(sortBtn);

    m_filtersBtn = CCMenuItemSpriteExtra::create(
        GeodeSquareSprite::createWithSpriteFrameName("GJ_filterIcon_001.png"),
        this, menu_selector(ModList::onFilters)
    );
    m_filtersBtn->setID("filters-button");
    searchFiltersMenu->addChild(m_filtersBtn);

    m_clearFiltersBtn = CCMenuItemSpriteExtra::create(
        GeodeSquareSprite::createWithSpriteFrameName("GJ_deleteIcon_001.png"),
        this, menu_selector(ModList::onClearFilters)
    );
    m_clearFiltersBtn->setID("clear-filters-button");
    searchFiltersMenu->addChild(m_clearFiltersBtn);

    searchFiltersMenu->setLayout(
        RowLayout::create()
            ->setAxisAlignment(AxisAlignment::End)
    );
    m_searchMenu->addChildAtPosition(searchFiltersMenu, Anchor::Right, ccp(-10, 0));

    m_topContainer->addChild(m_searchMenu);

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
    pageLeftMenu->setID("page-left-menu");
    pageLeftMenu->setContentWidth(30.f);
    pageLeftMenu->setAnchorPoint({ 1.f, .5f });

    m_pagePrevBtn = CCMenuItemSpriteExtra::create(
        CCSprite::createWithSpriteFrameName("GJ_arrow_02_001.png"),
        this, menu_selector(ModList::onPage)
    );
    m_pagePrevBtn->setID("page-previous-button");
    m_pagePrevBtn->setTag(-1);
    pageLeftMenu->addChild(m_pagePrevBtn);

    pageLeftMenu->setLayout(
        RowLayout::create()
            ->setAxisAlignment(AxisAlignment::End)
            ->setAxisReverse(true)
    );
    this->addChildAtPosition(pageLeftMenu, Anchor::Left, ccp(-5, 0));

    auto pageRightMenu = CCMenu::create();
    pageRightMenu->setID("page-right-menu");
    pageRightMenu->setContentWidth(30.f);
    pageRightMenu->setAnchorPoint({ 0.f, .5f });

    auto pageNextSpr = CCSprite::createWithSpriteFrameName("GJ_arrow_02_001.png");
    pageNextSpr->setFlipX(true);
    m_pageNextBtn = CCMenuItemSpriteExtra::create(
        pageNextSpr,
        this, menu_selector(ModList::onPage)
    );
    m_pageNextBtn->setID("page-next-button");
    m_pageNextBtn->setTag(1);
    pageRightMenu->addChild(m_pageNextBtn);

    pageRightMenu->setLayout(
        RowLayout::create()
            ->setAxisAlignment(AxisAlignment::Start)
    );
    this->addChildAtPosition(pageRightMenu, Anchor::Right, ccp(5, 0));

    // Status

    m_statusContainer = CCMenu::create();
    m_statusContainer->setID("status-container");
    m_statusContainer->setScale(.5f);
    m_statusContainer->setContentHeight(size.height / m_statusContainer->getScale());
    m_statusContainer->setAnchorPoint({ .5f, .5f });
    m_statusContainer->ignoreAnchorPointForPosition(false);

    m_statusTitle = CCLabelBMFont::create("", "bigFont.fnt");
    m_statusTitle->setID("status-title-label");
    m_statusTitle->setAlignment(kCCTextAlignmentCenter);
    m_statusContainer->addChild(m_statusTitle);

    m_statusDetailsBtn = CCMenuItemSpriteExtra::create(
        ButtonSprite::create("Details", "bigFont.fnt", "GJ_button_05.png", .75f),
        this, menu_selector(ModList::onShowStatusDetails)
    );
    m_statusDetailsBtn->setID("status-details-button");
    m_statusContainer->addChild(m_statusDetailsBtn);

    m_statusDetails = SimpleTextArea::create("", "chatFont.fnt", .6f);
    m_statusDetails->setID("status-details-input");
    m_statusDetails->setAlignment(kCCTextAlignmentCenter);
    m_statusContainer->addChild(m_statusDetails);

    m_statusLoadingCircle = createLoadingCircle(50);
    m_statusContainer->addChild(m_statusLoadingCircle);

    m_statusLoadingBar = Slider::create(this, nullptr);
    m_statusLoadingBar->setID("status-loading-bar");
    m_statusLoadingBar->m_touchLogic->m_thumb->setVisible(false);
    m_statusLoadingBar->setValue(0);
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

    m_downloadListener.bind([this](auto) { this->updateTopContainer(); });
    
    this->gotoPage(0);
    this->updateTopContainer();

    return true;
}

void ModList::onPromise(ModListSource::PageLoadTask::Event* event) {
    if (event->getValue()) {
        auto result = event->getValue();
        if (result->isOk()) {
            // Hide status
            m_statusContainer->setVisible(false);

            auto list = result->unwrap();

            // Create items
            bool first = true;
            for (auto item : list) {
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
        else {
            auto error = result->unwrapErr();
            this->showStatus(ModListErrorStatus(), error.message, error.details);
            this->updateState();
        }

        // Clear listener
        m_listener.setFilter(ModListSource::PageLoadTask());
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
    else if (event->isCancelled()) {
        this->showStatus(ModListErrorStatus(), "Loading Mods Cancelled");
        this->updateState();
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

void ModList::onCheckUpdates(typename server::ServerRequest<std::vector<std::string>>::Event* event) {
    if (event->getValue() && event->getValue()->isOk()) {
        if (auto mods = event->getValue()->unwrap(); mods.size() > 0) {
            if (mods.size() == 1) {
                m_updateCountLabel->setString(fmt::format("There is <cg>{}</c> update available!", mods.size()));
                m_updateAllSpr->setString("");
                m_showUpdatesSpr->setString("Show Update");
                m_hideUpdatesSpr->setString("Hide Update");
            }
            else {
                m_updateCountLabel->setString(fmt::format("There are <cg>{}</c> updates available!", mods.size()));
                m_updateAllSpr->setString("Update All");
                m_showUpdatesSpr->setString("Show Updates");
                m_hideUpdatesSpr->setString("Hide Updates");
            }

            // Recreate the button with the updated label.
            m_updateAllMenu->removeChild(m_updateAllBtn, false);
            m_updateAllBtn = CCMenuItemSpriteExtra::create(
                m_updateAllSpr, this, menu_selector(ModList::onUpdateAll)
            );
            m_updateAllBtn->setID("update-all-button");
            m_updateAllMenu->addChild(m_updateAllBtn);

            m_updateAllContainer->setVisible(true);
            this->updateTopContainer();
        }
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

    // If there are active downloads, hide the Update All button
    if (m_updateAllContainer) {
        auto shouldShowLoading = server::ModDownloadManager::get()->hasActiveDownloads();
        m_updateAllBtn->setEnabled(!shouldShowLoading);
        static_cast<IconButtonSprite*>(m_updateAllBtn->getNormalImage())->setOpacity(shouldShowLoading ? 90 : 255);
        m_updateAllLoadingCircle->setVisible(shouldShowLoading);
        m_updateAllMenu->updateLayout();
    }

    // If there are errors, show the error banner
    if (m_errorsContainer) {
        auto noErrors = Loader::get()->getProblems().empty();
        m_errorsContainer->setVisible(!noErrors);
    }

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
    // Update the "Show Updates" and "Show Errors" buttons on 
    // the updates available / errors banners
    if (auto src = typeinfo_cast<InstalledModListSource*>(m_source)) {
        if (m_toggleUpdatesOnlyBtn) {
            m_toggleUpdatesOnlyBtn->toggle(src->getQuery().type == InstalledModListType::OnlyUpdates);
        }
        if (m_toggleErrorsOnlyBtn) {
            m_toggleErrorsOnlyBtn->toggle(src->getQuery().type == InstalledModListType::OnlyErrors);
        }
    }

    auto pageCount = m_source->getPageCount();

    // Hide if page count hasn't been loaded
    m_pagePrevBtn->setVisible(pageCount && m_page > 0);
    m_pageNextBtn->setVisible(pageCount && m_page < pageCount.value() - 1);

    // Update filter button states
    auto isDefaultQuery = m_source->isDefaultQuery();

    auto filterSpr = static_cast<GeodeSquareSprite*>(m_filtersBtn->getNormalImage());
    filterSpr->setState(!isDefaultQuery);

    auto clearSpr = static_cast<GeodeSquareSprite*>(m_clearFiltersBtn->getNormalImage());
    clearSpr->setColor(isDefaultQuery ? ccGRAY : ccWHITE);
    clearSpr->setOpacity(isDefaultQuery ? 90 : 255);
    clearSpr->getTopSprite()->setColor(isDefaultQuery ? ccGRAY : ccWHITE);
    clearSpr->getTopSprite()->setOpacity(isDefaultQuery ? 90 : 255);

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
    m_listener.setFilter(m_source->loadPage(page, update));

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
    }

    // Update layout to automatically rearrange everything neatly in the status
    m_statusContainer->updateLayout();
}

void ModList::onFilters(CCObject*) {
    FiltersPopup::create(m_source)->show();
}
void ModList::onSort(CCObject*) {
    SortPopup::create(m_source)->show();
}
void ModList::onClearFilters(CCObject*) {
    m_searchInput->setString("", false);
    m_source->reset();
}
void ModList::onToggleUpdates(CCObject*) {
    if (auto src = typeinfo_cast<InstalledModListSource*>(m_source)) {
        auto mut = src->getQueryMut();
        mut->type = mut->type == InstalledModListType::OnlyUpdates ? 
            InstalledModListType::All : 
            InstalledModListType::OnlyUpdates;
    }
}
void ModList::onToggleErrors(CCObject*) {
    if (auto src = typeinfo_cast<InstalledModListSource*>(m_source)) {
        auto mut = src->getQueryMut();
        mut->type = mut->type == InstalledModListType::OnlyErrors ? 
            InstalledModListType::All : 
            InstalledModListType::OnlyErrors;
    }
}
void ModList::onUpdateAll(CCObject*) {
    server::ModDownloadManager::get()->startUpdateAll();
}

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
