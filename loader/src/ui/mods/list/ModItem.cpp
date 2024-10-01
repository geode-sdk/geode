#include "ModItem.hpp"
#include <Geode/ui/GeodeUI.hpp>
#include <Geode/utils/ColorProvider.hpp>
#include <Geode/binding/ButtonSprite.hpp>
#include <Geode/loader/Loader.hpp>
#include <vector>
#include "../GeodeStyle.hpp"
#include "../popups/ModPopup.hpp"
#include "../popups/DevPopup.hpp"
#include "ui/mods/popups/ModErrorPopup.hpp"
#include "ui/mods/sources/ModSource.hpp"
#include "../../GeodeUIEvent.hpp"

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
    this->addChild(m_bg);

    m_logo = m_source.createModLogo();
    m_logo->setID("logo-sprite");
    this->addChild(m_logo);

    m_infoContainer = CCNode::create();
    m_infoContainer->setID("info-container");
    m_infoContainer->setScale(.4f);
    m_infoContainer->setAnchorPoint({ .0f, .5f });
    m_infoContainer->setLayout(
        ColumnLayout::create()
            ->setAxisReverse(true)
            ->setAxisAlignment(AxisAlignment::Even)
            ->setCrossAxisLineAlignment(AxisAlignment::Start)
            ->setGap(0)
    );
    m_infoContainer->getLayout()->ignoreInvisibleChildren(true);

    m_titleContainer = CCNode::create();
    m_titleContainer->setID("title-container");
    m_titleContainer->setAnchorPoint({ .0f, .5f });
    m_titleContainer->setLayout(
        RowLayout::create()
            ->setDefaultScaleLimits(.1f, 1.f)
            ->setAxisAlignment(AxisAlignment::Start)
    );

    m_titleLabel = CCLabelBMFont::create(m_source.getMetadata().getName().c_str(), "bigFont.fnt");
    m_titleLabel->setID("title-label");
    m_titleLabel->setLayoutOptions(AxisLayoutOptions::create()->setScalePriority(1));
    m_titleContainer->addChild(m_titleLabel);

    m_versionLabel = CCLabelBMFont::create("", "bigFont.fnt");
    m_versionLabel->setID("version-label");
    m_versionLabel->setLayoutOptions(AxisLayoutOptions::create()->setScaleLimits(std::nullopt, .7f));
    m_titleContainer->addChild(m_versionLabel);
    
    m_infoContainer->addChild(m_titleContainer);
    
    m_developers = CCMenu::create();
    m_developers->setID("developers-menu");
    m_developers->ignoreAnchorPointForPosition(false);
    m_developers->setAnchorPoint({ .0f, .5f });

    auto by = "By " + m_source.formatDevelopers();
    m_developerLabel = CCLabelBMFont::create(by.c_str(), "goldFont.fnt");
    m_developerLabel->setID("developers-label");
    auto developersBtn = CCMenuItemSpriteExtra::create(
        m_developerLabel, this, menu_selector(ModItem::onDevelopers)
    );
    developersBtn->setID("developers-button");
    m_developers->addChild(developersBtn);

    m_developers->setLayout(
        RowLayout::create()
            ->setAxisAlignment(AxisAlignment::Start)
    );
    m_infoContainer->addChild(m_developers);

    m_restartRequiredLabel = createTagLabel(
        "Restart Required",
        {
            to3B(ColorProvider::get()->color("mod-list-restart-required-label"_spr)),
            to3B(ColorProvider::get()->color("mod-list-restart-required-label-bg"_spr))
        }
    );
    m_restartRequiredLabel->setID("restart-required-label");
    m_restartRequiredLabel->setLayoutOptions(AxisLayoutOptions::create()->setScaleLimits(std::nullopt, .75f));
    m_infoContainer->addChild(m_restartRequiredLabel);

    m_downloadBarContainer = CCNode::create();
    m_downloadBarContainer->setID("download-bar-container");
    m_downloadBarContainer->setContentSize({ 320, 30 });
    
    m_downloadBar = Slider::create(nullptr, nullptr);
    m_downloadBar->setID("download-bar");
    m_downloadBar->m_touchLogic->m_thumb->setVisible(false);
    m_downloadBar->setScale(1.5f);
    m_downloadBarContainer->addChildAtPosition(m_downloadBar, Anchor::Center, ccp(0, 0), ccp(0, 0));

    m_infoContainer->addChild(m_downloadBarContainer);

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

    m_infoContainer->addChild(m_downloadWaiting);

    this->addChild(m_infoContainer);

    m_viewMenu = CCMenu::create();
    m_viewMenu->setID("view-menu");
    m_viewMenu->setAnchorPoint({ 1.f, .5f });
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

    auto viewBtn = CCMenuItemSpriteExtra::create(
        spr,
        this, menu_selector(ModItem::onView)
    );
    viewBtn->setID("view-button");
    m_viewMenu->addChild(viewBtn);

    m_viewMenu->setLayout(
        RowLayout::create()
            ->setAxisReverse(true)
            ->setAxisAlignment(AxisAlignment::End)
            ->setGap(10)
    );
    m_viewMenu->getLayout()->ignoreInvisibleChildren(true);
    this->addChildAtPosition(m_viewMenu, Anchor::Right, ccp(-10, 0));

    // Handle source-specific stuff
    m_source.visit(makeVisitor {
        [this](Mod* mod) {
            // Add an enable button if the mod is enablable
            if (!mod->isInternal()) {
                m_enableToggle = CCMenuItemToggler::createWithStandardSprites(
                    this, menu_selector(ModItem::onEnable), 1.f
                );
                m_enableToggle->setID("enable-toggler");
                // Manually handle toggle state
                m_enableToggle->m_notClickable = true;
                m_viewMenu->addChild(m_enableToggle);
                m_viewMenu->updateLayout();
            }
            if (mod->hasProblems()) {
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
            if (metadata.featured) {
                auto star = CCSprite::createWithSpriteFrameName("tag-featured.png"_spr);
                star->setLayoutOptions(AxisLayoutOptions::create()->setScaleLimits(.1f, .8f));
                m_titleContainer->addChild(star);
            }
            if (metadata.tags.contains("paid")) {
                auto paidModLabel = CCSprite::createWithSpriteFrameName("tag-paid.png"_spr);
                paidModLabel->setLayoutOptions(AxisLayoutOptions::create()->setScaleLimits(.1f, .8f));
                m_titleContainer->addChild(paidModLabel);
            }
            if (metadata.tags.contains("modtober24")) {
                auto modtoberLabel = CCSprite::createWithSpriteFrameName("tag-modtober.png"_spr);
                modtoberLabel->setLayoutOptions(AxisLayoutOptions::create()->setScaleLimits(.1f, .8f));
                m_titleContainer->addChild(modtoberLabel);
            }

            // Show mod download count here already so people can make informed decisions 
            // on which mods to install
            auto downloadsContainer = CCNode::create();
            
            auto downloads = CCLabelBMFont::create(numToAbbreviatedString(metadata.downloadCount).c_str(), "bigFont.fnt");
            downloads->setID("downloads-label");
            downloads->setColor("mod-list-version-label"_cc3b);
            downloads->limitLabelWidth(80, .5f, .1f);
            downloadsContainer->addChildAtPosition(downloads, Anchor::Right, ccp(-0, 0), ccp(1, .5f));

            auto downloadsIcon = CCSprite::createWithSpriteFrameName("GJ_downloadsIcon_001.png");
            downloadsIcon->setID("downloads-icon-sprite");
            downloadsIcon->setScale(.75f);
            downloadsContainer->addChildAtPosition(downloadsIcon, Anchor::Right, ccp(-downloads->getScaledContentWidth() - 10, 0));

            downloadsContainer->setContentSize({
                downloads->getScaledContentWidth() + 10 + downloadsIcon->getScaledContentWidth() + 10,
                25
            });
            downloadsContainer->updateLayout();
            m_viewMenu->addChild(downloadsContainer);

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
                    m_infoContainer->addChild(m_recommendedBy);
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

    m_settingNodeListener.bind([this](SettingNodeValueChangeEventV3*) {
        this->updateState();
        return ListenerResult::Propagate;
    });

    return true;
}

void ModItem::updateState() {
    auto wantsRestart = m_source.wantsRestart();

    auto download = server::ModDownloadManager::get()->getDownload(m_source.getID());

    // If there is an active download ongoing, show that in place of developer name 
    if (download && download->isActive()) {
        m_updateBtn->setVisible(false);
        m_restartRequiredLabel->setVisible(false);
        m_developers->setVisible(false);

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
        m_developers->setVisible(!wantsRestart);
        m_downloadBarContainer->setVisible(false);
        m_downloadWaiting->setVisible(false);
    }
    m_infoContainer->updateLayout();

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
            if (isGeodeTheme() && metadata.featured) {
                m_bg->setColor("mod-list-featured-color"_cc3b);
                m_bg->setOpacity(65);
            }
        },
        [this](ModSuggestion const& suggestion) {
            m_bg->setColor("mod-list-recommended-bg"_cc3b);
            m_bg->setOpacity(isGeodeTheme() ? 25 : 90);
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

    m_viewMenu->updateLayout();
    m_titleContainer->updateLayout();

    // If there were problems, tint the BG red
    if (m_source.asMod() && m_source.asMod()->hasProblems()) {
        m_bg->setColor("mod-list-errors-found"_cc3b);
        m_bg->setOpacity(isGeodeTheme() ? 25 : 90);
    }

    // Highlight item via BG if it wants to restart for extra UI attention
    if (wantsRestart) {
        m_bg->setColor("mod-list-restart-required-label"_cc3b);
        m_bg->setOpacity(isGeodeTheme() ? 25 : 90);
    }

    // Update enable toggle state
    if (m_enableToggle && m_source.asMod()) {
        m_enableToggle->toggle(m_source.asMod()->isOrWillBeEnabled());

        // Disable the toggle if the mod has been uninstalled
        if (modRequestedActionIsUninstall(m_source.asMod()->getRequestedAction())) {
            m_enableToggle->setEnabled(false);
            auto off = typeinfo_cast<CCRGBAProtocol*>(m_enableToggle->m_offButton->getNormalImage());
            auto on = typeinfo_cast<CCRGBAProtocol*>(m_enableToggle->m_onButton->getNormalImage());
            off->setColor(ccGRAY);
            off->setOpacity(105);
            on->setColor(ccGRAY);
            on->setOpacity(105);
        }
    }

    ModItemUIEvent(std::make_unique<ModItemUIEvent::Impl>(this)).post();
}

void ModItem::updateSize(float width, bool big) {
    this->setContentSize({ width, big ? 40.f : 30.f });

    m_bg->setContentSize((m_obContentSize - ccp(6, 0)) / m_bg->getScale());
    m_bg->setPosition(m_obContentSize / 2);

    auto logoSize = m_obContentSize.height - 10;
    limitNodeSize(m_logo, { logoSize, logoSize }, 999, .1f);
    m_logo->setPosition(m_obContentSize.height / 2 + 5, m_obContentSize.height / 2);

    CCSize titleSpace {
        m_obContentSize.width / 2 - m_obContentSize.height,
        logoSize + 5
    };

    // Divide by scale of info container since that actually determines the size
    // (Since the scale of m_titleContainer and m_developers is managed by its layout)
    m_titleContainer->setContentWidth(titleSpace.width / m_infoContainer->getScale());
    m_titleContainer->updateLayout();

    m_developers->setContentWidth(titleSpace.width / m_infoContainer->getScale());
    m_developers->updateLayout();

    if (m_recommendedBy) {
        m_recommendedBy->setContentWidth(titleSpace.width / m_infoContainer->getScale());
        m_recommendedBy->updateLayout();
    }

    m_infoContainer->setPosition(m_obContentSize.height + 10, m_obContentSize.height / 2);
    m_infoContainer->setContentSize(ccp(titleSpace.width, titleSpace.height) / m_infoContainer->getScale());
    m_infoContainer->updateLayout();

    m_viewMenu->setContentWidth(m_obContentSize.width / m_viewMenu->getScaleX() / 2 - 20);
    m_viewMenu->updateLayout();

    this->updateLayout();
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
        ModErrorPopup::create(mod)->show();
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
    if (auto updates = m_source.hasUpdates()) {
        if (updates->replacement.has_value()) {
            server::ModDownloadManager::get()->startDownload(
                updates->replacement->id,
                updates->replacement->version,
                std::nullopt,
                m_source.getID()
            );
            return;
        }
    }
    server::ModDownloadManager::get()->startDownload(m_source.getID(), std::nullopt);
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
