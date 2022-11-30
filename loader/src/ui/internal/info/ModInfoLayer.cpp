#include "ModInfoLayer.hpp"

#include "../dev/HookListLayer.hpp"
#include "../list/ModListView.hpp"
#include "../settings/ModSettingsPopup.hpp"
#include "../settings/AdvancedSettingsPopup.hpp"
#include <InternalLoader.hpp>

#include <Geode/binding/ButtonSprite.hpp>
#include <Geode/binding/CCTextInputNode.hpp>
#include <Geode/binding/GJListLayer.hpp>
#include <Geode/binding/Slider.hpp>
#include <Geode/binding/SliderThumb.hpp>
#include <Geode/binding/SliderTouchLogic.hpp>
#include <Geode/loader/Mod.hpp>
#include <Geode/ui/BasedButton.hpp>
#include <Geode/ui/IconButtonSprite.hpp>
#include <Geode/ui/MDPopup.hpp>
#include <Geode/utils/casts.hpp>
#include <Geode/utils/ranges.hpp>
#include <InternalLoader.hpp>

// TODO: die
#undef min
#undef max

static constexpr int const TAG_CONFIRM_UNINSTALL = 5;
static constexpr int const TAG_DELETE_SAVEDATA = 6;

bool DownloadStatusNode::init() {
    if (!CCNode::init()) return false;

    this->setContentSize({ 150.f, 25.f });

    auto bg = CCScale9Sprite::create("square02b_001.png", { 0.0f, 0.0f, 80.0f, 80.0f });
    bg->setScale(.33f);
    bg->setColor({ 0, 0, 0 });
    bg->setOpacity(75);
    bg->setContentSize(m_obContentSize * 3);
    this->addChild(bg);

    m_bar = Slider::create(this, nullptr, .6f);
    m_bar->setValue(.0f);
    m_bar->updateBar();
    m_bar->setPosition(0.f, -5.f);
    m_bar->m_touchLogic->m_thumb->setVisible(false);
    this->addChild(m_bar);

    m_label = CCLabelBMFont::create("", "bigFont.fnt");
    m_label->setAnchorPoint({ .0f, .5f });
    m_label->setScale(.45f);
    m_label->setPosition(-m_obContentSize.width / 2 + 15.f, 5.f);
    this->addChild(m_label);

    return true;
}

DownloadStatusNode* DownloadStatusNode::create() {
    auto ret = new DownloadStatusNode();
    if (ret && ret->init()) {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

void DownloadStatusNode::setProgress(uint8_t progress) {
    m_bar->setValue(progress / 100.f);
    m_bar->updateBar();
}

void DownloadStatusNode::setStatus(std::string const& text) {
    m_label->setString(text.c_str());
    m_label->limitLabelWidth(m_obContentSize.width - 30.f, .5f, .1f);
}

void ModInfoLayer::onChangelog(CCObject* sender) {
    auto toggle = static_cast<CCMenuItemToggler*>(sender);
    auto winSize = CCDirector::get()->getWinSize();

    m_detailsArea->setVisible(toggle->isToggled());
    // as it turns out, cocos2d is stupid and still passes touch
    // events to invisible nodes
    m_detailsArea->setPositionX(
        toggle->isToggled() ? winSize.width / 2 - m_detailsArea->getScaledContentSize().width / 2
                            : -5000.f
    );

    m_changelogArea->setVisible(!toggle->isToggled());
    // as it turns out, cocos2d is stupid and still passes touch
    // events to invisible nodes
    m_changelogArea->setPositionX(
        !toggle->isToggled() ? winSize.width / 2 - m_changelogArea->getScaledContentSize().width / 2
                             : -5000.f
    );
}

bool ModInfoLayer::init(ModObject* obj, ModListView* list) {
    m_noElasticity = true;
    m_list = list;
    m_mod = obj->m_mod;

    bool isInstalledMod;
    switch (obj->m_type) {
        case ModObjectType::Mod:
            {
                m_info = obj->m_mod->getModInfo();
                isInstalledMod = true;
            }
            break;

        case ModObjectType::Index:
            {
                m_info = obj->m_index.m_info;
                isInstalledMod = false;
            }
            break;

        default: return false;
    }

    auto winSize = CCDirector::sharedDirector()->getWinSize();
    CCSize size { 440.f, 290.f };

    if (!this->initWithColor({ 0, 0, 0, 105 })) return false;
    m_mainLayer = CCLayer::create();
    this->addChild(m_mainLayer);

    auto bg = CCScale9Sprite::create("GJ_square01.png", { 0.0f, 0.0f, 80.0f, 80.0f });
    bg->setContentSize(size);
    bg->setPosition(winSize.width / 2, winSize.height / 2);
    bg->setZOrder(-10);
    m_mainLayer->addChild(bg);

    m_buttonMenu = CCMenu::create();
    m_mainLayer->addChild(m_buttonMenu);

    constexpr float logoSize = 40.f;
    constexpr float logoOffset = 10.f;

    auto nameLabel = CCLabelBMFont::create(m_info.m_name.c_str(), "bigFont.fnt");
    nameLabel->setAnchorPoint({ .0f, .5f });
    nameLabel->limitLabelWidth(200.f, .7f, .1f);
    m_mainLayer->addChild(nameLabel, 2);

    auto logoSpr = this->createLogoSpr(obj, { logoSize, logoSize });
    m_mainLayer->addChild(logoSpr);

    auto developerStr = "by " + m_info.m_developer;
    auto developerLabel = CCLabelBMFont::create(developerStr.c_str(), "goldFont.fnt");
    developerLabel->setScale(.5f);
    developerLabel->setAnchorPoint({ .0f, .5f });
    m_mainLayer->addChild(developerLabel);

    auto logoTitleWidth =
        std::max(
            nameLabel->getScaledContentSize().width, developerLabel->getScaledContentSize().width
        ) +
        logoSize + logoOffset;

    nameLabel->setPosition(
        winSize.width / 2 - logoTitleWidth / 2 + logoSize + logoOffset, winSize.height / 2 + 125.f
    );
    logoSpr->setPosition({ winSize.width / 2 - logoTitleWidth / 2 + logoSize / 2,
                           winSize.height / 2 + 115.f });
    developerLabel->setPosition(
        winSize.width / 2 - logoTitleWidth / 2 + logoSize + logoOffset, winSize.height / 2 + 105.f
    );

    auto versionLabel = CCLabelBMFont::create(m_info.m_version.toString().c_str(), "bigFont.fnt");
    versionLabel->setAnchorPoint({ .0f, .5f });
    versionLabel->setScale(.4f);
    versionLabel->setPosition(
        nameLabel->getPositionX() + nameLabel->getScaledContentSize().width + 5.f,
        winSize.height / 2 + 125.f
    );
    versionLabel->setColor({ 0, 255, 0 });
    m_mainLayer->addChild(versionLabel);

    CCDirector::sharedDirector()->getTouchDispatcher()->incrementForcePrio(2);
    this->registerWithTouchDispatcher();

    m_detailsArea = MDTextArea::create(
        m_info.m_details ? m_info.m_details.value() : "### No description provided.",
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
    if (m_info.m_changelog) {
        m_changelogArea = MDTextArea::create(m_info.m_changelog.value(), { 350.f, 137.5f });
        m_changelogArea->setPosition(
            -5000.f, winSize.height / 2 - m_changelogArea->getScaledContentSize().height / 2 - 20.f
        );
        m_changelogArea->setVisible(false);
        m_mainLayer->addChild(m_changelogArea);

        auto changelogBtnOffSpr = ButtonSprite::create(
            CCSprite::createWithSpriteFrameName("changelog.png"_spr), 0x20, true, 32.f,
            "GJ_button_01.png", 1.f
        );
        changelogBtnOffSpr->setScale(.65f);

        auto changelogBtnOnSpr = ButtonSprite::create(
            CCSprite::createWithSpriteFrameName("changelog.png"_spr), 0x20, true, 32.f,
            "GJ_button_02.png", 1.f
        );
        changelogBtnOnSpr->setScale(.65f);

        auto changelogBtn = CCMenuItemToggler::create(
            changelogBtnOffSpr, changelogBtnOnSpr, this, menu_selector(ModInfoLayer::onChangelog)
        );
        changelogBtn->setPosition(-size.width / 2 + 21.5f, .0f);
        m_buttonMenu->addChild(changelogBtn);
    }

    // mod info
    auto infoSpr = CCSprite::createWithSpriteFrameName("GJ_infoIcon_001.png");
    infoSpr->setScale(.85f);

    auto infoBtn =
        CCMenuItemSpriteExtra::create(infoSpr, this, menu_selector(ModInfoLayer::onInfo));
    infoBtn->setPosition(size.width / 2 - 25.f, size.height / 2 - 25.f);
    m_buttonMenu->addChild(infoBtn);

    // issue report button
    if (m_info.m_issues) {
        auto issuesBtnSpr =
            ButtonSprite::create("Report an Issue", "goldFont.fnt", "GJ_button_04.png", .8f);
        issuesBtnSpr->setScale(.75f);

        auto issuesBtn = CCMenuItemSpriteExtra::create(
            issuesBtnSpr, this, menu_selector(ModInfoLayer::onIssues)
        );
        issuesBtn->setPosition(0.f, -size.height / 2 + 25.f);
        m_buttonMenu->addChild(issuesBtn);
    }

    if (isInstalledMod) {
        // mod settings
        auto settingsSpr = CCSprite::createWithSpriteFrameName(
            "GJ_optionsBtn_001.png"
        );
        settingsSpr->setScale(.65f);

        auto settingsBtn = CCMenuItemSpriteExtra::create(
            settingsSpr, this, menu_selector(ModInfoLayer::onSettings)
        );
        settingsBtn->setPosition(-size.width / 2 + 25.f, -size.height / 2 + 25.f);
        m_buttonMenu->addChild(settingsBtn);

        // Check if a config directory for the mod exists
        // Mod::getConfigDir auto-creates the directory for user convenience, so
        // have to do it manually
        if (ghc::filesystem::exists(
                Loader::get()->getGeodeDirectory() / GEODE_CONFIG_DIRECTORY / m_mod->getID()
            )) {
            auto configSpr = CircleButtonSprite::createWithSpriteFrameName(
                "pencil.png"_spr, 1.f, CircleBaseColor::Green, CircleBaseSize::Medium2
            );
            configSpr->setScale(.65f);

            auto configBtn = CCMenuItemSpriteExtra::create(
                configSpr, this, menu_selector(ModInfoLayer::onOpenConfigDir)
            );
            configBtn->setPosition(-size.width / 2 + 65.f, -size.height / 2 + 25.f);
            m_buttonMenu->addChild(configBtn);
        }

        if (!m_mod->hasSettings()) {
            settingsSpr->setColor({ 150, 150, 150 });
            settingsBtn->setTarget(this, menu_selector(ModInfoLayer::onNoSettings));
        }

        if (m_mod->getModInfo().m_repository) {
            auto repoBtn = CCMenuItemSpriteExtra::create(
                CCSprite::createWithSpriteFrameName("github.png"_spr), this,
                menu_selector(ModInfoLayer::onRepository)
            );
            repoBtn->setPosition(size.width / 2 - 25.f, -size.height / 2 + 25.f);
            m_buttonMenu->addChild(repoBtn);
        }

        if (m_mod->getModInfo().m_supportInfo) {
            auto supportBtn = CCMenuItemSpriteExtra::create(
                CCSprite::createWithSpriteFrameName("gift.png"_spr), this,
                menu_selector(ModInfoLayer::onSupport)
            );
            supportBtn->setPosition(size.width / 2 - 60.f, -size.height / 2 + 25.f);
            m_buttonMenu->addChild(supportBtn);
        }

        auto enableBtnSpr = ButtonSprite::create("Enable", "bigFont.fnt", "GJ_button_01.png", .6f);
        enableBtnSpr->setScale(.6f);

        auto disableBtnSpr =
            ButtonSprite::create("Disable", "bigFont.fnt", "GJ_button_06.png", .6f);
        disableBtnSpr->setScale(.6f);

        auto enableBtn = CCMenuItemToggler::create(
            disableBtnSpr, enableBtnSpr, this, menu_selector(ModInfoLayer::onEnableMod)
        );
        enableBtn->setPosition(-155.f, 75.f);
        enableBtn->toggle(!obj->m_mod->isEnabled());
        m_buttonMenu->addChild(enableBtn);

        if (!m_info.m_supportsDisabling) {
            enableBtn->setTarget(this, menu_selector(ModInfoLayer::onDisablingNotSupported));
            enableBtnSpr->setColor({ 150, 150, 150 });
            disableBtnSpr->setColor({ 150, 150, 150 });
        }

        if (
            m_mod != Loader::get()->getInternalMod() &&
            m_mod != Mod::get()
        ) {
            // advanced settings
            auto advSettSpr = CCSprite::createWithSpriteFrameName("GJ_optionsBtn02_001.png");
            advSettSpr->setScale(.65f);

            auto advSettBtn = CCMenuItemSpriteExtra::create(
                advSettSpr, this, menu_selector(ModInfoLayer::onAdvancedSettings)
            );
            advSettBtn->setPosition(
                infoBtn->getPositionX() - 30.f,
                infoBtn->getPositionY()
            );
            m_buttonMenu->addChild(advSettBtn);

            auto uninstallBtnSpr = ButtonSprite::create(
                "Uninstall", "bigFont.fnt", "GJ_button_05.png", .6f
            );
            uninstallBtnSpr->setScale(.6f);

            auto uninstallBtn = CCMenuItemSpriteExtra::create(
                uninstallBtnSpr, this, menu_selector(ModInfoLayer::onUninstall)
            );
            uninstallBtn->setPosition(-85.f, 75.f);
            m_buttonMenu->addChild(uninstallBtn);

            // todo: show update button on loader that invokes the installer
            if (Index::get()->isUpdateAvailableForItem(m_info.m_id)) {
                m_installBtnSpr = IconButtonSprite::create(
                    "GE_button_01.png"_spr, CCSprite::createWithSpriteFrameName("install.png"_spr),
                    "Update", "bigFont.fnt"
                );
                m_installBtnSpr->setScale(.6f);

                m_installBtn = CCMenuItemSpriteExtra::create(
                    m_installBtnSpr, this, menu_selector(ModInfoLayer::onInstallMod)
                );
                m_installBtn->setPosition(-8.0f, 75.f);
                m_buttonMenu->addChild(m_installBtn);

                m_installStatus = DownloadStatusNode::create();
                m_installStatus->setPosition(winSize.width / 2 + 105.f, winSize.height / 2 + 75.f);
                m_installStatus->setVisible(false);
                m_mainLayer->addChild(m_installStatus);

                auto incomingVersion =
                    Index::get()->getKnownItem(m_info.m_id).m_info.m_version.toString();

                m_updateVersionLabel =
                    CCLabelBMFont::create(("Available: " + incomingVersion).c_str(), "bigFont.fnt");
                m_updateVersionLabel->setScale(.35f);
                m_updateVersionLabel->setAnchorPoint({ .0f, .5f });
                m_updateVersionLabel->setColor({ 94, 219, 255 });
                m_updateVersionLabel->setPosition(
                    winSize.width / 2 + 35.f, winSize.height / 2 + 75.f
                );
                m_mainLayer->addChild(m_updateVersionLabel);
            }
        }
    }
    else {
        m_installBtnSpr = IconButtonSprite::create(
            "GE_button_01.png"_spr, CCSprite::createWithSpriteFrameName("install.png"_spr),
            "Install", "bigFont.fnt"
        );
        m_installBtnSpr->setScale(.6f);

        m_installBtn = CCMenuItemSpriteExtra::create(
            m_installBtnSpr, this, menu_selector(ModInfoLayer::onInstallMod)
        );
        m_installBtn->setPosition(-143.0f, 75.f);
        m_buttonMenu->addChild(m_installBtn);

        m_installStatus = DownloadStatusNode::create();
        m_installStatus->setPosition(winSize.width / 2 - 25.f, winSize.height / 2 + 75.f);
        m_installStatus->setVisible(false);
        m_mainLayer->addChild(m_installStatus);
    }

    // check if this mod is being installed/updated, and if so, update UI
    if (auto handle = Index::get()->isInstallingItem(m_info.m_id)) {
        m_installation = handle;
        this->install();
    }

    auto closeSpr = CCSprite::createWithSpriteFrameName("GJ_closeBtn_001.png");
    closeSpr->setScale(.8f);

    auto closeBtn =
        CCMenuItemSpriteExtra::create(closeSpr, this, menu_selector(ModInfoLayer::onClose));
    closeBtn->setPosition(-size.width / 2 + 3.f, size.height / 2 - 3.f);
    m_buttonMenu->addChild(closeBtn);

    this->setKeypadEnabled(true);
    this->setTouchEnabled(true);

    return true;
}

void ModInfoLayer::onIssues(CCObject*) {
    ModInfoLayer::showIssueReportPopup(m_info);
}

void ModInfoLayer::onSupport(CCObject*) {
    MDPopup::create("Support " + m_mod->getName(), m_mod->getModInfo().m_supportInfo.value(), "OK")
        ->show();
}

void ModInfoLayer::onEnableMod(CCObject* pSender) {
    if (!InternalLoader::get()->shownInfoAlert("mod-disable-vs-unload")) {
        FLAlertLayer::create(
            "Notice",
            "You may still see some effects of the mod left, and you may "
            "need to <cg>restart</c> the game to have it fully unloaded.",
            "OK"
        )
            ->show();
        if (m_list) m_list->updateAllStates(nullptr);
        return;
    }
    if (as<CCMenuItemToggler*>(pSender)->isToggled()) {
        auto res = m_mod->loadBinary();
        if (!res) {
            FLAlertLayer::create(
                nullptr, "Error Loading Mod",
                res.unwrapErr(), "OK", nullptr
            )->show();
        }
    }
    else {
        auto res = m_mod->disable();
        if (!res) {
            FLAlertLayer::create(
                nullptr, "Error Disabling Mod",
                res.unwrapErr(), "OK", nullptr
            )->show();
        }
    }
    if (m_list) m_list->updateAllStates(nullptr);
    as<CCMenuItemToggler*>(pSender)->toggle(m_mod->isEnabled());
}

void ModInfoLayer::onRepository(CCObject*) {
    web::openLinkInBrowser(m_mod->getModInfo().m_repository.value());
}

void ModInfoLayer::onInstallMod(CCObject*) {
    auto ticketRes = Index::get()->installItem(Index::get()->getKnownItem(m_info.m_id));
    if (!ticketRes) {
        return FLAlertLayer::create(
            "Unable to install", ticketRes.unwrapErr(), "OK"
        )->show();
    }
    m_installation = ticketRes.unwrap();

    createQuickPopup(
        "Install",
        "The following <cb>mods</c> will be installed: " +
            ranges::join(m_installation->toInstall(), ",") + ".",
        "Cancel", "OK",
        [this](FLAlertLayer*, bool btn2) {
            if (btn2) {
                this->install();
            }
            else {
                this->updateInstallStatus("", 0);
            }
        }
    );
}

void ModInfoLayer::onCancelInstall(CCObject*) {
    m_installBtn->setEnabled(false);
    m_installBtnSpr->setString("Cancelling");
    m_installation->cancel();
    m_installation = nullptr;
    if (m_updateVersionLabel) {
        m_updateVersionLabel->setVisible(true);
    }
}

void ModInfoLayer::onOpenConfigDir(CCObject*) {
    file::openFolder(m_mod->getConfigDir());
}

void ModInfoLayer::onUninstall(CCObject*) {
    auto layer = FLAlertLayer::create(
        this, "Confirm Uninstall",
        "Are you sure you want to uninstall <cr>" + m_info.m_name + "</c>?", "Cancel", "OK"
    );
    layer->setTag(TAG_CONFIRM_UNINSTALL);
    layer->show();
}

void ModInfoLayer::FLAlert_Clicked(FLAlertLayer* layer, bool btn2) {
    switch (layer->getTag()) {
        case TAG_CONFIRM_UNINSTALL:
            {
                if (btn2) {
                    this->uninstall();
                }
            }
            break;

        case TAG_DELETE_SAVEDATA:
            {
                if (btn2) {
                    if (ghc::filesystem::remove_all(m_mod->getSaveDir())) {
                        FLAlertLayer::create("Deleted", "The mod's save data was deleted.", "OK")
                            ->show();
                    }
                    else {
                        FLAlertLayer::create(
                            "Error", "Unable to delete mod's save directory!", "OK"
                        )
                            ->show();
                    }
                }
                if (m_list) m_list->refreshList();
                this->onClose(nullptr);
            }
            break;
    }
}

void ModInfoLayer::updateInstallStatus(std::string const& status, uint8_t progress) {
    if (status.size()) {
        m_installStatus->setVisible(true);
        m_installStatus->setStatus(status);
        m_installStatus->setProgress(progress);
    }
    else {
        m_installStatus->setVisible(false);
    }
}

void ModInfoLayer::modInstallProgress(
    InstallHandle, UpdateStatus status, std::string const& info, uint8_t percentage
) {
    switch (status) {
        case UpdateStatus::Failed:
            {
                FLAlertLayer::create("Installation failed :(", info, "OK")->show();
                this->updateInstallStatus("", 0);

                m_installBtn->setEnabled(true);
                m_installBtn->setTarget(this, menu_selector(ModInfoLayer::onInstallMod));
                m_installBtnSpr->setString("Install");
                m_installBtnSpr->setBG("GE_button_01.png"_spr, false);

                m_installation = nullptr;
            }
            break;

        case UpdateStatus::Finished:
            {
                this->updateInstallStatus("", 100);

                FLAlertLayer::create(
                    "Install complete",
                    "Mod succesfully installed! :) "
                    "(You may need to <cy>restart the game</c> "
                    "for the mod to take full effect)",
                    "OK"
                )
                    ->show();

                m_installation = nullptr;

                if (m_list) m_list->refreshList();
                this->onClose(nullptr);
            }
            break;

        default:
            {
                this->updateInstallStatus(info, percentage);
            }
            break;
    }
}

void ModInfoLayer::install() {
    if (m_updateVersionLabel) {
        m_updateVersionLabel->setVisible(false);
    }
    this->updateInstallStatus("Starting install", 0);

    m_installBtn->setTarget(this, menu_selector(ModInfoLayer::onCancelInstall));
    m_installBtnSpr->setString("Cancel");
    m_installBtnSpr->setBG("GJ_button_06.png", false);

    m_callbackID = m_installation->start(std::bind(
        &ModInfoLayer::modInstallProgress, this, std::placeholders::_1, std::placeholders::_2,
        std::placeholders::_3, std::placeholders::_4
    ));
}

void ModInfoLayer::uninstall() {
    auto res = m_mod->uninstall();
    if (!res) {
        return FLAlertLayer::create(
            "Uninstall failed :(", res.unwrapErr(), "OK"
        )->show();
    }
    auto layer = FLAlertLayer::create(
        this, "Uninstall complete",
        "Mod was succesfully uninstalled! :) "
        "(You may need to <cy>restart the game</c> "
        "for the mod to take full effect). "
        "<co>Would you also like to delete the mod's "
        "save data?</c>",
        "Cancel", "Delete", 350.f
    );
    layer->setTag(TAG_DELETE_SAVEDATA);
    layer->show();
}

void ModInfoLayer::onDisablingNotSupported(CCObject* pSender) {
    FLAlertLayer::create("Unsupported", "<cr>Disabling</c> is not supported for this mod.", "OK")
        ->show();
    as<CCMenuItemToggler*>(pSender)->toggle(m_mod->isEnabled());
}

void ModInfoLayer::onHooks(CCObject*) {
    auto layer = HookListLayer::create(this->m_mod);
    this->addChild(layer);
    layer->showLayer(false);
}

void ModInfoLayer::onSettings(CCObject*) {
    ModSettingsPopup::create(m_mod)->show();
}

void ModInfoLayer::onNoSettings(CCObject*) {
    FLAlertLayer::create("No Settings Found", "This mod has no customizable settings.", "OK")
        ->show();
}

void ModInfoLayer::onAdvancedSettings(CCObject*) {
    AdvancedSettingsPopup::create(m_mod)->show();
}

void ModInfoLayer::onInfo(CCObject*) {
    FLAlertLayer::create(
        nullptr, ("About " + m_info.m_name).c_str(),
        "<cr>ID: " + m_info.m_id +
            "</c>\n"
            "<cg>Version: " +
            m_info.m_version.toString() +
            "</c>\n"
            "<cp>Developer: " +
            m_info.m_developer +
            "</c>\n"
            "<cb>Path: " +
            m_info.m_path.string() + "</c>\n",
        "OK", nullptr, 400.f
    )
        ->show();
}

void ModInfoLayer::keyDown(enumKeyCodes key) {
    if (key == KEY_Escape) return this->onClose(nullptr);
    if (key == KEY_Space) return;

    return FLAlertLayer::keyDown(key);
}

void ModInfoLayer::onClose(CCObject* pSender) {
    this->setKeyboardEnabled(false);
    this->removeFromParentAndCleanup(true);
    if (m_installation) {
        m_installation->leave(m_callbackID);
    }
};

ModInfoLayer* ModInfoLayer::create(Mod* mod, ModListView* list) {
    auto ret = new ModInfoLayer;
    if (ret && ret->init(new ModObject(mod), list)) {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

ModInfoLayer* ModInfoLayer::create(ModObject* obj, ModListView* list) {
    auto ret = new ModInfoLayer;
    if (ret && ret->init(obj, list)) {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

CCNode* ModInfoLayer::createLogoSpr(ModObject* modObj, CCSize const& size) {
    switch (modObj->m_type) {
        case ModObjectType::Mod:
            {
                return ModInfoLayer::createLogoSpr(modObj->m_mod, size);
            }
            break;

        case ModObjectType::Index:
            {
                return ModInfoLayer::createLogoSpr(modObj->m_index, size);
            }
            break;

        default:
            {
                auto spr = CCSprite::createWithSpriteFrameName("no-logo.png"_spr);
                spr->setScaleX(size.width / spr->getContentSize().width);
                spr->setScaleY(size.height / spr->getContentSize().height);
                if (!spr) {
                    return CCLabelBMFont::create("OwO", "goldFont.fnt");
                }
                return spr;
            }
            break;
    }
}

CCNode* ModInfoLayer::createLogoSpr(Mod* mod, CCSize const& size) {
    CCNode* spr = nullptr;
    if (mod == Loader::getInternalMod()) {
        spr = CCSprite::createWithSpriteFrameName("geode-logo.png"_spr);
    }
    else {
        spr = CCSprite::create(
            fmt::format("{}/logo.png", mod->getID()).c_str()
        );
    }
    if (!spr) spr = CCSprite::createWithSpriteFrameName("no-logo.png"_spr);
    if (!spr) spr = CCLabelBMFont::create("OwO", "goldFont.fnt");
    spr->setScaleX(size.width / spr->getContentSize().width);
    spr->setScaleY(size.height / spr->getContentSize().height);
    return spr;
}

CCNode* ModInfoLayer::createLogoSpr(IndexItem const& item, CCSize const& size) {
    CCNode* spr = nullptr;
    auto logoPath = ghc::filesystem::absolute(item.m_path / "logo.png");
    spr = CCSprite::create(logoPath.string().c_str());
    if (!spr) {
        spr = CCSprite::createWithSpriteFrameName("no-logo.png"_spr);
    }
    if (!spr) {
        spr = CCLabelBMFont::create("OwO", "goldFont.fnt");
    }

    if (Index::get()->isFeaturedItem(item.m_info.m_id)) {
        auto glowSize = size + CCSize(4.f, 4.f);

        auto logoGlow = CCSprite::createWithSpriteFrameName("logo-glow.png"_spr);
        logoGlow->setScaleX(glowSize.width / logoGlow->getContentSize().width);
        logoGlow->setScaleY(glowSize.height / logoGlow->getContentSize().height);
        
        // i dont know why + 1 is needed and its too late for me to figure out why
        spr->setPosition(
            logoGlow->getContentSize().width / 2, logoGlow->getContentSize().height / 2
        );
        // scary mathematics
        spr->setScaleX(size.width / spr->getContentSize().width / logoGlow->getScaleX());
        spr->setScaleY(size.height / spr->getContentSize().height / logoGlow->getScaleY());
        logoGlow->addChild(spr);
        spr = logoGlow;
    }
    else {
        spr->setScaleX(size.width / spr->getContentSize().width);
        spr->setScaleY(size.height / spr->getContentSize().height);
    }

    return spr;
}

void ModInfoLayer::showIssueReportPopup(ModInfo const& info) {
    if (info.m_issues) {
        MDPopup::create(
            "Issue Report",
            info.m_issues.value().m_info +
                "\n\n"
                "If your issue relates to a <cr>game crash</c>, <cb>please include</c> the "
                "latest crash log(s) from `" +
                Loader::get()->getCrashLogDirectory().string() + "`",
            "OK", (info.m_issues.value().m_url ? "Open URL" : ""),
            [info](bool btn2) {
                if (btn2) {
                    web::openLinkInBrowser(info.m_issues.value().m_url.value());
                }
            }
        )->show();
    }
    else {
        MDPopup::create(
            "Issue Report",
            "Please report your issue on the "
            "[#support](https://discord.com/channels/911701438269386882/979352389985390603) "
            "channnel in the [Geode Discord Server](https://discord.gg/9e43WMKzhp)\n\n"
            "If your issue relates to a <cr>game crash</c>, <cb>please include</c> the "
            "latest crash log(s) from `" +
                Loader::get()->getCrashLogDirectory().string() + "`",
            "OK"
        )
            ->show();
    }
}
