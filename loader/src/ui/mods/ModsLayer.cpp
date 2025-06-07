#include "ModsLayer.hpp"
#include <Geode/binding/CCMenuItemSpriteExtra.hpp>
#include <Geode/loader/Dirs.hpp>
#include <Geode/ui/BasedButtonSprite.hpp>
#include <Geode/utils/file.hpp>
#include <Geode/cocos/cocoa/CCObject.h>
#include <Geode/loader/Event.hpp>
#include "SwelvyBG.hpp"
#include <Geode/ui/TextInput.hpp>
#include <Geode/utils/ColorProvider.hpp>
#include <Geode/utils/ranges.hpp>
#include <Geode/ui/GeodeUI.hpp>
#include <Geode/ui/SimpleAxisLayout.hpp>
#include <Geode/binding/Slider.hpp>
#include <Geode/binding/SetTextPopup.hpp>
#include <Geode/binding/SetIDPopup.hpp>
#include <Geode/binding/ButtonSprite.hpp>
#include <Geode/binding/MenuLayer.hpp>
#include "popups/ConfirmInstall.hpp"
#include "GeodeStyle.hpp"
#include "ui/mods/sources/ModListSource.hpp"
#include <loader/LoaderImpl.hpp>
#include <Geode/ui/MDPopup.hpp>

bool ModsStatusNode::init() {
    if (!CCNode::init())
        return false;

    this->ignoreAnchorPointForPosition(false);
    this->setAnchorPoint({ .5f, 1.f });
    this->setContentSize({ 300, 35 });
    this->setID("ModsStatusNode");

    m_statusBG = CCScale9Sprite::create("black-square.png"_spr);
    m_statusBG->setID("status-bg");
    m_statusBG->setContentSize({ 570, 40 });
    m_statusBG->setScale(.5f);

    m_status = CCLabelBMFont::create("", "bigFont.fnt");
    m_status->setID("status-label");
    m_status->setScale(.8f);
    m_statusBG->addChildAtPosition(m_status, Anchor::Center);

    m_statusPercentage = CCLabelBMFont::create("", "bigFont.fnt");
    m_statusPercentage->setID("status-percentage-label");
    m_statusPercentage->setScale(.8f);
    m_statusBG->addChildAtPosition(m_statusPercentage, Anchor::Right, ccp(-25, 0));

    m_loadingCircle = createLoadingCircle(32);
    m_statusBG->addChildAtPosition(m_loadingCircle, Anchor::Left, ccp(25, 0));

    m_progressBar = Slider::create(nullptr, nullptr);
    m_progressBar->setID("progress-bar");
    m_progressBar->m_touchLogic->m_thumb->setVisible(false);
    m_progressBar->setScale(2.f);
    m_progressBar->setAnchorPoint({ 0, 0 }),
    m_statusBG->addChildAtPosition(m_progressBar, Anchor::Center);

    this->addChildAtPosition(m_statusBG, Anchor::Bottom);

    m_btnMenu = CCMenu::create();
    m_btnMenu->setID("button-menu");
    m_btnMenu->setContentWidth(m_obContentSize.width);
    m_btnMenu->setContentHeight(25.f);
    auto restartSpr = createGeodeButton("Restart Now");
    restartSpr->setScale(.65f);
    m_restartBtn = CCMenuItemSpriteExtra::create(
        restartSpr, this, menu_selector(ModsStatusNode::onRestart)
    );
    m_restartBtn->setID("restart-button");
    m_btnMenu->addChild(m_restartBtn);

    auto viewSpr = createGeodeButton("View");
    viewSpr->setScale(.65f);
    m_viewBtn = CCMenuItemSpriteExtra::create(viewSpr, this, nullptr);
    m_viewBtn->setID("view-button");
    m_btnMenu->addChild(m_viewBtn);

    auto cancelSpr = createGeodeButton("Cancel");
    cancelSpr->setScale(.65f);
    m_cancelBtn = CCMenuItemSpriteExtra::create(
        cancelSpr, this, menu_selector(ModsStatusNode::onCancel)
    );
    m_cancelBtn->setID("cancel-button");
    m_btnMenu->addChild(m_cancelBtn);

    m_btnMenu->setLayout(
        SimpleRowLayout::create()
            ->setGap(5.f)
    );
    m_btnMenu->getLayout()->ignoreInvisibleChildren(true);
    this->addChildAtPosition(m_btnMenu, Anchor::Center, ccp(0, 5));

    m_updateStateListener.bind([this](auto) { this->updateState(); });
    m_updateStateListener.setFilter(UpdateModListStateFilter());

    m_downloadListener.bind([this](auto) { this->updateState(); });

    m_settingNodeListener.bind([this](SettingNodeValueChangeEvent* ev) {
        if (!ev->isCommit()) {
            return ListenerResult::Propagate;
        }
        this->updateState();
        return ListenerResult::Propagate;
    });

    this->updateState();

    return true;
}

void ModsStatusNode::updateState() {
    DownloadState state = DownloadState::None;
    auto upgradeState = [&](DownloadState into) {
        if (static_cast<int>(state) < static_cast<int>(into)) {
            state = into;
        }
    };

    auto downloads = server::ModDownloadManager::get()->getDownloads();
    for (auto& download : downloads) {
        std::visit(makeVisitor {
            [&](server::DownloadStatusFetching const&) {
                upgradeState(DownloadState::SomeFetching);
            },
            [&](server::DownloadStatusConfirm const&) {
                upgradeState(DownloadState::SomeToBeConfirmed);
            },
            [&](server::DownloadStatusDownloading const&) {
                upgradeState(DownloadState::SomeDownloading);
            },
            [&](server::DownloadStatusDone const&) {
                upgradeState(DownloadState::AllDone);
            },
            [&](server::DownloadStatusError const&) {
                upgradeState(DownloadState::SomeErrored);
            },
            [&](server::DownloadStatusCancelled const&) {
                upgradeState(DownloadState::SomeCancelled);
            },
        }, download.getStatus());
    }

    // Reset the state to default
    m_statusBG->setVisible(false);
    m_status->setVisible(false);
    m_statusPercentage->setVisible(false);
    m_loadingCircle->setVisible(false);
    m_progressBar->setVisible(false);
    m_restartBtn->setVisible(false);
    m_cancelBtn->setVisible(false);
    m_viewBtn->setVisible(false);

    switch (state) {
        // If there are no downloads happening, just show the restart button if needed
        case DownloadState::None: {
            m_restartBtn->setVisible(LoaderImpl::get()->isRestartRequired());
        } break;

        // If some downloads were cancelled, show the restart button normally
        case DownloadState::SomeCancelled: {
            m_status->setString("Download(s) Cancelled");
            m_status->setColor(ccWHITE);
            m_status->setVisible(true);

            m_restartBtn->setVisible(LoaderImpl::get()->isRestartRequired());
        } break;

        // If all downloads were finished, show the restart button normally
        // but also a "all done" status
        case DownloadState::AllDone: {
            if (downloads.size() == 1) {
                m_status->setString(fmt::format("{} Mod Installed/Updated", downloads.size()).c_str());
            }
            else {
                m_status->setString(fmt::format("{} Mods Installed/Updated", downloads.size()).c_str());
            }
            m_status->setColor("mod-list-enabled"_cc3b);
            m_status->setVisible(true);
            m_statusBG->setVisible(true);

            m_restartBtn->setVisible(LoaderImpl::get()->isRestartRequired());
        } break;

        case DownloadState::SomeErrored: {
            m_status->setString("Some Download(s) Failed");
            m_status->setColor("mod-list-disabled"_cc3b);
            m_status->setVisible(true);
            m_statusBG->setVisible(true);

            m_viewBtn->setVisible(true);
            m_viewBtn->setTarget(this, menu_selector(ModsStatusNode::onViewErrors));
        } break;

        case DownloadState::SomeToBeConfirmed: {
            size_t totalToConfirm = 0;
            for (auto& download : downloads) {
                auto status = download.getStatus();
                if (auto loading = std::get_if<server::DownloadStatusConfirm>(&status)) {
                    totalToConfirm += 1;
                }
            }
            if (totalToConfirm == 1) {
                m_status->setString(fmt::format("Click to Confirm {} Download", totalToConfirm).c_str());
            }
            else {
                m_status->setString(fmt::format("Click to Confirm {} Downloads", totalToConfirm).c_str());
            }
            m_status->setColor(ccWHITE);
            m_status->setVisible(true);
            m_statusBG->setVisible(true);

            m_viewBtn->setVisible(true);
            m_viewBtn->setTarget(this, menu_selector(ModsStatusNode::onConfirm));
            if(m_lastState != state) askConfirmModInstalls();
        } break;

        case DownloadState::SomeFetching: {
            m_status->setString("Preparing Download(s)");
            m_status->setColor(ccWHITE);
            m_status->setVisible(true);
            m_loadingCircle->setVisible(true);
            m_statusBG->setVisible(true);

            m_cancelBtn->setVisible(true);
        } break;

        case DownloadState::SomeDownloading: {
            size_t totalProgress = 0;
            size_t totalDownloading = 0;
            for (auto& download : downloads) {
                auto status = download.getStatus();
                if (auto loading = std::get_if<server::DownloadStatusDownloading>(&status)) {
                    totalProgress += loading->percentage;
                    totalDownloading += 1;
                }
            }
            auto percentage = totalProgress / static_cast<float>(totalDownloading);

            m_statusPercentage->setString(fmt::format("{}%", static_cast<size_t>(percentage)).c_str());
            m_statusPercentage->setVisible(true);
            m_loadingCircle->setVisible(true);
            m_statusBG->setVisible(true);

            m_cancelBtn->setVisible(true);

            m_progressBar->setVisible(true);
            m_progressBar->setValue(percentage / 100.f);
        } break;
    }

    m_lastState = state;
    m_btnMenu->updateLayout();
}

void ModsStatusNode::onViewErrors(CCObject*) {
    auto downloads = server::ModDownloadManager::get()->getDownloads();
    std::vector<std::string> errors;
    for (auto& download : downloads) {
        auto status = download.getStatus();
        if (auto error = std::get_if<server::DownloadStatusError>(&status)) {
            errors.push_back(fmt::format("<cr>{}</c>: {}", download.getID(), error->details));
        }
    }
    MDPopup::create(
        "Download Errors", ranges::join(errors, "\n\n"),
        "OK", "Dismiss",
        [](bool btn2) {
            if (btn2) {
                server::ModDownloadManager::get()->dismissAll();
            }
        }
    )->show();
}
void ModsStatusNode::onConfirm(CCObject*) {
    askConfirmModInstalls();
}
void ModsStatusNode::onCancel(CCObject*) {
    server::ModDownloadManager::get()->cancelAll();
}

void ModsLayer::onOpenModsFolder(CCObject*) {
    file::openFolder(dirs::getModsDir());
}

void ModsLayer::onAddModFromFile(CCObject*) {
    if (!Mod::get()->setSavedValue("shown-manual-install-info", true)) {
        return FLAlertLayer::create(
            nullptr,
            "Manually Installing Mods",
            "You can <cg>manually install mods</c> by selecting their <cd>.geode</c> files. "
            "Do note that manually installed mods <co>are not verified to be safe and stable</c>!\n"
            "<cr>Proceed at your own risk!</c>",
            "OK", nullptr,
            350
        )->show();
    }
    file::pick(file::PickMode::OpenFile, file::FilePickOptions {
        .filters = { file::FilePickOptions::Filter {
            .description = "Geode Mods",
            .files = { "*.geode" },
        }}
    }).listen([](Result<std::filesystem::path>* path) {
        if (*path) {
            LoaderImpl::get()->installModManuallyFromFile(path->unwrap(), []() {
                InstalledModListSource::get(InstalledModListType::All)->clearCache();
            });
        }
        else {
            FLAlertLayer::create(
                "Unable to Select File",
                path->unwrapErr(),
                "OK"
            )->show();
        }
    });
}

void ModsStatusNode::onRestart(CCObject*) {
    // Update button state to let user know it's restarting but it might take a bit
    m_restartBtn->setEnabled(false);
    static_cast<ButtonSprite*>(m_restartBtn->getNormalImage())->setString("Restarting...");
    m_restartBtn->updateSprite();

    // Actually restart
    Loader::get()->queueInMainThread([] {
        // Delayed by 2 frames - one is needed to render the "Restarting text"
        Loader::get()->queueInMainThread([] {
            // the other never finishes rendering because the game actually restarts at this point
            game::restart(true);
        });
    });
}

ModsStatusNode* ModsStatusNode::create() {
    auto ret = new ModsStatusNode();
    if (ret->init()) {
        ret->autorelease();
        return ret;
    }
    delete ret;
    return nullptr;
}

bool ModsLayer::init() {
    if (!CCLayer::init())
        return false;

    this->setID("ModsLayer");

    auto safeArea = geode::utils::getSafeAreaRect();

    auto winSize = CCDirector::get()->getWinSize();
    const bool isSafeMode = LoaderImpl::get()->isSafeMode();

    const bool geodeTheme = isGeodeTheme();
    if (!isSafeMode) {
        if (geodeTheme) {
            this->addChild(SwelvyBG::create());
        }
        else {
            this->addChild(createLayerBG());
            addSideArt(this);
        }
    }

    m_modListDisplay = Mod::get()->getSavedValue<ModListDisplay>("mod-list-display-type");

    auto backMenu = CCMenu::create();
    backMenu->setID("back-menu");
    backMenu->setContentSize({100.f, 40.f});
    backMenu->setAnchorPoint({ .0f, .5f });

    auto backSpr = CCSprite::createWithSpriteFrameName("GJ_arrow_03_001.png");
    auto backBtn = CCMenuItemSpriteExtra::create(
        backSpr, this, menu_selector(ModsLayer::onBack)
    );
    backBtn->setID("back-button");
    backMenu->addChild(backBtn);

    backMenu->setLayout(
        SimpleRowLayout::create()
            ->setMainAxisAlignment(MainAxisAlignment::Start)
            ->setGap(5.f)
    );
    this->addChildAtPosition(backMenu, Anchor::TopLeft, ccp(12, -25), false);

    auto actionsMenu = CCMenu::create();
    actionsMenu->setID("actions-menu");
    actionsMenu->setContentSize({38.f, 200.f});
    actionsMenu->setAnchorPoint({ .5f, .0f });

    auto rightActionsMenu = CCMenu::create();
    rightActionsMenu->setID("right-actions-menu");
    rightActionsMenu->setContentSize({38.f, 200.f});
    rightActionsMenu->setAnchorPoint({ .5f, .0f });

    auto reloadSpr = createGeodeCircleButton(
        CCSprite::createWithSpriteFrameName("reload.png"_spr), 1.f,
        CircleBaseSize::Medium
    );
    reloadSpr->setScale(.8f);
    reloadSpr->setTopOffset(ccp(1, 0));
    auto reloadBtn = CCMenuItemSpriteExtra::create(
        reloadSpr, this, menu_selector(ModsLayer::onRefreshList)
    );
    reloadBtn->setID("reload-button");
    rightActionsMenu->addChild(reloadBtn);

    auto settingsSpr = createGeodeCircleButton(
        CCSprite::createWithSpriteFrameName("settings.png"_spr), 1.f,
        CircleBaseSize::Medium
    );
    settingsSpr->setScale(.8f);
    settingsSpr->setTopOffset(ccp(.5f, 0));
    auto settingsBtn = CCMenuItemSpriteExtra::create(
        settingsSpr, this, menu_selector(ModsLayer::onSettings)
    );
    settingsBtn->setID("settings-button");
    actionsMenu->addChild(settingsBtn);

    auto folderSpr = createGeodeCircleButton(
        CCSprite::createWithSpriteFrameName("gj_folderBtn_001.png"), 1.f,
        CircleBaseSize::Medium
    );
    folderSpr->setScale(0.8f);
    auto folderBtn = CCMenuItemSpriteExtra::create(
        folderSpr,
        this,
        menu_selector(ModsLayer::onOpenModsFolder)
    );
    folderBtn->setID("mods-folder-button");
    actionsMenu->addChild(folderBtn);

    auto addSpr = createGeodeCircleButton(
        CCSprite::createWithSpriteFrameName("file-add.png"_spr), 1.f,
        CircleBaseSize::Medium
    );
    addSpr->setScale(.8f);
    addSpr->setTopRelativeScale(.8f);
    auto addBtn = CCMenuItemSpriteExtra::create(
        addSpr,
        this,
        menu_selector(ModsLayer::onAddModFromFile)
    );
    addBtn->setID("mods-add-button");
    actionsMenu->addChild(addBtn);

    actionsMenu->setLayout(
        SimpleColumnLayout::create()
            ->setMainAxisAlignment(MainAxisAlignment::Start)
            ->setMainAxisDirection(AxisDirection::BottomToTop)
            ->setGap(2.f)
    );

    rightActionsMenu->setLayout(
        SimpleColumnLayout::create()
            ->setMainAxisAlignment(MainAxisAlignment::Start)
            ->setMainAxisDirection(AxisDirection::BottomToTop)
            ->setGap(2.f)
    );

    // positioning based on size of mod list frame and maximum width of buttons
    // i would apologize
    auto actionsMenuX = std::min(35.0f, (winSize.width - 380.0f - 10.0f) / 4.0f);
    auto safeOffsetRight = winSize.width - (safeArea.size.width + safeArea.origin.x);

    // center buttons when the actionsMenu is moved
    auto actionsMenuY = std::min(actionsMenuX - 20.0f, 12.0f);

    this->addChildAtPosition(actionsMenu, Anchor::BottomLeft, ccp(actionsMenuX + safeArea.origin.x, actionsMenuY), false);
    this->addChildAtPosition(rightActionsMenu, Anchor::BottomRight, ccp(-actionsMenuX - safeOffsetRight, actionsMenuY), false);

    m_frame = CCNode::create();
    m_frame->setID("mod-list-frame");
    m_frame->setAnchorPoint({ .5f, .5f });
    m_frame->setContentSize({ 380, 205 });

    auto frameBG = CCLayerColor::create(ColorProvider::get()->color("mod-list-bg"_spr));
    frameBG->setID("frame-bg");
    frameBG->setContentSize(m_frame->getContentSize());
    frameBG->ignoreAnchorPointForPosition(false);
    m_frame->addChildAtPosition(frameBG, Anchor::Center);

    auto tabsTop = CCSprite::createWithSpriteFrameName(geodeTheme ? "mods-list-top.png"_spr : "mods-list-top-gd.png"_spr);
    tabsTop->setID("frame-top-sprite");
    tabsTop->setAnchorPoint({ .5f, .0f });
    tabsTop->setZOrder(1);
    m_frame->addChildAtPosition(tabsTop, Anchor::Top, ccp(0, -2));

    auto tabsLeft = CCSprite::createWithSpriteFrameName(geodeTheme ? "mods-list-side.png"_spr : "mods-list-side-gd.png"_spr);
    tabsLeft->setID("frame-left-sprite");
    tabsLeft->setScaleY(m_frame->getContentHeight() / tabsLeft->getContentHeight());
    m_frame->addChildAtPosition(tabsLeft, Anchor::Left, ccp(6.5f, 1));

    auto tabsRight = CCSprite::createWithSpriteFrameName(geodeTheme ? "mods-list-side.png"_spr : "mods-list-side-gd.png"_spr);
    tabsRight->setID("frame-right-sprite");
    tabsRight->setFlipX(true);
    tabsRight->setScaleY(m_frame->getContentHeight() / tabsRight->getContentHeight());
    m_frame->addChildAtPosition(tabsRight, Anchor::Right, ccp(-6.5f, 1));

    auto tabsBottom = CCSprite::createWithSpriteFrameName(geodeTheme ? "mods-list-bottom.png"_spr : "mods-list-bottom-gd.png"_spr);
    tabsBottom->setID("frame-bottom-sprite");
    tabsBottom->setAnchorPoint({ .5f, 1.f });
    tabsBottom->setZOrder(1);
    m_frame->addChildAtPosition(tabsBottom, Anchor::Bottom, ccp(0, 3));

    this->addChildAtPosition(m_frame, Anchor::Center, ccp(0, -10), false);

    auto mainTabs = CCMenu::create();
    mainTabs->setID("tabs-menu");
    mainTabs->setContentWidth(tabsTop->getContentWidth() - 45);
    mainTabs->setContentHeight(32.f);
    mainTabs->setAnchorPoint({ .5f, .0f });
    mainTabs->setPosition(m_frame->convertToWorldSpace(tabsTop->getPosition() + ccp(0, 6)));
    // Increment touch priority so the mods in the list don't override
    mainTabs->setTouchPriority(-150);

    for (auto item : std::initializer_list<std::tuple<const char*, const char*, ModListSource*, const char*, bool>> {
        { "download.png"_spr, "Installed", InstalledModListSource::get(InstalledModListType::All), "installed-button", false },
        { "GJ_starsIcon_001.png", "Featured", ServerModListSource::get(ServerModListType::Featured), "featured-button", false },
        { "globe.png"_spr, "Download", ServerModListSource::get(ServerModListType::Download), "download-button", false },
        { "GJ_timeIcon_001.png", "Recent", ServerModListSource::get(ServerModListType::Recent), "recent-button", false },
    }) {
        auto btn = CCMenuItemSpriteExtra::create(
            GeodeTabSprite::create(std::get<0>(item), std::get<1>(item), 100, std::get<4>(item)),
            this, menu_selector(ModsLayer::onTab)
        );
        btn->setUserData(std::get<2>(item));
        btn->setID(std::get<3>(item));
        mainTabs->addChild(btn);
        m_tabs.push_back(btn);
    }

    mainTabs->setLayout(
        SimpleRowLayout::create()
            ->setMainAxisScaling(AxisScaling::Scale)
            ->setGap(5.f)
    );
    this->addChild(mainTabs);

    // Actions

    auto listDisplayMenu = CCMenu::create();
    listDisplayMenu->setID("list-actions-menu");
    listDisplayMenu->setContentSize({30, 100});
    listDisplayMenu->setAnchorPoint({ 1, 0 });
    listDisplayMenu->setScale(.65f);

    auto smallSizeBtn = CCMenuItemSpriteExtra::create(
        GeodeSquareSprite::createWithSpriteFrameName("GJ_smallModeIcon_001.png"),
        this, menu_selector(ModsLayer::onDisplay)
    );
    smallSizeBtn->setTag(static_cast<int>(ModListDisplay::SmallList));
    smallSizeBtn->setID("list-normal-size-button");
    listDisplayMenu->addChild(smallSizeBtn);
    m_displayBtns.push_back(smallSizeBtn);

    auto bigSizeBtn = CCMenuItemSpriteExtra::create(
        GeodeSquareSprite::createWithSpriteFrameName("GJ_extendedIcon_001.png"),
        this, menu_selector(ModsLayer::onDisplay)
    );
    bigSizeBtn->setTag(static_cast<int>(ModListDisplay::BigList));
    bigSizeBtn->setID("list-size-button");
    listDisplayMenu->addChild(bigSizeBtn);
    m_displayBtns.push_back(bigSizeBtn);

    auto gridBtn = CCMenuItemSpriteExtra::create(
        GeodeSquareSprite::createWithSpriteFrameName("grid-view.png"_spr),
        this, menu_selector(ModsLayer::onDisplay)
    );
    gridBtn->setTag(static_cast<int>(ModListDisplay::Grid));
    gridBtn->setID("list-size-button");
    listDisplayMenu->addChild(gridBtn);
    m_displayBtns.push_back(gridBtn);

    // auto searchBtn = CCMenuItemSpriteExtra::create(
    //     GeodeSquareSprite::createWithSpriteFrameName("search.png"_spr, &m_showSearch),
    //     this, menu_selector(ModsLayer::onSearch)
    // );
    // searchBtn->setID("search-button");
    // listDisplayMenu->addChild(searchBtn);

    listDisplayMenu->setLayout(
        SimpleColumnLayout::create()
            ->setMainAxisDirection(AxisDirection::TopToBottom)
            ->setMainAxisScaling(AxisScaling::Scale)
            ->setGap(2.f)
    );
    m_frame->addChildAtPosition(listDisplayMenu, Anchor::Left, ccp(-5, 25));

    m_statusNode = ModsStatusNode::create();
    m_statusNode->setZOrder(4);
    m_frame->addChildAtPosition(m_statusNode, Anchor::Bottom);

    m_pageMenu = CCMenu::create();
    m_pageMenu->setID("page-menu");
    m_pageMenu->setContentSize({200.f, 16.f});
    m_pageMenu->setAnchorPoint({ 1.f, 1.f });
    m_pageMenu->setScale(.65f);

    m_pageLabel = CCLabelBMFont::create("", "goldFont.fnt");
    m_pageLabel->setID("page-label");
    m_pageLabel->setAnchorPoint({ .5f, 1.f });
    m_pageMenu->addChild(m_pageLabel);

    m_goToPageBtn = CCMenuItemSpriteExtra::create(
        CCSprite::createWithSpriteFrameName("gj_navDotBtn_on_001.png"),
        this, menu_selector(ModsLayer::onGoToPage)
    );
    m_goToPageBtn->setID("go-to-page-button");
    m_pageMenu->addChild(m_goToPageBtn);

    m_pageMenu->setLayout(
        SimpleRowLayout::create()
            ->setMainAxisDirection(AxisDirection::RightToLeft)
            ->setMainAxisAlignment(MainAxisAlignment::Start)
            ->setCrossAxisAlignment(CrossAxisAlignment::End)
            ->setCrossAxisScaling(AxisScaling::ScaleDown)
            ->setGap(5.f)
    );
    this->addChildAtPosition(m_pageMenu, Anchor::TopRight, ccp(-5, -5), false);

    // Go to installed mods list
    this->gotoTab(InstalledModListSource::get(InstalledModListType::All));

    this->setKeypadEnabled(true);
    cocos::handleTouchPriority(this, true);

    this->updateState();

    // Listen for state changes
    m_updateStateListener.setFilter(UpdateModListStateFilter(UpdateWholeState()));
    m_updateStateListener.bind([this](UpdateModListStateEvent* event) {
        if (auto whole = std::get_if<UpdateWholeState>(&event->target)) {
            if (whole->searchByDeveloper) {
                auto src = ServerModListSource::get(ServerModListType::Download);
                src->getQueryMut()->developer = *whole->searchByDeveloper;
                this->gotoTab(src);

                m_showSearch = true;
                m_lists.at(src)->activateSearch(m_showSearch);
            }
        }
        this->updateState();
    });

    // add safe mode label
    if (isSafeMode) {
        auto* label = CCLabelBMFont::create("Safe Mode Enabled", "bigFont.fnt");
        label->setPosition(winSize.width, 0);
        label->setAnchorPoint(ccp(1, 0));
        label->setOpacity(128);
        label->setZOrder(999);
        label->setScale(0.55f);
        this->addChild(label);
        static int _ = [this] {
            auto* alert = FLAlertLayer::create(
                "Safe Mode Enabled",
                "Safe Mode has been enabled. This means no mods will be loaded to prevent crashes. Feel free to manage any problematic mods.",
                "OK"
            );
            alert->m_scene = this;
            alert->m_noElasticity = true;
            alert->show();
            return 0;
        }();
    }

    return true;
}

void ModsLayer::gotoTab(ModListSource* src) {
    // Update selected tab
    for (auto tab : m_tabs) {
        auto selected = tab->getUserData() == static_cast<void*>(src);
        static_cast<GeodeTabSprite*>(tab->getNormalImage())->select(selected);
        tab->setEnabled(!selected);
    }

    // Remove current list from UI (it's Ref'd so it stays in memory)
    if (m_currentSource) {
        m_lists.at(m_currentSource)->removeFromParent();
    }

    // Lazily create new list and add it to UI
    if (!m_lists.contains(src)) {
        auto list = ModList::create(src, m_frame->getContentSize() - ccp(30, 0));
        list->setPosition(m_frame->getContentSize() / 2);
        m_frame->addChild(list);
        m_lists.emplace(src, list);
    }
    // Add list to UI
    else {
        m_frame->addChild(m_lists.at(src));
    }

    // Update current source
    m_currentSource = src;

    // Update the state of the current list
    m_lists.at(m_currentSource)->updateDisplay(m_modListDisplay);
    m_lists.at(m_currentSource)->activateSearch(m_showSearch);
    m_lists.at(m_currentSource)->updateState();
}

void ModsLayer::keyDown(enumKeyCodes key) {
    auto list = m_lists.at(m_currentSource);

    switch(key) {
        case KEY_Left:
        case CONTROLLER_Left:
            if (m_currentSource->getPageCount() && list->getPage() > 0) {
                list->gotoPage(list->getPage() - 1);
            }
            break;
        case KEY_Right:
        case CONTROLLER_Right:
            if (m_currentSource->getPageCount() && list->getPage() < m_currentSource->getPageCount().value() - 1) {
                list->gotoPage(list->getPage() + 1);
            }
            break;
        default:
            CCLayer::keyDown(key);
    }
}

void ModsLayer::keyBackClicked() {
    this->onBack(nullptr);
}

void ModsLayer::setIDPopupClosed(SetIDPopup* popup, int num) {
    if (popup->getID() == "go-to-page"_spr) {
        if (num > 0) num -= 1;
        if (m_currentSource) {
            m_lists.at(m_currentSource)->gotoPage(num);
        }
    }
}

void ModsLayer::updateState() {
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

    // Update display button
    for (auto btn : m_displayBtns) {
        static_cast<GeodeSquareSprite*>(btn->getNormalImage())->setState(
            static_cast<ModListDisplay>(btn->getTag()) == m_modListDisplay
        );
    }
}

void ModsLayer::onTab(CCObject* sender) {
    this->gotoTab(static_cast<ModListSource*>(static_cast<CCNode*>(sender)->getUserData()));
}
void ModsLayer::onRefreshList(CCObject*) {
    if (m_currentSource) {
        m_lists.at(m_currentSource)->reloadPage();
    }
}
void ModsLayer::onBack(CCObject*) {
    // Tell every list that we are about to exit the layer.
    // This prevents any page from being cached when the
    // cache invalidation event fires.
    for (auto& list : m_lists) {
        list.second->setIsExiting(true);
    }
    CCDirector::get()->replaceScene(CCTransitionFade::create(.5f, MenuLayer::scene(false)));

    // To avoid memory overloading, clear caches after leaving the layer
    server::clearServerCaches(true);
    ModListSource::clearAllCaches();
}
void ModsLayer::onGoToPage(CCObject*) {
    auto popup = SetIDPopup::create(m_lists.at(m_currentSource)->getPage() + 1, 1, m_currentSource->getPageCount().value(), "Go to Page", "Go", true, 1, 60.f, false, false);
    popup->m_delegate = this;
    popup->setID("go-to-page"_spr);
    popup->show();
}
void ModsLayer::onDisplay(CCObject* sender) {
    m_modListDisplay = static_cast<ModListDisplay>(sender->getTag());
    Mod::get()->setSavedValue("mod-list-display-type", m_modListDisplay);

    // Make sure to avoid a crash
    if (m_currentSource) {
        if (m_lists.at(m_currentSource)->getDisplay() == m_modListDisplay) {
            // No need to do stuff
            return;
        }
        m_lists.at(m_currentSource)->updateDisplay(m_modListDisplay);

        // Hi, Flame here, I made all display types have the same page size
        // So there's no need for this now
        // If anything breaks, make sure to tell me I'm a dumbass
        // m_lists.at(m_currentSource)->reloadPage();
    }
    this->updateState();
}
void ModsLayer::onSearch(CCObject*) {
    m_showSearch = !m_showSearch;
    // Make sure to avoid a crash
    if (m_currentSource) {
        m_lists.at(m_currentSource)->activateSearch(m_showSearch);
    }
}
void ModsLayer::onTheme(CCObject*) {
    auto old = Mod::get()->getSettingValue<bool>("enable-geode-theme");
    createQuickPopup(
        "Switch Theme",
        fmt::format(
            "Do you want to switch the <cp>color scheme</c> of the Geode menu "
            "to {}?",
            (old ? "<cy>GD-style colors</c>" : "<ca>Geode-style colors</c>")
        ),
        "Cancel", "Switch",
        [old](auto*, bool btn2) {
            if (btn2) {
                Mod::get()->setSettingValue("enable-geode-theme", !old);
                // todo: the touch priority on the new scene is screwed up and i can't figure out how to fix it
                Loader::get()->queueInMainThread([] {
                    ModsLayer::scene();
                });
            }
        }
    );
}
void ModsLayer::onSettings(CCObject*) {
    openSettingsPopup(Mod::get(), false);
}

ModsLayer* ModsLayer::create() {
    auto ret = new ModsLayer();
    if (ret->init()) {
        ret->autorelease();
        return ret;
    }
    delete ret;
    return nullptr;
}

ModsLayer* ModsLayer::scene() {
    auto scene = CCScene::create();
    auto layer = ModsLayer::create();
    scene->addChild(layer);
    CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(.5f, scene));
    return layer;
}

server::ServerRequest<std::vector<std::string>> ModsLayer::checkInstalledModsForUpdates() {
    return server::checkAllUpdates().map([](auto* result) -> Result<std::vector<std::string>, server::ServerError> {
        if (result->isOk()) {
            std::vector<std::string> updatesFound;
            for (auto& update : result->unwrap()) {
                if (update.hasUpdateForInstalledMod()) {
                    updatesFound.push_back(update.id);
                }
            }
            return Ok(updatesFound);
        }
        return Err(result->unwrapErr());
    });
}
