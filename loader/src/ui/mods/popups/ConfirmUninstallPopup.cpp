#include "ConfirmUninstallPopup.hpp"
#include <Geode/binding/ButtonSprite.hpp>
#include <Geode/binding/TextArea.hpp>

bool ConfirmUninstallPopup::setup(Mod* mod) {
    m_mod = mod;

    this->setTitle("Uninstall " + mod->getName());

    auto text = TextArea::create(
        fmt::format("Are you sure you want to \n<cr>uninstall</c> <cy>{}</c>?", mod->getName()),
        "chatFont.fnt",
        1.f, m_size.width - 35,
        ccp(.5f, .5f), 20.f, false
    );
    m_mainLayer->addChildAtPosition(text, Anchor::Center, ccp(0, 20));

    auto deleteDataLabel = CCLabelBMFont::create("Delete the Mod's save data", "bigFont.fnt");
    deleteDataLabel->setScale(.35f);
    m_buttonMenu->addChildAtPosition(deleteDataLabel, Anchor::Center, ccp(-70, -15), ccp(0, .5f));

    if (mod->isInternal()) {
        deleteDataLabel->setString("Delete ALL mods and their save data");
        deleteDataLabel->setScale(0.275f);
    }

    m_deleteDataToggle = CCMenuItemToggler::createWithStandardSprites(this, nullptr, .6f);
    m_buttonMenu->addChildAtPosition(m_deleteDataToggle, Anchor::Center, ccp(-88, -15));

    auto cancelSpr = ButtonSprite::create("Cancel", "goldFont.fnt", "GJ_button_01.png", .8f);
    cancelSpr->setScale(.85f);
    auto cancelBtn = CCMenuItemSpriteExtra::create(
        cancelSpr, this, menu_selector(ConfirmUninstallPopup::onClose)
    );
    m_buttonMenu->addChildAtPosition(cancelBtn, Anchor::Bottom, ccp(-52, 28));

    auto uninstallSpr = ButtonSprite::create("Uninstall", "goldFont.fnt", "GJ_button_01.png", .8f);
    uninstallSpr->setScale(.85f);
    auto uninstallBtn = CCMenuItemSpriteExtra::create(
        uninstallSpr, this, menu_selector(ConfirmUninstallPopup::onUninstall)
    );
    m_buttonMenu->addChildAtPosition(uninstallBtn, Anchor::Bottom, ccp(42, 28));

    return true;
}

void ConfirmUninstallPopup::onUninstall(CCObject*) {
    auto res = m_mod->uninstall(m_deleteDataToggle->isToggled());

    if (res) {
        FLAlertLayer::create(
            "Mod Uninstalled",
            m_mod->getName() + " has been uninstalled!",
            "OK"
        )->show();
    }
    else {
        FLAlertLayer::create(
            "Error Uninstalling",
            "Error uninstalling " + m_mod->getName() + ": " + res.unwrapErr(),
            "OK"
        )->show();
    }

    UpdateModListStateEvent(UpdateModState(m_mod->getID())).post();

    this->onClose(nullptr);
}

ConfirmUninstallPopup* ConfirmUninstallPopup::create(Mod* mod) {
    auto ret = new ConfirmUninstallPopup();
    if (ret->initAnchored(300, 150, mod, "square01_001.png", { 0, 0, 94, 94 })) {
        ret->autorelease();
        return ret;
    }
    delete ret;
    return nullptr;
}
