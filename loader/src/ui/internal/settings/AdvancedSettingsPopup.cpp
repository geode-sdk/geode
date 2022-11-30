#include "AdvancedSettingsPopup.hpp"
#include <Geode/loader/Loader.hpp>
#include <Geode/binding/CCMenuItemToggler.hpp>
#include <Geode/binding/GameToolbox.hpp>

bool AdvancedSettingsPopup::setup(Mod* mod) {
    m_mod = mod;

    this->setTitle("Advanced Settings for " + mod->getName());

    return true;
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
