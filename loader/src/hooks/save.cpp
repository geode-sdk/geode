#include <chrono>
#include <Geode/loader/Loader.hpp>
#include <Geode/modify/AppDelegate.hpp>
#include <Geode/modify/CCApplication.hpp>

using namespace geode::prelude;

namespace {
    void saveModData() {
        log::info("Saving mod data...");
        log::pushNest();

        auto startTime = std::chrono::high_resolution_clock::now();

        Loader::get()->saveData();

        auto endTime = std::chrono::high_resolution_clock::now();
        auto elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count();
        log::info("Took {}s", static_cast<float>(elapsedTime) / 1000.f);

        log::popNest();
    }
}

struct SaveLoader : Modify<SaveLoader, AppDelegate> {
    GEODE_FORWARD_COMPAT_DISABLE_HOOKS("save moved to CCApplication::gameDidSave()")
    void trySaveGame(bool p0) {
        saveModData();
        AppDelegate::trySaveGame(p0);
    }
};

#ifdef GEODE_IS_WINDOWS

struct FallbackSaveLoader : Modify<FallbackSaveLoader, CCApplication> {
    GEODE_FORWARD_COMPAT_ENABLE_HOOKS("")
    void gameDidSave() {
        saveModData();
        CCApplication::gameDidSave();
    }
};

#endif

#ifdef GEODE_IS_ANDROID

#include <Geode/modify/FileOperation.hpp>
#include <Geode/loader/Dirs.hpp>

struct FileOperationOverride : Modify<FileOperationOverride, FileOperation> {
    gd::string getFilePath() {
        return dirs::getSaveDir().string() + "/";
    }
};

#endif
