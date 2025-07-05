#include "ModItem.hpp"

#include <optional>
#include <string>
#include <vector>

#include <Geode/ui/GeodeUI.hpp>
#include <Geode/ui/SimpleAxisLayout.hpp>
#include <Geode/utils/ColorProvider.hpp>
#include <Geode/binding/ButtonSprite.hpp>
#include <Geode/loader/Event.hpp>
#include <Geode/loader/Loader.hpp>
#include "server/DownloadManager.hpp"
#include "ui/mods/GeodeStyle.hpp"
#include "ui/mods/popups/ModPopup.hpp"
#include "ui/mods/popups/DevPopup.hpp"
#include "ui/mods/popups/ModErrorPopup.hpp"
#include "ui/mods/sources/ModSource.hpp"
#include "ui/GeodeUIEvent.hpp"

bool ModItem::init(ModSource&& source) {
    if (!CCNode::init())
        return false;

    m_source = std::move(source);
    this->setID("ModItem");

    m_bg = CCScale9Sprite::create("square02b_small.png");
    m_bg->setID("bg");
    m_bg->setOpacity(0);
    m_bg->ignoreAnchorPointForPosition(false);
    m_bg->setAnchorPoint({ .5f, .5f });
    m_bg->setScale(.7f);
    this->addChildAtPosition(m_bg, Anchor::Center);

    m_logo = m_source.createModLogo();
    m_logo->setID("logo-sprite");
    this->addChild(m_logo);

    m_infoContainer = CCNode::create();
    m_infoContainer->setID("info-container");
    m_infoContainer->setScale(.4f);
    m_infoContainer->setAnchorPoint({ .0f, .5f });

    m_titleContainer = CCNode::create();
    m_titleContainer->setID("title-container");
    m_titleContainer->setAnchorPoint({ .0f, .5f });

    m_titleLabel = CCLabelBMFont::create(m_source.getMetadata().getName().c_str(), "bigFont.fnt");
    m_titleLabel->setID("title-label");
    m_titleContainer->addChild(m_titleLabel);

    m_versionLabel = CCLabelBMFont::create("", "bigFont.fnt");
    m_versionLabel->setID("version-label");
    m_versionLabel->setScale(0.7f);
    m_versionLabel->setLayoutOptions(
        SimpleAxisLayoutOptions::create()
            ->setMinRelativeScale(.7f)
            ->setMaxRelativeScale(1.f)
        );
    m_titleContainer->addChild(m_versionLabel);

    m_versionDownloadSeparator = CCLabelBMFont::create("•", "bigFont.fnt");
    m_versionDownloadSeparator->setOpacity(155);
    m_titleContainer->addChild(m_versionDownloadSeparator);

    m_titleContainer->setLayout(
        SimpleRowLayout::create()
            ->setMainAxisAlignment(MainAxisAlignment::Start)
            ->setMainAxisScaling(AxisScaling::Scale)
            ->setCrossAxisScaling(AxisScaling::ScaleDownGaps)
            ->setGap(5.f)
    );
    m_titleContainer->getLayout()->ignoreInvisibleChildren(true);
    m_infoContainer->addChildAtPosition(m_titleContainer, Anchor::Left);

    m_developers = CCMenu::create();
    m_developers->setID("developers-menu");
    m_developers->ignoreAnchorPointForPosition(false);
    m_developers->setAnchorPoint({ .0f, .5f });

    auto by = m_source.formatDevelopers();
    m_developerLabel = CCLabelBMFont::create(by.c_str(), "goldFont.fnt");
    m_developerLabel->setID("developers-label");
    auto developersBtn = CCMenuItemSpriteExtra::create(
        m_developerLabel, this, menu_selector(ModItem::onDevelopers)
    );
    developersBtn->setID("developers-button");
    m_developers->addChild(developersBtn);

    m_developers->setLayout(
        SimpleRowLayout::create()
            ->setMainAxisAlignment(MainAxisAlignment::Start)
            ->setGap(5.f)
    );
    m_infoContainer->addChildAtPosition(m_developers, Anchor::Left);

    m_description = CCScale9Sprite::create("square02b_001.png");
    m_description->setScale(.5f);
    m_description->setContentSize(ccp(450, 30) / m_description->getScale());
    m_description->setColor(ccBLACK);
    m_description->setOpacity(90);

    auto desc = m_source.getMetadata().getDescription();
    auto descLabel = CCLabelBMFont::create(
        desc.value_or("[No Description Provided]").c_str(),
        "chatFont.fnt"
    );
    descLabel->setColor(desc ? ccWHITE : ccGRAY);
    limitNodeWidth(descLabel, m_description->getContentWidth() - 20, 2.f, .1f);
    m_description->addChildAtPosition(descLabel, Anchor::Left, ccp(10, 0), ccp(0, .5f));

    m_infoContainer->addChildAtPosition(m_description, Anchor::Left);

    m_restartRequiredLabel = createTagLabel(
        "Restart Required",
        {
            to3B(ColorProvider::get()->color("mod-list-restart-required-label"_spr)),
            to3B(ColorProvider::get()->color("mod-list-restart-required-label-bg"_spr))
        }
    );
    m_restartRequiredLabel->setID("restart-required-label");
    m_restartRequiredLabel->setScale(.75f);
    m_infoContainer->addChildAtPosition(m_restartRequiredLabel, Anchor::Left);

    m_outdatedLabel = createTagLabel(
        "Outdated",
        {
            to3B(ColorProvider::get()->color("mod-list-outdated-label"_spr)),
            to3B(ColorProvider::get()->color("mod-list-outdated-label-bg"_spr))
        }
    );
    m_outdatedLabel->setID("outdated-label");
    m_outdatedLabel->setScale(.75f);
    m_infoContainer->addChildAtPosition(m_outdatedLabel, Anchor::Left);

    m_downloadBarContainer = CCNode::create();
    m_downloadBarContainer->setID("download-bar-container");
    m_downloadBarContainer->setContentSize({ 320, 30 });

    m_downloadBar = Slider::create(nullptr, nullptr);
    m_downloadBar->setID("download-bar");
    m_downloadBar->m_touchLogic->m_thumb->setVisible(false);
    m_downloadBar->setScale(1.5f);
    m_downloadBarContainer->addChildAtPosition(m_downloadBar, Anchor::Center, ccp(0, 0), ccp(0, 0));

    m_infoContainer->addChildAtPosition(m_downloadBarContainer, Anchor::Left);

    m_downloadWaiting = CCNode::create();
    m_downloadWaiting->setID("download-waiting-container");
    m_downloadWaiting->setContentSize({ 225, 30 });

    auto downloadWaitingLabel = CCLabelBMFont::create("Preparing Download...", "bigFont.fnt");
    downloadWaitingLabel->setScale(.75f);
    downloadWaitingLabel->setID("download-waiting-label");
    m_downloadWaiting->addChildAtPosition(
        downloadWaitingLabel, Anchor::Left,
        ccp(m_downloadWaiting->getContentHeight(), 0), ccp(0, .5f)
    );

    auto downloadWaitingSpinner = createLoadingCircle(20);
    m_downloadWaiting->addChildAtPosition(
        downloadWaitingSpinner, Anchor::Left,
        ccp(m_downloadWaiting->getContentHeight() / 2, 0)
    );

    m_infoContainer->addChildAtPosition(m_downloadWaiting, Anchor::Left);

    this->addChildAtPosition(m_infoContainer, Anchor::Left);

    m_viewMenu = CCMenu::create();
    m_viewMenu->setID("view-menu");
    m_viewMenu->setScale(.55f);

    ButtonSprite* spr = nullptr;
    if (auto serverMod = m_source.asServer(); serverMod != nullptr) {
        auto version = serverMod->latestVersion();

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
        } else {
            spr = createGeodeButton("Get", 50, false, true, GeodeButtonSprite::Install);
        }
    }

    auto viewBtn = CCMenuItemSpriteExtra::create(spr, this, menu_selector(ModItem::onView));
    viewBtn->setID("view-button");
    m_viewMenu->addChild(viewBtn);

    m_viewMenu->setLayout(
        SimpleRowLayout::create()
            ->setMainAxisDirection(AxisDirection::RightToLeft)
            ->setMainAxisAlignment(MainAxisAlignment::Start)
            ->setMainAxisScaling(AxisScaling::Scale)
            ->setCrossAxisScaling(AxisScaling::Scale)
            ->setMinRelativeScale(1.f)
            ->setGap(10)
    );
    m_viewMenu->getLayout()->ignoreInvisibleChildren(true);
    this->addChildAtPosition(m_viewMenu, Anchor::Right, ccp(-10, 0));

    m_badgeContainer = CCNode::create();
    m_badgeContainer->setID("badge-container");
    m_badgeContainer->setLayoutOptions(
        SimpleAxisLayoutOptions::create()
            ->setMinRelativeScale(.6f)
            ->setMaxRelativeScale(1.f)
    );

    // Handle source-specific stuff
    m_source.visit(makeVisitor {
        [this](Mod* mod) {
            // Add an enable button if the mod is enablable
            if (!mod->isInternal()) {
                m_enableToggle = CCMenuItemToggler::createWithStandardSprites(
                    this, menu_selector(ModItem::onEnable), 1.f
                );
                m_enableToggle->setScale(0.9f);
                m_enableToggle->setLayoutOptions(
                    SimpleAxisLayoutOptions::create()
                        ->setMaxRelativeScale(1.f)
                );
                m_enableToggle->setID("enable-toggler");
                // Manually handle toggle state
                m_enableToggle->m_notClickable = true;
                m_viewMenu->addChild(m_enableToggle);
                m_viewMenu->updateLayout();
            }
            if (mod->hasLoadProblems() || mod->targetsOutdatedVersion()) {
                auto viewErrorSpr = createGeodeCircleButton(
                    CCSprite::createWithSpriteFrameName("exclamation.png"_spr), 1.f,
                    CircleBaseSize::Small
                );
                auto viewErrorBtn = CCMenuItemSpriteExtra::create(
                    viewErrorSpr, this, menu_selector(ModItem::onViewError)
                );
                viewErrorBtn->setID("view-error-button");
                m_viewMenu->addChild(viewErrorBtn);
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
            if (metadata.tags.contains("modtober24winner") || m_source.getID() == "rainixgd.geome3dash") {
                auto shortVer = CCSprite::createWithSpriteFrameName("tag-modtober-winner.png"_spr);
                shortVer->setTag(1);
                m_badgeContainer->addChild(shortVer);
                auto longVer = CCSprite::createWithSpriteFrameName("tag-modtober-winner-long.png"_spr);
                longVer->setTag(2);
                m_badgeContainer->addChild(longVer);
            }
            // Only show default Modtober tag if not a winner
            else if (metadata.tags.contains("modtober24")) {
                auto shortVer = CCSprite::createWithSpriteFrameName("tag-modtober.png"_spr);
                m_badgeContainer->addChild(shortVer);
            }

            // Show mod download count here already so people can make informed decisions
            // on which mods to install
            m_downloadCountContainer = CCNode::create();

            auto downloads = CCLabelBMFont::create(numToAbbreviatedString(metadata.downloadCount).c_str(), "bigFont.fnt");
            downloads->setID("downloads-label");
            downloads->setColor("mod-list-version-label"_cc3b);
            downloads->limitLabelWidth(125, 1.f, .1f);
            m_downloadCountContainer->addChildAtPosition(downloads, Anchor::Right, ccp(-0, 0), ccp(1, .5f));

            auto downloadsIcon = CCSprite::createWithSpriteFrameName("GJ_downloadsIcon_001.png");
            downloadsIcon->setID("downloads-icon-sprite");
            downloadsIcon->setScale(1.2f);
            m_downloadCountContainer->addChildAtPosition(downloadsIcon, Anchor::Left, ccp(8, 0));

            // m_downloadCountContainer scale is controlled in updateState
            m_downloadCountContainer->setContentSize({
                downloads->getScaledContentWidth() + downloadsIcon->getScaledContentWidth(),
                30
            });
            m_downloadCountContainer->updateLayout();

            // Check if mod is recommended by any others, only if not installed
            if (!Loader::get()->isModInstalled(metadata.id)) {
                std::vector<Mod*> recommends {};
                for (auto& recommend : Loader::get()->getRecommendations()) {
                    auto suggestionID = recommend.message.substr(0, recommend.message.find(' '));
                    if (suggestionID != metadata.id) {
                        continue;
                    }
                    recommends.push_back(std::get<2>(recommend.cause));
                }

                if (recommends.size() > 0) {
                    m_recommendedBy = CCNode::create();
                    m_recommendedBy->setID("recommended-container");
                    m_recommendedBy->setContentWidth(225);
                    auto byLabel = CCLabelBMFont::create("Recommended by ", "bigFont.fnt");
                    byLabel->setID("recommended-label");
                    byLabel->setColor("mod-list-recommended-by"_cc3b);
                    m_recommendedBy->addChild(byLabel);

                    std::string recommendStr = "";
                    if (recommends.size() == 1) {
                        recommendStr = recommends[0]->getName();
                    } else {
                        recommendStr = fmt::format("{} installed mods", recommends.size());
                    }

                    auto nameLabel = CCLabelBMFont::create(recommendStr.c_str(), "bigFont.fnt");
                    nameLabel->setID("recommended-name-label");
                    nameLabel->setColor("mod-list-recommended-by-2"_cc3b);
                    m_recommendedBy->addChild(nameLabel);

                    m_recommendedBy->setLayout(
                        RowLayout::create()
                            ->setDefaultScaleLimits(.1f, 1.f)
                            ->setAxisAlignment(AxisAlignment::Start)
                    );
                    m_infoContainer->addChildAtPosition(m_recommendedBy, Anchor::Left);
                }
            }
        }
    });

    auto updateSpr = createGeodeCircleButton(
        CCSprite::createWithSpriteFrameName("update.png"_spr), 1.15f,
        CircleBaseSize::Medium, true
    );
    m_updateBtn = CCMenuItemSpriteExtra::create(
        updateSpr, this, menu_selector(ModItem::onInstall)
    );
    m_updateBtn->setID("update-button");
    m_viewMenu->addChild(m_updateBtn);

    if (m_source.asMod()) {
        m_checkUpdateListener.bind(this, &ModItem::onCheckUpdates);
        m_checkUpdateListener.setFilter(m_source.checkUpdates());
    }

    this->updateState();

    // Only listen for updates on this mod specifically
    m_updateStateListener.bind([this](auto) { this->updateState(); });
    m_updateStateListener.setFilter(UpdateModListStateFilter(UpdateModState(m_source.getID())));

    m_downloadListener.bind([this](auto) { this->updateState(); });
    m_downloadListener.setFilter(server::ModDownloadFilter(m_source.getID()));

    m_settingNodeListener.bind([this](SettingNodeValueChangeEvent* ev) {
        if (!ev->isCommit()) {
            return ListenerResult::Propagate;
        }
        this->updateState();
        return ListenerResult::Propagate;
    });

    return true;
}

void ModItem::updateState() {
    auto wantsRestart = m_source.wantsRestart();
    auto download = server::ModDownloadManager::get()->getDownload(m_source.getID());
    bool isDownloading = download && download->isActive();

    // Update the size of the mod cell itself
    if (m_display == ModListDisplay::Grid) {
        auto widthWithoutGaps = m_targetWidth - 7.5f;
        this->setContentSize(ccp(widthWithoutGaps / roundf(widthWithoutGaps / 80), 100));
        m_bg->setContentSize(m_obContentSize / m_bg->getScale());
    }
    else {
        this->setContentSize(ccp(m_targetWidth, m_display == ModListDisplay::BigList ? 40 : 30));
        m_bg->setContentSize((m_obContentSize - ccp(6, 0)) / m_bg->getScale());
    }

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
    m_versionDownloadSeparator->setVisible(m_downloadCountContainer && m_display == ModListDisplay::Grid);

    // Download counts go next to the version like on the website on grid view
    if (m_downloadCountContainer) {
        m_downloadCountContainer->setScale(0.6f);
        m_downloadCountContainer->removeFromParent();
        if (m_display == ModListDisplay::Grid) {
            m_titleContainer->insertAfter(m_downloadCountContainer, m_versionDownloadSeparator);
            m_downloadCountContainer->setLayoutOptions(
                SimpleAxisLayoutOptions::create()
                    ->setMinRelativeScale(.1f)
                    ->setMaxRelativeScale(1.f)
                );
        }
        else {
            m_viewMenu->addChild(m_downloadCountContainer);
            m_downloadCountContainer->setLayoutOptions(
                SimpleAxisLayoutOptions::create()
                    ->setMinRelativeScale(.1f)
                    ->setMaxRelativeScale(1.f)
            );
        }
    }

    // Move badges to either be next to the title or in the top left corner in grid view
    if (m_badgeContainer) {
        m_badgeContainer->removeFromParent();
        if (m_display == ModListDisplay::Grid) {
            m_badgeContainer->setScale(.35f);
            m_badgeContainer->setContentWidth(30.f);
            m_badgeContainer->setLayout(
                SimpleColumnLayout::create()
                    ->setMainAxisAlignment(MainAxisAlignment::Start)
                    ->setMainAxisDirection(AxisDirection::TopToBottom)
                    ->setMainAxisScaling(AxisScaling::Fit)
                    ->setCrossAxisScaling(AxisScaling::Scale)
                    ->setGap(5.f)
            );
            m_badgeContainer->getLayout()->ignoreInvisibleChildren(true);
            this->addChildAtPosition(m_badgeContainer, Anchor::TopLeft, ccp(4, -4), ccp(0, 1));
        }
        else {
            m_badgeContainer->setContentHeight(30.f);
            m_badgeContainer->setScale(1.f);
            m_badgeContainer->setLayout(
                SimpleRowLayout::create()
                    ->setMainAxisAlignment(MainAxisAlignment::Start)
                    ->setMainAxisScaling(AxisScaling::Fit)
                    ->setCrossAxisScaling(AxisScaling::Scale)
                    ->setGap(5.f)
            );
            m_badgeContainer->getLayout()->ignoreInvisibleChildren(true);
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

    // On Grid View logo has constant size
    if (m_display == ModListDisplay::Grid) {
        limitNodeSize(m_logo, ccp(30, 30), 999, .1f);
        m_logo->setPosition(m_obContentSize.width / 2, m_obContentSize.height - 20);
    }
    else {
        auto logoSize = m_obContentSize.height - 10;
        limitNodeSize(m_logo, ccp(logoSize, logoSize), 999, .1f);
        m_logo->setPosition(m_obContentSize.height / 2 + 5, m_obContentSize.height / 2);
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
        CCSize(m_obContentSize.width - 10, 35) :
        CCSize(m_obContentSize.width / 1.75 - m_obContentSize.height, m_obContentSize.height - 5);

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
            m_downloadBar->setValue(prog->percentage / 100.f);
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

    // Set default colors based on source to start off with
    // (possibly overriding later based on state)
    m_source.visit(makeVisitor {
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
            m_developerLabel->setOpacity(mod->isOrWillBeEnabled() ? 255 : 155);
        },
        [this](server::ServerModMetadata const& metadata) {
            m_bg->setColor(isGeodeTheme() ? ccWHITE : ccBLACK);
            m_bg->setOpacity(isGeodeTheme() ? 25 : 90);

            if (metadata.tags.contains("paid")) {
                m_bg->setColor("mod-list-paid-color"_cc3b);
                m_bg->setOpacity(55);
            }
            if (metadata.tags.contains("modtober24")) {
                m_bg->setColor(ccc3(63, 91, 138));
                m_bg->setOpacity(85);
            }
            if (metadata.tags.contains("modtober24winner") || m_source.getID() == "rainixgd.geome3dash") {
                m_bg->setColor(ccc3(104, 63, 138));
                m_bg->setOpacity(85);
            }
            if (isGeodeTheme() && metadata.featured) {
                m_bg->setColor("mod-list-featured-color"_cc3b);
                m_bg->setOpacity(65);
            }
        }
    });

    if (
        auto update = m_source.hasUpdates();
        update && !(download && (download->isActive() || download->isDone()))
    ) {
        m_updateBtn->setVisible(true);
        std::string updateString = "";
        if (update->replacement.has_value()) {
            updateString += " -> " + update->replacement.value().id;
        } else {
            updateString += m_source.getMetadata().getVersion().toVString() + " -> " + update->version.toVString();
        }
        m_versionLabel->setString(updateString.c_str());
        m_versionLabel->setColor(to3B(ColorProvider::get()->color("mod-list-version-label-updates-available"_spr)));

        m_bg->setColor(to3B(ColorProvider::get()->color("mod-list-version-bg-updates-available"_spr)));
        m_bg->setOpacity(isGeodeTheme() ? 25 : 90);
    }
    else {
        m_updateBtn->setVisible(false);
        m_versionLabel->setString(m_source.getMetadata().getVersion().toVString().c_str());
        m_versionLabel->setColor(to3B(ColorProvider::get()->color("mod-list-version-label"_spr)));
    }

    // Hide by default
    m_outdatedLabel->setVisible(false);

    // If there were problems, tint the BG red
    if (m_source.asMod()) {
        std::optional<LoadProblem> targetsOutdated = m_source.asMod()->targetsOutdatedVersion();
        if (m_source.asMod()->hasLoadProblems()) {
            m_bg->setColor("mod-list-errors-found"_cc3b);
            m_bg->setOpacity(isGeodeTheme() ? 25 : 90);
        }
        if (!wantsRestart && targetsOutdated && !isDownloading) {
            m_bg->setColor("mod-list-outdated-label"_cc3b);
            m_bg->setOpacity(isGeodeTheme() ? 25 : 90);
            m_outdatedLabel->setVisible(true);
            elementToReplaceWithOtherAbnormalElement->setVisible(false);
            if (m_display == ModListDisplay::Grid) {
                m_outdatedLabel->setString("Outdated");
            }
            else {
                if (targetsOutdated->type == LoadProblem::Type::UnsupportedGeodeVersion || targetsOutdated->type == LoadProblem::Type::NeedsNewerGeodeVersion) {
                    m_outdatedLabel->setString(fmt::format(
                        "Outdated (Geode {})", m_source.getMetadata().getGeodeVersion().toNonVString()
                    ).c_str());
                } else {
                    // TODO: this is dumb but i didn't want to figure out the LoadProblem. sorry
                    if (m_source.getMetadata().getGameVersion() == "0.000") {
                        m_outdatedLabel->setString("Unavailable");
                    } else {
                        m_outdatedLabel->setString(fmt::format(
                            "Outdated (GD {})", m_source.getMetadata().getGameVersion().value_or("*")
                        ).c_str());
                    }
                }
            }
        }
    }

    // Update size and direction of title
    // On grid view, m_titleContainer contains the version and download count
    // but not the actual title lol
    m_titleLabel->setScale(1.f);
    m_titleContainer->setContentHeight(30.f);
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
    m_developers->setContentHeight(30.f);
    m_developers->updateLayout();

    if (m_recommendedBy) {
        m_recommendedBy->setContentWidth(titleSpace.width / m_infoContainer->getScale());
        m_recommendedBy->updateLayout();
    }

    limitNodeWidth(m_downloadWaiting, m_titleContainer->getContentWidth(), 1.f, .1f);
    limitNodeWidth(m_downloadBarContainer, m_titleContainer->getContentWidth(), 1.f, .1f);

    // Update positioning (jesus)
    switch (m_display) {
        case ModListDisplay::Grid: {
            m_infoContainer->updateAnchoredPosition(Anchor::Center, ccp(0, -5), ccp(.5f, .5f));
            // m_description is hidden
            m_titleLabel->setLayoutOptions(AnchorLayoutOptions::create()
                ->setAnchor(Anchor::Top)
                ->setOffset(ccp(0, -10))
            );
            limitNodeWidth(m_titleLabel, m_titleContainer->getContentWidth(), .8f, .1f);
            m_titleContainer->updateAnchoredPosition(Anchor::Center, ccp(0, 0), ccp(.5f, .5f));
            m_developers->updateAnchoredPosition(Anchor::Bottom, ccp(0, 10), ccp(.5f, .5f));
            m_restartRequiredLabel->updateAnchoredPosition(Anchor::Bottom, ccp(0, 10), ccp(.5f, .5f));
            m_outdatedLabel->updateAnchoredPosition(Anchor::Bottom, ccp(0, 10), ccp(.5f, .5f));
            m_downloadBarContainer->updateAnchoredPosition(Anchor::Bottom, ccp(0, 10), ccp(.5f, .5f));
            m_downloadWaiting->updateAnchoredPosition(Anchor::Bottom, ccp(0, 10), ccp(.5f, .5f));

            if (m_recommendedBy) {
                m_recommendedBy->updateAnchoredPosition(Anchor::Bottom, ccp(0, 10), ccp(.5f, .5f));
            }
        } break;

        default:
        case ModListDisplay::SmallList: {
            m_infoContainer->updateAnchoredPosition(Anchor::Left, ccp(m_obContentSize.height + 10, 0), ccp(0, .5f));
            m_titleContainer->updateAnchoredPosition(Anchor::TopLeft, ccp(0, 2), ccp(0, 1));
            m_titleLabel->setLayoutOptions(
                SimpleAxisLayoutOptions::create()
                    ->setMinRelativeScale(.5f)
                    ->setMaxRelativeScale(1.f)
                );
            // m_description is hidden
            m_developers->updateAnchoredPosition(Anchor::BottomLeft, ccp(0, 3), ccp(0, 0));
            m_restartRequiredLabel->updateAnchoredPosition(Anchor::BottomLeft, ccp(0, 3), ccp(0, 0));
            m_outdatedLabel->updateAnchoredPosition(Anchor::BottomLeft, ccp(0, 3), ccp(0, 0));
            m_downloadBarContainer->updateAnchoredPosition(Anchor::BottomLeft, ccp(0, 3), ccp(0, 0));
            m_downloadWaiting->updateAnchoredPosition(Anchor::BottomLeft, ccp(0, 3), ccp(0, 0));

            if (m_recommendedBy) {
                m_recommendedBy->updateAnchoredPosition(Anchor::BottomLeft, ccp(0, 3), ccp(0, 0));
            }
        } break;

        case ModListDisplay::BigList: {
            m_infoContainer->updateAnchoredPosition(Anchor::Left, ccp(m_obContentSize.height + 10, 0), ccp(0, .5f));
            m_titleContainer->updateAnchoredPosition(Anchor::TopLeft, ccp(0, 0), ccp(0, 1));
            m_titleLabel->setLayoutOptions(
                SimpleAxisLayoutOptions::create()
                    ->setMinRelativeScale(.5f)
                    ->setMaxRelativeScale(1.f)
                );
            m_developers->updateAnchoredPosition(Anchor::Left, ccp(0, 0), ccp(0, .5f));

            m_description->updateAnchoredPosition(Anchor::BottomLeft, ccp(0, 0), ccp(0, 0));
            m_restartRequiredLabel->updateAnchoredPosition(Anchor::BottomLeft, ccp(0, 0), ccp(0, 0));
            m_outdatedLabel->updateAnchoredPosition(Anchor::BottomLeft, ccp(0, 0), ccp(0, 0));
            m_downloadBarContainer->updateAnchoredPosition(Anchor::BottomLeft, ccp(0, 0), ccp(0, 0));
            m_downloadWaiting->updateAnchoredPosition(Anchor::BottomLeft, ccp(0, 0), ccp(0, 0));

            if (m_recommendedBy) {
                m_recommendedBy->updateAnchoredPosition(Anchor::BottomLeft, ccp(0, 0), ccp(0, 0));
            }
        } break;
    }

    m_infoContainer->setContentSize(ccp(titleSpace.width, titleSpace.height) / m_infoContainer->getScale());
    m_infoContainer->updateLayout();

    m_titleContainer->updateLayout();
    // Update button menu state

    if (m_display == ModListDisplay::Grid) {
        m_viewMenu->setContentWidth(m_obContentSize.width / m_viewMenu->getScaleX());
        m_viewMenu->updateAnchoredPosition(Anchor::Bottom, ccp(0, 5), ccp(.5f, 0));
        m_viewMenu->setScale(.45f);
        static_cast<SimpleRowLayout*>(m_viewMenu->getLayout())->setMainAxisAlignment(MainAxisAlignment::Center);
    }
    else {
        m_viewMenu->setContentWidth(m_obContentSize.width / m_viewMenu->getScaleX() / 2 - 20);
        m_viewMenu->updateAnchoredPosition(Anchor::Right, ccp(-10, 0), ccp(1, .5f));
        m_viewMenu->setScale(.55f);
        static_cast<SimpleRowLayout*>(m_viewMenu->getLayout())->setMainAxisAlignment(MainAxisAlignment::Start);
    }

    m_viewMenu->setContentHeight(40.f);
    m_viewMenu->updateLayout();

    // Highlight item via BG if it wants to restart for extra UI attention
    if (wantsRestart) {
        m_bg->setColor("mod-list-restart-required-label"_cc3b);
        m_bg->setOpacity(isGeodeTheme() ? 25 : 90);
    }

    // Update enable toggle state
    if (m_enableToggle && m_source.asMod()) {
        m_enableToggle->toggle(m_source.asMod()->isOrWillBeEnabled());

        // Disable the toggle if the mod has been uninstalled or if the mod is
        // outdated
        if (
            modRequestedActionIsUninstall(m_source.asMod()->getRequestedAction()) ||
            m_source.asMod()->targetsOutdatedVersion()
        ) {
            m_enableToggle->setEnabled(false);
            auto off = typeinfo_cast<CCRGBAProtocol*>(m_enableToggle->m_offButton->getNormalImage());
            auto on = typeinfo_cast<CCRGBAProtocol*>(m_enableToggle->m_onButton->getNormalImage());
            off->setColor(ccGRAY);
            off->setOpacity(105);
            on->setColor(ccGRAY);
            on->setOpacity(105);
        }
    }

    this->updateLayout();

    ModItemUIEvent(std::make_unique<ModItemUIEvent::Impl>(this)).post();
}

void ModItem::updateDisplay(float width, ModListDisplay display) {
    m_display = display;
    m_targetWidth = width;
    this->updateState();
}

void ModItem::onCheckUpdates(typename server::ServerRequest<std::optional<server::ServerModUpdate>>::Event* event) {
    if (event->getValue() && event->getValue()->isOk()) {
        this->updateState();
    }
}

void ModItem::onView(CCObject*) {
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
            "<cy>Geode does not handle any payments. The mod handles all transactions in their own way.</c>\n\n"
            "<cp>The paid content may not be available in your country.</c>",
            "OK", nullptr, 360
        )->show();
    }

    // Show popups for invalid mods
    if (m_source.asServer()) {
        auto version = m_source.asServer()->latestVersion();
        auto gameVersion = version.getGameVersion();
        if (gameVersion == "0.000") {
            return FLAlertLayer::create(
                nullptr,
                "Invalid Platform",
                "This mod is <cr>not available</c> for your current platform.",
                "OK", nullptr, 360
            )->show();
        }
        if (gameVersion && gameVersion != "*" && gameVersion != GEODE_STR(GEODE_GD_VERSION)) {
            return FLAlertLayer::create(
                nullptr,
                "Unavailable",
                "This mod targets an <cr>unsupported version of Geometry Dash</c>.",
                "OK", nullptr, 360
            )->show();
        }
        if (!Loader::get()->isModVersionSupported(version.getGeodeVersion())) {
            return FLAlertLayer::create(
                nullptr,
                "Unavailable",
                "This mod targets an <cr>unsupported version of Geode</c>.",
                "OK", nullptr, 360
            )->show();
        }
    }

    // Always open up the popup for the installed mod page if that is possible
    ModPopup::create(m_source.convertForPopup())->show();
}
void ModItem::onViewError(CCObject*) {
    if (auto mod = m_source.asMod()) {
        if (auto problem = mod->targetsOutdatedVersion()) {
            std::string issue;
            std::string howToFix;
            switch (problem->type) {
                default:
                case LoadProblem::Type::UnsupportedVersion: {
                    issue = fmt::format("<cy>{}</c>", problem->message);
                    howToFix = "wait for the developer to <cj>release an update to "
                        "the mod</c> that supports the newer version.";
                } break;

                case LoadProblem::Type::NeedsNewerGeodeVersion: {
                    issue = "This mod is made for a <cp>newer version of Geode</c>.";
                    howToFix = "<cp>update Geode</c> by enabling <co>Automatic Updates</c> "
                        "or redownloading it from the Geode website.";
                } break;

                case LoadProblem::Type::UnsupportedGeodeVersion: {
                    issue = "This mod is made for an <cy>older version of Geode</c>.";
                    howToFix = "wait for the developer to <cj>release an update to "
                        "the mod</c> that supports the newer version.";
                } break;
            }
            FLAlertLayer::create(
                "Outdated",
                fmt::format("{} Please {}", issue, howToFix),
                "OK"
            )->show();
        }
        else {
            ModErrorPopup::create(mod)->show();
        }
    }
}
void ModItem::onEnable(CCObject*) {
    if (auto mod = m_source.asMod()) {
        // Toggle the mod state
        auto res = mod->isOrWillBeEnabled() ? mod->disable() : mod->enable();
        if (!res) {
            FLAlertLayer::create(
                "Error Toggling Mod",
                res.unwrapErr(),
                "OK"
            )->show();
        }
    }

    // Update state of the mod item
    UpdateModListStateEvent(UpdateModState(m_source.getID())).post();
}
void ModItem::onInstall(CCObject*) {
    m_source.startInstall();
}
void ModItem::onDevelopers(CCObject*) {
    DevListPopup::create(m_source)->show();
}

ModItem* ModItem::create(ModSource&& source) {
    auto ret = new ModItem();
    if (ret->init(std::move(source))) {
        ret->autorelease();
        return ret;
    }
    delete ret;
    return nullptr;
}

ModSource& ModItem::getSource() & {
    return m_source;
}
