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
    static void onModify(const auto& self) {
        if (!Loader::get()->isForwardCompatMode())
            return;
        log::warn("save moved in forward compat");
        for (const auto& [_, hook] : self.m_hooks) {
            hook->setAutoEnable(false);
        }
    }

    void trySaveGame(bool p0) {
        saveModData();
        return AppDelegate::trySaveGame(p0);
    }
};

struct FallbackSaveLoader : Modify<FallbackSaveLoader, CCApplication> {
    static void onModify(const auto& self) {
        if (Loader::get()->isForwardCompatMode())
            return;
        for (const auto& [_, hook] : self.m_hooks) {
            hook->setAutoEnable(false);
        }
    }

    void gameDidSave() {
        saveModData();
        return CCApplication::gameDidSave();
    }
};
