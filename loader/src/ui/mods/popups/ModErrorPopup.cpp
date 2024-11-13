#include "ModErrorPopup.hpp"
#include <Geode/ui/Layout.hpp>
#include "ui/mods/GeodeStyle.hpp"
#include "ui/mods/list/ModProblemList.hpp"

#include <Geode/cocos/platform/CCPlatformMacros.h>
#include <Geode/cocos/cocoa/CCGeometry.h>
#include <Geode/cocos/cocoa/CCArray.h>
#include <Geode/loader/Loader.hpp>
#include <fmt/core.h>

bool ModErrorPopup::setup(Mod* mod) {
    m_mod = mod;
    this->setTitle(fmt::format("Errors for {}", mod->getName()));

    this->createList();

    return true;
}

void ModErrorPopup::createList() {
    CCSize contentSize = m_mainLayer->getContentSize();
    CCArray* elements = CCArray::create();
    std::vector<LoadProblem> problems = m_mod->getProblems();
    m_list = ModProblemList::create(m_mod, {400.f, 215.f});
    m_mainLayer->addChildAtPosition(
        m_list,
        Anchor::Center,
        { 0.0f, -10.0f }
    );
}

ModErrorPopup* ModErrorPopup::create(Mod *mod) {
    ModErrorPopup* ret = new ModErrorPopup();
    if (ret->init(440.f, 280.f, mod, GeodePopupStyle::Default)) {
        ret->autorelease();
        return ret;
    }

    delete ret;
    return nullptr;
}