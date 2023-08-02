#include <Geode/DefaultInclude.hpp>

#if defined(GEODE_IS_MACOS)

#import <Cocoa/Cocoa.h>
#include "../load.hpp"
#include <dlfcn.h>
#include <mach-o/dyld.h>
#include <unistd.h>
#include <tulip/TulipHook.hpp>
#include <array>
#include <ghc/filesystem.hpp>
#include <Geode/Loader.hpp>
#include "../../loader/LoaderImpl.hpp"
#include <thread>
#include <variant>

using namespace geode::prelude;

std::length_error::~length_error() _NOEXCEPT {} // do not ask...

// camila has an old ass macos and this function turned
// from dynamic to static thats why she needs to define it
// this is what old versions does to a silly girl

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

void applicationDidFinishLaunchingHook(void* self, SEL sel, NSNotification* notification) {
    updateGeode();

    std::array<uint8_t, 6> patchBytes = {
        0x55,
        0x48, 0x89, 0xe5,
        0x41, 0x57
    };

    auto res = tulip::hook::writeMemory((void*)(base::get() + 0x69a0), patchBytes.data(), 6);
    if (!res)
        return;
    
    int exitCode = geodeEntry(nullptr);
    if (exitCode != 0)
        return;

    return reinterpret_cast<void(*)(void*, SEL, NSNotification*)>(geode::base::get() + 0x69a0)(self, sel, notification);
}


bool loadGeode() {
    auto detourAddr = reinterpret_cast<uintptr_t>(&applicationDidFinishLaunchingHook) - geode::base::get() - 0x69a5;
    auto detourAddrPtr = reinterpret_cast<uint8_t*>(&detourAddr);

    std::array<uint8_t, 5> patchBytes = {
        0xe9, detourAddrPtr[0], detourAddrPtr[1], detourAddrPtr[2], detourAddrPtr[3]
    };

    auto res = tulip::hook::writeMemory((void*)(base::get() + 0x69a0), patchBytes.data(), 5);
    if (!res)
        return false;
    
    return true;
}

__attribute__((constructor)) void _entry() {
    if (!loadGeode())
        return;
}

#endif