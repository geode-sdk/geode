#include <ghc/filesystem.hpp>
#include <mach-o/dyld.h>
#include <unistd.h>
#include <dlfcn.h>
#include <array>
#include <iostream>
#include <CoreFoundation/CoreFoundation.h>

void displayError(std::string alertMessage) {
    CFStringRef cfTitle = CFStringCreateWithCString(NULL, "Geode Bootstrapper", kCFStringEncodingUTF8);
    CFStringRef cfMessage = CFStringCreateWithCString(NULL, alertMessage.c_str(), kCFStringEncodingUTF8);

    CFUserNotificationDisplayNotice(0, kCFUserNotificationCautionAlertLevel, NULL, NULL, NULL, cfTitle, cfMessage, NULL);
}

void loadGeode() {
    auto dylib = dlopen("Geode.dylib", RTLD_NOW);
    if (!dylib)  {
        displayError(std::string("Couldn't load Geode: ") + dlerror());
        return;
    }
    auto trigger = dlsym(dylib, "dynamicTrigger");
    if (!trigger)  {
        displayError(std::string("Couldn't start Geode: ") + dlerror());
        return;
    }
    reinterpret_cast<void(*)()>(trigger)();
    return;
}

__attribute__((constructor)) void _entry() {
    std::array<char, PATH_MAX> gddir;

    uint32_t out = PATH_MAX;
    _NSGetExecutablePath(gddir.data(), &out);

    ghc::filesystem::path gdpath = gddir.data();
    auto workingDir = gdpath.parent_path().parent_path();

    auto updatesDir = workingDir / "geode" / "update";
    auto libDir = workingDir / "Frameworks";
	auto resourcesDir = workingDir / "geode" / "resources";

	auto error = std::error_code();

	if (ghc::filesystem::exists(updatesDir / "Geode.dylib", error) && !error) {
        ghc::filesystem::rename(
            updatesDir / "Geode.dylib", 
            libDir / "Geode.dylib", error
        );
        if (error) {
            displayError(std::string("Couldn't update Geode: ") + error.message());
            return loadGeode();
        }
    }

    if (ghc::filesystem::exists(updatesDir / "resources", error) && !error) {
        ghc::filesystem::remove_all(resourcesDir / "geode.loader", error);

        if (error) {
            displayError(std::string("Couldn't update Geode resources: ") + error.message());
            return loadGeode();
        }

        ghc::filesystem::rename(
            updatesDir / "resources", 
            resourcesDir / "geode.loader", error
        );
        
        if (error) {
            displayError(std::string("Couldn't update Geode resources: ") + error.message());
            return loadGeode();
        }
    }    

    return loadGeode();
}