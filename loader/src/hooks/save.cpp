#include <Geode/loader/Loader.hpp>

using namespace geode::prelude;

#include <Geode/modify/AppDelegate.hpp>
#include <Geode/modify/CCApplication.hpp>

namespace {
    void saveModData() {
        log::info("Saving mod data...");
        log::pushNest();

        auto begin = std::chrono::high_resolution_clock::now();

        (void)Loader::get()->saveData();

        auto end = std::chrono::high_resolution_clock::now();
        auto time = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count();
        log::info("Took {}s", static_cast<float>(time) / 1000.f);

        log::popNest();
    }
}

struct SaveLoader : Modify<SaveLoader, AppDelegate> {
    GEODE_FORWARD_COMPAT_DISABLE_HOOKS("save moved to CCApplication::gameDidSave()")
    void trySaveGame(bool p0) {
        saveModData();
        return AppDelegate::trySaveGame(p0);
    }
};

#ifdef GEODE_IS_WINDOWS

struct FallbackSaveLoader : Modify<FallbackSaveLoader, CCApplication> {
    GEODE_FORWARD_COMPAT_ENABLE_HOOKS("")
    void gameDidSave() {
        saveModData();
        return CCApplication::gameDidSave();
    }
};

#endif