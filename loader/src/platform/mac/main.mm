#include <Geode/DefaultInclude.hpp>

#import <Cocoa/Cocoa.h>
#include "../load.hpp"
#include <dlfcn.h>
#include <mach-o/dyld.h>
#include <unistd.h>
#include <tulip/TulipHook.hpp>
#include <array>
#include <ghc/fs_fwd.hpp>
#include <Geode/Loader.hpp>
#include "../../loader/LoaderImpl.hpp"
#include <Geode/Utils.hpp>
#include <thread>
#include <variant>
#include <loader/updater.hpp>

using namespace geode::prelude;

void updateFiles() {
    auto frameworkDir = dirs::getGameDir() / "Frameworks";
    auto updatesDir = dirs::getGeodeDir() / "update";
    auto resourcesDir = dirs::getGeodeResourcesDir();

    if (ghc::filesystem::exists(frameworkDir) && ghc::filesystem::exists(updatesDir)) {
        std::error_code error;
        auto bootFile = "GeodeBootstrapper.dylib";
        auto geodeFile = "Geode.dylib";

        if (ghc::filesystem::exists(updatesDir / bootFile)) {
            ghc::filesystem::remove(frameworkDir / bootFile, error);
            if (error) {
                log::warn("Couldn't remove old GeodeBootstrapper.dylib: {}", error.message());
            }
            else {
                ghc::filesystem::rename(updatesDir / bootFile, frameworkDir / bootFile, error);
                if (error) {
                    log::warn("Couldn't move new GeodeBootstrapper.dylib: {}", error.message());
                }
                else {
                    log::info("Updated GeodeBootstrapper.dylib");
                }
            }
        }
        if (ghc::filesystem::exists(updatesDir / geodeFile)) {
            ghc::filesystem::remove(frameworkDir / geodeFile, error);
            if (error) {
                log::warn("Couldn't remove old Geode.dylib: {}", error.message());
            }
            else {
                ghc::filesystem::rename(updatesDir / geodeFile, frameworkDir / geodeFile, error);
                if (error) {
                    log::warn("Couldn't move new Geode.dylib: {}", error.message());
                }
                else {
                    log::info("Updated Geode.dylib");
                }
            }
        }
        if (ghc::filesystem::exists(updatesDir / "resources")) {
            ghc::filesystem::remove_all(resourcesDir / "geode.loader", error);
            if (error) {
                log::warn("Couldn't remove old resources: {}", error.message());
            }
            else {
                ghc::filesystem::rename(updatesDir / "resources", resourcesDir / "geode.loader", error);
                if (error) {
                    log::warn("Couldn't move new resources: {}", error.message());
                }
                else {
                    log::info("Updated resources");
                }
            }
        }
        ghc::filesystem::remove_all(updatesDir, error);
        if (error) {
            log::warn("Couldn't remove old update directory: {}", error.message());
        }
    }
}

$execute {
    using namespace geode::updater;
    new EventListener(+[](LoaderUpdateEvent* event) {
        if (std::holds_alternative<UpdateFinished>(event->status)) {
            updateFiles();
        }
        return;
    }, LoaderUpdateFilter());
};

void updateGeode() {
    ghc::filesystem::path oldSavePath = "/Users/Shared/Geode/geode";
    auto newSavePath = dirs::getSaveDir() / "geode";
    if (ghc::filesystem::exists(oldSavePath)) {
        std::error_code error;

        ghc::filesystem::rename(oldSavePath, newSavePath, error);
        if (error) {
            log::warn("Couldn't migrate old save files from {} to {}", oldSavePath.string(), newSavePath.string());
        }
    }

    updateFiles();
}

extern "C" void fake() {}

static void(*s_applicationDidFinishLaunchingOrig)(void*, SEL, NSNotification*);

void applicationDidFinishLaunchingHook(void* self, SEL sel, NSNotification* notification) {
    updateGeode();

    int exitCode = geodeEntry(nullptr);
    if (exitCode != 0)
        return;

    return s_applicationDidFinishLaunchingOrig(self, sel, notification);
}


bool loadGeode() {
    auto orig = geode::hook::replaceObjcMethod("AppController", "applicationDidFinishLaunching:", (void*)applicationDidFinishLaunchingHook);
    if (!orig)
        return false;

    s_applicationDidFinishLaunchingOrig = reinterpret_cast<void(*)(void*, SEL, NSNotification*)>(orig.unwrap());
    return true;
}

__attribute__((constructor)) void _entry() {
    if (!loadGeode())
        return;
}
