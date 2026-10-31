#include "ConfirmUninstallPopup.hpp"
#include <Geode/binding/ButtonSprite.hpp>
#include <Geode/binding/TextArea.hpp>
#include <Geode/ui/Label.hpp>

using namespace geode::prelude;

bool ConfirmUninstallPopup::init(Mod* mod) {
    if (!Popup::init(300.f, 150.f, "square01_001.png")) return false;

    m_mod = mod;

    this->setTitle(fmt::format("Uninstall {}", mod->getName()));

    auto text = Label::createRich(fmt::format("Are you sure you want to \n<cr>uninstall</c> <cy>{}</c>?", mod->getName()), "chatFont.fnt");
    text->setMaxWidth(m_size.width - 35.f);
    text->setAlignment(Label::Alignment::Center);
    m_mainLayer->addChildAtPosition(text, Anchor::Center, {.0f, 20.f});

    auto deleteDataLabel = Label::create("Delete the Mod's save data", "bigFont.fnt");
    deleteDataLabel->setScale(.35f);
    m_buttonMenu->addChildAtPosition(deleteDataLabel, Anchor::Center, {-70.f, -15.f}, {.0f, .5f});

    if (mod->isInternal()) {
        deleteDataLabel->setText("Delete ALL mods and their save data");
        deleteDataLabel->setScale(0.275f);
    }

    m_deleteDataToggle = CCMenuItemToggler::createWithStandardSprites(this, nullptr, .6f);
    m_buttonMenu->addChildAtPosition(m_deleteDataToggle, Anchor::Center, {-88.f, -15.f});

    auto cancelBtn = Button::createWithNode(
        ButtonSprite::create("Cancel", "goldFont.fnt", "GJ_button_01.png", .8f), [this](auto) {
            this->onClose(nullptr);
        }
    );
    cancelBtn->setScale(.85f);
    cancelBtn->setScaleMultiplier(1.125f);
    m_buttonMenu->addChildAtPosition(cancelBtn, Anchor::Bottom, {-52.f, 28.f});

    auto uninstallBtn = Button::createWithNode(
        ButtonSprite::create("Uninstall", "goldFont.fnt", "GJ_button_01.png", .8f),
        [this](auto) {
            auto res = m_mod->uninstall(m_deleteDataToggle->isToggled());

            if (res) {
                createQuickPopup(
                    "Mod Uninstalled",
                    fmt::format("{:.400} has been uninstalled!", m_mod->getName()),
                    "OK", nullptr, nullptr
                );
            }
            else {
                createQuickPopup(
                    "Error Uninstalling",
                    fmt::format("Error uninstalling {:.400}: {}", m_mod->getName(), res.unwrapErr()),
                    "OK", nullptr, nullptr
                );
            }

            UpdateModListStateEvent().send(UpdateModState(m_mod->getID()));

            this->onClose(nullptr);
        }
    );
    uninstallBtn->setScale(.85f);
    uninstallBtn->setScaleMultiplier(1.125f);
    m_buttonMenu->addChildAtPosition(uninstallBtn, Anchor::Bottom, {42.f, 28.f});

    return true;
}

ConfirmUninstallPopup* ConfirmUninstallPopup::create(Mod* mod) {
    auto ret = new ConfirmUninstallPopup();
    if (ret->init(mod)) {
        ret->autorelease();
        return ret;
    }
    delete ret;
    return nullptr;
}