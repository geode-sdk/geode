#include "ModPopup.hpp"

#include "../../../internal/about.hpp"
#include "../settings/ModSettingsPopup.hpp"
#include "ConfirmUninstallPopup.hpp"

#include <Geode/cocos/cocoa/CCBool.h>
#include <Geode/loader/Loader.hpp>
#include <Geode/loader/ModSettingsManager.hpp>
#include <Geode/ui/GeodeUI.hpp>
#include <Geode/ui/Label.hpp>
#include <Geode/ui/MDTextArea.hpp>
#include <Geode/utils/ColorProvider.hpp>
#include <Geode/utils/web.hpp>
#include <optional>

class FetchTextArea : public CCNode {
public:
    using Request = server::ServerFuture<std::optional<std::string>>;

protected:
    ListenerHandle m_handle;
    async::TaskHolder<server::ServerResult<std::optional<std::string>>> m_listener;
    MDTextArea* m_textarea;
    CCNode* m_loading;
    std::string m_noneText;

    bool init(Request&& req, std::string noneText, CCSize const& size) {
        if (!CCNode::init()) return false;

        this->setAnchorPoint({.5f, .5f});
        this->setContentSize(size);

        m_noneText = std::move(noneText);

        m_textarea = MDTextArea::create("", size);
        m_textarea->setID("textarea");
        this->addChildAtPosition(m_textarea, Anchor::Center);

        m_loading = createLoadingCircle(30);
        this->addChildAtPosition(m_loading, Anchor::Center);

        m_listener.spawn(std::move(req), [this](auto val) {
            this->onRequest(std::move(val));
        });

        return true;
    }

    void onRequest(server::ServerResult<std::optional<std::string>> result) {
        m_loading->removeFromParent();
        if (result && result.isOk()) {
            auto value = std::move(result).unwrap();
            if (value) {
                std::string str = std::move(value).value();
                m_textarea->setString(str.c_str());
                return;
            }
        }
        m_textarea->setString(m_noneText.c_str());
    }

public:
    static FetchTextArea* create(Request&& req, std::string noneText, CCSize const& size) {
        auto ret = new FetchTextArea();
        if (ret->init(std::move(req), std::move(noneText), size)) {
            ret->autorelease();
            return ret;
        }
        delete ret;
        return nullptr;
    }
};

bool ModPopup::init(ModSource&& src) {
    auto style = src.asServer() ? GeodePopupStyle::Alt : GeodePopupStyle::Default;

    if (!GeodePopup::init(440.f, 280.f, style)) return false;

    m_source = std::move(src);
    m_noElasticity = true;

    this->setID(Mod::get()->expandSpriteName(fmt::format("popup-{}", m_source.getID())));
    this->setUserObject("mod", CCString::create(m_source.getID()));

    auto isGeode = m_source.asMod() == Mod::get();
    if (isGeode) {
        // Display commit hashes
        auto loaderHash = about::getLoaderCommitHash();
        auto bindingsHash = about::getBindingsCommitHash();

        auto hashLabel = Label::create(
            fmt::format("Loader: {}, Bindings: {}", loaderHash, bindingsHash), "chatFont.fnt"
        );
        hashLabel->setAnchorPoint({.5f, 1.f});
        hashLabel->setOpacity(51);
        hashLabel->setScale(.7f);
        m_mainLayer->addChildAtPosition(hashLabel, Anchor::Bottom, {.0f, -5.f});
    }

    auto mainContainer = CCNode::create();
    mainContainer->setID("main-container");
    mainContainer->setContentSize(m_mainLayer->getContentSize() - CCSize{20.f, 20.f});
    mainContainer->setAnchorPoint({.5f, .5f});
    mainContainer->setLayout(
        RowLayout::create()->setGap(7.5f)->setCrossAxisLineAlignment(AxisAlignment::Start)
    );

    auto leftColumn = CCNode::create();
    leftColumn->setContentSize({145, mainContainer->getContentHeight()});

    m_titleContainer = CCMenu::create();
    m_titleContainer->setContentSize({leftColumn->getContentWidth(), 25});
    m_titleContainer->setAnchorPoint({.5f, .5f});

    auto getLogo = [this]() {
        auto logo = m_source.createModLogo();
        logo->setID("mod-logo");
        limitNodeSize(
            logo, {m_titleContainer->getContentHeight(), m_titleContainer->getContentHeight()}, 5.f, .1f
        );
        return logo;
    };

    if (isGeode) {
        auto counter = Mod::get()->getSavedValue("alternate-geode-style", false) ? 3 : 0;
        auto* logoButton =
            CCMenuItemExt::createSpriteExtra(getLogo(), [this, counter, getLogo](auto* item) mutable {
                counter++;
                if (counter % 6 == 3) {
                    Mod::get()->setSavedValue("alternate-geode-style", true);
                }
                else if (counter % 6 == 0) {
                    Mod::get()->getSaveContainer().erase("alternate-geode-style");
                }
            });
        m_titleContainer->addChildAtPosition(
            logoButton, Anchor::Left, {m_titleContainer->getContentHeight() / 2.f, .0f}
        );
    }
    else {
        m_titleContainer->addChildAtPosition(
            getLogo(), Anchor::Left, {m_titleContainer->getContentHeight() / 2.f, .0f}
        );
    }

    // Lil padding
    auto devAndTitlePos = m_titleContainer->getContentHeight() + 5;

    StringBuffer title;
    title.append("{:.40}", m_source.getMetadata().getName());
    if (m_source.getMetadata().getName().size() > 40) {
        title.append("...");
    }

    auto titleLabel = Label::create(title.str(), "bigFont.fnt");
    titleLabel->setLimitLabelWidth(m_titleContainer->getContentWidth() - devAndTitlePos, .45f, .1f);
    titleLabel->setAnchorPoint({.0f, .5f});
    titleLabel->setID("mod-name-label");
    m_titleContainer->addChildAtPosition(
        titleLabel, Anchor::TopLeft, {devAndTitlePos, -m_titleContainer->getContentHeight() * .25f}
    );

    auto dev = Label::create(fmt::format("By {}", m_source.formatDevelopers()), "goldFont.fnt");
    dev->setLimitLabelWidth(m_titleContainer->getContentWidth() - devAndTitlePos, .35f, .05f);
    dev->setAnchorPoint({.0f, .5f});
    dev->setID("mod-developer-label");
    m_titleContainer->addChildAtPosition(
        dev, Anchor::BottomLeft, {devAndTitlePos, m_titleContainer->getContentHeight() * .25f}
    );

    // Suggestions
    // todo: bring this back once we add "suggestions" in mod.json
    // if (!Loader::get()->isModInstalled(m_source.getMetadata().getID())) {
    //     std::vector<Mod*> recommends {};
    //     for (auto& problem : Loader::get()->getRecommendations()) {
    //         auto suggestionID = problem.message.substr(0, problem.message.find(' '));
    //         if (suggestionID != m_source.getMetadata().getID()) {
    //             continue;
    //         }
    //         recommends.push_back(std::get<2>(problem.cause));
    //     }

    //     if (recommends.size() > 0) {
    //         title->updateAnchoredPosition(Anchor::TopLeft, ccp(devAndTitlePos, -2));
    //         dev->updateAnchoredPosition(Anchor::Left, ccp(devAndTitlePos, 0));

    //         auto recommendedBy = CCNode::create();
    //         recommendedBy->setContentWidth(m_titleContainer->getContentWidth() - devAndTitlePos);
    //         recommendedBy->setAnchorPoint({ .0f, .5f });

    //         auto byLabel = Label::create("Recommended by ", "bigFont.fnt");
    //         byLabel->setColor("mod-list-recommended-by"_cc3b);
    //         recommendedBy->addChild(byLabel);

    //         std::string suggestionStr {};
    //         if (recommends.size() == 1) {
    //             suggestionStr = recommends[0]->getName();
    //         } else {
    //             suggestionStr = fmt::format("{} installed mods", recommends.size());
    //         }

    //         auto nameLabel = Label::create(suggestionStr.c_str(), "bigFont.fnt");
    //         nameLabel->setColor("mod-list-recommended-by-2"_cc3b);
    //         recommendedBy->addChild(nameLabel);

    //         recommendedBy->setLayout(
    //             RowLayout::create()
    //                 ->setDefaultScaleLimits(.1f, 1.f)
    //                 ->setAxisAlignment(AxisAlignment::Start)
    //         );
    //         m_titleContainer->addChildAtPosition(recommendedBy, Anchor::BottomLeft, ccp(devAndTitlePos, 4));
    //     }
    // }

    leftColumn->addChild(m_titleContainer);

    auto idLabel =
        Label::create(fmt::format("(ID: {})", m_source.getMetadata().getID()), "bigFont.fnt");
    idLabel->setLimitLabelWidth(leftColumn->getContentWidth(), .25f, .05f);
    idLabel->setColor({150, 150, 150});
    idLabel->setOpacity(140);
    idLabel->setID("mod-id-label");
    leftColumn->addChild(idLabel);

    auto statsContainer = CCNode::create();
    statsContainer->setContentSize({leftColumn->getContentWidth(), 80});
    statsContainer->setAnchorPoint({.5f, .5f});

    auto statsBG = NineSlice::create("square02b_001.png");
    statsBG->setColor({0, 0, 0});
    statsBG->setOpacity(75);
    statsBG->setScale(.3f);
    statsBG->setContentSize(statsContainer->getContentSize() / statsBG->getScale());
    statsContainer->addChildAtPosition(statsBG, Anchor::Center);

    m_stats = CCNode::create();
    m_stats->setContentSize(statsContainer->getContentSize() - CCSize{10.f, 10.f});
    m_stats->setAnchorPoint({.5f, .5f});
    m_stats->setID("mod-stats-container");

    struct ModStatsLabel {
        std::optional<std::string> sprite; // 0
        std::string label; // 1
        std::string id; // 2
        std::optional<std::string> value; // 3
        std::string stat; // 4
    };

    auto stats = std::array{
        ModStatsLabel{
            "GJ_downloadsIcon_001.png",
            "Downloads",
            "downloads",
            std::nullopt,
            "stats",
        },
        ModStatsLabel{
            "GJ_timeIcon_001.png",
            "Released",
            "release-date",
            std::nullopt,
            "stats",
        },
        ModStatsLabel{
            "GJ_timeIcon_001.png",
            "Updated",
            "update-date",
            std::nullopt,
            "stats",
        },
        ModStatsLabel{
            "version.png"_spr,
            "Version",
            "version",
            m_source.getMetadata().getVersion().toVString(),
            "client",
        },
        ModStatsLabel{
            std::nullopt,
            "Checking for updates",
            "update-check",
            std::nullopt,
            "updates",
        },
    };

    for (auto& stat : stats) {
        auto container = CCNode::create();
        container->setID(std::move(stat.id));
        container->setContentSize({m_stats->getContentWidth(), 10});
        container->setUserObject(std::move(stat.stat), CCBool::create(true));

        auto labelContainer = CCNode::create();
        labelContainer->setID("labels");
        labelContainer->setLayout(RowLayout::create());
        labelContainer->setAnchorPoint({1.f, .5f});
        labelContainer->setScale(.3f);
        labelContainer->setContentWidth(
            (container->getContentWidth() - container->getContentHeight() - 5) /
            labelContainer->getScale()
        );
        container->addChildAtPosition(labelContainer, Anchor::Right);

        auto label = Label::create("", "bigFont.fnt");
        label->setID("label");
        labelContainer->addChild(label);

        labelContainer->addChild(SpacerNode::create());

        auto valueLabel = Label::create("", "bigFont.fnt");
        valueLabel->setID("value-label");
        labelContainer->addChild(valueLabel);

        labelContainer->addChild(
            createLoadingCircle(container->getContentHeight() / labelContainer->getScale())
        );

        if (stat.sprite.has_value()) {
            this->setStatIcon(container, stat.sprite.value());
        }

        this->setStatLabel(container, std::move(stat.label));
        this->setStatValue(container, std::move(stat.value));

        m_stats->addChild(container);
    }

    m_stats->setLayout(
        ColumnLayout::create()->setAxisReverse(true)->setDefaultScaleLimits(.1f, 1)->setAxisAlignment(
            AxisAlignment::Even
        )
    );
    statsContainer->addChildAtPosition(m_stats, Anchor::Center);

    leftColumn->addChild(statsContainer);

    // Tags

    auto tagsTitle = Label::create("Tags", "bigFont.fnt");
    tagsTitle->setLimitLabelWidth(leftColumn->getContentWidth(), .25f, .05f);
    tagsTitle->setOpacity(195);
    tagsTitle->setID("tags-title");
    leftColumn->addChild(tagsTitle);

    auto tagsContainer = this->createContainer({leftColumn->getContentWidth(), 35});

    m_tags = CCNode::create();
    m_tags->ignoreAnchorPointForPosition(false);
    m_tags->setContentSize(tagsContainer->getContentSize() - CCSize{10.f, 10.f});
    m_tags->setAnchorPoint({.5f, .5f});
    m_tags->setID("tags-container");

    m_tags->addChild(createLoadingCircle(50));

    m_tags->setLayout(
        RowLayout::create()
            ->setDefaultScaleLimits(.1f, .3f)
            ->setGrowCrossAxis(true)
            ->setCrossAxisOverflow(false)
            ->setAxisAlignment(AxisAlignment::Start)
            ->setCrossAxisAlignment(AxisAlignment::End)
    );
    tagsContainer->addChildAtPosition(m_tags, Anchor::Center);

    leftColumn->addChild(tagsContainer);

    // Installing

    auto manageContainer = CCNode::create();
    manageContainer->setContentSize({leftColumn->getContentWidth(), 10.f});

    auto manageTitle = Label::create("Manage", "bigFont.fnt");
    manageTitle->setScale(.25f);
    manageTitle->setOpacity(195);
    manageTitle->setID("manage-title");
    manageContainer->addChildAtPosition(manageTitle, Anchor::Left, {}, {.0f, .5f});

    m_restartRequiredLabel = createTagLabel(
        "Restart Required",
        {to3B(ColorProvider::get()->color("mod-list-restart-required-label"_spr)),
         to3B(ColorProvider::get()->color("mod-list-restart-required-label-bg"_spr))}
    );
    m_restartRequiredLabel->setScale(.3f);
    manageContainer->addChildAtPosition(m_restartRequiredLabel, Anchor::Right, {}, {1.f, .5f});

    m_enabledStatusLabel = Label::create("", "bigFont.fnt");
    m_enabledStatusLabel->setScale(.25f);
    m_enabledStatusLabel->setOpacity(140);
    m_enabledStatusLabel->setID("enabled-status");
    manageContainer->addChildAtPosition(m_enabledStatusLabel, Anchor::Right, {}, {1.f, .5f});

    leftColumn->addChild(manageContainer);

    auto installContainer = this->createContainer({leftColumn->getContentWidth(), 25});
    m_installBG = installContainer->getChildByType<NineSlice>(0);

    m_installMenu = CCMenu::create();
    m_installMenu->ignoreAnchorPointForPosition(false);
    m_installMenu->setContentSize(installContainer->getContentSize() - CCSize{10.f, 10.f});
    m_installMenu->setAnchorPoint({.5f, .5f});

    auto updateModSpr = createGeodeButton(
        CCSprite::createWithSpriteFrameName("update.png"_spr),
        "Update",
        GeodeButtonSprite::Install,
        m_forceDisableTheme
    );
    updateModSpr->setScale(.5f);
    m_updateBtn =
        CCMenuItemSpriteExtra::create(updateModSpr, this, menu_selector(ModPopup::onInstall));
    m_updateBtn->setID("update-button");
    m_installMenu->addChild(m_updateBtn);

    auto enableModOffSpr = createGeodeButton(
        CCSprite::createWithSpriteFrameName("GJ_completesIcon_001.png"),
        "Enable",
        GeodeButtonSprite::Enable,
        m_forceDisableTheme
    );
    enableModOffSpr->setScale(.5f);
    auto enableModOnSpr = createGeodeButton(
        CCSprite::createWithSpriteFrameName("GJ_deleteIcon_001.png"),
        "Disable",
        GeodeButtonSprite::Delete,
        m_forceDisableTheme
    );
    enableModOnSpr->setScale(.5f);

    m_enableBtn = CCMenuItemToggler::create(
        enableModOffSpr, enableModOnSpr, this, menu_selector(ModPopup::onEnable)
    );
    m_enableBtn->m_notClickable = true;
    m_enableBtn->setID("enable-button");
    m_installMenu->addChild(m_enableBtn);

    auto reenableModOffSpr = createGeodeButton(
        CCSprite::createWithSpriteFrameName("reset.png"_spr),
        "Re-Enable",
        GeodeButtonSprite::Default,
        m_forceDisableTheme
    );
    reenableModOffSpr->setScale(.5f);
    auto reenableModOnSpr = createGeodeButton(
        CCSprite::createWithSpriteFrameName("reset.png"_spr), "Re-Disable", GeodeButtonSprite::Default
    );
    reenableModOnSpr->setScale(.5f);

    m_reenableBtn = CCMenuItemToggler::create(
        reenableModOffSpr, reenableModOnSpr, this, menu_selector(ModPopup::onEnable)
    );
    m_reenableBtn->m_notClickable = true;
    m_reenableBtn->setID("reenable-button");
    m_installMenu->addChild(m_reenableBtn);

    auto unavailableSpr = createGeodeButton(
        CCSprite::createWithSpriteFrameName("exclamation.png"_spr),
        "Unavailable",
        GeodeButtonSprite::Gray,
        m_forceDisableTheme
    );
    unavailableSpr->setColor({155, 155, 155});
    unavailableSpr->setScale(.5f);

    m_unavailableBtn = CCMenuItemSpriteExtra::create(unavailableSpr, this, nullptr);
    m_unavailableBtn->setID("unavailable-button");
    m_unavailableBtn->setEnabled(false);
    m_installMenu->addChild(m_unavailableBtn);

    auto installModSpr = createGeodeButton(
        CCSprite::createWithSpriteFrameName("GJ_downloadsIcon_001.png"),
        "Install",
        GeodeButtonSprite::Install,
        m_forceDisableTheme
    );
    installModSpr->setScale(.5f);
    m_installBtn =
        CCMenuItemSpriteExtra::create(installModSpr, this, menu_selector(ModPopup::onInstall));
    m_installBtn->setID("install-button");
    m_installMenu->addChild(m_installBtn);

    auto uninstallModSpr = createGeodeButton(
        CCSprite::createWithSpriteFrameName("delete-white.png"_spr),
        "Uninstall",
        GeodeButtonSprite::Default,
        m_forceDisableTheme
    );
    uninstallModSpr->setScale(.5f);
    m_uninstallBtn =
        CCMenuItemSpriteExtra::create(uninstallModSpr, this, menu_selector(ModPopup::onUninstall));
    m_uninstallBtn->setID("uninstall-button");
    m_installMenu->addChild(m_uninstallBtn);

    auto cancelDownloadSpr = createGeodeButton(
        CCSprite::createWithSpriteFrameName("GJ_deleteIcon_001.png"),
        "Cancel",
        GeodeButtonSprite::Default,
        m_forceDisableTheme
    );
    cancelDownloadSpr->setScale(.5f);
    m_cancelBtn = CCMenuItemSpriteExtra::create(
        cancelDownloadSpr, this, menu_selector(ModPopup::onCancelDownload)
    );
    m_cancelBtn->setID("cancel-button");
    m_installMenu->addChild(m_cancelBtn);

    m_installStatusLabel = Label::create("", "bigFont.fnt");
    m_installStatusLabel->setOpacity(120);
    m_installStatusLabel->setVisible(false);
    m_installMenu->addChild(m_installStatusLabel);

    m_installMenu->setLayout(
        RowLayout::create()->setDefaultScaleLimits(.1f, 1)->setAxisAlignment(AxisAlignment::Center)
    );
    installContainer->addChildAtPosition(m_installMenu, Anchor::Center);

    leftColumn->addChild(installContainer);

    // Links

    auto linksContainer = this->createContainer({leftColumn->getContentWidth() - 40, 30});

    auto linksMenu = CCMenu::create();
    linksMenu->ignoreAnchorPointForPosition(false);
    linksMenu->setContentSize(linksContainer->getContentSize() - CCSize{10.f, 10.f});
    linksMenu->setAnchorPoint({.5f, .5f});
    linksMenu->setID("links-container");

    auto useGithubIcon = m_source.getMetadata()
                             .getLinks()
                             .getSourceURL()
                             .transform([](auto const& url) {
                                 // bad logic
                                 auto hostBegin = url.find_first_of("://");
                                 if (hostBegin == std::string::npos) {
                                     return false;
                                 }

                                 hostBegin += 3;

                                 auto hostEnd = url.find_first_of('/', hostBegin);
                                 if (hostEnd == std::string::npos) {
                                     hostEnd = url.size();
                                 }

                                 auto hostname = url.substr(hostBegin, hostEnd - hostBegin);
                                 utils::string::toLowerIP(hostname);

                                 // if you add a port or username to your source url, it's your
                                 // fault and you have to fix the parsing

                                 // accept www.github.com and github.com
                                 return hostname == "github.com" || hostname == "www.github.com";
                             })
                             .value_or(false);

    // alias for ccmenuitemext callbacks
    using CCMIExtCallback = Function<void(CCMenuItemSpriteExtra*)>;

    struct ModLinkButton {
        std::string id; // 0
        std::string sprite; // 1
        std::optional<std::string> url; // 2
        CCMIExtCallback callback = nullptr; // 3
    };

    auto linkButtons = std::array{
        ModLinkButton{
            "homepage",
            "homepage.png"_spr,
            m_source.getMetadata().getLinks().getHomepageURL(),
        },
        ModLinkButton{
            "github",
            useGithubIcon ? "github.png"_spr : "source_generic.png"_spr,
            m_source.getMetadata().getLinks().getSourceURL(),
        },
        ModLinkButton{
            "discord",
            "gj_discordIcon_001.png",
            m_source.getMetadata().getLinks().getCommunityURL(),
        },
        ModLinkButton{
            "support",
            "gift.png"_spr,
            m_source.getMetadata().getSupportInfo(),
            [this](CCMenuItemSpriteExtra* sender) {
                openSupportPopup(m_source.getMetadata());
            },
        },
    };

    for (auto& btn : linkButtons) {
        auto spr = CCSprite::createWithSpriteFrameName(btn.sprite.c_str());
        spr->setScale(.75f);

        if (!btn.url.has_value()) {
            spr->setColor({155, 155, 155});
            spr->setOpacity(155);
        }

        CCMIExtCallback handler = btn.url.has_value() ?
            (btn.callback ? std::move(btn.callback) :
                            [url = *btn.url](auto) {
                                web::openLinkInBrowser(url);
                            }) :
            nullptr;

        auto button = CCMenuItemExt::createSpriteExtra(spr, std::move(handler));
        button->setEnabled(btn.url.has_value());

        if (btn.url.has_value()) {
            button->setUserObject("url", CCString::create(*btn.url));
        }

        button->setID(std::move(btn.id));
        linksMenu->addChild(button);
    }

    linksMenu->setLayout(
        RowLayout::create()->setDefaultScaleLimits(.1f, 1)->setAxisAlignment(AxisAlignment::Center)
    );
    linksContainer->addChildAtPosition(linksMenu, Anchor::Center);

    leftColumn->addChild(linksContainer);

    leftColumn->setLayout(
        ColumnLayout::create()
            ->setAxisReverse(true)
            ->setCrossAxisOverflow(false)
            ->setAutoScale(false)
            ->setAxisAlignment(AxisAlignment::Start)
            ->setCrossAxisLineAlignment(AxisAlignment::Start)
            ->setGap(4)
    );
    leftColumn->setID("left-column");
    mainContainer->addChild(leftColumn);

    m_rightColumn = CCNode::create();
    m_rightColumn->setContentSize(
        {// The -5 is to give a little bit of padding
         mainContainer->getContentWidth() - leftColumn->getContentWidth() -
             static_cast<AxisLayout*>(mainContainer->getLayout())->getGap(),
         mainContainer->getContentHeight()
        }
    );

    auto tabsMenu = CCMenu::create();
    tabsMenu->ignoreAnchorPointForPosition(false);
    tabsMenu->setScale(.65f);
    tabsMenu->setContentWidth(m_rightColumn->getContentWidth() / tabsMenu->getScale());
    tabsMenu->setAnchorPoint({.5f, 1.f});
    tabsMenu->setID("tabs-menu");

    struct ModTab {
        std::string sprite; // 0
        std::string label; // 1
        std::string id; // 2
        Tab tab; // 3
    };

    auto mdTabs = std::array{
        ModTab{
            "message.png"_spr,
            "Description",
            "description",
            Tab::Details,
        },
        ModTab{
            "changelog.png"_spr,
            "Changelog",
            "changelog",
            Tab::Changelog,
        },
    };

    for (auto& tab : mdTabs) {
        auto spr = GeodeTabSprite::create(tab.sprite, std::move(tab.label), 140, m_source.asServer());

        auto btn = CCMenuItemSpriteExtra::create(spr, this, menu_selector(ModPopup::onTab));
        btn->setTag(static_cast<int>(tab.tab));
        btn->setID(std::move(tab.id));

        tabsMenu->addChild(btn);

        m_tabs.insert({tab.tab, {spr, nullptr}});
    }

    // placeholder external link until versions tab is implemented
    auto modUrl = fmt::format("https://geode-sdk.org/mods/{}", m_source.getID());
    auto externalLinkSpr = CCSprite::createWithSpriteFrameName("external-link.png"_spr);

    externalLinkSpr->setScale(0.35f);
    externalLinkSpr->setOpacity(127);

    auto externalLinkBtn = CCMenuItemExt::createSpriteExtra(externalLinkSpr, [modUrl](auto) {
        web::openLinkInBrowser(modUrl);
    });
    externalLinkBtn->setID("mod-online-page-button");
    externalLinkBtn->setUserObject("url", CCString::create(modUrl));
    m_buttonMenu->addChildAtPosition(externalLinkBtn, Anchor::TopRight, {-14.f, -16.f});

    if (isGeode) {
        externalLinkBtn->setVisible(false);
    }

    tabsMenu->setLayout(RowLayout::create()->setAxisAlignment(AxisAlignment::Start));
    m_rightColumn->addChildAtPosition(tabsMenu, Anchor::Top);

    m_rightColumn->setID("right-column");
    mainContainer->addChildAtPosition(m_rightColumn, Anchor::Right, {-20.f, .0f});

    mainContainer->updateLayout();
    m_mainLayer->addChildAtPosition(mainContainer, Anchor::Center);

    m_settingsBG = NineSlice::create("square02b_001.png");
    m_settingsBG->setColor({0, 0, 0});
    m_settingsBG->setOpacity(75);
    m_settingsBG->setScaleMultiplier(.3f);
    m_settingsBG->setContentSize({35.f, 30.f});
    m_buttonMenu->addChildAtPosition(m_settingsBG, Anchor::BottomLeft, {28., 25.f});

    auto settingsSpr = createGeodeCircleButton(
        CCSprite::createWithSpriteFrameName("settings.png"_spr),
        1.f,
        CircleBaseSize::Medium,
        false,
        m_forceDisableTheme
    );
    settingsSpr->setScale(.6f);
    auto settingsBtn =
        CCMenuItemSpriteExtra::create(settingsSpr, this, menu_selector(ModPopup::onSettings));
    settingsBtn->setID("settings-button");
    m_buttonMenu->addChildAtPosition(settingsBtn, Anchor::BottomLeft, {28.f, 25.f});

    if (!m_source.asMod() || !m_source.asMod()->hasSettings()) {
        settingsSpr->setOpacity(90);
        static_cast<CCSprite*>(settingsSpr->getTopNode())->setOpacity(90);

        settingsSpr->setColor(ccGRAY);
        static_cast<CCSprite*>(settingsSpr->getTopNode())->setColor(ccGRAY);

        settingsBtn->setEnabled(false);
    }

    // Select details tab
    this->loadTab(Tab::Details);

    // Update enabling and installing buttons
    this->updateState();

    // Load stats from server (or just from the source if it already has them)
    m_statsListener.spawn("ModPopup stats listener", m_source.fetchServerInfo(), [this](auto res) {
        this->onLoadServerInfo(std::move(res));
    });
    m_tagsListener.spawn("ModPopup tags listener", m_source.fetchValidTags(), [this](auto res) {
        this->onLoadTags(std::move(res));
    });

    if (m_source.asMod()) {
        m_checkUpdateListener.spawn(
            "ModPopup update checker", m_source.checkUpdates(), [this](auto res) {
                this->onCheckUpdates(std::move(res));
            }
        );
    }
    else {
        auto updatesStat = m_stats->getChildByID("update-check");
        this->setStatLabel(updatesStat, "Not Installed", true, ccc3(125, 125, 125));
    }

    // Only listen for updates on this mod specifically
    m_updateStateHandle = UpdateModListStateEvent().listen([this](UpdateState const& state) {
        this->updateState();
        return ListenerResult::Propagate;
    });

    m_downloadListener = server::ModDownloadEvent(m_source.getID()).listen([this]() {
        this->updateState();
        return ListenerResult::Propagate;
    });

    m_source.visit(
        makeVisitor{
            [this](Mod* mod) {
                m_settingNodeHandle = SettingNodeValueChangeEvent().listen(
                    [this](std::string_view modID, std::string_view key, SettingNodeV3*, bool isCommit) {
                        if (!isCommit) {
                            return ListenerResult::Propagate;
                        }
                        this->updateState();
                        return ListenerResult::Propagate;
                    }
                );
            },
            [this](server::ServerModMetadata const& metadata) {

            }
        }
    );

    return true;
}

CCNode* ModPopup::createContainer(CCSize const& size) {
    auto container = CCNode::create();
    container->setContentSize(size);
    container->setAnchorPoint({.5f, .5f});
    container->setLayoutOptions(AxisLayoutOptions::create()->setCrossAxisAlignment(AxisAlignment::End));

    auto bg = NineSlice::create("square02b_001.png");
    bg->setColor({0, 0, 0});
    bg->setOpacity(75);
    bg->setScaleMultiplier(.3f);
    bg->setContentSize(container->getContentSize());
    container->addChildAtPosition(bg, Anchor::Center);

    return container;
}

void ModPopup::updateState() {
    auto asMod = m_source.asMod();
    auto wantsRestart = m_source.wantsRestart();
    auto wantsRestartBecauseOfSettings = asMod && ModSettingsManager::from(asMod)->restartRequired();

    m_installBG->setColor(
        (wantsRestart && !wantsRestartBecauseOfSettings) ?
            to3B(ColorProvider::get()->color("mod-list-restart-required-label"_spr)) :
            ccBLACK
    );
    m_installBG->setOpacity((wantsRestart && !wantsRestartBecauseOfSettings) ? 40 : 75);
    m_settingsBG->setColor(
        wantsRestartBecauseOfSettings ?
            to3B(ColorProvider::get()->color("mod-list-restart-required-label"_spr)) :
            ccBLACK
    );
    m_settingsBG->setOpacity(wantsRestartBecauseOfSettings ? 40 : 75);
    m_settingsBG->setVisible(wantsRestartBecauseOfSettings);
    m_restartRequiredLabel->setVisible(wantsRestart);

    if (!wantsRestart && asMod) {
        if (asMod->isLoaded()) {
            m_enabledStatusLabel->setText("Enabled");
            m_enabledStatusLabel->setColor(to3B(ColorProvider::get()->color("mod-list-enabled"_spr)));
        }
        else {
            m_enabledStatusLabel->setText("Disabled");
            m_enabledStatusLabel->setColor(to3B(ColorProvider::get()->color("mod-list-disabled"_spr)));
        }
        m_enabledStatusLabel->setVisible(true);
    }
    else {
        m_enabledStatusLabel->setVisible(false);
    }

    m_cancelBtn->setVisible(false);

    m_enableBtn->toggle(asMod && asMod->isOrWillBeEnabled());
    m_enableBtn->setVisible(
        asMod && asMod->getRequestedAction() == ModRequestedAction::None &&
        !asMod->targetsOutdatedVersion()
    );

    m_reenableBtn->toggle(m_enableBtn->isToggled());
    m_reenableBtn->setVisible(asMod && modRequestedActionIsToggle(asMod->getRequestedAction()));

    m_updateBtn->setVisible(
        m_source.hasUpdates().update.has_value() &&
        asMod->getRequestedAction() == ModRequestedAction::None
    );
    m_installBtn->setVisible(this->availableForInstall());
    m_unavailableBtn->setVisible(m_source.asServer() && !this->availableForInstall());
    m_uninstallBtn->setVisible(asMod && asMod->getRequestedAction() == ModRequestedAction::None);

    if (asMod && modRequestedActionIsUninstall(asMod->getRequestedAction())) {
        m_installStatusLabel->setText("Mod has been uninstalled");
        m_installStatusLabel->setVisible(true);
    }
    else {
        m_installStatusLabel->setText("");
        m_installStatusLabel->setVisible(false);
    }

    if (m_enableBtn->isVisible() && m_updateBtn->isVisible() && m_uninstallBtn->isVisible()) {
        static_cast<IconButtonSprite*>(m_enableBtn->m_onButton->getNormalImage())->setString("");
        static_cast<IconButtonSprite*>(m_enableBtn->m_offButton->getNormalImage())->setString("");
        static_cast<IconButtonSprite*>(m_uninstallBtn->getNormalImage())->setString("");
    }
    else {
        static_cast<IconButtonSprite*>(m_enableBtn->m_onButton->getNormalImage())
            ->setString("Disable");
        static_cast<IconButtonSprite*>(m_enableBtn->m_offButton->getNormalImage())
            ->setString("Enable");
        static_cast<IconButtonSprite*>(m_uninstallBtn->getNormalImage())->setString("Uninstall");
    }
    m_enableBtn->updateSprite();
    m_uninstallBtn->updateSprite();

    if (asMod && asMod->isInternal()) {
        m_enableBtn->setVisible(false);
// you can uninstall loader ingame just fine on windows
#if !defined(GEODE_IS_WINDOWS)
        m_uninstallBtn->setVisible(false);
        m_installStatusLabel->setText("N/A");
        m_installStatusLabel->setVisible(true);
#endif
    }

    auto download = server::ModDownloadManager::get()->getDownload(m_source.getID());
    if (download) {
        if (download->isActive()) {
            m_enableBtn->setVisible(false);
            m_reenableBtn->setVisible(false);
            m_updateBtn->setVisible(false);
            m_installBtn->setVisible(false);
            m_uninstallBtn->setVisible(false);
            m_cancelBtn->setVisible(true);

            auto status = download->getStatus();
            if (auto d = std::get_if<server::DownloadStatusDownloading>(&status)) {
                m_enabledStatusLabel->setText(fmt::format("Downloading {}%", d->percentage));
                m_enabledStatusLabel->setColor(ccWHITE);
                // todo: progress bar
            }
            else {
                m_enabledStatusLabel->setText("Preparing");
                m_enabledStatusLabel->setColor(ccWHITE);
                // todo: spinner
            }
        }
        else {
            std::visit(
                makeVisitor{
                    [this](server::DownloadStatusError const& e) {
                        m_enabledStatusLabel->setText("Error");
                        m_enabledStatusLabel->setColor(
                            to3B(ColorProvider::get()->color("mod-list-disabled"_spr))
                        );
                        // todo: show error details somewhere (like an info button)
                    },
                    [this](server::DownloadStatusCancelled const&) {
                        m_enabledStatusLabel->setText("Cancelled");
                        m_enabledStatusLabel->setColor(
                            to3B(ColorProvider::get()->color("mod-list-disabled"_spr))
                        );
                    },
                    [this](server::DownloadStatusDone const&) {
                        m_enableBtn->setVisible(false);
                        m_reenableBtn->setVisible(false);
                        m_updateBtn->setVisible(false);
                        m_installBtn->setVisible(false);
                        m_uninstallBtn->setVisible(false);
                        m_cancelBtn->setVisible(false);

                        m_installStatusLabel->setText("Mod has been installed");
                        m_installStatusLabel->setVisible(true);
                    },
                    // rest are unreachable due to the isActive() check
                    [this](auto const&) {},
                },
                download->getStatus()
            );
        }
    }

    m_installMenu->updateLayout();

    ModPopupUIEvent().send(this, m_source.getID(), std::nullopt);
}

void ModPopup::setStatIcon(CCNode* stat, ZStringView spr) {
    // Remove old icon
    stat->removeChildByID("icon");

    // Create new icon
    auto iconSize = stat->getContentHeight();
    auto icon = CCSprite::createWithSpriteFrameName(spr.c_str());
    limitNodeSize(icon, {iconSize, iconSize}, 1.f, .1f);
    icon->setID("icon");
    stat->addChildAtPosition(icon, Anchor::Left, {stat->getContentHeight() / 2.f, .0f});
}

void ModPopup::setStatLabel(CCNode* stat, std::string value, bool noValue, ccColor3B color) {
    auto container = stat->getChildByID("labels");

    // Update label
    auto label = static_cast<Label*>(container->getChildByID("label"));
    label->setText(std::move(value));
    label->setColor(color);

    // Remove value if requested
    if (noValue) {
        container->getChildByID("value-label")->setVisible(false);
        container->getChildByID("loading-spinner")->setVisible(false);
    }

    container->updateLayout();
}

void ModPopup::setStatValue(CCNode* stat, std::optional<std::string> value) {
    auto container = stat->getChildByID("labels");
    auto valueLabel = static_cast<Label*>(container->getChildByID("value-label"));
    auto spinner = container->getChildByID("loading-spinner");

    // Show loading if no value provided
    valueLabel->setVisible(value.has_value());
    spinner->setVisible(!value.has_value());

    // Update value
    if (value.has_value()) {
        valueLabel->setText(std::move(value).value());
    }

    // Update layout
    container->updateLayout();
}

// helper class for making an std::locale
class comma_numpunct : public std::numpunct<char> {
protected:
    virtual char do_thousands_sep() const {
        return ',';
    }

    virtual std::string do_grouping() const {
        return "\03";
    }
};

void ModPopup::onLoadServerInfo(server::ServerResult<server::ServerModMetadata> result) {
    if (result.isOk()) {
        auto data = std::move(result).unwrap();
        auto timeToString = [](auto const& time) {
            if (time.has_value()) {
                return utils::timeToAgoString(time->value);
            }
            return std::string("N/A");
        };

        static std::locale commaLocale(std::locale(), new comma_numpunct());

        struct ModStats {
            std::string id;
            std::string value;
        };

        auto stats = std::array{
            ModStats{
                "downloads",
                fmt::format(commaLocale, "{:L}", data.downloadCount),
            },
            ModStats{
                "release-date",
                timeToString(data.createdAt),
            },
            ModStats{
                "update-date",
                timeToString(data.updatedAt),
            },
        };

        // Update server stats
        for (auto& s : stats) {
            if (auto stat = m_stats->getChildByID(s.id)) {
                this->setStatValue(stat, std::move(s.value));
            }
        }
        ModPopupUIEvent().send(this, m_source.getID(), std::nullopt);
    }
    else {
        for (auto child : CCArrayExt<CCNode*>(m_stats->getChildren())) {
            if (child->getUserObject("stats")) {
                this->setStatValue(child, "N/A");
            }
        }
        ModPopupUIEvent().send(this, m_source.getID(), std::nullopt);
    }
}

void ModPopup::onCheckUpdates(server::ServerResult<server::ServerModUpdateOneCheck> result) {
    if (result.isOk()) {
        auto resolved = std::move(result).unwrap();
        // Check if this has updates for an installed mod
        auto updatesStat = m_stats->getChildByID("update-check");
        if (resolved.update.has_value()) {
            this->setStatIcon(updatesStat, "updates-available.png"_spr);
            this->setStatLabel(
                updatesStat,
                "Update Found",
                false,
                ColorProvider::get()->color3b("mod-list-version-label-updates-available"_spr)
            );
            this->setStatValue(updatesStat, resolved.update->version.toVString());
            this->updateState();
        }
        else {
            this->setStatIcon(updatesStat, "GJ_completesIcon_001.png");
            this->setStatLabel(
                updatesStat,
                "Up to Date!",
                true,
                ColorProvider::get()->color3b("mod-list-version-label"_spr)
            );
        }
    }
    else {
        auto updatesStat = m_stats->getChildByID("update-check");
        this->setStatLabel(updatesStat, "No Updates Found", true, ccc3(125, 125, 125));
    }
}

void ModPopup::onLoadTags(server::ServerResult<std::vector<server::ServerTag>> result) {
    if (result.isOk()) {
        auto data = std::move(result).unwrap();
        m_tags->removeAllChildren();

        for (auto& tag : data) {
            m_tags->addChild(createGeodeTagLabel(tag));
        }

        if (data.empty()) {
            auto label = Label::create("No tags found", "bigFont.fnt");
            label->setOpacity(120);
            m_tags->addChild(label);
        }
        // This should probably be kept even after modtober ends,
        // so the banner sprite must be kept
        // If the build times from the cool popup become too long then we can
        // probably move that to a normal FLAlert that explains "Modtober was
        // this contest blah blah this mod was made for it"
        else if (auto tag = ranges::find(
                     data,
                     [](auto const& tag) {
                         return tag.name.starts_with("modtober");
                     }
                 );
                 tag.has_value()) {
            auto year = tag->name.substr(tag->name.size() - 2);
            auto menu = CCMenu::create();
            menu->setID("modtober-banner");
            menu->ignoreAnchorPointForPosition(false);
            menu->setContentSize({m_rightColumn->getContentWidth(), 25});

            auto banner = CCSprite::createWithSpriteFrameName(
                fmt::format("modtober{}-banner-2.png"_spr, year).c_str()
            );
            limitNodeWidth(banner, m_rightColumn->getContentWidth(), 1.f, .1f);
            menu->addChildAtPosition(banner, Anchor::Center);

            auto label = Label::create(fmt::format("Entry for Modtober 20{}", year), "bigFont.fnt");
            label->setScale(.35f);
            menu->addChildAtPosition(label, Anchor::Left, {10.f, .0f}, {.0f, .5f});

            auto aboutSpr =
                createGeodeButton("About", false, GeodeButtonSprite::Default, m_forceDisableTheme);
            aboutSpr->setScale(.35f);
            auto aboutBtn = CCMenuItemSpriteExtra::create(
                aboutSpr,
                this,
                year == "24" ? menu_selector(ModPopup::onModtober24Info) :
                               menu_selector(ModPopup::onModtober25Info)
            );
            menu->addChildAtPosition(aboutBtn, Anchor::Right, {-25.f, .0f});

            m_rightColumn->addChildAtPosition(menu, Anchor::Bottom, {}, {.5f, .0f});

            m_modtoberBanner = menu;

            // Force reload of all the tabs since otherwise their contents will overflow
            for (auto& [_, tab] : m_tabs) {
                if (tab.second && tab.second->getParent()) {
                    tab.second->removeFromParent();
                }
                tab.second = nullptr;
            }

            m_currentTabPage = nullptr;

            // This might cause a minor inconvenience to someone who opens the popup and
            // immediately switches to changelog but is then forced back into details
            this->loadTab(Tab::Details);
        }

        m_tags->updateLayout();

        ModPopupUIEvent().send(this, m_source.getID(), std::nullopt);
    }
    else {
        m_tags->removeAllChildren();

        auto label = Label::create("No tags found", "bigFont.fnt");
        label->setOpacity(120);
        m_tags->addChild(label);

        m_tags->updateLayout();

        ModPopupUIEvent().send(this, m_source.getID(), std::nullopt);
    }
}

void ModPopup::loadTab(ModPopup::Tab tab) {
    // Remove current page
    if (m_currentTabPage) {
        m_currentTabPage->removeFromParent();
    }

    // Highlight selected tab
    for (auto [value, btn] : m_tabs) {
        btn.first->select(value == tab);
    }

    float modtoberBannerHeight = 0;
    if (m_modtoberBanner) {
        modtoberBannerHeight = 30;
    }

    if (auto existing = m_tabs.at(tab).second) {
        m_currentTabPage = existing;
        m_rightColumn->addChildAtPosition(existing, Anchor::Bottom, {.0f, modtoberBannerHeight});
    }
    else {
        auto const size =
            (m_rightColumn->getContentSize() - CCSize{.0f, 30.f + modtoberBannerHeight});
        float const mdScale = .85f;
        switch (tab) {
            case Tab::Details: {
                m_currentTabPage = FetchTextArea::create(
                    m_source.fetchAbout(), "No description provided", size / mdScale
                );
                m_currentTabPage->setID("description-container");
                m_currentTabPage->setScale(mdScale);
            } break;

            case Tab::Changelog: {
                m_currentTabPage = FetchTextArea::create(
                    m_source.fetchChangelog(), "No changelog provided", size / mdScale
                );
                m_currentTabPage->setID("changelog-container");
                m_currentTabPage->setScale(mdScale);
            } break;

            case Tab::Versions: {
                m_currentTabPage = CCNode::create();
                m_currentTabPage->setContentSize(size);
                m_currentTabPage->setID("versions-container");
            } break;
        }
        m_currentTabPage->setAnchorPoint({.5f, .0f});
        m_rightColumn->addChildAtPosition(
            m_currentTabPage, Anchor::Bottom, {0.f, modtoberBannerHeight}
        );
        m_tabs.at(tab).second = m_currentTabPage;
    }
}

void ModPopup::onTab(CCObject* sender) {
    this->loadTab(static_cast<Tab>(sender->getTag()));
}

void ModPopup::onEnable(CCObject*) {
    m_source.requestEnable();
}

void ModPopup::onInstall(CCObject*) {
    // A futile attempt to try and prevent users from blindly installing
    // everything on the index
    if (m_source.asServer() && Loader::get()->getAllMods().size() > 10) {
        if (!Mod::get()->setSavedValue("shown-dont-install-everything-warning", true)) {
            FLAlertLayer::create(
                "Warning",
                "Installing too many mods can lead to <cr>instability</c>, <cr>bugs</c>, and "
                "<cr>crashes</c>. <cy>You should only install mods you need!</c>",
                "OK"
            )
                ->show();
            return;
        }
    }

    m_source.startInstall();

    this->onClose(nullptr);
}

void ModPopup::onUninstall(CCObject*) {
    if (auto mod = m_source.asMod()) {
        ConfirmUninstallPopup::create(mod)->show();
    }
    else {
        FLAlertLayer::create(
            "Error Uninstalling Mod",
            "This mod can not be uninstalled! (It is not installed at all)",
            "OK"
        )
            ->show();
    }
}

void ModPopup::onCancelDownload(CCObject*) {
    auto download = server::ModDownloadManager::get()->getDownload(m_source.getID());
    if (download) {
        download->cancel();
    }
}

void ModPopup::onSettings(CCObject*) {
    if (auto mod = m_source.asMod()) {
        ModSettingsPopup::create(mod)->show();
    }
}

void ModPopup::onLink(CCObject* sender) {
    auto url = static_cast<CCString*>(static_cast<CCNode*>(sender)->getUserObject("url"));
    web::openLinkInBrowser(url->getCString());
}

void ModPopup::onModtober24Info(CCObject*) {
    FLAlertLayer::create(
        "Modtober 2024",
        "This mod was an entry for <cj>Modtober 2024</c>, a contest to create "
        "the best mod with the theme <cc>\"The Sky's the Limit\"</c>",
        "OK"
    )
        ->show();
}

void ModPopup::onModtober25Info(CCObject*) {
    FLAlertLayer::create(
        "Modtober 2025",
        "This mod was an entry for <co>Modtober 2025</c>, a contest to create "
        "the best mod with the theme <cc>\"What The Heck!?\"</c>",
        "OK"
    )
        ->show();
}

ModPopup* ModPopup::create(ModSource&& src) {
    auto ret = new ModPopup();
    if (ret->init(std::move(src))) {
        ret->autorelease();
        return ret;
    }
    delete ret;
    return nullptr;
}

ModSource& ModPopup::getSource() & {
    return m_source;
}

bool ModPopup::availableForInstall() const {
    // Adapted from ModItem.cpp ModItem::onView
    if (auto serverSource = m_source.asServer()) {
        auto& version = serverSource->latestVersion();
        auto gameVersion = version.getGameVersion();

        return (!gameVersion || gameVersion == "*" || gameVersion == GEODE_STR(GEODE_GD_VERSION)) &&
            Loader::get()->isModVersionSupported(version.getGeodeVersion());
    }
    return false;
}
