#include "ModList.hpp"
#include <Geode/utils/ColorProvider.hpp>
#include "TagsPopup.hpp"
#include "GeodeStyle.hpp"

bool ModList::init(ModListSource* src, CCSize const& size) {
    if (!CCNode::init())
        return false;
    
    this->setContentSize(size);
    this->setAnchorPoint({ .5f, .5f });

    m_source = src;
    src->reset();
    
    m_list = ScrollLayer::create(size);
    m_list->m_contentLayer->setLayout(
        ColumnLayout::create()
            ->setAxisReverse(true)
            ->setAxisAlignment(AxisAlignment::End)
            ->setAutoGrowAxis(size.height)
            ->setGap(2.5f)
    );
    this->addChildAtPosition(m_list, Anchor::Bottom, ccp(-m_list->getScaledContentWidth() / 2, 0));

    m_searchMenu = CCNode::create();
    m_searchMenu->ignoreAnchorPointForPosition(false);
    m_searchMenu->setContentSize({ size.width, 30 });
    m_searchMenu->setAnchorPoint({ .5f, 1.f });

    auto searchBG = CCLayerColor::create(
        ColorProvider::get()->define("mod-list-search-bg"_spr, { 83, 65, 109, 255 })
    );
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
        if (m_source->isInMemory()) {
            m_source->setQuery(m_searchInput->getString());
            this->gotoPage(0);
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
                    m_source->setQuery(m_searchInput->getString());
                    this->gotoPage(0);
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

    // Do not add search menu; that's handled by onSearch

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

    m_statusLoadingCircle = CCSprite::create("loadingCircle.png");
    m_statusLoadingCircle->setBlendFunc({ GL_ONE, GL_ONE });
    m_statusLoadingCircle->setScale(.6f);
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

    this->gotoPage(0);

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
            item->onUpdateParentState(m_updateParentState);
        }
        this->updateSize(m_bigSize);

        // Scroll list to top
        auto listTopScrollPos = -m_list->m_contentLayer->getContentHeight() + m_list->getContentHeight();
        m_list->m_contentLayer->setPositionY(listTopScrollPos);

        // Update page UI
        this->updatePageNumber();
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
        this->updatePageNumber();
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

void ModList::activateSearch(bool activate) {
    // Add the menu or remove it depending on new state
    if (activate) {
        if (!m_searchMenu->getParent()) {
            this->addChildAtPosition(m_searchMenu, Anchor::Top);
        }
    }
    else {
        m_searchMenu->removeFromParent();
    }

    // Store old relative scroll position (ensuring no divide by zero happens)
    auto oldPositionArea = m_list->m_contentLayer->getContentHeight() - m_list->getContentHeight();
    auto oldPosition = oldPositionArea > 0.f ?
        m_list->m_contentLayer->getPositionY() / oldPositionArea : 
        -1.f;
        
    // Update list size to account for the search menu 
    // (giving a little bit of extra padding for it, the same size as gap)
    m_list->setContentHeight(
        activate ?
            this->getContentHeight() - m_searchMenu->getContentHeight() - 
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

void ModList::updatePageNumber() {
    auto pageCount = m_source->getPageCount();

    // Hide if page count hasn't been loaded
    m_pagePrevBtn->setVisible(pageCount && m_page > 0);
    m_pageNextBtn->setVisible(pageCount && m_page < pageCount.value() - 1);

    // Notify container about page count update
    if (m_updateParentState) {
        m_updateParentState();
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
    this->showStatus(ModListUnkProgressStatus(), "Loading...");
    m_listener.setFilter(m_source->loadPage(page, update).listen());

    // Do initial eager update on page UI (to prevent user spamming arrows 
    // to access invalid pages)
    this->updatePageNumber();
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

    // The loading circle action gets stopped for some reason so just reactivate it
    if (m_statusLoadingCircle->isVisible()) {
        m_statusLoadingCircle->runAction(CCRepeatForever::create(CCRotateBy::create(1.f, 360.f)));
    }
    // Update progress bar
    if (auto per = std::get_if<ModListProgressStatus>(&status)) {
        m_statusLoadingBar->setValue(per->percentage / 100.f);
        m_statusLoadingBar->updateBar();
    }

    // Update layout to automatically rearrange everything neatly in the status
    m_statusContainer->updateLayout();
}

void ModList::onUpdateParentState(MiniFunction<void()> listener) {
    m_updateParentState = listener;
}

void ModList::onFilters(CCObject*) {
    TagsPopup::create(m_source, [this]() {
        this->gotoPage(0);
    })->show();
}

void ModList::onClearFilters(CCObject*) {
    m_searchInput->setString("", true);
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

