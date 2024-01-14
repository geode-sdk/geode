#include "ModInfoPopup.hpp"

#include "../list/ModListLayer.hpp"
#include "../settings/ModSettingsPopup.hpp"
#include <Geode/loader/Dirs.hpp>
#include <about.hpp>

#include <Geode/binding/ButtonSprite.hpp>
#include <Geode/binding/CCTextInputNode.hpp>
#include <Geode/binding/GJListLayer.hpp>
#include <Geode/binding/Slider.hpp>
#include <Geode/binding/SliderThumb.hpp>
#include <Geode/binding/SliderTouchLogic.hpp>
#include <Geode/loader/Mod.hpp>
#include <Geode/ui/BasedButton.hpp>
#include <Geode/ui/GeodeUI.hpp>
#include <Geode/ui/IconButtonSprite.hpp>
#include <Geode/ui/MDPopup.hpp>
#include <Geode/utils/casts.hpp>
#include <Geode/utils/ranges.hpp>
#include <Geode/utils/web.hpp>
#include <loader/LoaderImpl.hpp>
#include <ui/internal/list/InstallListPopup.hpp>

static constexpr int const TAG_CONFIRM_UNINSTALL = 5;
static constexpr int const TAG_CONFIRM_UPDATE = 6;
static constexpr int const TAG_CONFIRM_UNINSTALL_WITH_SAVEDATA = 7;
static const CCSize LAYER_SIZE = {440.f, 290.f};

bool ModInfoPopup::init(ModMetadata const& metadata, ModListLayer* list) {
    m_noElasticity = true;
    m_layer = list;

    auto winSize = CCDirector::sharedDirector()->getWinSize();

    if (!this->initWithColor({0, 0, 0, 105})) return false;
    m_mainLayer = CCLayer::create();
    this->addChild(m_mainLayer);

    auto bg = CCScale9Sprite::create("GJ_square01.png", {0.0f, 0.0f, 80.0f, 80.0f});
    bg->setContentSize(LAYER_SIZE);
    bg->setPosition(winSize.width / 2, winSize.height / 2);
    bg->setZOrder(-10);
    m_mainLayer->addChild(bg);

    m_buttonMenu = CCMenu::create();
    m_mainLayer->addChild(m_buttonMenu);

    constexpr float logoSize = 40.f;
    constexpr float logoOffset = 10.f;

    auto topNode = CCNode::create();
    topNode->setContentSize({350.f, 80.f});
    topNode->setLayout(
        RowLayout::create()
            ->setAxisAlignment(AxisAlignment::Center)
            ->setAutoScale(false)
            ->setCrossAxisOverflow(true)
    ); 
    m_mainLayer->addChild(topNode);
    topNode->setAnchorPoint({.5f, .5f});
    topNode->setPosition(winSize.width / 2, winSize.height / 2 + 115.f);

    auto logoSpr = this->createLogo({logoSize, logoSize});
    topNode->addChild(logoSpr);

    auto labelNode = CCNode::create();
    labelNode->setLayout(
        ColumnLayout::create()
            ->setAxisAlignment(AxisAlignment::Center)
            ->setCrossAxisLineAlignment(AxisAlignment::Start)
            ->setGap(0.f)
            ->setAutoScale(false)
            ->setCrossAxisOverflow(true)
    );
    labelNode->setContentSize({200.f, 80.f});
    topNode->addChild(labelNode);

    auto nameLabel = CCLabelBMFont::create(metadata.getName().c_str(), "bigFont.fnt");
    nameLabel->limitLabelWidth(200.f, .7f, .1f);
    labelNode->addChild(nameLabel, 2);

    auto developerStr = "by " + metadata.getDeveloper();
    auto developerLabel = CCLabelBMFont::create(developerStr.c_str(), "goldFont.fnt");
    developerLabel->setScale(.5f);
    labelNode->addChild(developerLabel);

    auto versionLabel = CCLabelBMFont::create(metadata.getVersion().toString().c_str(),
        "bigFont.fnt"
    );
    versionLabel->setScale(.4f);
    versionLabel->setColor({0, 255, 0});
    topNode->addChild(versionLabel);

    labelNode->updateLayout();
    topNode->updateLayout();

    this->setTouchEnabled(true);

    m_detailsArea = MDTextArea::create(
        (metadata.getDetails() ? metadata.getDetails().value() : "### No description provided."),
        { 350.f, 137.5f }
    );
    m_detailsArea->setPosition(
        winSize.width / 2 - m_detailsArea->getScaledContentSize().width / 2,
        winSize.height / 2 - m_detailsArea->getScaledContentSize().height / 2 - 20.f
    );
    m_mainLayer->addChild(m_detailsArea);

    m_scrollbar = Scrollbar::create(m_detailsArea->getScrollLayer());
    m_scrollbar->setPosition(
        winSize.width / 2 + m_detailsArea->getScaledContentSize().width / 2 + 20.f,
        winSize.height / 2 - 20.f
    );
    m_mainLayer->addChild(m_scrollbar);

    // changelog
    if (metadata.getChangelog()) {
        // m_changelogArea is only created if the changelog button is clicked 
        // because changelogs can get really long and take a while to load

        auto changelogBtnOffSpr = ButtonSprite::create(
            CCSprite::createWithSpriteFrameName("changelog.png"_spr),
            0x20,
            true,
            32.f,
            "GJ_button_01.png",
            1.f
        );
        changelogBtnOffSpr->setScale(.65f);

        auto changelogBtnOnSpr = ButtonSprite::create(
            CCSprite::createWithSpriteFrameName("changelog.png"_spr),
            0x20,
            true,
            32.f,
            "GJ_button_02.png",
            1.f
        );
        changelogBtnOnSpr->setScale(.65f);

        auto changelogBtn = CCMenuItemToggler::create(
            changelogBtnOffSpr, changelogBtnOnSpr, this, menu_selector(ModInfoPopup::onChangelog)
        );
        changelogBtn->setPosition(-LAYER_SIZE.width / 2 + 21.5f, .0f);
        m_buttonMenu->addChild(changelogBtn);
    }

    // mod metadata
    auto infoSpr = CCSprite::createWithSpriteFrameName("GJ_infoIcon_001.png");
    infoSpr->setScale(.85f);

    m_infoBtn = CCMenuItemSpriteExtra::create(infoSpr, this, menu_selector(ModInfoPopup::onInfo));
    m_infoBtn->setPosition(LAYER_SIZE.width / 2 - 25.f, LAYER_SIZE.height / 2 - 25.f);
    m_buttonMenu->addChild(m_infoBtn);

    // repo button
    if (metadata.getRepository()) {
        auto repoBtn = CCMenuItemSpriteExtra::create(
            CCSprite::createWithSpriteFrameName("github.png"_spr),
            this,
            menu_selector(ModInfoPopup::onRepository)
        );
        repoBtn->setPosition(LAYER_SIZE.width / 2 - 25.f, -LAYER_SIZE.height / 2 + 25.f);
        m_buttonMenu->addChild(repoBtn);
    }

    // support button
    if (metadata.getSupportInfo()) {
        auto supportBtn = CCMenuItemSpriteExtra::create(
            CCSprite::createWithSpriteFrameName("gift.png"_spr),
            this,
            menu_selector(ModInfoPopup::onSupport)
        );
        supportBtn->setPosition(LAYER_SIZE.width / 2 - 60.f, -LAYER_SIZE.height / 2 + 25.f);
        m_buttonMenu->addChild(supportBtn);
    }

    auto closeSpr = CCSprite::createWithSpriteFrameName("GJ_closeBtn_001.png");
    closeSpr->setScale(.8f);

    auto closeBtn =
        CCMenuItemSpriteExtra::create(closeSpr, this, menu_selector(ModInfoPopup::onClose));
    closeBtn->setPosition(-LAYER_SIZE.width / 2 + 3.f, LAYER_SIZE.height / 2 - 3.f);
    m_buttonMenu->addChild(closeBtn);

    this->setKeypadEnabled(true);
    this->setTouchEnabled(true);

    return true;
}

void ModInfoPopup::onSupport(CCObject*) {
    MDPopup::create(
        "Support " + this->getMetadata().getName(),
        this->getMetadata().getSupportInfo().value(),
        "OK"
    )->show();
}

void ModInfoPopup::onRepository(CCObject*) {
    auto repo = this->getMetadata().getRepository();
    if (repo) {
        web::openLinkInBrowser(repo.value());
    }
}

void ModInfoPopup::onInfo(CCObject*) {
    auto info = this->getMetadata();
    FLAlertLayer::create(
        nullptr,
        ("About " + info.getName()).c_str(),
        fmt::format(
            "<cr>ID: {}</c>\n"
            "<cg>Version: {}</c>\n"
            "<cp>Developer: {}</c>\n"
            "<cb>Path: {}</c>\n",
            info.getID(),
            info.getVersion().toString(),
            info.getDeveloper(),
            info.getPath().string()
        ),
        "OK",
        nullptr,
        400.f
    )
        ->show();
}

void ModInfoPopup::onChangelog(CCObject* sender) {
    auto winSize = CCDirector::get()->getWinSize();

    if (!m_changelogArea) {
        m_changelogArea = MDTextArea::create(this->getMetadata().getChangelog().value(), { 350.f, 137.5f });
        m_changelogArea->setPosition(
            -5000.f, winSize.height / 2 - m_changelogArea->getScaledContentSize().height / 2 - 20.f
        );
        m_changelogArea->setVisible(false);
        m_mainLayer->addChild(m_changelogArea);
    }

    auto toggle = static_cast<CCMenuItemToggler*>(sender);

    m_detailsArea->setVisible(toggle->isToggled());
    // as it turns out, cocos2d is stupid and still passes touch
    // events to invisible nodes
    m_detailsArea->setPositionX(
        toggle->isToggled() ? winSize.width / 2 - m_detailsArea->getScaledContentSize().width / 2 :
                              -5000.f
    );

    m_changelogArea->setVisible(!toggle->isToggled());
    // as it turns out, cocos2d is stupid and still passes touch
    // events to invisible nodes
    m_changelogArea->setPositionX(
        !toggle->isToggled() ? winSize.width / 2 - m_changelogArea->getScaledContentSize().width / 2 :
                               -5000.f
    );

    m_scrollbar->setTarget(
        toggle->isToggled() ?
            m_detailsArea->getScrollLayer() :
            m_changelogArea->getScrollLayer()
    );
}

void ModInfoPopup::keyDown(enumKeyCodes key) {
    if (key == KEY_Escape) return this->onClose(nullptr);
    if (key == KEY_Space) return;

    return FLAlertLayer::keyDown(key);
}

void ModInfoPopup::onClose(CCObject* pSender) {
    this->setKeyboardEnabled(false);
    this->removeFromParentAndCleanup(true);
};

void ModInfoPopup::setInstallStatus(std::optional<UpdateProgress> const& progress) {
    if (progress) {
        m_installStatus->setVisible(true);
        m_installStatus->setStatus(progress.value().second);
        m_installStatus->setProgress(progress.value().first);
    } else {
        m_installStatus->setVisible(false);
    }
}

void ModInfoPopup::popupInstallItem(IndexItemHandle item) {
    auto deps = item->getMetadata().getDependencies();
    enum class DepState {
        None,
        HasOnlyRequired,
        HasOptional
    } depState = DepState::None;
    for (auto const& dep : deps) {
        // resolved means it's already installed, so
        // no need to ask the user whether they want to install it
        if (Loader::get()->isModLoaded(dep.id))
            continue;
        if (dep.importance != ModMetadata::Dependency::Importance::Required) {
            depState = DepState::HasOptional;
            break;
        }
        depState = DepState::HasOnlyRequired;
    }

    std::string content;
    char const* btn1;
    char const* btn2;
    switch (depState) {
        case DepState::None:
            content = fmt::format(
                "Are you sure you want to install <cg>{}</c>?",
                item->getMetadata().getName()
            );
            btn1 = "Info";
            btn2 = "Install";
            break;
        case DepState::HasOnlyRequired:
            content =
                "Installing this mod requires other mods to be installed. "
                "Would you like to <cy>proceed</c> with the installation or "
                "<cb>view</c> which mods are going to be installed?";
            btn1 = "View";
            btn2 = "Proceed";
            break;
        case DepState::HasOptional:
            content =
                "This mod recommends installing other mods alongside it. "
                "Would you like to continue with <cy>recommended settings</c> or "
                "<cb>customize</c> which mods to install?";
            btn1 = "Customize";
            btn2 = "Recommended";
            break;
    }

    createQuickPopup("Confirm Install", content, btn1, btn2, 320.f, [&](FLAlertLayer*, bool btn2) {
        if (btn2) {
            auto canInstall = Index::get()->canInstall(m_item);
            if (!canInstall) {
                FLAlertLayer::create(
                    "Unable to Install",
                    canInstall.unwrapErr(),
                    "OK"
                )->show();
                return;
            }
            this->preInstall();
            Index::get()->install(m_item);
        }
        else {
            InstallListPopup::create(m_item, [&](IndexInstallList const& list) {
                this->preInstall();
                Index::get()->install(list);
            })->show();
        }
    }, true, true);
}

void ModInfoPopup::preInstall() {
    if (m_latestVersionLabel) {
        m_latestVersionLabel->setVisible(false);
    }
    if (m_minorVersionLabel) {
        m_minorVersionLabel->setVisible(false);
    }
    this->setInstallStatus(UpdateProgress(0, "Starting install"));

    m_installBtn->setTarget(
        this, menu_selector(ModInfoPopup::onCancelInstall)
    );
    m_installBtnSpr->setString("Cancel");
    m_installBtnSpr->setBG("GJ_button_06.png", false);
}

void ModInfoPopup::onCancelInstall(CCObject*) {
    Index::get()->cancelInstall(m_item);
}

// LocalModInfoPopup
LocalModInfoPopup::LocalModInfoPopup()
  : m_installListener(
        this, &LocalModInfoPopup::onUpdateProgress,
        ModInstallFilter("")
    ) {}


bool LocalModInfoPopup::init(Mod* mod, ModListLayer* list) {
    m_item = Index::get()->getMajorItem(mod->getMetadata().getID());
    if (m_item)
        m_installListener.setFilter(m_item->getMetadata().getID());
    m_mod = mod;

    if (!ModInfoPopup::init(mod->getMetadata(), list)) return false;

    auto winSize = CCDirector::sharedDirector()->getWinSize();

    // mod settings
    auto settingsSpr = CCSprite::createWithSpriteFrameName("GJ_optionsBtn_001.png");
    settingsSpr->setScale(.65f);

    auto settingsBtn =
        CCMenuItemSpriteExtra::create(settingsSpr, this, menu_selector(LocalModInfoPopup::onSettings));
    settingsBtn->setPosition(-LAYER_SIZE.width / 2 + 25.f, -LAYER_SIZE.height / 2 + 25.f);
    m_buttonMenu->addChild(settingsBtn);

    // Check if a config directory for the mod exists
    if (ghc::filesystem::exists(mod->getConfigDir(false))) {
        auto configSpr = CircleButtonSprite::createWithSpriteFrameName(
            "pencil.png"_spr, 1.f, CircleBaseColor::Green, CircleBaseSize::MediumAlt
        );
        configSpr->setScale(.65f);

        auto configBtn = CCMenuItemSpriteExtra::create(
            configSpr, this, menu_selector(LocalModInfoPopup::onOpenConfigDir)
        );
        configBtn->setPosition(-LAYER_SIZE.width / 2 + 65.f, -LAYER_SIZE.height / 2 + 25.f);
        m_buttonMenu->addChild(configBtn);
    }

    if (!mod->hasSettings()) {
        settingsSpr->setColor({150, 150, 150});
        settingsBtn->setTarget(this, menu_selector(LocalModInfoPopup::onNoSettings));
    }

    auto enableBtnSpr = ButtonSprite::create("Enable", "bigFont.fnt", "GJ_button_01.png", .6f);
    enableBtnSpr->setScale(.6f);

    auto disableBtnSpr = ButtonSprite::create("Disable", "bigFont.fnt", "GJ_button_06.png", .6f);
    disableBtnSpr->setScale(.6f);

    auto enableBtn = CCMenuItemToggler::create(
        disableBtnSpr, enableBtnSpr, this, menu_selector(LocalModInfoPopup::onEnableMod)
    );
    enableBtn->setPosition(-155.f, 75.f);
    enableBtn->toggle(!mod->shouldLoad());
    m_buttonMenu->addChild(enableBtn);

    if (!mod->supportsDisabling()) {
        enableBtn->setTarget(this, menu_selector(LocalModInfoPopup::onDisablingNotSupported));
        enableBtnSpr->setColor({150, 150, 150});
        disableBtnSpr->setColor({150, 150, 150});
    }

    bool shouldShowUninstall = mod != Mod::get();
#if defined(GEODE_IS_WINDOWS)
    shouldShowUninstall = shouldShowUninstall ||
        exists((dirs::getGameDir() / "GeodeUninstaller.exe"));
#endif
    if (shouldShowUninstall) {
        auto uninstallBtnSpr =
            ButtonSprite::create("Uninstall", "bigFont.fnt", "GJ_button_05.png", .6f);
        uninstallBtnSpr->setScale(.6f);

        auto uninstallBtn = CCMenuItemSpriteExtra::create(
            uninstallBtnSpr, this, menu_selector(LocalModInfoPopup::onUninstall)
        );
        uninstallBtn->setPosition(-85.f, 75.f);
        m_buttonMenu->addChild(uninstallBtn);

        // todo: show update button on loader that invokes the installer
        if (m_item && Index::get()->isUpdateAvailable(m_item)) {
            m_installBtnSpr = IconButtonSprite::create(
                "GE_button_01.png"_spr,
                CCSprite::createWithSpriteFrameName("install.png"_spr),
                "Update",
                "bigFont.fnt"
            );
            m_installBtnSpr->setScale(.6f);

            m_installBtn = CCMenuItemSpriteExtra::create(m_installBtnSpr, this, menu_selector(LocalModInfoPopup::onUpdate));
            m_installBtn->setPosition(-8.0f, 75.f);
            m_buttonMenu->addChild(m_installBtn);

            m_installStatus = DownloadStatusNode::create();
            m_installStatus->setPosition(winSize.width / 2 + 105.f, winSize.height / 2 + 75.f);
            m_installStatus->setVisible(false);
            m_mainLayer->addChild(m_installStatus);

            auto minorIndexItem = Index::get()->getItem(
                mod->getMetadata().getID(),
                ComparableVersionInfo(mod->getMetadata().getVersion(), VersionCompare::MoreEq)
            );

            // TODO: use column layout here?

            if (m_item->getMetadata().getVersion().getMajor() > minorIndexItem->getMetadata().getVersion().getMajor()) {
                // has major update
                m_latestVersionLabel = CCLabelBMFont::create(
                    ("Available: " + m_item->getMetadata().getVersion().toString()).c_str(),
                    "bigFont.fnt"
                );
                m_latestVersionLabel->setScale(.35f);
                m_latestVersionLabel->setAnchorPoint({.0f, .5f});
                m_latestVersionLabel->setColor({94, 219, 255});
                m_latestVersionLabel->setPosition(winSize.width / 2 + 35.f, winSize.height / 2 + 75.f);
                m_mainLayer->addChild(m_latestVersionLabel);
            }

            if (minorIndexItem->getMetadata().getVersion() > mod->getMetadata().getVersion()) {
                // has minor update
                m_minorVersionLabel = CCLabelBMFont::create(
                    ("Available: " + minorIndexItem->getMetadata().getVersion().toString()).c_str(),
                    "bigFont.fnt"
                );
                m_minorVersionLabel->setScale(.35f);
                m_minorVersionLabel->setAnchorPoint({.0f, .5f});
                m_minorVersionLabel->setColor({94, 219, 255});
                if (m_latestVersionLabel) {
                    m_latestVersionLabel->setPosition(
                        winSize.width / 2 + 35.f, winSize.height / 2 + 81.f
                    );
                    m_minorVersionLabel->setPosition(
                        winSize.width / 2 + 35.f, winSize.height / 2 + 69.f
                    );
                } else {
                    m_minorVersionLabel->setPosition(
                        winSize.width / 2 + 35.f, winSize.height / 2 + 75.f
                    );
                }
                m_mainLayer->addChild(m_minorVersionLabel);
            }
        }
    }
    if (mod == Mod::get()) {
        // we're showing the internal geode mod :-)
        auto* label = CCLabelBMFont::create(
            fmt::format("Bindings: {}\nLoader: {}", BINDINGS_COMMIT_HASH, LOADER_COMMIT_HASH).c_str(),
            "chatFont.fnt"
        );
        label->setAlignment(kCCTextAlignmentRight);
        label->setAnchorPoint(ccp(1, 0));
        label->setScale(0.6f);
        label->setPosition(winSize.width - 3.f, 3.f);
        label->setOpacity(89);
        m_mainLayer->addChild(label);
    }

    // issue report button
    if (mod->getMetadata().getIssues()) {
        auto issuesBtnSpr = ButtonSprite::create(
            "Report an Issue", "goldFont.fnt", "GJ_button_04.png", .8f
        );
        issuesBtnSpr->setScale(.75f);

        auto issuesBtn = CCMenuItemSpriteExtra::create(
            issuesBtnSpr, this, menu_selector(LocalModInfoPopup::onIssues)
        );
        issuesBtn->setPosition(0.f, -LAYER_SIZE.height / 2 + 25.f);
        m_buttonMenu->addChild(issuesBtn);
    }

    return true;
}

CCNode* LocalModInfoPopup::createLogo(CCSize const& size) {
    return geode::createModLogo(m_mod, size);
}

ModMetadata LocalModInfoPopup::getMetadata() const {
    return m_mod->getMetadata();
}

void LocalModInfoPopup::onIssues(CCObject*) {
    geode::openIssueReportPopup(m_mod);
}

void LocalModInfoPopup::onUpdateProgress(ModInstallEvent* event) {
    std::visit(makeVisitor {
        [&](UpdateFinished const&) {
            this->setInstallStatus(std::nullopt);

            FLAlertLayer::create(
                "Update Complete",
                "Mod updated successfully!\n"
                "<cy>Restart</c> the game to apply changes.",
                "OK"
            )->show();

            if (m_layer) {
                m_layer->reloadList();
            }
            this->onClose(nullptr);
        },
        [&](UpdateProgress const& progress) {
            this->setInstallStatus(progress);
        },
        [&](UpdateFailed const& info) {
            this->setInstallStatus(std::nullopt);

            FLAlertLayer::create(
                "Update Failed", info, "OK"
            )->show();

            m_installBtn->setEnabled(true);
            m_installBtn->setTarget(
                this, menu_selector(LocalModInfoPopup::onUpdate)
            );
            m_installBtnSpr->setString("Update");
            m_installBtnSpr->setBG("GE_button_01.png"_spr, false);
        }
    }, event->status);
}

void LocalModInfoPopup::onUpdate(CCObject*) {
    this->popupInstallItem(m_item);
}

void LocalModInfoPopup::onUninstall(CCObject*) {
    auto layer = FLAlertLayer::create(
        this,
        "Confirm Uninstall",
        fmt::format("Are you sure you want to uninstall <cr>{}</c>?{}", m_mod->getName(),
            m_mod == Mod::get() ?
            "\nThis will close the game and launch the <co>Geode Uninstaller</c>. "
            "You will have to restart the game <cy>manually</c> after that." : ""),
        "Cancel",
        "OK"
    );
    layer->setTag(TAG_CONFIRM_UNINSTALL);
    layer->show();
}

void LocalModInfoPopup::onEnableMod(CCObject* sender) {
    if (!Mod::get()->setSavedValue("shown-mod-toggle-info", true)) {
        FLAlertLayer::create(
            "Notice",
            "<cb>Toggling</c> a mod requires you to <cg>restart</c> the game.\n"
            "When a mod is <cr>disabled</c>, it will not get loaded at all.",
            "OK"
        )->show();
    }
    if (as<CCMenuItemToggler*>(sender)->isToggled()) {
        auto res = m_mod->enable();
        if (!res) {
            FLAlertLayer::create(nullptr, "Error Enabling Mod", res.unwrapErr(), "OK", nullptr)->show();
        }
    }
    else {
        auto res = m_mod->disable();
        if (!res) {
            FLAlertLayer::create(nullptr, "Error Disabling Mod", res.unwrapErr(), "OK", nullptr)->show();
        }
    }
    if (m_layer) {
        m_layer->reloadList();
    }
    as<CCMenuItemToggler*>(sender)->toggle(m_mod->shouldLoad());
}

void LocalModInfoPopup::onOpenConfigDir(CCObject*) {
    file::openFolder(m_mod->getConfigDir());
}

void LocalModInfoPopup::onDisablingNotSupported(CCObject* pSender) {
    FLAlertLayer::create("Unsupported", "<cr>Disabling</c> is not supported for this mod.", "OK")->show();
    as<CCMenuItemToggler*>(pSender)->toggle(m_mod->shouldLoad());
}

void LocalModInfoPopup::onSettings(CCObject*) {
    ModSettingsPopup::create(m_mod)->show();
}

void LocalModInfoPopup::onNoSettings(CCObject*) {
    FLAlertLayer::create("No Settings Found", "This mod has no customizable settings.", "OK")
        ->show();
}

void LocalModInfoPopup::FLAlert_Clicked(FLAlertLayer* layer, bool btn2) {
    switch (layer->getTag()) {
        case TAG_CONFIRM_UNINSTALL: {
            if (btn2) {
                auto layer2 = FLAlertLayer::create(
                    this,
                    "Confirm Uninstall",
                    "Would you also like to <cr>delete</c> the mod's <co>save data</c>?",
                    "Keep",
                    "Delete",
                    350.f
                );
                layer2->setTag(TAG_CONFIRM_UNINSTALL_WITH_SAVEDATA);
                layer2->show();
            }
        } break;

        case TAG_CONFIRM_UNINSTALL_WITH_SAVEDATA: {
            auto res = m_mod->uninstall(btn2);
            if (!res) {
                return FLAlertLayer::create("Uninstall Failed", res.unwrapErr(), "OK")->show();
            }
            if (m_layer) {
                m_layer->reloadList();
            }
            this->onClose(nullptr);
        } break;
    }
}

LocalModInfoPopup* LocalModInfoPopup::create(Mod* mod, ModListLayer* list) {
    auto ret = new LocalModInfoPopup;
    if (ret && ret->init(mod, list)) {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

// IndexItemInfoPopup

IndexItemInfoPopup::IndexItemInfoPopup()
  : m_installListener(
        this, &IndexItemInfoPopup::onInstallProgress,
        ModInstallFilter("")
    ) {}

bool IndexItemInfoPopup::init(IndexItemHandle item, ModListLayer* list) {
    m_item = item;
    m_installListener.setFilter(m_item->getMetadata().getID());

    auto winSize = CCDirector::sharedDirector()->getWinSize();

    if (!ModInfoPopup::init(item->getMetadata(), list)) return false;

    // bruh why is this here if we are allowing for browsing already installed mods
    // if (item->isInstalled()) return true;

    m_installBtnSpr = IconButtonSprite::create(
        "GE_button_01.png"_spr,
        CCSprite::createWithSpriteFrameName("install.png"_spr),
        "Install",
        "bigFont.fnt"
    );
    m_installBtnSpr->setScale(.6f);

    m_installBtn = CCMenuItemSpriteExtra::create(
        m_installBtnSpr, this, menu_selector(IndexItemInfoPopup::onInstall)
    );
    m_installBtn->setPosition(-143.0f, 75.f);
    m_buttonMenu->addChild(m_installBtn);

    m_installStatus = DownloadStatusNode::create();
    m_installStatus->setPosition(winSize.width / 2 - 25.f, winSize.height / 2 + 75.f);
    m_installStatus->setVisible(false);
    m_mainLayer->addChild(m_installStatus);

    return true;
}

void IndexItemInfoPopup::onInstallProgress(ModInstallEvent* event) {
    std::visit(makeVisitor {
        [&](UpdateFinished const&) {
            this->setInstallStatus(std::nullopt);

            FLAlertLayer::create(
                "Install Complete",
                "Mod installed successfully!\n"
                "<cy>Restart</c> the game to apply changes.",
                "OK"
            )->show();

            if (m_layer) {
                m_layer->reloadList();
            }
            this->onClose(nullptr);
        },
        [&](UpdateProgress const& progress) {
            this->setInstallStatus(progress);
        },
        [&](UpdateFailed const& info) {
            this->setInstallStatus(std::nullopt);

            FLAlertLayer::create(
                "Installation Failed", info, "OK"
            )->show();

            m_installBtn->setEnabled(true);
            m_installBtn->setTarget(
                this, menu_selector(IndexItemInfoPopup::onInstall)
            );
            m_installBtnSpr->setString("Install");
            m_installBtnSpr->setBG("GE_button_01.png"_spr, false);
        }
    }, event->status);
}

void IndexItemInfoPopup::onInstall(CCObject*) {
    this->popupInstallItem(m_item);
}

CCNode* IndexItemInfoPopup::createLogo(CCSize const& size) {
    return geode::createIndexItemLogo(m_item, size);
}

ModMetadata IndexItemInfoPopup::getMetadata() const {
    return m_item->getMetadata();
}

IndexItemInfoPopup* IndexItemInfoPopup::create(
    IndexItemHandle item, ModListLayer* list
) {
    auto ret = new IndexItemInfoPopup;
    if (ret && ret->init(item, list)) {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}
