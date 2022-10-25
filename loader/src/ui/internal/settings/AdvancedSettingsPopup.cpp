#include "AdvancedSettingsPopup.hpp"
#include <Geode/loader/Loader.hpp>
#include <Geode/binding/CCMenuItemToggler.hpp>
#include <Geode/binding/GameToolbox.hpp>

bool AdvancedSettingsPopup::setup(Mod* mod) {
    m_mod = mod;

    this->setTitle("Advanced Settings for " + mod->getName());

    auto enableBtn = CCMenuItemToggler::createWithStandardSprites(
        this, menu_selector(AdvancedSettingsPopup::onEnableEarlyLoad), .75f
    );
    enableBtn->setPosition(-100.f, 0.f);
    m_buttonMenu->addChild(enableBtn);

    auto enableText = CCLabelBMFont::create("Enable Early Load", "bigFont.fnt");
    enableText->setPosition(-70.f, 0.f);
    enableText->setAnchorPoint({ .0f, .5f });
    enableText->setScale(.65f);
    m_buttonMenu->addChild(enableText);

    return true;
}

void AdvancedSettingsPopup::onEnableEarlyLoad(CCObject* sender) {
    Loader::get()->setEarlyLoadMod(
        m_mod, !static_cast<CCMenuItemToggler*>(sender)->isToggled()
    );
}

AdvancedSettingsPopup* AdvancedSettingsPopup::create(Mod* mod) {
    auto ret = new AdvancedSettingsPopup;
    if (ret && ret->init(356.f, 220.f, mod)) {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}
