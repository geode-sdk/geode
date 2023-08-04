#include "ModInfoPopup.hpp"

#include "../dev/HookListLayer.hpp"
#include "../list/ModListLayer.hpp"
#include "../settings/ModSettingsPopup.hpp"
#include <Geode/loader/Dirs.hpp>

#include <Geode/binding/ButtonSprite.hpp>
#include <Geode/binding/CCTextInputNode.hpp>
#include <Geode/binding/GJListLayer.hpp>
#include <Geode/binding/Slider.hpp>
#include <Geode/binding/SliderThumb.hpp>
#include <Geode/binding/SliderTouchLogic.hpp>
#include <Geode/loader/Dirs.hpp>
#include <Geode/loader/Mod.hpp>
#include <Geode/ui/BasedButton.hpp>
#include <Geode/ui/GeodeUI.hpp>
#include <Geode/ui/IconButtonSprite.hpp>
#include <Geode/ui/GeodeUI.hpp>
#include <Geode/ui/MDPopup.hpp>
#include <Geode/utils/casts.hpp>
#include <Geode/utils/ranges.hpp>
#include <Geode/utils/web.hpp>
#include <loader/LoaderImpl.hpp>

static constexpr int const TAG_CONFIRM_UNINSTALL = 5;
static constexpr int const TAG_CONFIRM_UPDATE = 6;
static constexpr int const TAG_DELETE_SAVEDATA = 7;
static const CCSize LAYER_SIZE = {440.f, 290.f};

bool ModInfoPopup::init(ModInfo const& info, ModListLayer* list) {
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

    auto nameLabel = CCLabelBMFont::create(info.name().c_str(), "bigFont.fnt");
    nameLabel->setAnchorPoint({ .0f, .5f });
    nameLabel->limitLabelWidth(200.f, .7f, .1f);
    m_mainLayer->addChild(nameLabel, 2);

    auto logoSpr = this->createLogo({logoSize, logoSize});
    m_mainLayer->addChild(logoSpr);

    auto developerStr = "by " + info.developer();
    auto developerLabel = CCLabelBMFont::create(developerStr.c_str(), "goldFont.fnt");
    developerLabel->setScale(.5f);
    developerLabel->setAnchorPoint({.0f, .5f});
    m_mainLayer->addChild(developerLabel);

    auto logoTitleWidth =
        std::max(nameLabel->getScaledContentSize().width, developerLabel->getScaledContentSize().width) +
        logoSize + logoOffset;

    nameLabel->setPosition(
        winSize.width / 2 - logoTitleWidth / 2 + logoSize + logoOffset, winSize.height / 2 + 125.f
    );
    logoSpr->setPosition(
        {winSize.width / 2 - logoTitleWidth / 2 + logoSize / 2, winSize.height / 2 + 115.f}
    );
    developerLabel->setPosition(
        winSize.width / 2 - logoTitleWidth / 2 + logoSize + logoOffset, winSize.height / 2 + 105.f
    );

    auto versionLabel = CCLabelBMFont::create(
        info.version().toString().c_str(),
        "bigFont.fnt"
    );
    versionLabel->setAnchorPoint({ .0f, .5f });
    versionLabel->setScale(.4f);
    versionLabel->setPosition(
        nameLabel->getPositionX() + nameLabel->getScaledContentSize().width + 5.f,
        winSize.height / 2 + 125.f
    );
    versionLabel->setColor({0, 255, 0});
    m_mainLayer->addChild(versionLabel);

    this->setTouchEnabled(true);

    m_detailsArea = MDTextArea::create(
        (info.details() ? info.details().value() : "### No description provided."),
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
    if (info.changelog()) {
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

    // mod info
    auto infoSpr = CCSprite::createWithSpriteFrameName("GJ_infoIcon_001.png");
    infoSpr->setScale(.85f);

    m_infoBtn = CCMenuItemSpriteExtra::create(infoSpr, this, menu_selector(ModInfoPopup::onInfo));
    m_infoBtn->setPosition(LAYER_SIZE.width / 2 - 25.f, LAYER_SIZE.height / 2 - 25.f);
    m_buttonMenu->addChild(m_infoBtn);

    // repo button
    if (info.repository()) {
        auto repoBtn = CCMenuItemSpriteExtra::create(
            CCSprite::createWithSpriteFrameName("github.png"_spr),
            this,
            menu_selector(ModInfoPopup::onRepository)
        );
        repoBtn->setPosition(LAYER_SIZE.width / 2 - 25.f, -LAYER_SIZE.height / 2 + 25.f);
        m_buttonMenu->addChild(repoBtn);
    }

    // support button
    if (info.supportInfo()) {
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
        "Support " + this->getModInfo().name(),
        this->getModInfo().supportInfo().value(),
        "OK"
    )->show();
}

void ModInfoPopup::onRepository(CCObject*) {
    web::openLinkInBrowser(this->getModInfo().repository().value());
}

void ModInfoPopup::onInfo(CCObject*) {
    auto info = this->getModInfo();
    FLAlertLayer::create(
        nullptr,
        ("About " + info.name()).c_str(),
        fmt::format(
            "<cr>ID: {}</c>\n"
            "<cg>Version: {}</c>\n"
            "<cp>Developer: {}</c>\n"
            "<cb>Path: {}</c>\n",
            info.id(),
            info.version().toString(),
            info.developer(),
            info.path().string()
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
        m_changelogArea = MDTextArea::create(this->getModInfo().changelog().value(), { 350.f, 137.5f });
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

// LocalModInfoPopup
LocalModInfoPopup::LocalModInfoPopup()
  : m_installListener(
        this, &LocalModInfoPopup::onUpdateProgress,
        ModInstallFilter("")
    ) {}


bool LocalModInfoPopup::init(Mod* mod, ModListLayer* list) {
    m_item = Index::get()->getMajorItem(mod->getModInfo().id());
    if (m_item)
        m_installListener.setFilter(m_item->getModInfo().id());
    m_mod = mod;

    if (!ModInfoPopup::init(mod->getModInfo(), list)) return false;

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
    enableBtn->toggle(!mod->isEnabled());
    m_buttonMenu->addChild(enableBtn);

    if (!mod->supportsDisabling()) {
        enableBtn->setTarget(this, menu_selector(LocalModInfoPopup::onDisablingNotSupported));
        enableBtnSpr->setColor({150, 150, 150});
        disableBtnSpr->setColor({150, 150, 150});
    }

    if (mod != Loader::get()->getModImpl()) {
        auto uninstallBtnSpr = ButtonSprite::create(
            "Uninstall", "bigFont.fnt", "GJ_button_05.png", .6f
        );
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
                mod->getModInfo().id(),
                ComparableVersionInfo(mod->getModInfo().version(), VersionCompare::MoreEq)
            );

            // TODO: use column layout here?

            if (m_item->getModInfo().version().getMajor() > minorIndexItem->getModInfo().version().getMajor()) {
                // has major update
                m_latestVersionLabel = CCLabelBMFont::create(
                    ("Available: " + m_item->getModInfo().version().toString()).c_str(),
                    "bigFont.fnt"
                );
                m_latestVersionLabel->setScale(.35f);
                m_latestVersionLabel->setAnchorPoint({.0f, .5f});
                m_latestVersionLabel->setColor({94, 219, 255});
                m_latestVersionLabel->setPosition(winSize.width / 2 + 35.f, winSize.height / 2 + 75.f);
                m_mainLayer->addChild(m_latestVersionLabel);
            }

            if (minorIndexItem->getModInfo().version() > mod->getModInfo().version()) {
                // has minor update
                m_minorVersionLabel = CCLabelBMFont::create(
                    ("Available: " + minorIndexItem->getModInfo().version().toString()).c_str(),
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
    } else {
        auto* label = CCLabelBMFont::create(LOADER_COMMIT_HASH, "chatFont.fnt");
        label->setAnchorPoint(ccp(1, 0));
        label->setScale(0.775f);
        label->setPosition(winSize.width - 1.f, 1.f);
        label->setOpacity(89);
        m_mainLayer->addChild(label);
    }

    // issue report button
    if (mod->getModInfo().issues()) {
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

ModInfo LocalModInfoPopup::getModInfo() const {
    return m_mod->getModInfo();
}

void LocalModInfoPopup::onIssues(CCObject*) {
    geode::openIssueReportPopup(m_mod);
}

void LocalModInfoPopup::onUpdateProgress(ModInstallEvent* event) {
    std::visit(makeVisitor {
        [&](UpdateFinished const&) {
            this->setInstallStatus(std::nullopt);
            
            FLAlertLayer::create(
                "Update complete",
                "Mod succesfully updated! :) "
                "(You may need to <cy>restart the game</c> "
                "for the mod to take full effect)",
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
                "Update failed :(", info, "OK"
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
    auto list = Index::get()->getInstallList(m_item);
    if (!list) {
        return FLAlertLayer::create(
            "Unable to Update",
            list.unwrapErr(),
            "OK"
        )->show();
    }
    auto layer = FLAlertLayer::create(
        this,
        "Confirm Update",
        fmt::format(
            "The following mods will be updated:\n {}",
            // le nest
            ranges::join(
                ranges::map<std::vector<std::string>>(
                    list.unwrap().list,
                    [](IndexItemHandle handle) {
                        return fmt::format(
                            " - <cr>{}</c> (<cy>{}</c>)",
                            handle->getModInfo().name(),
                            handle->getModInfo().id()
                        );
                    }
                ),
                "\n "
            )
        ),
        "Cancel", "OK"
    );
    layer->setTag(TAG_CONFIRM_UPDATE);
    layer->show();
}

void LocalModInfoPopup::onCancel(CCObject*) {
    Index::get()->cancelInstall(m_item);
}

void LocalModInfoPopup::doUpdate() {
    if (m_latestVersionLabel) {
        m_latestVersionLabel->setVisible(false);
    }

    if (m_minorVersionLabel) {
        m_minorVersionLabel->setVisible(false);
    }
    this->setInstallStatus(UpdateProgress(0, "Starting update"));

    m_installBtn->setTarget(
        this, menu_selector(LocalModInfoPopup::onCancel)
    );
    m_installBtnSpr->setString("Cancel");
    m_installBtnSpr->setBG("GJ_button_06.png", false);

    Index::get()->install(m_item);
}

void LocalModInfoPopup::onUninstall(CCObject*) {
    auto layer = FLAlertLayer::create(
        this,
        "Confirm Uninstall",
        fmt::format("Are you sure you want to uninstall <cr>{}</c>?", m_mod->getName()),
        "Cancel",
        "OK"
    );
    layer->setTag(TAG_CONFIRM_UNINSTALL);
    layer->show();
}

void LocalModInfoPopup::onEnableMod(CCObject* sender) {
    if (!Mod::get()->setSavedValue("shown-disable-vs-unload-info", true)) {
        FLAlertLayer::create(
            "Notice",
            "<cb>Disabling</c> a <cy>mod</c> removes its hooks & patches and "
            "calls its user-defined disable function if one exists. You may "
            "still see some effects of the mod left however, and you may "
            "need to <cg>restart</c> the game to have it fully unloaded.",
            "OK"
        )->show();
    }
    if (as<CCMenuItemToggler*>(sender)->isToggled()) {
        auto res = m_mod->loadBinary();
        if (!res) {
            FLAlertLayer::create(nullptr, "Error Loading Mod", res.unwrapErr(), "OK", nullptr)->show();
        }
    }
    else {
        auto res = m_mod->disable();
        if (!res) {
            FLAlertLayer::create(nullptr, "Error Disabling Mod", res.unwrapErr(), "OK", nullptr)->show();
        }
    }
    if (m_layer) {
        m_layer->updateAllStates(nullptr);
    }
    as<CCMenuItemToggler*>(sender)->toggle(m_mod->isEnabled());
}

void LocalModInfoPopup::onOpenConfigDir(CCObject*) {
    file::openFolder(m_mod->getConfigDir());
}

void LocalModInfoPopup::onDisablingNotSupported(CCObject* pSender) {
    FLAlertLayer::create("Unsupported", "<cr>Disabling</c> is not supported for this mod.", "OK")->show();
    as<CCMenuItemToggler*>(pSender)->toggle(m_mod->isEnabled());
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
                this->doUninstall();
            }
        } break;

        case TAG_DELETE_SAVEDATA: {
            if (btn2) {
                if (ghc::filesystem::remove_all(m_mod->getSaveDir())) {
                    FLAlertLayer::create("Deleted", "The mod's save data was deleted.", "OK")->show();
                }
                else {
                    FLAlertLayer::create("Error", "Unable to delete mod's save directory!", "OK")->show();
                }
            }
            if (m_layer) {
                m_layer->reloadList();
            }
            this->onClose(nullptr);
        } break;

        case TAG_CONFIRM_UPDATE: {
            if (btn2) {
                this->doUpdate();
            }
        } break;
    }
}

void LocalModInfoPopup::doUninstall() {
    auto res = m_mod->uninstall();
    if (!res) {
        return FLAlertLayer::create("Uninstall failed :(", res.unwrapErr(), "OK")->show();
    }
    auto layer = FLAlertLayer::create(
        this,
        "Uninstall complete",
        "Mod was succesfully uninstalled! :) "
        "(You may need to <cy>restart the game</c> "
        "for the mod to take full effect). "
        "<co>Would you also like to delete the mod's "
        "save data?</c>",
        "Cancel",
        "Delete",
        350.f
    );
    layer->setTag(TAG_DELETE_SAVEDATA);
    layer->show();
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
    m_installListener.setFilter(m_item->getModInfo().id());

    auto winSize = CCDirector::sharedDirector()->getWinSize();

    if (!ModInfoPopup::init(item->getModInfo(), list)) return false;

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
                "Install complete",
                "Mod succesfully installed! :) "
                "(You may need to <cy>restart the game</c> "
                "for the mod to take full effect)",
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
                "Installation failed :(", info, "OK"
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
    auto list = Index::get()->getInstallList(m_item);
    if (!list) {
        return FLAlertLayer::create(
            "Unable to Install",
            list.unwrapErr(),
            "OK"
        )->show();
    }
    FLAlertLayer::create(
        this,
        "Confirm Install",
        fmt::format(
            "The following mods will be installed:\n {}",
            // le nest
            ranges::join(
                ranges::map<std::vector<std::string>>(
                    list.unwrap().list,
                    [](IndexItemHandle handle) {
                        return fmt::format(
                            " - <cr>{}</c> (<cy>{}</c>)",
                            handle->getModInfo().name(),
                            handle->getModInfo().id()
                        );
                    }
                ),
                "\n "
            )
        ),
        "Cancel", "OK"
    )->show();
}

void IndexItemInfoPopup::onCancel(CCObject*) {
    Index::get()->cancelInstall(m_item);
}

void IndexItemInfoPopup::doInstall() {
    if (m_latestVersionLabel) {
        m_latestVersionLabel->setVisible(false);
    }
    this->setInstallStatus(UpdateProgress(0, "Starting install"));

    m_installBtn->setTarget(
        this, menu_selector(IndexItemInfoPopup::onCancel)
    );
    m_installBtnSpr->setString("Cancel");
    m_installBtnSpr->setBG("GJ_button_06.png", false);

    Index::get()->install(m_item);
}

void IndexItemInfoPopup::FLAlert_Clicked(FLAlertLayer*, bool btn2) {
    if (btn2) {
        this->doInstall();
    }
}

CCNode* IndexItemInfoPopup::createLogo(CCSize const& size) {
    return geode::createIndexItemLogo(m_item, size);
}

ModInfo IndexItemInfoPopup::getModInfo() const {
    return m_item->getModInfo();
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
