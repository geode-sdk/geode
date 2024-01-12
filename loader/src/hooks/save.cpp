#include <Geode/loader/Loader.hpp>

using namespace geode::prelude;

#include <Geode/modify/AppDelegate.hpp>

struct SaveLoader : Modify<SaveLoader, AppDelegate> {
    static void onModify(const auto& self) {
        if (!Loader::get()->isForwardCompatMode())
            return;
        log::warn("save disabled in forward compat");
        for (const auto& [_, hook] : self.m_hooks) {
            hook->setAutoEnable(false);
        }
    }

    void trySaveGame(bool p0) {
        log::info("Saving mod data...");
        log::pushNest();

        auto begin = std::chrono::high_resolution_clock::now();

        (void)Loader::get()->saveData();

        auto end = std::chrono::high_resolution_clock::now();
        auto time = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count();
        log::info("Took {}s", static_cast<float>(time) / 1000.f);

        log::popNest();

        return AppDelegate::trySaveGame(p0);
    }
};
