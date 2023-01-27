#include "loader/LoaderImpl.hpp"

#include <Geode/loader/IPC.hpp>
#include <Geode/loader/Loader.hpp>
#include <Geode/loader/Log.hpp>
#include <Geode/loader/Mod.hpp>
#include <Geode/loader/Setting.hpp>
#include <Geode/loader/SettingEvent.hpp>
#include <Geode/loader/ModJsonTest.hpp>
#include <Geode/utils/JsonValidation.hpp>
#include <array>

USE_GEODE_NAMESPACE();

int geodeEntry(void* platformData);
// platform-specific entry points

#if defined(GEODE_IS_IOS) || defined(GEODE_IS_MACOS)
    #include <dlfcn.h>
    #include <mach-o/dyld.h>
    #include <unistd.h>

std::length_error::~length_error() _NOEXCEPT {} // do not ask...

// camila has an old ass macos and this function turned
// from dynamic to static thats why she needs to define it
// this is what old versions does to a silly girl

void dynamicEntry() {
    auto dylib = dlopen("GeodeBootstrapper.dylib", RTLD_NOLOAD);
    dlclose(dylib);

    std::array<char, PATH_MAX> gddir;

    uint32_t out = PATH_MAX;
    _NSGetExecutablePath(gddir.data(), &out);

    ghc::filesystem::path gdpath = gddir.data();
    ghc::filesystem::current_path(gdpath.parent_path().parent_path());

    auto workingDir = gdpath.parent_path().parent_path();
    auto libDir = workingDir / "Frameworks";
    auto updatesDir = workingDir / "geode" / "update";

    auto error = std::error_code();

    if (ghc::filesystem::exists(updatesDir / "GeodeBootstrapper.dylib", error) && !error) {
        ghc::filesystem::rename(
            updatesDir / "GeodeBootstrapper.dylib", libDir / "GeodeBootstrapper.dylib", error
        );
        if (error) return;
    }

    geodeEntry(nullptr);
}

extern "C" __attribute__((visibility("default"))) void dynamicTrigger() {
    std::thread(&dynamicEntry).detach();
}

// remove when we can figure out how to not remove it
auto dynamicTriggerRef = &dynamicTrigger;

#elif defined(GEODE_IS_WINDOWS)
    #include <Windows.h>

DWORD WINAPI loadThread(void* arg) {
    bool canMoveBootstrapper = true;
    if (auto mod = GetModuleHandleA("GeodeBootstrapper.dll")) {
        if (WaitForSingleObject(mod, 1000) != WAIT_OBJECT_0) {
            canMoveBootstrapper = false;
        }
    }

    if (canMoveBootstrapper) {
        auto workingDir = ghc::filesystem::current_path();
        auto updatesDir = workingDir / "geode" / "update";

        auto error = std::error_code();

        if (ghc::filesystem::exists(updatesDir / "GeodeBootstrapper.dll", error) && !error) {
            ghc::filesystem::rename(
                updatesDir / "GeodeBootstrapper.dll", workingDir / "GeodeBootstrapper.dll", error
            );
            if (error) return error.value();
        }
    }

    return geodeEntry(arg);
}

BOOL WINAPI DllMain(HINSTANCE lib, DWORD reason, LPVOID) {
    switch (reason) {
        case DLL_PROCESS_ATTACH:
            // Prevents threads from notifying this DLL on creation or destruction.
            // Kind of redundant for a game that isn't multi-threaded but will provide
            // some slight optimizations if a mod frequently creates and deletes threads.
            DisableThreadLibraryCalls(lib);

            // loading thread
            HANDLE _ = CreateThread(0, 0, loadThread, lib, 0, nullptr);
            if (_) CloseHandle(_);

            break;
    }
    return TRUE;
}
#endif
$execute {
    listenForSettingChanges("show-platform-console", +[](bool value) {
        if (value) {
            Loader::get()->openPlatformConsole();
        }
        else {
            Loader::get()->closePlatformConsole();
        }
    });
    
    listenForIPC("ipc-test", [](IPCEvent* event) -> json::Value {
        return "Hello from Geode!";
    });

    listenForIPC("loader-info", [](IPCEvent* event) -> json::Value {
        return Loader::get()->getModImpl()->getModInfo();
    });

    listenForIPC("list-mods", [](IPCEvent* event) -> json::Value {
        std::vector<json::Value> res;

        auto args = *event->messageData;
        JsonChecker checker(args);
        auto root = checker.root("").obj();

        auto includeRunTimeInfo = root.has("include-runtime-info").template get<bool>();
        auto dontIncludeLoader = root.has("dont-include-loader").template get<bool>();

        if (!dontIncludeLoader) {
            res.push_back(
                includeRunTimeInfo ? Loader::get()->getModImpl()->getRuntimeInfo() :
                                    Loader::get()->getModImpl()->getModInfo().toJSON()
            );
        }

        for (auto& mod : Loader::get()->getAllMods()) {
            res.push_back(includeRunTimeInfo ? mod->getRuntimeInfo() : mod->getModInfo().toJSON());
        }

        return res;
    });
}

int geodeEntry(void* platformData) {

    // set up internal mod, settings and data
    auto internalSetupRes = LoaderImpl::get()->setupInternalMod();
    if (!internalSetupRes) {
        LoaderImpl::get()->platformMessageBox(
            "Unable to Load Geode!",
            "There was an unknown fatal error setting up "
            "the internal mod and Geode can not be loaded." + internalSetupRes.unwrapErr()
        );
        LoaderImpl::get()->reset();
        return 1;
    }

    // set up loader, load mods, etc.
    auto setupRes = LoaderImpl::get()->setup();
    if (!setupRes) {
        LoaderImpl::get()->platformMessageBox(
            "Unable to Load Geode!",
            "There was an unknown fatal error setting up "
            "the loader and Geode can not be loaded. " 
            "(" + setupRes.unwrapErr() + ")"
        );
        LoaderImpl::get()->reset();
        return 1;
    }

    log::debug("Set up loader");

    // open console
    if (Mod::get()->getSettingValue<bool>("show-platform-console")) {
        Loader::get()->openPlatformConsole();
    }

    // download and install new loader update in the background
    if (Mod::get()->getSettingValue<bool>("auto-check-updates")) {
        LoaderImpl::get()->checkForLoaderUpdates();
    }

    log::debug("Entry done.");

    return 0;
}
