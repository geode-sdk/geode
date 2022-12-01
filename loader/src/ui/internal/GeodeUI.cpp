
#include <Geode/loader/Index.hpp>
#include "info/ModInfoLayer.hpp"
#include "list/ModListLayer.hpp"
#include "settings/ModSettingsPopup.hpp"

#include <Geode/ui/GeodeUI.hpp>

void geode::openModsList() {
    ModListLayer::scene();
}

void geode::openInfoPopup(Mod* mod) {
    ModInfoLayer::create(mod, nullptr)->show();
}

void geode::openIndexPopup(Mod* mod) {
    if (Index::get()->isKnownItem(mod->getID())) {
        ModInfoLayer::create(
            new ModObject(Index::get()->getKnownItem(mod->getID()))
        )->show();
    }
}

void geode::openSettingsPopup(Mod* mod) {
    if (mod->hasSettings()) {
        ModSettingsPopup::create(mod)->show();
    }
}
