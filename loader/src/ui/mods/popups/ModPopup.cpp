#include "ModPopup.hpp"

#include <optional>

#include <Geode/binding/ButtonSprite.hpp>
#include <Geode/ui/MDTextArea.hpp>
#include <Geode/ui/TextInput.hpp>
#include <Geode/utils/web.hpp>
#include <Geode/loader/Event.hpp>
#include <Geode/loader/Loader.hpp>
#include <Geode/loader/ModSettingsManager.hpp>
#include <Geode/ui/GeodeUI.hpp>
#include <Geode/utils/ColorProvider.hpp>
#include <optional>
#include "ConfirmUninstallPopup.hpp"
#include "../settings/ModSettingsPopup.hpp"
#include "../../../internal/about.hpp"
#include "../../GeodeUIEvent.hpp"
#include "server/DownloadManager.hpp"

class FetchTextArea : public CCNode {
public:
    using Request = server::ServerRequest<std::optional<std::string>>;

protected:
    EventListener<Request> m_listener;
    MDTextArea* m_textarea;
    CCNode* m_loading;
    std::string m_noneText;

    bool init(Request const& req, std::string const& noneText, CCSize const& size) {
        if (!CCNode::init())
            return false;

        this->setAnchorPoint({ .5f, .5f });
        this->setContentSize(size);

        m_noneText = noneText;

        m_textarea = MDTextArea::create("", size);
        m_textarea->setID("textarea");
        this->addChildAtPosition(m_textarea, Anchor::Center);

        m_loading = createLoadingCircle(30);
        this->addChildAtPosition(m_loading, Anchor::Center);

        m_listener.bind(this, &FetchTextArea::onRequest);
        m_listener.setFilter(req);

        return true;
    }

    void onRequest(Request::Event* event) {
        if (auto* res = event->getValue(); res && res->isOk()) {
            auto value = std::move(*res).unwrap();
            if (value) {
                m_loading->removeFromParent();
                std::string str = std::move(value).value();
                m_textarea->setString(str.c_str());
                return;
            }
        }
        if (!event->getProgress()) {
            m_loading->removeFromParent();
            m_textarea->setString(m_noneText.c_str());
        }
    }

public:
    static FetchTextArea* create(Request const& req, std::string const& noneText, CCSize const& size) {
        auto ret = new FetchTextArea();
        if (ret->init(req, noneText, size)) {
            ret->autorelease();
            return ret;
        }
        delete ret;
        return nullptr;
    }
};

bool ModPopup::setup(ModSource&& src) {
    m_source = std::move(src);
    m_noElasticity = true;

    this->setID(std::string(Mod::get()->expandSpriteName(fmt::format("popup-{}", src.getID()))));

    auto isGeode = src.asMod() == Mod::get();
    if (isGeode) {
        // Display commit hashes
        auto loaderHash = about::getLoaderCommitHash();
        auto bindingsHash = about::getBindingsCommitHash();

        auto string = fmt::format("Loader: {}, Bindings: {}", loaderHash, bindingsHash);
        auto hashLabel = CCLabelBMFont::create(string.c_str(), "chatFont.fnt");
        hashLabel->setAnchorPoint({ .5f, 1.f });
        hashLabel->setOpacity(51);
        hashLabel->setScale(.7f);
        m_mainLayer->addChildAtPosition(hashLabel, Anchor::Bottom, ccp(0, -5));
    }

    auto mainContainer = CCNode::create();
    mainContainer->setContentSize(m_mainLayer->getContentSize() - ccp(20, 20));
    mainContainer->setAnchorPoint({ .5f, .5f });
    mainContainer->setLayout(
        RowLayout::create()
            ->setGap(7.5f)
            ->setCrossAxisLineAlignment(AxisAlignment::Start)
    );

    auto leftColumn = CCNode::create();
    leftColumn->setContentSize({ 145, mainContainer->getContentHeight() });

    m_titleContainer = CCMenu::create();
    m_titleContainer->setContentSize({ leftColumn->getContentWidth(), 25 });
    m_titleContainer->setAnchorPoint({ .5f, .5f });

    auto getLogo = [this]() {
        auto logo = m_source.createModLogo();
        logo->setID("mod-logo");
        limitNodeSize(
            logo,
            ccp(m_titleContainer->getContentHeight(), m_titleContainer->getContentHeight()),
            5.f, .1f
        );
        return logo;
    };

    if (isGeode) {
        auto counter = Mod::get()->getSavedValue("alternate-geode-style", false) ? 3 : 0;
        auto* logoButton = CCMenuItemExt::createSpriteExtra(getLogo(), [this, counter, getLogo](auto* item) mutable {
            counter++;
            if (counter % 6 == 3) {
                Mod::get()->setSavedValue("alternate-geode-style", true);
                ColorProvider::get()->override("swelvy-bg-0"_spr, { 216, 132, 132, 255 });
                ColorProvider::get()->override("swelvy-bg-1"_spr, { 210, 189, 119, 255 });
                ColorProvider::get()->override("swelvy-bg-2"_spr, { 195, 212, 136, 255 });
                ColorProvider::get()->override("swelvy-bg-3"_spr, { 95, 184, 134, 255 });
                ColorProvider::get()->override("swelvy-bg-4"_spr, { 100, 174, 189, 255 });
                ColorProvider::get()->override("swelvy-bg-5"_spr, { 118, 90, 148, 255 });
            }
            else if (counter % 6 == 0) {
                Mod::get()->getSaveContainer().erase("alternate-geode-style");
                for (int i = 0; i < 6; i++) {
                    ColorProvider::get()->reset(fmt::format("swelvy-bg-{}"_spr, i));
                }
            }
        });
        m_titleContainer->addChildAtPosition(
            logoButton, Anchor::Left, ccp(m_titleContainer->getContentHeight() / 2, 0)
        );
    }
    else {
        m_titleContainer->addChildAtPosition(
            getLogo(), Anchor::Left, ccp(m_titleContainer->getContentHeight() / 2, 0)
        );
    }

    // Lil padding
    auto devAndTitlePos = m_titleContainer->getContentHeight() + 5;

    auto title = CCLabelBMFont::create(m_source.getMetadata().getName().c_str(), "bigFont.fnt");
    title->limitLabelWidth(m_titleContainer->getContentWidth() - devAndTitlePos, .45f, .1f);
    title->setAnchorPoint({ .0f, .5f });
    title->setID("mod-name-label");
    m_titleContainer->addChildAtPosition(title, Anchor::TopLeft, ccp(devAndTitlePos, -m_titleContainer->getContentHeight() * .25f));

    auto by = "By " + m_source.formatDevelopers();
    auto dev = CCLabelBMFont::create(by.c_str(), "goldFont.fnt");
    dev->limitLabelWidth(m_titleContainer->getContentWidth() - devAndTitlePos, .35f, .05f);
    dev->setAnchorPoint({ .0f, .5f });
    dev->setID("mod-developer-label");
    m_titleContainer->addChildAtPosition(dev, Anchor::BottomLeft, ccp(devAndTitlePos, m_titleContainer->getContentHeight() * .25f));

    // Suggestions
    if (!Loader::get()->isModInstalled(m_source.getMetadata().getID())) {
        std::vector<Mod*> recommends {};
        for (auto& problem : Loader::get()->getRecommendations()) {
            auto suggestionID = problem.message.substr(0, problem.message.find(' '));
            if (suggestionID != m_source.getMetadata().getID()) {
                continue;
            }
            recommends.push_back(std::get<2>(problem.cause));
        }

        if (recommends.size() > 0) {
            title->updateAnchoredPosition(Anchor::TopLeft, ccp(devAndTitlePos, -2));
            dev->updateAnchoredPosition(Anchor::Left, ccp(devAndTitlePos, 0));

            auto recommendedBy = CCNode::create();
            recommendedBy->setContentWidth(m_titleContainer->getContentWidth() - devAndTitlePos);
            recommendedBy->setAnchorPoint({ .0f, .5f });

            auto byLabel = CCLabelBMFont::create("Recommended by ", "bigFont.fnt");
            byLabel->setColor("mod-list-recommended-by"_cc3b);
            recommendedBy->addChild(byLabel);

            std::string suggestionStr {};
            if (recommends.size() == 1) {
                suggestionStr = recommends[0]->getName();
            } else {
                suggestionStr = fmt::format("{} installed mods", recommends.size());
            }

            auto nameLabel = CCLabelBMFont::create(suggestionStr.c_str(), "bigFont.fnt");
            nameLabel->setColor("mod-list-recommended-by-2"_cc3b);
            recommendedBy->addChild(nameLabel);

            recommendedBy->setLayout(
                RowLayout::create()
                    ->setDefaultScaleLimits(.1f, 1.f)
                    ->setAxisAlignment(AxisAlignment::Start)
            );
            m_titleContainer->addChildAtPosition(recommendedBy, Anchor::BottomLeft, ccp(devAndTitlePos, 4));
        }
    }

    leftColumn->addChild(m_titleContainer);

    auto idStr = "(ID: " + m_source.getMetadata().getID() + ")";
    auto idLabel = CCLabelBMFont::create(idStr.c_str(), "bigFont.fnt");
    idLabel->limitLabelWidth(leftColumn->getContentWidth(), .25f, .05f);
    idLabel->setColor({ 150, 150, 150 });
    idLabel->setOpacity(140);
    idLabel->setID("mod-id-label");
    leftColumn->addChild(idLabel);

    auto statsContainer = CCNode::create();
    statsContainer->setContentSize({ leftColumn->getContentWidth(), 80 });
    statsContainer->setAnchorPoint({ .5f, .5f });

    auto statsBG = CCScale9Sprite::create("square02b_001.png");
    statsBG->setColor({ 0, 0, 0 });
    statsBG->setOpacity(75);
    statsBG->setScale(.3f);
    statsBG->setContentSize(statsContainer->getContentSize() / statsBG->getScale());
    statsContainer->addChildAtPosition(statsBG, Anchor::Center);

    m_stats = CCNode::create();
    m_stats->setContentSize(statsContainer->getContentSize() - ccp(10, 10));
    m_stats->setAnchorPoint({ .5f, .5f });
    m_stats->setID("mod-stats-container");

    for (auto stat : std::initializer_list<std::tuple<
        const char*, const char*, const char*, std::optional<std::string>, const char*
    >> {
        { "GJ_downloadsIcon_001.png", "Downloads", "downloads", std::nullopt, "stats" },
        { "GJ_timeIcon_001.png", "Released", "release-date", std::nullopt, "stats" },
        { "GJ_timeIcon_001.png", "Updated", "update-date", std::nullopt, "stats" },
        { "version.png"_spr, "Version", "version", m_source.getMetadata().getVersion().toVString(), "client" },
        { nullptr, "Checking for updates", "update-check", std::nullopt, "updates" },
    }) {
        auto container = CCNode::create();
        container->setContentSize({ m_stats->getContentWidth(), 10 });
        container->setID(std::get<2>(stat));
        container->setUserObject(std::get<4>(stat), CCBool::create(true));

        auto labelContainer = CCNode::create();
        labelContainer->setID("labels");
        labelContainer->setLayout(RowLayout::create());
        labelContainer->getLayout()->ignoreInvisibleChildren(true);
        labelContainer->setAnchorPoint({ 1.f, .5f });
        labelContainer->setScale(.3f);
        labelContainer->setContentWidth(
            (container->getContentWidth() - container->getContentHeight() - 5) /
                labelContainer->getScale()
        );
        container->addChildAtPosition(labelContainer, Anchor::Right);

        auto label = CCLabelBMFont::create("", "bigFont.fnt");
        label->setID("label");
        labelContainer->addChild(label);

        labelContainer->addChild(SpacerNode::create());

        auto valueLabel = CCLabelBMFont::create("", "bigFont.fnt");
        valueLabel->setID("value-label");
        labelContainer->addChild(valueLabel);

        labelContainer->addChild(createLoadingCircle(container->getContentHeight() / labelContainer->getScale()));

        this->setStatIcon(container, std::get<0>(stat));
        this->setStatLabel(container, std::get<1>(stat));
        this->setStatValue(container, std::get<3>(stat));

        m_stats->addChild(container);
    }

    m_stats->setLayout(
        ColumnLayout::create()
            ->setAxisReverse(true)
            ->setDefaultScaleLimits(.1f, 1)
            ->setAxisAlignment(AxisAlignment::Even)
    );
    statsContainer->addChildAtPosition(m_stats, Anchor::Center);

    leftColumn->addChild(statsContainer);

    // Tags

    auto tagsTitle = CCLabelBMFont::create("Tags", "bigFont.fnt");
    tagsTitle->limitLabelWidth(leftColumn->getContentWidth(), .25f, .05f);
    tagsTitle->setOpacity(195);
    leftColumn->addChild(tagsTitle);

    auto tagsContainer = CCNode::create();
    tagsContainer->setContentSize({ leftColumn->getContentWidth(), 35 });
    tagsContainer->setAnchorPoint({ .5f, .5f });

    auto tagsBG = CCScale9Sprite::create("square02b_001.png");
    tagsBG->setColor({ 0, 0, 0 });
    tagsBG->setOpacity(75);
    tagsBG->setScale(.3f);
    tagsBG->setContentSize(tagsContainer->getContentSize() / tagsBG->getScale());
    tagsContainer->addChildAtPosition(tagsBG, Anchor::Center);

    m_tags = CCNode::create();
    m_tags->ignoreAnchorPointForPosition(false);
    m_tags->setContentSize(tagsContainer->getContentSize() - ccp(10, 10));
    m_tags->setAnchorPoint({ .5f, .5f });
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
    manageContainer->setContentSize({ leftColumn->getContentWidth(), 10 });

    auto manageTitle = CCLabelBMFont::create("Manage", "bigFont.fnt");
    manageTitle->setScale(.25f);
    manageTitle->setOpacity(195);
    manageContainer->addChildAtPosition(manageTitle, Anchor::Left, ccp(0, 0), ccp(0, .5f));

    m_restartRequiredLabel = createTagLabel(
        "Restart Required",
        {
            to3B(ColorProvider::get()->color("mod-list-restart-required-label"_spr)),
            to3B(ColorProvider::get()->color("mod-list-restart-required-label-bg"_spr))
        }
    );
    m_restartRequiredLabel->setScale(.3f);
    manageContainer->addChildAtPosition(m_restartRequiredLabel, Anchor::Right, ccp(0, 0), ccp(1, .5f));

    m_enabledStatusLabel = CCLabelBMFont::create("", "bigFont.fnt");
    m_enabledStatusLabel->setScale(.25f);
    m_enabledStatusLabel->setOpacity(140);
    manageContainer->addChildAtPosition(m_enabledStatusLabel, Anchor::Right, ccp(0, 0), ccp(1, .5f));

    leftColumn->addChild(manageContainer);

    auto installContainer = CCNode::create();
    installContainer->setContentSize({ leftColumn->getContentWidth(), 25 });
    installContainer->setAnchorPoint({ .5f, .5f });

    m_installBG = CCScale9Sprite::create("square02b_001.png");
    m_installBG->setScale(.3f);
    m_installBG->setContentSize(installContainer->getContentSize() / m_installBG->getScale());
    installContainer->addChildAtPosition(m_installBG, Anchor::Center);

    m_installMenu = CCMenu::create();
    m_installMenu->ignoreAnchorPointForPosition(false);
    m_installMenu->setContentSize(installContainer->getContentSize() - ccp(10, 10));
    m_installMenu->setAnchorPoint({ .5f, .5f });

    auto updateModSpr = createGeodeButton(
        CCSprite::createWithSpriteFrameName("update.png"_spr),
        "Update",
        GeodeButtonSprite::Install,
        m_forceDisableTheme
    );
    updateModSpr->setScale(.5f);
    m_updateBtn = CCMenuItemSpriteExtra::create(
        updateModSpr, this, menu_selector(ModPopup::onInstall)
    );
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
        enableModOffSpr, enableModOnSpr,
        this, menu_selector(ModPopup::onEnable)
    );
    m_enableBtn->m_notClickable = true;
    m_installMenu->addChild(m_enableBtn);

    auto reenableModOffSpr = createGeodeButton(
        CCSprite::createWithSpriteFrameName("reset.png"_spr),
        "Re-Enable",
        GeodeButtonSprite::Default,
        m_forceDisableTheme
    );
    reenableModOffSpr->setScale(.5f);
    auto reenableModOnSpr = createGeodeButton(
        CCSprite::createWithSpriteFrameName("reset.png"_spr),
        "Re-Disable",
        GeodeButtonSprite::Default
    );
    reenableModOnSpr->setScale(.5f);
    m_reenableBtn = CCMenuItemToggler::create(
        reenableModOffSpr, reenableModOnSpr,
        this, menu_selector(ModPopup::onEnable)
    );
    m_reenableBtn->m_notClickable = true;
    m_installMenu->addChild(m_reenableBtn);

    auto unavailableSpr = createGeodeButton(
        CCSprite::createWithSpriteFrameName("exclamation.png"_spr),
        "Unavailable",
        GeodeButtonSprite::Gray,
        m_forceDisableTheme
    );
    unavailableSpr->setColor({ 155, 155, 155 });
    unavailableSpr->setScale(.5f);
    m_unavailableBtn = CCMenuItemSpriteExtra::create(
        unavailableSpr, this, nullptr
    );
    m_unavailableBtn->setEnabled(false);
    m_installMenu->addChild(m_unavailableBtn);

    auto installModSpr = createGeodeButton(
        CCSprite::createWithSpriteFrameName("GJ_downloadsIcon_001.png"),
        "Install",
        GeodeButtonSprite::Install,
        m_forceDisableTheme
    );
    installModSpr->setScale(.5f);
    m_installBtn = CCMenuItemSpriteExtra::create(
        installModSpr, this, menu_selector(ModPopup::onInstall)
    );
    m_installMenu->addChild(m_installBtn);

    auto uninstallModSpr = createGeodeButton(
        CCSprite::createWithSpriteFrameName("delete-white.png"_spr),
        "Uninstall",
        GeodeButtonSprite::Default,
        m_forceDisableTheme
    );
    uninstallModSpr->setScale(.5f);
    m_uninstallBtn = CCMenuItemSpriteExtra::create(
        uninstallModSpr, this, menu_selector(ModPopup::onUninstall)
    );
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
    m_installMenu->addChild(m_cancelBtn);

    m_installStatusLabel = CCLabelBMFont::create("", "bigFont.fnt");
    m_installStatusLabel->setOpacity(120);
    m_installStatusLabel->setVisible(false);
    m_installMenu->addChild(m_installStatusLabel);

    m_installMenu->setLayout(
        RowLayout::create()
            ->setDefaultScaleLimits(.1f, 1)
            ->setAxisAlignment(AxisAlignment::Center)
    );
    m_installMenu->getLayout()->ignoreInvisibleChildren(true);
    installContainer->addChildAtPosition(m_installMenu, Anchor::Center);

    leftColumn->addChild(installContainer);

    // Links

    auto linksContainer = CCNode::create();
    linksContainer->setContentSize({ leftColumn->getContentWidth() - 40, 30 });
    linksContainer->setAnchorPoint({ .5f, .5f });
    linksContainer->setLayoutOptions(
        AxisLayoutOptions::create()
            ->setCrossAxisAlignment(AxisAlignment::End)
    );

    auto linksBG = CCScale9Sprite::create("square02b_001.png");
    linksBG->setColor({ 0, 0, 0 });
    linksBG->setOpacity(75);
    linksBG->setScale(.3f);
    linksBG->setContentSize(linksContainer->getContentSize() / linksBG->getScale());
    linksContainer->addChildAtPosition(linksBG, Anchor::Center);

    auto linksMenu = CCMenu::create();
    linksMenu->ignoreAnchorPointForPosition(false);
    linksMenu->setContentSize(linksContainer->getContentSize() - ccp(10, 10));
    linksMenu->setAnchorPoint({ .5f, .5f });
    linksMenu->setID("links-container");

    // auto linksLabel = CCLabelBMFont::create("Links", "bigFont.fnt");
    // linksLabel->setLayoutOptions(
    //     AxisLayoutOptions::create()
    //         ->setRelativeScale(.35f)
    // );
    // linksMenu->addChild(linksLabel);

    for (auto stat : std::initializer_list<std::tuple<
        const char*, const char*, std::optional<std::string>, SEL_MenuHandler
    >> {
        { "homepage", "homepage.png"_spr, m_source.getMetadata().getLinks().getHomepageURL(), nullptr },
        { "github", "github.png"_spr, m_source.getMetadata().getLinks().getSourceURL(), nullptr },
        { "discord", "gj_discordIcon_001.png", m_source.getMetadata().getLinks().getCommunityURL(), nullptr },
        { "support", "gift.png"_spr, m_source.getMetadata().getSupportInfo(), menu_selector(ModPopup::onSupport) },
    }) {
        auto spr = CCSprite::createWithSpriteFrameName(std::get<1>(stat));
        spr->setScale(.75f);
        if (!std::get<2>(stat).has_value()) {
            spr->setColor({ 155, 155, 155 });
            spr->setOpacity(155);
        }

        SEL_MenuHandler handler = std::get<2>(stat).has_value() ?
            (std::get<3>(stat) ? std::get<3>(stat) : menu_selector(ModPopup::onLink)) :
            nullptr;

        auto btn = CCMenuItemSpriteExtra::create(
            spr, this, handler
        );

        btn->setEnabled(handler != nullptr);

        btn->setID(std::get<0>(stat));
        if (!std::get<3>(stat) && std::get<2>(stat)) {
            btn->setUserObject("url", CCString::create(*std::get<2>(stat)));
        }
        linksMenu->addChild(btn);
    }

    linksMenu->setLayout(
        RowLayout::create()
            ->setDefaultScaleLimits(.1f, 1)
            ->setAxisAlignment(AxisAlignment::Center)
    );
    linksContainer->addChildAtPosition(linksMenu, Anchor::Center);

    leftColumn->addChild(linksContainer);

    // auto bottomPadding = CCNode::create();
    // bottomPadding->setContentHeight(13);
    // leftColumn->addChild(bottomPadding);

    leftColumn->setLayout(
        ColumnLayout::create()
            ->setAxisReverse(true)
            ->setCrossAxisOverflow(false)
            ->setAutoScale(false)
            ->setAxisAlignment(AxisAlignment::Start)
            ->setCrossAxisLineAlignment(AxisAlignment::Start)
            ->setGap(4)
    );
    mainContainer->addChild(leftColumn);

    m_rightColumn = CCNode::create();
    m_rightColumn->setContentSize({
        // The -5 is to give a little bit of padding
        mainContainer->getContentWidth() - leftColumn->getContentWidth() -
            static_cast<AxisLayout*>(mainContainer->getLayout())->getGap(),
        mainContainer->getContentHeight()
    });

    auto tabsMenu = CCMenu::create();
    tabsMenu->ignoreAnchorPointForPosition(false);
    tabsMenu->setScale(.65f);
    tabsMenu->setContentWidth(m_rightColumn->getContentWidth() / tabsMenu->getScale());
    tabsMenu->setAnchorPoint({ .5f, 1.f });
    tabsMenu->setID("tabs-menu");

    for (auto mdTab : std::initializer_list<std::tuple<const char*, const char*, const char*, Tab>> {
        { "message.png"_spr,   "Description", "description", Tab::Details },
        { "changelog.png"_spr, "Changelog",   "changelog",   Tab::Changelog }
        // { "version.png"_spr,   "Versions",    Tab::Versions },
    }) {
        auto spr = GeodeTabSprite::create(std::get<0>(mdTab), std::get<1>(mdTab), 140, m_source.asServer());
        auto btn = CCMenuItemSpriteExtra::create(spr, this, menu_selector(ModPopup::onTab));
        btn->setTag(static_cast<int>(std::get<3>(mdTab)));
        btn->setID(std::get<2>(mdTab));
        tabsMenu->addChild(btn);
        m_tabs.insert({ std::get<3>(mdTab), { spr, nullptr } });
    }

    // placeholder external link until versions tab is implemented
    auto modUrl = fmt::format("https://geode-sdk.org/mods/{}", m_source.getID());
    auto externalLinkSpr = CCSprite::createWithSpriteFrameName("external-link.png"_spr);

    externalLinkSpr->setScale(0.35f);
    externalLinkSpr->setOpacity(127);

    auto externalLinkBtn = CCMenuItemSpriteExtra::create(externalLinkSpr, this, menu_selector(ModPopup::onLink));
    externalLinkBtn->setUserObject("url", CCString::create(modUrl));
    externalLinkBtn->setID("mod-online-page-button");
    m_buttonMenu->addChildAtPosition(externalLinkBtn, Anchor::TopRight, ccp(-14, -16));

    if (isGeode) {
        externalLinkBtn->setVisible(false);
    }

    tabsMenu->setLayout(RowLayout::create()->setAxisAlignment(AxisAlignment::Start));
    m_rightColumn->addChildAtPosition(tabsMenu, Anchor::Top);

    mainContainer->addChildAtPosition(m_rightColumn, Anchor::Right, ccp(-20, 0));

    mainContainer->updateLayout();
    m_mainLayer->addChildAtPosition(mainContainer, Anchor::Center);

    m_settingsBG = CCScale9Sprite::create("square02b_001.png");
    m_settingsBG->setColor({ 0, 0, 0 });
    m_settingsBG->setOpacity(75);
    m_settingsBG->setScale(.3f);
    m_settingsBG->setContentSize(ccp(35, 30) / linksBG->getScale());
    m_buttonMenu->addChildAtPosition(m_settingsBG, Anchor::BottomLeft, ccp(28, 25));

    auto settingsSpr = createGeodeCircleButton(CCSprite::createWithSpriteFrameName("settings.png"_spr), 1.f, CircleBaseSize::Medium, false, m_forceDisableTheme);
    settingsSpr->setScale(.6f);
    auto settingsBtn = CCMenuItemSpriteExtra::create(
        settingsSpr, this, menu_selector(ModPopup::onSettings)
    );
    settingsBtn->setID("settings-button");
    m_buttonMenu->addChildAtPosition(settingsBtn, Anchor::BottomLeft, ccp(28, 25));

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
    m_statsListener.bind(this, &ModPopup::onLoadServerInfo);
    m_statsListener.setFilter(m_source.fetchServerInfo());
    m_tagsListener.bind(this, &ModPopup::onLoadTags);
    m_tagsListener.setFilter(m_source.fetchValidTags());

    if (m_source.asMod()) {
        m_checkUpdateListener.bind(this, &ModPopup::onCheckUpdates);
        m_checkUpdateListener.setFilter(m_source.checkUpdates());
    }
    else {
        auto updatesStat = m_stats->getChildByID("update-check");
        this->setStatLabel(updatesStat, "Not Installed", true, ccc3(125, 125, 125));
    }

    // Only listen for updates on this mod specifically
    m_updateStateListener.bind([this](auto) { this->updateState(); });
    m_updateStateListener.setFilter(UpdateModListStateFilter(UpdateModState(m_source.getID())));

    m_downloadListener.bind([this](auto) { this->updateState(); });
    m_downloadListener.setFilter(m_source.getID());

    m_settingNodeListener.bind([this](SettingNodeValueChangeEvent* ev) {
        if (!ev->isCommit()) {
            return ListenerResult::Propagate;
        }
        this->updateState();
        return ListenerResult::Propagate;
    });

    return true;
}

void ModPopup::updateState() {
    auto asMod = m_source.asMod();
    auto wantsRestart = m_source.wantsRestart();
    auto wantsRestartBecauseOfSettings = asMod && ModSettingsManager::from(asMod)->restartRequired();

    m_installBG->setColor((wantsRestart && !wantsRestartBecauseOfSettings) ?
        to3B(ColorProvider::get()->color("mod-list-restart-required-label"_spr)) :
        ccBLACK
    );
    m_installBG->setOpacity((wantsRestart && !wantsRestartBecauseOfSettings) ? 40 : 75);
    m_settingsBG->setColor(wantsRestartBecauseOfSettings ?
        to3B(ColorProvider::get()->color("mod-list-restart-required-label"_spr)) :
        ccBLACK
    );
    m_settingsBG->setOpacity(wantsRestartBecauseOfSettings ? 40 : 75);
    m_settingsBG->setVisible(wantsRestartBecauseOfSettings);
    m_restartRequiredLabel->setVisible(wantsRestart);

    if (!wantsRestart && asMod) {
        if (asMod->isEnabled()) {
            m_enabledStatusLabel->setString("Enabled");
            m_enabledStatusLabel->setColor(to3B(ColorProvider::get()->color("mod-list-enabled"_spr)));
        }
        else {
            m_enabledStatusLabel->setString("Disabled");
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
        asMod &&
        asMod->getRequestedAction() == ModRequestedAction::None &&
        !asMod->targetsOutdatedVersion()
    );

    m_reenableBtn->toggle(m_enableBtn->isToggled());
    m_reenableBtn->setVisible(asMod && modRequestedActionIsToggle(asMod->getRequestedAction()));

    m_updateBtn->setVisible(m_source.hasUpdates().has_value() && asMod->getRequestedAction() == ModRequestedAction::None);
    m_installBtn->setVisible(this->availableForInstall());
    m_unavailableBtn->setVisible(m_source.asServer() && !this->availableForInstall());
    m_uninstallBtn->setVisible(asMod && asMod->getRequestedAction() == ModRequestedAction::None);

    if (asMod && modRequestedActionIsUninstall(asMod->getRequestedAction())) {
        m_installStatusLabel->setString("Mod has been uninstalled");
        m_installStatusLabel->setVisible(true);
    }
    else {
        m_installStatusLabel->setString("");
        m_installStatusLabel->setVisible(false);
    }

    if (m_enableBtn->isVisible() && m_updateBtn->isVisible() && m_uninstallBtn->isVisible()) {
        static_cast<IconButtonSprite*>(m_enableBtn->m_onButton->getNormalImage())->setString("");
        static_cast<IconButtonSprite*>(m_enableBtn->m_offButton->getNormalImage())->setString("");
        static_cast<IconButtonSprite*>(m_uninstallBtn->getNormalImage())->setString("");
    }
    else {
        static_cast<IconButtonSprite*>(m_enableBtn->m_onButton->getNormalImage())->setString("Disable");
        static_cast<IconButtonSprite*>(m_enableBtn->m_offButton->getNormalImage())->setString("Enable");
        static_cast<IconButtonSprite*>(m_uninstallBtn->getNormalImage())->setString("Uninstall");
    }
    m_enableBtn->updateSprite();
    m_uninstallBtn->updateSprite();

    if (asMod && asMod->isInternal()) {
        m_enableBtn->setVisible(false);
        // you can uninstall loader ingame just fine on windows
        #if !defined(GEODE_IS_WINDOWS)
        m_uninstallBtn->setVisible(false);
        m_installStatusLabel->setString("N/A");
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
                m_enabledStatusLabel->setString(fmt::format("Downloading {}%", d->percentage).c_str());
                m_enabledStatusLabel->setColor(ccWHITE);
                // todo: progress bar
            }
            else {
                m_enabledStatusLabel->setString("Preparing");
                m_enabledStatusLabel->setColor(ccWHITE);
                // todo: spinner
            }
        }
        else {
            std::visit(makeVisitor {
                [this](server::DownloadStatusError const& e) {
                    m_enabledStatusLabel->setString("Error");
                    m_enabledStatusLabel->setColor(to3B(ColorProvider::get()->color("mod-list-disabled"_spr)));
                    // todo: show error details somewhere (like an info button)
                },
                [this](server::DownloadStatusCancelled const&) {
                    m_enabledStatusLabel->setString("Cancelled");
                    m_enabledStatusLabel->setColor(to3B(ColorProvider::get()->color("mod-list-disabled"_spr)));
                },
                [this](server::DownloadStatusDone const&) {
                    m_enableBtn->setVisible(false);
                    m_reenableBtn->setVisible(false);
                    m_updateBtn->setVisible(false);
                    m_installBtn->setVisible(false);
                    m_uninstallBtn->setVisible(false);
                    m_cancelBtn->setVisible(false);

                    m_installStatusLabel->setString("Mod has been installed");
                    m_installStatusLabel->setVisible(true);
                },
                // rest are unreachable due to the isActive() check
                [this](auto const&) {},
            }, download->getStatus());
        }
    }

    m_installMenu->updateLayout();

    ModPopupUIEvent(std::make_unique<ModPopupUIEvent::Impl>(this)).post();
}

void ModPopup::setStatIcon(CCNode* stat, const char* spr) {
    // Remove old icon
    stat->removeChildByID("icon");

    // Create new icon
    if (spr) {
        auto iconSize = stat->getContentHeight();
        auto icon = CCSprite::createWithSpriteFrameName(spr);
        limitNodeSize(icon, { iconSize, iconSize }, 1.f, .1f);
        icon->setID("icon");
        stat->addChildAtPosition(icon, Anchor::Left, ccp(stat->getContentHeight() / 2, 0));
    }
}

void ModPopup::setStatLabel(CCNode* stat, std::string const& value, bool noValue, ccColor3B color) {
    auto container = stat->getChildByID("labels");

    // Update label
    auto label = static_cast<CCLabelBMFont*>(container->getChildByID("label"));
    label->setString(value.c_str());
    label->setColor(color);

    // Remove value if requested
    if (noValue) {
        container->getChildByID("value-label")->setVisible(false);
        container->getChildByID("loading-spinner")->setVisible(false);
    }

    container->updateLayout();
}

void ModPopup::setStatValue(CCNode* stat, std::optional<std::string> const& value) {
    auto container = stat->getChildByID("labels");
    auto valueLabel = static_cast<CCLabelBMFont*>(container->getChildByID("value-label"));
    auto spinner = container->getChildByID("loading-spinner");

    // Show loading if no value provided
    valueLabel->setVisible(value.has_value());
    spinner->setVisible(!value.has_value());

    // Update value
    if (value) {
        valueLabel->setString(value.value().c_str());
    }

    // Update layout
    container->updateLayout();
}

// helper class for making an std::locale
class comma_numpunct : public std::numpunct<char> {
protected:
    virtual char do_thousands_sep() const
    {
        return ',';
    }

    virtual std::string do_grouping() const
    {
        return "\03";
    }
};

void ModPopup::onLoadServerInfo(typename server::ServerRequest<server::ServerModMetadata>::Event* event) {
    if (event->getValue() && event->getValue()->isOk()) {
        auto data = event->getValue()->unwrap();
        auto timeToString = [](auto const& time) {
            if (time.has_value()) {
                return time.value().toAgoString();
            }
            return std::string("N/A");
        };

        static std::locale commaLocale(std::locale(), new comma_numpunct());

        // Update server stats
        for (auto id : std::initializer_list<std::pair<const char*, std::string>> {
            { "downloads", fmt::format(commaLocale, "{:L}", data.downloadCount) },
            { "release-date", timeToString(data.createdAt) },
            { "update-date", timeToString(data.updatedAt) },
        }) {
            if (auto stat = m_stats->getChildByID(id.first)) {
                this->setStatValue(stat, id.second);
            }
        }
        ModPopupUIEvent(std::make_unique<ModPopupUIEvent::Impl>(this)).post();
    }
    else if (event->isCancelled() || (event->getValue() && event->getValue()->isErr())) {
        for (auto child : CCArrayExt<CCNode*>(m_stats->getChildren())) {
            if (child->getUserObject("stats")) {
                this->setStatValue(child, "N/A");
            }
        }
        ModPopupUIEvent(std::make_unique<ModPopupUIEvent::Impl>(this)).post();
    }
}

void ModPopup::onCheckUpdates(typename server::ServerRequest<std::optional<server::ServerModUpdate>>::Event* event) {
    if (event->getValue() && event->getValue()->isOk()) {
        auto resolved = event->getValue()->unwrap();
        // Check if this has updates for an installed mod
        auto updatesStat = m_stats->getChildByID("update-check");
        if (resolved.has_value()) {
            this->setStatIcon(updatesStat, "updates-available.png"_spr);
            this->setStatLabel(
                updatesStat, "Update Found", false,
                ColorProvider::get()->color3b("mod-list-version-label-updates-available"_spr)
            );
            this->setStatValue(updatesStat, resolved.value().version.toVString());
            this->updateState();
        }
        else {
            this->setStatIcon(updatesStat, "GJ_completesIcon_001.png");
            this->setStatLabel(
                updatesStat, "Up to Date!", true,
                ColorProvider::get()->color3b("mod-list-version-label"_spr)
            );
        }
    }
    else if (event->isCancelled() || (event->getValue() && event->getValue()->isErr())) {
        auto updatesStat = m_stats->getChildByID("update-check");
        this->setStatLabel(updatesStat, "No Updates Found", true, ccc3(125, 125, 125));
    }
}

void ModPopup::onLoadTags(typename server::ServerRequest<std::vector<server::ServerTag>>::Event* event) {
    if (event->getValue() && event->getValue()->isOk()) {
        auto data = event->getValue()->unwrap();
        m_tags->removeAllChildren();

        for (auto& tag : data) {
            m_tags->addChild(createGeodeTagLabel(tag));
        }

        if (data.empty()) {
            auto label = CCLabelBMFont::create("No tags found", "bigFont.fnt");
            label->setOpacity(120);
            m_tags->addChild(label);
        }
        // This should probably be kept even after modtober ends,
        // so the banner sprite must be kept
        // If the build times from the cool popup become too long then we can
        // probably move that to a normal FLAlert that explains "Modtober was
        // this contest blah blah this mod was made for it"
        else if (ranges::contains(data, [](auto const& tag) { return tag.name == "modtober24"; })) {
            auto menu = CCMenu::create();
            menu->setID("modtober-banner");
            menu->ignoreAnchorPointForPosition(false);
            menu->setContentSize({ m_rightColumn->getContentWidth(), 25 });

            auto banner = CCSprite::createWithSpriteFrameName("modtober24-banner-2.png"_spr);
            limitNodeWidth(banner, m_rightColumn->getContentWidth(), 1.f, .1f);
            menu->addChildAtPosition(banner, Anchor::Center);

            auto label = CCLabelBMFont::create("Entry for Modtober 2024", "bigFont.fnt");
            label->setScale(.35f);
            menu->addChildAtPosition(label, Anchor::Left, ccp(10, 0), ccp(0, .5f));

            auto aboutSpr = createGeodeButton("About", false, GeodeButtonSprite::Default, m_forceDisableTheme);
            aboutSpr->setScale(.35f);
            auto aboutBtn = CCMenuItemSpriteExtra::create(
                aboutSpr, this, menu_selector(ModPopup::onModtoberInfo)
            );
            menu->addChildAtPosition(aboutBtn, Anchor::Right, ccp(-25, 0));

            m_rightColumn->addChildAtPosition(menu, Anchor::Bottom, ccp(0, 0), ccp(.5f, 0));

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

        ModPopupUIEvent(std::make_unique<ModPopupUIEvent::Impl>(this)).post();
    }
    else if (event->isCancelled() || (event->getValue() && event->getValue()->isErr())) {
        m_tags->removeAllChildren();

        auto label = CCLabelBMFont::create("No tags found", "bigFont.fnt");
        label->setOpacity(120);
        m_tags->addChild(label);

        m_tags->updateLayout();

        ModPopupUIEvent(std::make_unique<ModPopupUIEvent::Impl>(this)).post();
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
        m_rightColumn->addChildAtPosition(existing, Anchor::Bottom, ccp(0, modtoberBannerHeight));
    }
    else {
        const auto size = (m_rightColumn->getContentSize() - ccp(0, 30 + modtoberBannerHeight));
        const float mdScale = .85f;
        switch (tab) {
            case Tab::Details: {
                m_currentTabPage = FetchTextArea::create(
                    m_source.fetchAbout(),
                    "No description provided",
                    size / mdScale
                );
                m_currentTabPage->setID("description-container");
                m_currentTabPage->setScale(mdScale);
            } break;

            case Tab::Changelog: {
                m_currentTabPage = FetchTextArea::create(
                    m_source.fetchChangelog(),
                    "No changelog provided",
                    size / mdScale
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
        m_currentTabPage->setAnchorPoint({ .5f, .0f });
        m_rightColumn->addChildAtPosition(m_currentTabPage, Anchor::Bottom, ccp(0, modtoberBannerHeight));
        m_tabs.at(tab).second = m_currentTabPage;
    }
}

void ModPopup::onTab(CCObject* sender) {
    this->loadTab(static_cast<Tab>(sender->getTag()));
}

void ModPopup::onEnable(CCObject*) {
    if (auto mod = m_source.asMod()) {
        // Toggle the mod state
        auto res = mod->isOrWillBeEnabled() ? mod->disable() : mod->enable();
        if (!res) {
            FLAlertLayer::create("Error Toggling Mod", res.unwrapErr(), "OK")->show();
        }
    }
    else {
        FLAlertLayer::create("Error Toggling Mod", "This mod can not be toggled!", "OK")->show();
    }
    UpdateModListStateEvent(UpdateModState(m_source.getID())).post();
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
            )->show();
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
        )->show();
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

void ModPopup::onSupport(CCObject*) {
    openSupportPopup(m_source.getMetadata());
}
void ModPopup::onModtoberInfo(CCObject*) {
    FLAlertLayer::create(
        "Modtober 2024",
        "This mod was an entry for <cj>Modtober 2024</c>, a contest to create "
        "the best mod with the theme <cc>\"The Sky's the Limit\"</c>",
        "OK"
    )->show();
}

ModPopup* ModPopup::create(ModSource&& src) {
    auto ret = new ModPopup();
    GeodePopupStyle style = GeodePopupStyle::Default;
    if (src.asServer()) {
        style = GeodePopupStyle::Alt;
    }
    if (ret->init(440, 280, std::move(src), style)) {
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
        auto version = serverSource->latestVersion();
        auto gameVersion = version.getGameVersion();

        if (
            (gameVersion == "0.000") ||
            (gameVersion && gameVersion != "*" && gameVersion != GEODE_STR(GEODE_GD_VERSION)) ||
            (!Loader::get()->isModVersionSupported(version.getGeodeVersion()))) {
            return false;
        }
        return true;
    }
    return false;
}
