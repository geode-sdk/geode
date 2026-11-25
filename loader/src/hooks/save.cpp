#include <Geode/loader/Loader.hpp>

using namespace geode::prelude;

#include <Geode/modify/AppDelegate.hpp>
#include <Geode/modify/CCApplication.hpp>

namespace {
    void saveModData() {
        log::info("Saving mod data...");
        log::NestScope nest;

        auto begin = asp::Instant::now();

        Loader::get()->saveData();

        log::debug("Took {}", begin.elapsed());
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

#ifdef GEODE_IS_ANDROID

#include <Geode/modify/FileOperation.hpp>
#include <Geode/loader/Dirs.hpp>

// redirects the save path to what geode knows, in case launcher's fopen hook fails
struct FileOperationOverride : Modify<FileOperationOverride, FileOperation> {
    static std::string getFilePath() {
        return dirs::getSaveDir().string() + "/";
    }
};

#endif