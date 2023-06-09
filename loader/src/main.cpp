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

using namespace geode::prelude;

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

    auto workingDir = dirs::getGameDir();
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

#elif defined(GEODE_IS_ANDROID)

#include <jni.h>

extern "C" [[gnu::visibility("default")]] jint JNI_OnLoad(JavaVM* vm, void* reserved) {
    geodeEntry(nullptr);
    return JNI_VERSION_1_1;
}

#elif defined(GEODE_IS_WINDOWS)
    #include <Windows.h>

int updateGeode() {
    auto workingDir = dirs::getGameDir();
    auto updatesDir = workingDir / "geode" / "update";

    auto error = std::error_code();

    if (!ghc::filesystem::is_empty(updatesDir, error) && !error) {
        if (ghc::filesystem::exists(updatesDir / "GeodeUpdater.exe", error) && !error)
            ghc::filesystem::rename(updatesDir / "GeodeUpdater.exe", workingDir / "GeodeUpdater.exe", error);
        if (error)
            return error.value();

        char gdPath[MAX_PATH];
        GetModuleFileName(nullptr, gdPath, MAX_PATH);
        auto gdName = ghc::filesystem::path(gdPath).filename().string();

        // launch updater
        auto updaterPath = (workingDir / "GeodeUpdater.exe").string();
        ShellExecute(NULL, "open", updaterPath.c_str(), gdName.c_str(), workingDir.string().c_str(), FALSE);

        // quit gd before it can even start
        exit(0);
        return 0;
    }
    if (error)
        return error.value();

    return 0;
}

int WINAPI gdMainHook(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow) {
    int exitCode = updateGeode();
    if (exitCode != 0)
        return exitCode;

    exitCode = geodeEntry(hInstance);
    if (exitCode != 0)
        return exitCode;

    return reinterpret_cast<decltype(&wWinMain)>(geode::base::get() + 0x260ff8)(hInstance, hPrevInstance, lpCmdLine, nCmdShow);
}

extern "C" __declspec(dllexport) DWORD WINAPI loadGeode(void* arg) {
    auto process = GetCurrentProcess();

    auto patchAddr = reinterpret_cast<void*>(geode::base::get() + 0x260ff8);
    constexpr size_t patchLength = 13;
    auto detourAddr = reinterpret_cast<uintptr_t>(&gdMainHook) - geode::base::get() - 0x261005;
    auto detourAddrPtr = reinterpret_cast<uint8_t*>(&detourAddr);

    uint8_t patchBytes[patchLength] = {
        0x55,
        0x8b, 0xec,
        0x83, 0xe4, 0xf8,
        0xeb, 0x06,
        0xe9, detourAddrPtr[0], detourAddrPtr[1], detourAddrPtr[2], detourAddrPtr[3]
    };

    DWORD oldProtect;
    BOOL res = TRUE;
    res = res && VirtualProtectEx(process, patchAddr, patchLength, PAGE_EXECUTE_READWRITE, &oldProtect);
    res = res && WriteProcessMemory(process, patchAddr, patchBytes, patchLength, nullptr);
    res = res && VirtualProtectEx(process, patchAddr, patchLength, oldProtect, &oldProtect);

    if (!res) {
        LoaderImpl::get()->platformMessageBox(
            "Unable to Load Geode!",
            "There was an unknown fatal error hooking "
            "the GD main function and Geode can not be loaded."
        );
        return 1;
    }

    return 0;
}

DWORD WINAPI upgradeThread(void*) {
    return updateGeode() == 0;
}
BOOL WINAPI DllMain(HINSTANCE module, DWORD reason, LPVOID) {
    if (reason == DLL_PROCESS_ATTACH) {
        DisableThreadLibraryCalls(module);

        // if we find the old bootstrapper dll, don't load geode, copy new updater and let it do the rest
        auto workingDir = dirs::getGameDir();
        auto error = std::error_code();
        if (ghc::filesystem::exists(workingDir / "GeodeBootstrapper.dll", error) && !error)
            CreateThread(nullptr, 0, upgradeThread, nullptr, 0, nullptr);
        if (error)
            return FALSE;
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

    log::info("Set up loader");

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
