#include <Geode/DefaultInclude.hpp>

#import <Cocoa/Cocoa.h>
#include <objc/runtime.h>
#include "../load.hpp"
#include <dlfcn.h>
#include <mach-o/dyld.h>
#include <unistd.h>
#include <tulip/TulipHook.hpp>
#include <Geode/utils/ObjcHook.hpp>
#include <array>
#include <filesystem>
#include <Geode/Loader.hpp>
#include "../../loader/console.hpp"
#include "../../loader/LoaderImpl.hpp"
#include <thread>
#include <variant>
#include <loader/updater.hpp>

using namespace geode::prelude;

std::length_error::~length_error() _NOEXCEPT {} // do not ask...

// camila has an old ass macos and this function turned
// from dynamic to static thats why she needs to define it
// this is what old versions does to a silly girl

void updateFiles() {
    auto frameworkDir = dirs::getGameDir() / "Frameworks";
    auto updatesDir = dirs::getGeodeDir() / "update";
    auto resourcesDir = dirs::getGeodeResourcesDir();

    if (std::filesystem::exists(frameworkDir) && std::filesystem::exists(updatesDir)) {
        std::error_code error;
        auto bootFile = "GeodeBootstrapper.dylib";
        auto geodeFile = "Geode.dylib";

        if (std::filesystem::exists(updatesDir / bootFile)) {
            std::filesystem::remove(frameworkDir / bootFile, error);
            if (error) {
                log::warn("Couldn't remove old GeodeBootstrapper.dylib: {}", error.message());
            }
            else {
                std::filesystem::rename(updatesDir / bootFile, frameworkDir / bootFile, error);
                if (error) {
                    log::warn("Couldn't move new GeodeBootstrapper.dylib: {}", error.message());
                }
                else {
                    log::info("Updated GeodeBootstrapper.dylib");
                }
            }
        }
        if (std::filesystem::exists(updatesDir / geodeFile)) {
            std::filesystem::remove(frameworkDir / geodeFile, error);
            if (error) {
                log::warn("Couldn't remove old Geode.dylib: {}", error.message());
            }
            else {
                std::filesystem::rename(updatesDir / geodeFile, frameworkDir / geodeFile, error);
                if (error) {
                    log::warn("Couldn't move new Geode.dylib: {}", error.message());
                }
                else {
                    log::info("Updated Geode.dylib");
                }
            }
        }
        if (std::filesystem::exists(updatesDir / "resources")) {
            std::filesystem::remove_all(resourcesDir / "geode.loader", error);
            if (error) {
                log::warn("Couldn't remove old resources: {}", error.message());
            }
            else {
                std::filesystem::rename(updatesDir / "resources", resourcesDir / "geode.loader", error);
                if (error) {
                    log::warn("Couldn't move new resources: {}", error.message());
                }
                else {
                    log::info("Updated resources");
                }
            }
        }
        std::filesystem::remove_all(updatesDir, error);
        if (error) {
            log::warn("Couldn't remove old update directory: {}", error.message());
        }
    }
}

$execute {
    using namespace geode::updater;
    globalListen([](LoaderUpdateEvent* event) {
        if (std::holds_alternative<UpdateFinished>(event->status)) {
            updateFiles();
        }
        return ListenerResult::Propagate;
    }, LoaderUpdateFilter());
};

void updateGeode() {
    std::filesystem::path oldSavePath = "/Users/Shared/Geode/geode";
    auto newSavePath = dirs::getSaveDir() / "geode";
    if (std::filesystem::exists(oldSavePath)) {
        std::error_code error;

        std::filesystem::rename(oldSavePath, newSavePath, error);
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

// For some reason, keyUp isn't called if command is pressed, despite keyDown being called in the same situation.
static void(*s_sendEventOrig)(NSApplication* self, SEL sel, NSEvent* event);

void sendEventHook(NSApplication* self, SEL sel, NSEvent* event) {
    if ([event type] == NSEventTypeKeyUp) {
        [[[self mainWindow] firstResponder] tryToPerform:@selector(keyUp:) with:event];
        return;
    }

    s_sendEventOrig(self, sel, event);
}


bool loadGeode() {
    if (GEODE_STR(GEODE_GD_VERSION) != LoaderImpl::get()->getGameVersion()) {
        console::messageBox(
            "Unable to Load Geode!",
            fmt::format(
                "This version of Geode is made for Geometry Dash {} "
                "but you're trying to play with GD {}.\n"
                "Please, update your game.",
                GEODE_STR(GEODE_GD_VERSION),
                LoaderImpl::get()->getGameVersion()
            )
        );
        return false;
    }

    // this uses the internal hooking system because it needs to be fast
    if (auto imp = hook::replaceObjcMethod("AppController", "applicationDidFinishLaunching:", (void*)applicationDidFinishLaunchingHook)) {
        s_applicationDidFinishLaunchingOrig = reinterpret_cast<decltype(s_applicationDidFinishLaunchingOrig)>(imp.unwrap());
    }
    else {
        return false;
    }
    if (auto imp = hook::replaceObjcMethod("NSApplication", "sendEvent:", (void*)sendEventHook)) {
        s_sendEventOrig = reinterpret_cast<decltype(s_sendEventOrig)>(imp.unwrap());
    }
    else {
        return false;
    }

    return true;
}

__attribute__((constructor)) void _entry() {
    if (!loadGeode())
        return;
}
