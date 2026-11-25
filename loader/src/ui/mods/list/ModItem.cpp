#include "ModItem.hpp"

#include "../ModsLayer.hpp"

#include <asp/iter.hpp>
#include <Geode/binding/ButtonSprite.hpp>
#include <Geode/loader/Event.hpp>
#include <Geode/loader/Loader.hpp>
#include <Geode/ui/GeodeUI.hpp>
#include <Geode/ui/Label.hpp>
#include <Geode/ui/MDPopup.hpp>
#include <Geode/ui/NineSlice.hpp>
#include <Geode/ui/SimpleAxisLayout.hpp>
#include <Geode/utils/ColorProvider.hpp>
#include <optional>
#include <server/DownloadManager.hpp>
#include <ui/GeodeStyle.hpp>
#include <ui/mods/popups/DevPopup.hpp>
#include <ui/mods/popups/ModPopup.hpp>
#include <ui/mods/sources/ModListSource.hpp>
#include <ui/mods/sources/ModSource.hpp>
#include <vector>
#include <ranges>

using namespace geode::prelude;

static constexpr auto MODTOBER_WINNERS = std::to_array<std::pair<std::string_view, std::optional<std::string_view>>>({
    {"24", "rainixgd.geome3dash"},
    {"25", "alphalaneous.kablammo"},
});

bool ModItem::init(ModSource&& source, bool listItem) {
    if (!ModListItem::init()) return false;

    m_source = std::move(source);
    this->setID("ModItem");

    m_logo = m_source.createModLogo();
    m_logo->setID("logo-sprite");
    this->addChild(m_logo);

    m_infoContainer = CCNode::create();
    m_infoContainer->setID("info-container");
    m_infoContainer->setScale(.4f);
    m_infoContainer->setAnchorPoint({0.0f, .5f});

    m_titleContainer = CCNode::create();
    m_titleContainer->setID("title-container");
    m_titleContainer->setAnchorPoint({0.0f, .5f});

    StringBuffer title;
    title.append("{:.40}", m_source.getMetadata().getName());
    if (m_source.getMetadata().getName().size() > 40) {
        title.append("...");
    }

    m_titleLabel = Label::create(title.str(), "bigFont.fnt");
    m_titleLabel->setID("title-label");
    m_titleContainer->addChild(m_titleLabel);

    m_versionLabel = Label::create("", "bigFont.fnt");
    m_versionLabel->setID("version-label");
    m_versionLabel->setScale(.7f);
    m_versionLabel->setLayoutOptions(
        SimpleAxisLayoutOptions::create()
            ->setMinRelativeScale(.7f)
            ->setMaxRelativeScale(1.0f)
        );
    m_titleContainer->addChild(m_versionLabel);

    m_versionDownloadSeparator = Label::create("•", "bigFont.fnt");
    m_versionDownloadSeparator->setOpacity(155);
    m_titleContainer->addChild(m_versionDownloadSeparator);

    m_titleContainer->setLayout(
        SimpleRowLayout::create()
            ->setMainAxisAlignment(MainAxisAlignment::Start)
            ->setMainAxisScaling(AxisScaling::Scale)
            ->setCrossAxisScaling(AxisScaling::ScaleDownGaps)
            ->setGap(5.0f)
    );
    m_infoContainer->addChildAtPosition(m_titleContainer, Anchor::Left);

    m_developers = CCNode::create();
    m_developers->setID("developers-menu");
    m_developers->ignoreAnchorPointForPosition(false);
    m_developers->setAnchorPoint({0.0f, .5f});

    auto by = m_source.formatDevelopers();
    m_developersBtn = Button::createWithLabel(by, "goldFont.fnt", [this](auto) {
        DevListPopup::create(m_source)->show();
    });
    m_developersBtn->setID("developers-button");
    m_developersBtn->setScaleMultiplier(1.125f);
    m_developers->addChild(m_developersBtn);

    m_developers->setLayout(
        SimpleRowLayout::create()
            ->setMainAxisAlignment(MainAxisAlignment::Start)
            ->setGap(5.0f)
    );
    m_infoContainer->addChildAtPosition(m_developers, Anchor::Left);

    m_description = NineSlice::create("square02b_001.png");
    m_description->setScaleMultiplier(.5f);
    m_description->setContentSize({450.0f, 30.0f});
    m_description->setColor(ccBLACK);
    m_description->setOpacity(90);

    auto desc = m_source.getMetadata().getDescription();
    auto descLabel = Label::create(
        desc.value_or("[No Description Provided]"),
        "chatFont.fnt"
    );
    descLabel->setColor(desc ? ccWHITE : ccGRAY);
    descLabel->setLimitLabelWidth(m_description->getContentWidth() - 20.0f, 1.5f, .1f);
    m_description->addChildAtPosition(descLabel, Anchor::Left, {10.0f, 0.0f}, {0.0f, .5f});

    m_infoContainer->addChildAtPosition(m_description, Anchor::Left);

    m_modLabelsContainer = CCNode::create();
    m_modLabelsContainer->setID("labels-container");
    m_modLabelsContainer->setLayout(
        SimpleRowLayout::create()
            ->setMainAxisAlignment(MainAxisAlignment::Start)
            ->setMainAxisScaling(AxisScaling::Grow)
            ->setCrossAxisScaling(AxisScaling::Grow)
            ->setCrossAxisAlignment(CrossAxisAlignment::End)
            ->setGap(5.0f)
    );
    m_infoContainer->addChildAtPosition(m_modLabelsContainer, Anchor::Left);

    m_restartRequiredLabel = createTagLabel(
        "Restart Required",
        {
            to3B(ColorProvider::get()->color("mod-list-restart-required-label"_spr)),
            to3B(ColorProvider::get()->color("mod-list-restart-required-label-bg"_spr)),
        }
    );
    m_restartRequiredLabel->setID("restart-required-label");
    m_restartRequiredLabel->setScale(.75f);
    m_modLabelsContainer->addChild(m_restartRequiredLabel);

    m_outdatedLabel = createTagLabel(
        "Outdated",
        {
            to3B(ColorProvider::get()->color("mod-list-outdated-label"_spr)),
            to3B(ColorProvider::get()->color("mod-list-outdated-label-bg"_spr)),
        }
    );
    m_outdatedLabel->setID("outdated-label");
    m_outdatedLabel->setScale(.75f);
    m_modLabelsContainer->addChild(m_outdatedLabel);

    m_deprecatedLabel = createTagLabel(
        "Deprecated",
        {
            to3B(ColorProvider::get()->color("mod-list-deprecated-label"_spr)),
            to3B(ColorProvider::get()->color("mod-list-deprecated-label-bg"_spr)),
        }
    );
    m_deprecatedLabel->setID("deprecated-label");
    m_deprecatedLabel->setScale(.75f);
    m_modLabelsContainer->addChild(m_deprecatedLabel);

    m_downloadBarContainer = CCNode::create();
    m_downloadBarContainer->setID("download-bar-container");
    m_downloadBarContainer->setContentSize({320.0f, 30.0f});

    m_downloadBar = ProgressBar::create(ProgressBar::Style::Slider);
    m_downloadBar->setID("download-bar");
    m_downloadBar->setScale(1.5f);
    m_downloadBarContainer->addChildAtPosition(m_downloadBar, Anchor::Center);

    m_infoContainer->addChildAtPosition(m_downloadBarContainer, Anchor::Left);

    m_downloadWaiting = CCNode::create();
    m_downloadWaiting->setID("download-waiting-container");
    m_downloadWaiting->setContentSize({225.0f, 30.0f});

    auto downloadWaitingLabel = Label::create("Preparing Download...", "bigFont.fnt");
    downloadWaitingLabel->setScale(.75f);
    downloadWaitingLabel->setID("download-waiting-label");
    m_downloadWaiting->addChildAtPosition(
        downloadWaitingLabel, Anchor::Left,
        {m_downloadWaiting->getContentHeight(), 0.0f}, {0.0f, .5f}
    );

    auto downloadWaitingSpinner = createLoadingCircle(20);
    m_downloadWaiting->addChildAtPosition(
        downloadWaitingSpinner, Anchor::Left,
        {m_downloadWaiting->getContentHeight() / 2.0f, 0.0f}
    );

    m_infoContainer->addChildAtPosition(m_downloadWaiting, Anchor::Left);

    this->addChildAtPosition(m_infoContainer, Anchor::Left);

    ButtonSprite* spr = nullptr;
    if (auto serverMod = m_source.asServer()) {
        auto& version = serverMod->latestVersion();

        auto geodeValid = Loader::get()->isModVersionSupported(version.getGeodeVersion());
        auto gameVersion = version.getGameVersion();
        auto gdValid = !gameVersion || gameVersion == "*" || gameVersion == GEODE_STR(GEODE_GD_VERSION);

        if (!geodeValid || !gdValid) {
            spr = createGeodeButton("N/A", 50, false, true, GeodeButtonSprite::Gray);
        }
    }

    if (!spr) {
        if (Loader::get()->isModInstalled(m_source.getID())) {
            spr = createGeodeButton("View", 50, false, true);
        }
        else {
            spr = createGeodeButton("Get", 50, false, true, GeodeButtonSprite::Install);
        }
    }

    auto viewBtn = Button::createWithNode(spr, [this](auto) {
        this->onView();
    });
    viewBtn->setID("view-button");
    m_viewMenu->addChild(viewBtn);

    m_viewMenu->updateLayout();

    m_badgeContainer = CCNode::create();
    m_badgeContainer->setID("badge-container");
    m_badgeContainer->setLayoutOptions(
        SimpleAxisLayoutOptions::create()
            ->setMinRelativeScale(.6f)
            ->setMaxRelativeScale(1.0f)
    );

    // Handle source-specific stuff
    m_source.visit(makeVisitor {
        [this, listItem](Mod* mod) {
            // Add an enable button if the mod is enablable
            if (!mod->isInternal()) {
                m_enableToggle = Toggler::createWithStandardSprites([this](Toggler* sender, bool toggled) {
                    sender->setToggled(!toggled);
                    m_source.requestEnable();
                });
                m_enableToggle->setID("enable-toggler");
                m_enableToggle->setScale(.9f);

                m_viewMenu->addChild(m_enableToggle);
                m_viewMenu->updateLayout();
            }

            // Add a pin button if the mod is in a list and enablable
            if (listItem && !mod->isInternal()) {
                auto pinOff = CCSprite::createWithSpriteFrameName("pin.png"_spr);
                pinOff->setOpacity(105);
                auto pinOn = CCSprite::createWithSpriteFrameName("pin.png"_spr);

                if (isGeodeTheme()) {
                    pinOn->setColor({245, 174, 125});
                    pinOff->setColor({220, 190, 230});
                } else {
                    // someone with better colour decisions can pick colours for gd
                    pinOn->setColor({245, 174, 125});
                    pinOff->setColor({220, 190, 230});
                }

                m_pinToggle = Toggler::create(pinOff, pinOn, [this](auto, auto) {
                    if (auto mod = m_source.asMod()) {
                        mod->setPinned(!mod->isPinned());
                    }
                    if (auto list = CCScene::get()->getChildByType<ModsLayer*>(0)) {
                        list->refreshList();
                    }
                });
                m_pinToggle->setID("pin-toggler");
                m_pinToggle->setScale(.75f);
                m_pinToggle->setLayoutOptions(
                    SimpleAxisLayoutOptions::create()
                        ->setMaxRelativeScale(1.0f)
                );

                m_viewMenu->addChild(m_pinToggle);
                m_viewMenu->updateLayout();
            }

            if (mod->getLoadProblem() || m_source.hasUpdates().deprecation.has_value()) {
                auto viewErrorBtn = Button::createWithNode(
                    CircleButtonSprite::create(
                        CCSprite::createWithSpriteFrameName("exclamation.png"_spr),
                        CircleBaseColor::Red,
                        CircleBaseSize::Small
                    ),
                    [this](auto) {
                        this->onViewError();
                    }
                );
                viewErrorBtn->setID("view-error-button");
                m_viewMenu->addChild(viewErrorBtn);
            }

            m_settingNodeHandle = SettingNodeValueChangeEvent().listen([this](std::string_view modID, std::string_view key, SettingNodeV3*, bool isCommit) {
                if (!isCommit) {
                    return ListenerResult::Propagate;
                }
                this->updateState();
                return ListenerResult::Propagate;
            });

            auto updatedAt = server::ModDownloadManager::get()->getRecentlyUpdatedInfo(mod->getID());
            if (updatedAt) {
                m_updatedAtContainer = CCNode::create();

                auto installedLabel = Label::create(
                    utils::timeToAgoString(updatedAt->updateTime, true),
                    "bigFont.fnt"
                );
                installedLabel->setID("installed-ago-label");
                installedLabel->setLimitLabelWidth(125, 1.0f, .1f);
                m_updatedAtContainer->addChildAtPosition(installedLabel, Anchor::Right, {-0.0f, 0.0f}, {1.0f, .5f});

                auto installedIcon = CCSprite::createWithSpriteFrameName("GJ_timeIcon_001.png");
                installedIcon->setID("installed-ago-sprite");
                installedIcon->setScale(1.2f);
                m_updatedAtContainer->addChildAtPosition(installedIcon, Anchor::Left, {8.0f, 0.0f});

                // m_updatedAtContainer scale is controlled in updateState
                m_updatedAtContainer->setContentSize({
                    installedLabel->getScaledContentWidth() + installedIcon->getScaledContentWidth(),
                    30
                });
                m_updatedAtContainer->updateLayout();
                m_updatedAtContainer->setLayoutOptions(
                    SimpleAxisLayoutOptions::create()
                        ->setMinRelativeScale(.1f)
                        ->setMaxRelativeScale(1.0f)
                );
            }
        },
        [this](server::ServerModMetadata const& metadata) {
            // todo: there has to be a better way to deal with the short/long alternatives
            if (metadata.featured) {
                m_badgeContainer->addChild(CCSprite::createWithSpriteFrameName("tag-featured.png"_spr));
            }

            if (metadata.tags.contains("paid")) {
                auto shortVer = CCSprite::createWithSpriteFrameName("tag-paid.png"_spr);
                shortVer->setTag(1);
                m_badgeContainer->addChild(shortVer);

                auto longVer = CCSprite::createWithSpriteFrameName("tag-paid-long.png"_spr);
                longVer->setTag(2);
                m_badgeContainer->addChild(longVer);
            }

            if (metadata.tags.contains("joke")) {
                m_badgeContainer->addChild(CCSprite::createWithSpriteFrameName("tag-joke.png"_spr));
            }

            if (metadata.tags.contains("api")) {
                m_badgeContainer->addChild(CCSprite::createWithSpriteFrameName("tag-api.png"_spr));
            }

            for (auto const& [year, winner] : MODTOBER_WINNERS) {
                if (metadata.tags.contains(fmt::format("modtober{}winner", year)) || (winner.has_value() && m_source.getID() == winner)) {
                    auto shortVer = CCSprite::createWithSpriteFrameName(fmt::format("tag-modtober{}-winner.png"_spr, year).c_str());
                    shortVer->setTag(1);
                    m_badgeContainer->addChild(shortVer);

                    auto longVer = CCSprite::createWithSpriteFrameName(fmt::format("tag-modtober{}-winner-long.png"_spr, year).c_str());
                    longVer->setTag(2);
                    m_badgeContainer->addChild(longVer);

                    break;
                }
                // Only show default Modtober tag if not a winner
                else if (metadata.tags.contains(fmt::format("modtober{}", year))) {
                    auto shortVer = CCSprite::createWithSpriteFrameName(fmt::format("tag-modtober{}.png"_spr, year).c_str());
                    m_badgeContainer->addChild(shortVer);

                    break;
                }
            }

            // Show mod download count here already so people can make informed decisions
            // on which mods to install
            m_downloadCountContainer = CCNode::create();

            auto downloads = Label::create(
                numToAbbreviatedString(metadata.downloadCount),
                "bigFont.fnt"
            );
            downloads->setID("downloads-label");
            downloads->setColor("mod-list-version-label"_cc3b);
            downloads->setLimitLabelWidth(125, 1.0f, .1f);
            m_downloadCountContainer->addChildAtPosition(downloads, Anchor::Right, {-0.0f, 0.0f}, {1.0f, .5f});

            auto downloadsIcon = CCSprite::createWithSpriteFrameName("GJ_downloadsIcon_001.png");
            downloadsIcon->setID("downloads-icon-sprite");
            downloadsIcon->setScale(1.2f);
            m_downloadCountContainer->addChildAtPosition(downloadsIcon, Anchor::Left, {8.0f, 0.0f});

            // m_downloadCountContainer scale is controlled in updateState
            m_downloadCountContainer->setContentSize({
                downloads->getScaledContentWidth() + downloadsIcon->getScaledContentWidth(),
                30
            });
            m_downloadCountContainer->updateLayout();
            m_downloadCountContainer->setLayoutOptions(
                SimpleAxisLayoutOptions::create()
                    ->setMinRelativeScale(.1f)
                    ->setMaxRelativeScale(1.0f)
            );

            // Check if mod is recommended by any others, only if not installed
            // todo: bring this back once we add "suggestions" field in mod
            // if (!Loader::get()->isModInstalled(metadata.id)) {
            //     std::vector<Mod*> recommends {};
            //     for (auto& recommend : Loader::get()->getRecommendations()) {
            //         auto suggestionID = recommend.message.substr(0, recommend.message.find(' '));
            //         if (suggestionID != metadata.id) {
            //             continue;
            //         }
            //         recommends.push_back(std::get<2>(recommend.cause));
            //     }

            //     if (recommends.size() > 0) {
            //         m_recommendedBy = CCNode::create();
            //         m_recommendedBy->setID("recommended-container");
            //         m_recommendedBy->setContentWidth(225);
            //         auto byLabel = Label::create("Recommended by ", "bigFont.fnt");
            //         byLabel->setID("recommended-label");
            //         byLabel->setColor("mod-list-recommended-by"_cc3b);
            //         m_recommendedBy->addChild(byLabel);

            //         std::string recommendStr = "";
            //         if (recommends.size() == 1) {
            //             recommendStr = recommends[0]->getName();
            //         } else {
            //             recommendStr = fmt::format("{} installed mods", recommends.size());
            //         }

            //         auto nameLabel = Label::create(recommendStr.c_str(), "bigFont.fnt");
            //         nameLabel->setID("recommended-name-label");
            //         nameLabel->setColor("mod-list-recommended-by-2"_cc3b);
            //         m_recommendedBy->addChild(nameLabel);

            //         m_recommendedBy->setLayout(
            //             RowLayout::create()
            //                 ->setDefaultScaleLimits(.1f, 1.0f)
            //                 ->setAxisAlignment(AxisAlignment::Start)
            //         );
            //         m_infoContainer->addChildAtPosition(m_recommendedBy, Anchor::Left);
            //     }
            // }
        }
    });

    m_updateBtn = Button::createWithNode(
        createGeodeCircleButton(
            CCSprite::createWithSpriteFrameName("update.png"_spr), 1.15f, CircleBaseSize::Medium, true
        ),
        [this](auto) {
            m_source.startInstall();
        }
    );
    m_updateBtn->setID("update-button");
    m_viewMenu->addChild(m_updateBtn);

    if (m_source.asMod()) {
        m_checkUpdateListener.spawn(
            "ModItem update check",
            m_source.checkUpdates(),
            [this](server::ServerResult<server::ServerModUpdateOneCheck>) {
                this->updateState();
            }
        );
    }

    this->updateState();

    // Only listen for updates on this mod specifically
    m_updateStateHandle = UpdateModListStateEvent().listen([this](UpdateState const& state) {
        this->updateState();
        return ListenerResult::Propagate;
    });

    m_downloadHandle = server::ModDownloadEvent(std::move(m_source.getID())).listen([this]() {
        this->updateState();
        return ListenerResult::Propagate;
    });

    return true;
}

void ModItem::updateState() {
    ModListItem::updateState();

    auto wantsRestart = m_source.wantsRestart();
    auto download = server::ModDownloadManager::get()->getDownload(m_source.getID());
    bool isDownloading = download && download->isActive();

    // On Grid layout the title is a direct child of info so it can be positioned
    // more cleanly, while m_titleContainer is just used to position the version
    // and downloads next to each other
    m_titleLabel->removeFromParent();
    if (m_display == ModListDisplay::Grid) {
        m_infoContainer->addChildAtPosition(m_titleLabel, Anchor::Top);
    }
    else {
        m_titleContainer->insertBefore(m_titleLabel, nullptr);
    }

    // Show download separator if there is something to separate and we're in grid view
    m_versionDownloadSeparator->setVisible(false);

    // Download counts go next to the version like on the website on grid view
    if (m_downloadCountContainer) {
        m_downloadCountContainer->setScale(.6f);
        m_downloadCountContainer->removeFromParent();
        if (m_display == ModListDisplay::Grid) {
            m_versionDownloadSeparator->setVisible(true);
            m_titleContainer->insertAfter(m_downloadCountContainer, m_versionDownloadSeparator);
        }
        else {
            m_viewMenu->addChild(m_downloadCountContainer);
        }
    }

    // Show the "Updated at" label if the installed mods list is being sorted
    // by "Recently installed" (to let people know when they've installed or
    // updated the mod)
    // Hide the enable and pin toggles to make space for install times :3
    // (Pinning doesn't make sense for that sorting anyway)
    if (m_enableToggle) m_enableToggle->setVisible(true);
    if (m_pinToggle) m_pinToggle->setVisible(true);
    if (m_updatedAtContainer) {
        auto listSource = typeinfo_cast<InstalledModListSource*>(m_source.getListSource());
        m_updatedAtContainer->removeFromParent();
        if (listSource && listSource->getSort() == static_cast<size_t>(InstalledModListSort::RecentlyUpdated)) {
            // Hide these
            if (m_enableToggle) m_enableToggle->setVisible(false);
            if (m_pinToggle) m_pinToggle->setVisible(false);

            m_updatedAtContainer->setScale(.6f);
            if (m_display == ModListDisplay::Grid) {
                m_versionDownloadSeparator->setVisible(true);
                m_titleContainer->insertAfter(m_updatedAtContainer, m_versionDownloadSeparator);
            }
            else {
                m_viewMenu->addChild(m_updatedAtContainer);
            }
        }
    }

    // Move badges to either be next to the title or in the top left corner in grid view
    if (m_badgeContainer) {
        m_badgeContainer->removeFromParent();
        if (m_display == ModListDisplay::Grid) {
            m_badgeContainer->setScale(.35f);
            m_badgeContainer->setContentWidth(30.0f);
            m_badgeContainer->setLayout(
                SimpleColumnLayout::create()
                    ->setMainAxisAlignment(MainAxisAlignment::Start)
                    ->setMainAxisDirection(AxisDirection::TopToBottom)
                    ->setMainAxisScaling(AxisScaling::Fit)
                    ->setCrossAxisScaling(AxisScaling::Scale)
                    ->setGap(5.0f)
            );
            this->addChildAtPosition(m_badgeContainer, Anchor::TopLeft, {4.0f, -4.0f}, {0.0f, 1.0f});
        }
        else {
            m_badgeContainer->setContentHeight(30.0f);
            m_badgeContainer->setScale(1.0f);
            m_badgeContainer->setLayout(
                SimpleRowLayout::create()
                    ->setMainAxisAlignment(MainAxisAlignment::Start)
                    ->setMainAxisScaling(AxisScaling::Fit)
                    ->setCrossAxisScaling(AxisScaling::Scale)
                    ->setGap(5.0f)
            );
            m_titleContainer->addChild(m_badgeContainer);
        }
        // Long tags don't fit in the grid UI
        for (auto child : CCArrayExt<CCNode*>(m_badgeContainer->getChildren())) {
            if (child->getTag() > 0) {
                child->setVisible(child->getTag() == (m_display == ModListDisplay::Grid ? 1 : 2));
            }
        }
        m_badgeContainer->updateLayout();
    }

    auto const size = this->getContentSize();

    // On Grid View logo has constant size
    if (m_display == ModListDisplay::Grid) {
        limitNodeSize(m_logo, {30.0f, 30.0f}, 999.0f, .1f);
        m_logo->setPosition({size.width / 2.0f, size.height - 20.0f});
    }
    else {
        auto logoSize = size.height - 7.5f;
        limitNodeSize(m_logo, {logoSize, logoSize}, 999.0f, .1f);
        m_logo->setPosition({size.height / 2.0f + 5.0f, size.height / 2});
    }

    // There's space to show the description only on the big list
    // When we do, elements like the download progress bar should replace it
    // over the developer name since it's less important
    // Couldn't figure out a more concise name
    m_description->setVisible(m_display == ModListDisplay::BigList);
    m_developers->setVisible(true);
    auto elementToReplaceWithOtherAbnormalElement =
        m_display == ModListDisplay::BigList ? m_description : m_developers;

    auto titleSpace = m_display == ModListDisplay::Grid ?
        CCSize{size.width - 10.0f, 35.0f} :
        CCSize{size.width - 145.0f, size.height - 5.0f};

    // Divide by scale of info container since that actually determines the size
    // (Since the scale of m_titleContainer and m_developers is managed by its layout)

    // If there is an active download ongoing, show that in place of developer name
    // (or description on big view)
    if (isDownloading) {
        m_updateBtn->setVisible(false);
        m_restartRequiredLabel->setVisible(false);
        elementToReplaceWithOtherAbnormalElement->setVisible(false);

        auto status = download->getStatus();
        if (auto prog = std::get_if<server::DownloadStatusDownloading>(&status)) {
            m_downloadWaiting->setVisible(false);
            m_downloadBarContainer->setVisible(true);
            m_downloadBar->updateProgress(prog->percentage);
        }
        else {
            m_downloadBarContainer->setVisible(false);
            m_downloadWaiting->setVisible(true);
            // Make sure the spinner is spinning by ticking its setVisible
            m_downloadWaiting->getChildByID("loading-spinner")->setVisible(true);
        }
    }
    // Otherwise show "Restart Required" button if needed in place of dev name
    else {
        m_restartRequiredLabel->setVisible(wantsRestart);
        elementToReplaceWithOtherAbnormalElement->setVisible(!wantsRestart);
        m_downloadBarContainer->setVisible(false);
        m_downloadWaiting->setVisible(false);
    }

    struct CellStyle {
        ccColor3B color;
        GLubyte opacity;
    };

    // Set default colors based on source to start off with
    // (possibly overriding later based on state)
    m_source.visit(
        makeVisitor{
            [this](Mod* mod) {
                if (isGeodeTheme()) {
                    m_bg->setColor(ccWHITE);
                    m_bg->setOpacity(mod->isOrWillBeEnabled() ? 25 : 10);
                }
                else {
                    m_bg->setColor(ccBLACK);
                    m_bg->setOpacity(mod->isOrWillBeEnabled() ? 90 : 60);
                }
                m_titleLabel->setOpacity(mod->isOrWillBeEnabled() ? 255 : 155);
                m_versionLabel->setOpacity(mod->isOrWillBeEnabled() ? 255 : 155);
                m_developersBtn->setOpacity(mod->isOrWillBeEnabled() ? 255 : 155);
            },
            [this](server::ServerModMetadata const& metadata) {
                auto geode = isGeodeTheme();
                auto has = [&metadata](ZStringView tag) {
                    return metadata.tags.contains(tag);
                };

                auto const defaultStyle = [geode]() {
                    return geode ? CellStyle{ccWHITE, 25} : CellStyle{ccBLACK, 90};
                }();

                auto const style = [this, &defaultStyle, &metadata, &has, geode]() -> CellStyle {
                    if (geode && metadata.featured) return {"mod-list-featured-color"_cc3b, 65};

                    for (auto const& year : MODTOBER_WINNERS | std::views::keys) {
                        auto const win = fmt::format("modtober{}winner", year);
                        if (has(win)) return {this->getModtoberStyle(year, true).value_or(defaultStyle.color), 85};

                        auto const entry = fmt::format("modtober{}", year);
                        if (has(entry)) return {this->getModtoberStyle(year).value_or(defaultStyle.color), 85};
                    }

                    if (has("paid")) return {"mod-list-paid-color"_cc3b, 55};

                    return defaultStyle;
                }();

                m_bg->setColor(style.color);
                m_bg->setOpacity(style.opacity);
            }
        }
    );

    m_deprecatedLabel->setVisible(false);
    m_updateBtn->setVisible(false);

    auto update = m_source.hasUpdates();
    if ((update.update || update.deprecation) && !(download && (download->isActive() || download->isDone()))) {
        if (update.update) {
            m_updateBtn->setVisible(true);

            m_versionLabel->setText(fmt::format("{} -> {}", m_source.getMetadata().getVersion().toVString(), update.update->version.toVString()));
            m_versionLabel->setColor(to3B(ColorProvider::get()->color("mod-list-version-label-updates-available"_spr)));

            m_bg->setColor(to3B(ColorProvider::get()->color("mod-list-version-bg-updates-available"_spr)));
            m_bg->setOpacity(isGeodeTheme() ? 25 : 90);
        }
        else {
            m_deprecatedLabel->setVisible(true);
            elementToReplaceWithOtherAbnormalElement->setVisible(false);
            m_bg->setColor(to3B(ColorProvider::get()->color("mod-list-version-bg-deprecated"_spr)));
            m_bg->setOpacity(isGeodeTheme() ? 25 : 90);
        }
    }
    else {
        m_versionLabel->setText(m_source.getMetadata().getVersion().toVString());
        m_versionLabel->setColor(to3B(ColorProvider::get()->color("mod-list-version-label"_spr)));
    }

    // Hide by default
    m_outdatedLabel->setVisible(false);

    // If there were problems, tint the BG red
    if (m_source.asMod()) {
        std::optional<LoadProblem> targetsOutdated = m_source.asMod()->targetsOutdatedVersion();
        if (m_source.asMod()->failedToLoad()) {
            m_bg->setColor("mod-list-errors-found"_cc3b);
            m_bg->setOpacity(isGeodeTheme() ? 25 : 90);
        }
        // Deprecation takes precedence over "Outdated" (since you need to be
        // able to update a deprecated outdated mod)
        if (!wantsRestart && targetsOutdated && !isDownloading && !update.deprecation) {
            m_bg->setColor("mod-list-outdated-label"_cc3b);
            m_bg->setOpacity(isGeodeTheme() ? 25 : 90);
            m_outdatedLabel->setVisible(true);
            elementToReplaceWithOtherAbnormalElement->setVisible(false);
            if (m_display == ModListDisplay::Grid) {
                if (!m_source.getMetadata().checkPlatformSupported()) {
                    m_outdatedLabel->setString("Unsupported");
                } else {
                    m_outdatedLabel->setString("Outdated");
                }
            }
            else {
                if (!m_source.getMetadata().checkPlatformSupported()) {
                    m_outdatedLabel->setString("Unsupported Platform");
                }
                else if (!m_source.getMetadata().checkGameVersion()) {
                    m_outdatedLabel->setString(fmt::format("Outdated (GD {})", *m_source.getMetadata().getGameVersion()).c_str());
                }
                else {
                    m_outdatedLabel->setString(fmt::format("Outdated (Geode {})", m_source.getMetadata().getGeodeVersion().toNonVString()).c_str());
                }
            }
        }
    }

    // Update size and direction of title
    // On grid view, m_titleContainer contains the version and download count
    // but not the actual title lol
    m_titleLabel->setScale(1.0f);
    m_titleContainer->setContentHeight(30.0f);
    m_titleContainer->setContentWidth(titleSpace.width / m_infoContainer->getScale());
    if (m_display == ModListDisplay::Grid) {
        static_cast<SimpleRowLayout*>(m_titleContainer->getLayout())
            ->setGap(10)
            ->setMainAxisAlignment(MainAxisAlignment::Center);
        static_cast<SimpleRowLayout*>(m_developers->getLayout())
            ->setMainAxisAlignment(MainAxisAlignment::Center);
    }
    else {
        static_cast<SimpleRowLayout*>(m_titleContainer->getLayout())
            ->setGap(5)
            ->setMainAxisAlignment(MainAxisAlignment::Start);
        static_cast<SimpleRowLayout*>(m_developers->getLayout())
            ->setMainAxisAlignment(MainAxisAlignment::Start);
    }
    m_developers->setContentWidth(titleSpace.width / m_infoContainer->getScale());
    m_developers->setContentHeight(30.0f);
    m_developers->updateLayout();

    if (m_recommendedBy) {
        m_recommendedBy->setContentWidth(titleSpace.width / m_infoContainer->getScale());
        m_recommendedBy->updateLayout();
    }

    limitNodeWidth(m_downloadWaiting, m_titleContainer->getContentWidth(), 1.0f, .1f);
    limitNodeWidth(m_downloadBarContainer, m_titleContainer->getContentWidth(), 1.0f, .1f);

    // Update positioning (jesus)
    switch (m_display) {
        case ModListDisplay::Grid: {
            m_infoContainer->updateAnchoredPosition(Anchor::Center, {0.0f, -5.0f}, {.5f, .5f});
            // m_description is hidden
            m_titleLabel->setLayoutOptions(AnchorLayoutOptions::create()
                ->setAnchor(Anchor::Top)
                ->setOffset({0.0f, -10.0f})
            );
            limitNodeWidth(m_titleLabel, m_titleContainer->getContentWidth(), .8f, .1f);
            m_titleContainer->updateAnchoredPosition(Anchor::Center, { 0.0f, 0.0f }, {.5f, .5f});
            m_developers->updateAnchoredPosition(Anchor::Bottom, {0.0f, 10.0f}, {.5f, .5f});
            m_modLabelsContainer->updateAnchoredPosition(Anchor::Bottom, {0.0f, 10.0f}, {.5f, .5f});
            m_downloadBarContainer->updateAnchoredPosition(Anchor::Bottom, {0.0f, 10.0f}, {.5f, .5f});
            m_downloadWaiting->updateAnchoredPosition(Anchor::Bottom, {0.0f, 10.0f}, {.5f, .5f});

            if (m_recommendedBy) {
                m_recommendedBy->updateAnchoredPosition(Anchor::Bottom, {0.0f, 10.0f}, {.5f, .5f});
            }
        } break;

        default:
        case ModListDisplay::SmallList: {
            m_infoContainer->updateAnchoredPosition(Anchor::Left, {size.height + 10.0f, 0.0f}, {0.0f, .5f});
            m_titleContainer->updateAnchoredPosition(Anchor::TopLeft, {0.0f, 2.0f}, {0.0f, 1.0f});
            m_titleLabel->setLayoutOptions(
                SimpleAxisLayoutOptions::create()
                    ->setMinRelativeScale(.5f)
                    ->setMaxRelativeScale(1.0f)
                );
            // m_description is hidden
            m_developers->updateAnchoredPosition(Anchor::BottomLeft, {0.0f, 3.0f}, { 0.0f, 0.0f });
            m_modLabelsContainer->updateAnchoredPosition(Anchor::BottomLeft, {0.0f, 3.0f}, { 0.0f, 0.0f });
            m_downloadBarContainer->updateAnchoredPosition(Anchor::BottomLeft, {0.0f, 3.0f}, { 0.0f, 0.0f });
            m_downloadWaiting->updateAnchoredPosition(Anchor::BottomLeft, {0.0f, 3.0f}, { 0.0f, 0.0f });

            if (m_recommendedBy) {
                m_recommendedBy->updateAnchoredPosition(Anchor::BottomLeft, {0.0f, 3.0f}, { 0.0f, 0.0f });
            }
        } break;

        case ModListDisplay::BigList: {
            m_infoContainer->updateAnchoredPosition(Anchor::Left, {size.height + 10.0f, 0.0f}, {0.0f, .5f});
            m_titleContainer->updateAnchoredPosition(Anchor::TopLeft, { 0.0f, 0.0f }, {0.0f, 1.0f});
            m_titleLabel->setLayoutOptions(
                SimpleAxisLayoutOptions::create()
                    ->setMinRelativeScale(.5f)
                    ->setMaxRelativeScale(1.0f)
                );
            m_developers->updateAnchoredPosition(Anchor::Left, { 0.0f, 0.0f }, {0.0f, .5f});

            m_description->updateAnchoredPosition(Anchor::BottomLeft, { 0.0f, 0.0f }, { 0.0f, 0.0f });
            m_modLabelsContainer->updateAnchoredPosition(Anchor::BottomLeft, {0.0f, 3.0f}, { 0.0f, 0.0f });
            m_downloadBarContainer->updateAnchoredPosition(Anchor::BottomLeft, { 0.0f, 0.0f }, { 0.0f, 0.0f });
            m_downloadWaiting->updateAnchoredPosition(Anchor::BottomLeft, { 0.0f, 0.0f }, { 0.0f, 0.0f });

            if (m_recommendedBy) {
                m_recommendedBy->updateAnchoredPosition(Anchor::BottomLeft, { 0.0f, 0.0f }, { 0.0f, 0.0f });
            }
        } break;
    }

    m_infoContainer->setContentSize(CCSize{titleSpace.width, titleSpace.height} / m_infoContainer->getScale());
    m_infoContainer->updateLayout();

    m_titleContainer->updateLayout();
    m_viewMenu->updateLayout();
    m_modLabelsContainer->updateLayout();

    // Highlight item via BG if it wants to restart for extra UI attention
    if (wantsRestart) {
        m_bg->setColor("mod-list-restart-required-label"_cc3b);
        m_bg->setOpacity(isGeodeTheme() ? 25 : 90);
    }

    // Update enable toggle state
    if (m_enableToggle && m_source.asMod()) {
        m_enableToggle->setToggled(m_source.asMod()->isOrWillBeEnabled());

        // Disable the toggle if the mod has been uninstalled or if the mod is
        // outdated
        if (modRequestedActionIsUninstall(m_source.asMod()->getRequestedAction()) ||
            m_source.asMod()->targetsOutdatedVersion()) {
            m_enableToggle->setEnabled(false);
            m_enableToggle->setColor(ccGRAY);
            m_enableToggle->setOpacity(105);
        }
    }

    if (m_pinToggle && m_source.asMod()) {
        m_pinToggle->setToggled(m_source.asMod()->isPinned());
    }

    this->updateLayout();

    ModItemUIEvent().send(this, m_source.getID(), std::nullopt);
}

std::optional<ccColor3B> ModItem::getModtoberStyle(std::string_view year, bool winner) {
    if (year == "24") return winner ? ccColor3B{104, 63, 138} : ccColor3B{63, 91, 138};
    if (year == "25") return winner ? ccColor3B{179, 107, 36} : ccColor3B{135, 103, 64};

    return std::nullopt;
}

void ModItem::onView() {
    // This is a local static and not a mod saved value because we might want
    // to periodically remind users that paid mods are paid
    static bool shownPaidNotif = false;
    if (m_source.asServer() && m_source.asServer()->tags.contains("paid") && !shownPaidNotif) {
        shownPaidNotif = true;
        return FLAlertLayer::create(
            nullptr,
            "Paid Content",
            "This mod contains <cg>Paid Content</c>. This means that some or all "
            "features of the mod <cj>require money to use</c>.\n\n"
            "<cy>Geode does not handle any payments. The mod handles all transactions "
            "in their own way.</c>\n\n"
            "<cp>The paid content may not be available in your country.</c>",
            "OK",
            nullptr,
            360.0f
        )->show();
    }

    // Show popups for invalid mods
    if (m_source.asServer()) {
        auto const& version = m_source.asServer()->latestVersion();

        auto gameVersion = version.getGameVersion();
        if (gameVersion == "0.000") {
            return FLAlertLayer::create(
                nullptr,
                "Invalid Platform",
                "This mod is <cr>not available</c> for your current platform.",
                "OK",
                nullptr,
                360.0f
            )->show();
        }

        if (gameVersion && gameVersion != "*" && gameVersion != GEODE_STR(GEODE_GD_VERSION)) {
            return FLAlertLayer::create(
                nullptr,
                "Unavailable",
                "This mod targets an <cr>unsupported version of Geometry Dash</c>.",
                "OK",
                nullptr,
                360.0f
            )->show();
        }

        if (!Loader::get()->isModVersionSupported(version.getGeodeVersion())) {
            return FLAlertLayer::create(
                nullptr,
                "Unavailable",
                "This mod targets an <cr>unsupported version of Geode</c>.",
                "OK",
                nullptr,
                360.0f
            )->show();
        }
    }

    // Always open up the popup for the installed mod page if that is possible
    ModPopup::create(m_source.convertForPopup())->show();
};

void ModItem::onViewError() {
    if (auto mod = m_source.asMod()) {
        // Deprecation gets special treatement
        // Note: this statement below should copy to and not move
        if (auto dep = m_source.hasUpdates().deprecation) {
            // The world if C++ had a nice way to map and join vectors rather
            // than having to do this bs
            // NOTE: the deprecation reason should describe why there
            // is no replacement, or if there will be one in the future
            StringBuffer byStr;
            if (!dep->by.empty()) {
                byStr.append("It is recommended to <cj>install these mods instead</c>:\n\n");

                for (auto const& id : dep->by) {
                    byStr.append("<mod:{}>\n\n", id);
                }
            }

            MDPopup::create(
                "Deprecated",
                fmt::format(
                    "<cy>{0}</c> has been <co>deprecated</c>, meaning that it "
                    "is no longer being developed. You may start running into "
                    "<co>bugs and crashes</c>, and the mod may not be "
                    "available "
                    "for future GD versions.\n\n"
                    "**Reason:** {1}\n\n"
                    "{2}\n\n"
                    "It is recommended to <cr>uninstall {0}</c>.",
                    mod->getName(),
                    dep->reason,
                    byStr
                ),
                "OK"
            )->show();
        }
        else if (auto problem = mod->getLoadProblem()) {
            std::string title;

            switch (problem->type) {
                default:

                case LoadProblem::Type::Unknown: title = "Unknown error"; break;
                case LoadProblem::Type::InvalidGeodeFile: title = "Invalid Geode File"; break;
                case LoadProblem::Type::MissingDependencies: title = "Missing dependencies"; break;
                case LoadProblem::Type::Outdated: title = "Outdated"; break;
                case LoadProblem::Type::HasIncompatibilities: title = "Incompatibilities"; break;
            }

            MDPopup::create(title, std::move(*problem).message, "OK")->show();
        }
    }
};

ModItem* ModItem::create(ModSource&& source, bool listItem) {
    auto ret = new ModItem();
    if (ret->init(std::move(source), listItem)) {
        ret->autorelease();
        return ret;
    }
    delete ret;
    return nullptr;
}

ModSource& ModItem::getSource() & {
    return m_source;
}

bool AnyModItem::init(ZStringView modID) {
    if (!ModListItem::init()) return false;

    m_bg->setColor(ccBLACK);
    m_bg->setOpacity(90);

    m_loading = LoadingSpinner::create(20);
    this->addChildAtPosition(m_loading, Anchor::Center);

    if (auto mod = Loader::get()->getInstalledMod(modID)) {
        this->gotSrc(ModSource(mod));
    }
    else {
        m_listener.spawn(
            server::getMod(modID),
            [this, id = std::string(modID)](server::ServerResult<server::ServerModMetadata> result) {
                if (result.isOk()) {
                    this->gotSrc(ModSource(std::move(result).unwrap()));
                }
                else {
                    auto err = std::move(result).unwrapErr();
                    log::error("Error fetching mod {}: {} (code {})", id, err.details, err.code);

                    m_bg->setColor(ccRED);
                    m_bg->setOpacity(90);
                    auto errorLabel = Label::create(std::move(err.details), "bigFont.fnt");
                    errorLabel->setAnchorPoint({0.0f, .5f});
                    errorLabel->setScale(.35f);
                    this->addChildAtPosition(errorLabel, Anchor::Left, {10.0f, 0.0f});

                    m_loading->setVisible(false);

                    this->updateState();
                }
            }
        );
    }

    this->updateState();

    return true;
}

void AnyModItem::gotSrc(ModSource&& src) {
    m_item = ModItem::create(std::move(src), false);
    m_item->updateDisplay(m_targetWidth, m_display);
    this->addChildAtPosition(m_item, Anchor::Center, { 0.0f, 0.0f }, {.5f, .5f});
    m_bg->setVisible(false);
    m_loading->setVisible(false);
}

AnyModItem* AnyModItem::create(ZStringView modID) {
    auto ret = new AnyModItem();
    if (ret->init(modID)) {
        ret->autorelease();
        return ret;
    }
    delete ret;
    return nullptr;
}

void AnyModItem::updateDisplay(float width, ModListDisplay display) {
    ModListItem::updateDisplay(width, display);
    if (m_item) {
        m_item->updateDisplay(width, display);
    }
}