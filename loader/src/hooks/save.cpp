#include <Geode/Modify.hpp>
#include <Geode/loader/Loader.hpp>

USE_GEODE_NAMESPACE();

class $modify(AppDelegate) {
    void trySaveGame() {
        log::log(Severity::Info, Loader::getInternalMod(), "Saving...");

        auto r = Loader::get()->saveSettings();
        if (!r) {
            log::log(Severity::Error, Loader::getInternalMod(), "{}", r.error());
        }

        log::log(Severity::Info, Loader::getInternalMod(), "Saved");
        
        return AppDelegate::trySaveGame();
    }
};
