#include "ConfirmUninstallPopup.hpp"
#include "../UpdateModListState.hpp"
#include <Geode/binding/ButtonSprite.hpp>
#include <Geode/binding/TextArea.hpp>
#include <Geode/ui/Label.hpp>

bool ConfirmUninstallPopup::init(Mod* mod) {
    if (!Popup::init(300.0f, 150.0f, "square01_001.png")) return false;

    m_mod = mod;

    this->setTitle(fmt::format("Uninstall {}", mod->getName()));

    auto text = Label::createRich(fmt::format("Are you sure you want to \n<cr>uninstall</c> <cy>{}</c>?", mod->getName()), "chatFont.fnt");
    text->setMaxWidth(m_mainLayer->getContentWidth() - 35.0f);
    text->setAlignment(Label::Alignment::Center);
    m_mainLayer->addChildAtPosition(text, Anchor::Center, {0.0f, 20.0f});

    auto deleteDataLabel = Label::create("Delete the Mod's save data", "bigFont.fnt");
    deleteDataLabel->setScale(0.35f);
    m_mainLayer->addChildAtPosition(deleteDataLabel, Anchor::Center, {-70.0f, -15.0f}, {0.0f, 0.5f});

    if (mod->isInternal()) {
        deleteDataLabel->setText("Delete ALL mods and their save data");
        deleteDataLabel->setScale(0.275f);
    }

    m_deleteDataToggle = Toggler::createWithStandardSprites(nullptr);
    m_deleteDataToggle->setScale(0.6f);
    m_mainLayer->addChildAtPosition(m_deleteDataToggle, Anchor::Center, {-88.0f, -15.0f});

    auto cancelBtn = Button::createWithNode(
        ButtonSprite::create("Cancel", "goldFont.fnt", "GJ_button_01.png", 0.8f),
        [this](Button* sender) {
            this->onClose(sender);
        }
    );
    cancelBtn->setScale(0.85f);
    cancelBtn->setScaleMultiplier(1.125f);
    m_mainLayer->addChildAtPosition(cancelBtn, Anchor::Bottom, {-52.0f, 28.0f});

    auto uninstallBtn = Button::createWithNode(
        ButtonSprite::create("Uninstall", "goldFont.fnt", "GJ_button_01.png", 0.8f),
        [this](auto) {
            auto res = m_mod->uninstall(m_deleteDataToggle->isToggled());

            if (res) {
                createQuickPopup(
                    "Mod Uninstalled",
                    fmt::format("{} has been uninstalled!", m_mod->getName()),
                    "OK", nullptr, nullptr
                );
            }
            else {
                createQuickPopup(
                    "Error Uninstalling",
                    fmt::format("Error uninstalling {}: {}", m_mod->getName(), res.unwrapErr()),
                    "OK", nullptr, nullptr
                );
            }

            UpdateModListStateEvent().send(UpdateModState(m_mod->getID()));

            this->onClose(nullptr);
        }
    );
    uninstallBtn->setScale(0.85f);
    uninstallBtn->setScaleMultiplier(1.125f);
    m_mainLayer->addChildAtPosition(uninstallBtn, Anchor::Bottom, {42.0f, 28.0f});

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