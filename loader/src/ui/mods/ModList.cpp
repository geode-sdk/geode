#include "ModList.hpp"
#include <Geode/ui/TextInput.hpp>

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
            // This is half the normal size for separators
            ->setGap(2.5f)
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
                auto separator = CCLayerColor::create({ 255, 255, 255, 45 });
                separator->setContentSize({ m_obContentSize.width - 10, .5f });
                m_list->m_contentLayer->addChild(separator);
            }
            first = false;
            m_list->m_contentLayer->addChild(item);
        }
        this->updateSize(m_bigSize);
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
        // todo: details
        this->updatePageUI(true);
    }

    if (event->isFinally()) {
        // Clear listener
        m_listener.setFilter(ModListSource::PageLoadEventFilter());
    }
}

void ModList::updateSize(bool big) {
    m_bigSize = big;
    for (auto& node : CCArrayExt<CCNode*>(m_list->m_contentLayer->getChildren())) {
        if (auto item = typeinfo_cast<BaseModItem*>(node)) {
            item->updateSize(m_list->getContentWidth(), big);
        }
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

void ModList::onShowStatusDetails(CCObject*) {
    m_statusDetails->setVisible(!m_statusDetails->isVisible());
    m_statusContainer->updateLayout();
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
        auto fmt = fmt::format(
            "Page {}/{} (Total {})",
            m_page + 1, pageCount.value(), m_source->getItemCount().value()
        );
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
    this->showStatus(ModListUnkProgressStatus(), "Loading...");
    m_listener.setFilter(m_source->loadPage(page, update).listen());

    // Do initial eager update on page UI (to prevent user spamming arrows 
    // to access invalid pages)
    this->updatePageUI();
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

ModList* ModList::create(ModListSource* src, CCSize const& size) {
    auto ret = new ModList();
    if (ret && ret->init(src, size)) {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

