#include <Geode/DefaultInclude.hpp>

#import <Cocoa/Cocoa.h>
#include <AppKit/AppKit.h>
#include <objc/runtime.h>
#include "../load.hpp"
#include <dlfcn.h>
#include <mach-o/dyld.h>
#include <unistd.h>
#include <tulip/TulipHook.hpp>
#include <Geode/utils/ObjcHook.hpp>
#include <array>
#include <asp/fs/fs.hpp>
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

    if (frameworkDir.exists() && updatesDir.exists()) {
        auto overwrite = [&](std::string_view name) {
            auto src = updatesDir / name;
            auto dst = frameworkDir / name;

            if (!src.exists()) return;

            if (auto e = asp::fs::remove(dst).err()) {
                log::warn("Couldn't remove old {}: {}", name, *e);
                return;
            }

            if (auto e = asp::fs::rename(src, dst).err()) {
                log::warn("Couldn't move new {}: {}", name, *e);
                return;
            }
            log::info("Updated {}", name);
        };

        overwrite("GeodeBootstrapper.dylib");
        overwrite("Geode.dylib");

        auto srcResources = updatesDir / "resources";
        auto dstResources = resourcesDir / "geode.loader";

        if (asp::fs::exists(srcResources)) {
            if (auto e = asp::fs::remove(dstResources).err()) {
                log::warn("Couldn't remove old resources: {}", *e);
            }
            else {
                if (auto e = asp::fs::move(srcResources, dstResources).err()) {
                    log::warn("Couldn't move new resources: {}", *e);
                }
                else {
                    log::info("Updated resources");
                }
            }
        }

        if (auto e = asp::fs::removeDirAll(updatesDir).err()) {
            log::warn("Couldn't remove old update directory: {}", *e);
        }
    }
}

$execute {
    using namespace geode::updater;
    LoaderUpdateEvent().listen([](UpdateStatus const& status) {
        if (std::holds_alternative<UpdateFinished>(status)) {
            updateFiles();
        }
        return ListenerResult::Propagate;
    }).leak();
};

void updateGeode() {
    asp::fs::Path oldSavePath = "/Users/Shared/Geode/geode";
    auto newSavePath = dirs::getSaveDir() / "geode";
    if (oldSavePath.exists()) {
        if (auto e = asp::fs::move(oldSavePath, newSavePath).err()) {
            log::warn("Couldn't migrate old save files from {} to {}: {}", oldSavePath, newSavePath, *e);
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

// I like to call this the "Super Mega Ultra Safe Mode™"
// However for the sake of not getting side-eyed it's getting this boring name
bool cleanModeCheck() {
    if (
        CGEventSourceKeyState(kCGEventSourceStateHIDSystemState, (CGKeyCode)58) && // 58 = Option
        CGEventSourceKeyState(kCGEventSourceStateHIDSystemState, (CGKeyCode)56) // 56 = LShift
    ) {
        NSAlert* alert = [NSAlert new];
        alert.messageText = @"The Shift and Option keys were held down, do you want to open Geometry Dash without Geode?";
        [alert addButtonWithTitle:@"Yes"];
        NSButton *cancelButton = [alert addButtonWithTitle:@"No"];
        alert.window.defaultButtonCell = cancelButton.cell;
        NSModalResponse choice = [alert runModal];
        return choice == NSAlertFirstButtonReturn;
    }
    return false;
}

bool loadGeode() {
    if (cleanModeCheck()) return false;

    if (GEODE_STR(GEODE_GD_VERSION) != LoaderImpl::get()->getGameVersion()) {
        console::messageBox(
            "Unable to Load Geode!",
            fmt::format(
                "Geometry Dash is outdated!\n"
                "Geode requires GD {} but you have {}.\n"
                "Please, update Geometry Dash to {}.",
                GEODE_STR(GEODE_GD_VERSION),
                LoaderImpl::get()->getGameVersion(),
                GEODE_STR(GEODE_GD_VERSION)
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
