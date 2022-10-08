#include <Geode/loader/Mod.hpp>
#include <Geode/loader/Loader.hpp>
#include <Geode/loader/SettingEvent.hpp>
#include <InternalLoader.hpp>
#include <InternalMod.hpp>
#include <Geode/loader/Log.hpp>
#include <Geode/loader/Interface.hpp>
#include "../core/Core.hpp"
#include <array>

int geodeEntry(void* platformData);
// platform-specific entry points

#if defined(GEODE_IS_IOS) || defined(GEODE_IS_MACOS)
#include <mach-o/dyld.h>
#include <unistd.h>

std::length_error::~length_error() _NOEXCEPT {} // do not ask...

// camila has an old ass macos and this function turned 
// from dynamic to static thats why she needs to define it 
// this is what old versions does to a silly girl

__attribute__((constructor)) void _entry() {
    std::array<char, PATH_MAX> gddir;

    uint32_t out = PATH_MAX;
    _NSGetExecutablePath(gddir.data(), &out);

    ghc::filesystem::path gdpath = gddir.data();
    ghc::filesystem::current_path(gdpath.parent_path().parent_path());

    auto workingDir = ghc::filesystem::current_path();
    auto updatesDir = workingDir / "geode" / "update";

    auto error = std::error_code();

    if (ghc::filesystem::exists(updatesDir / "GeodeBootstrapper.dylib", error) && !error) {
        ghc::filesystem::rename(
            updatesDir / "GeodeBootstrapper.dylib", 
            workingDir / "GeodeBootstrapper.dylib", error
        );
        if (error) return;
    }

    geodeEntry(nullptr);
}

#elif defined(GEODE_IS_WINDOWS)
#include <Windows.h>

DWORD WINAPI loadThread(void* arg) {
    auto module = GetModuleHandleA("GeodeBootstrapper.dll");
    FreeLibrary(module);

    auto workingDir = ghc::filesystem::current_path();
    auto updatesDir = workingDir / "geode" / "update";

    auto error = std::error_code();

    if (ghc::filesystem::exists(updatesDir / "GeodeBootstrapper.dll", error) && !error) {
        ghc::filesystem::rename(
            updatesDir / "GeodeBootstrapper.dll", 
            workingDir / "GeodeBootstrapper.dll", error
        );
        if (error) return error.value();
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

static SettingChangedEventHandler<BoolSetting> _(
    "geode.loader", "show-platform-console",
    [](auto setting) {
        if (setting->getValue()) {
            Loader::get()->openPlatformConsole();
        } else {
            Loader::get()->closePlatfromConsole();
        }
    }
);

int geodeEntry(void* platformData) {
    // setup internals

    if (!InternalLoader::get()) {
        InternalLoader::platformMessageBox(
            "Unable to Load Geode!",
            "There was an unknown fatal error setting up "
            "internal tools and Geode can not be loaded. "
            "(InternalLoader::get returned nullptr)"
        );
    }

    if (!geode::core::hook::initialize()) {
        InternalLoader::platformMessageBox(
            "Unable to load Geode!",
            "There was an unknown fatal error setting up "
            "internal tools and Geode can not be loaded. "
            "(Unable to set up hook manager)"
        );
    }

    Interface::get()->init(InternalMod::get());

    if (!InternalLoader::get()->setup()) {
        // if we've made it here, Geode will 
        // be gettable (otherwise the call to 
        // setup would've immediately crashed)

        InternalLoader::platformMessageBox(
            "Unable to Load Geode!",
            "There was an unknown fatal error setting up "
            "internal tools and Geode can not be loaded. "
            "(InternalLoader::setup) returned false"
        );
        return 1;
    }

    internal_log(Severity::Debug, "Loaded internal Geode class");

    // set up loader, load mods, etc.
    if (!Loader::get()->setup()) {
        InternalLoader::platformMessageBox(
            "Unable to Load Geode!",
            "There was an unknown fatal error setting up "
            "the loader and Geode can not be loaded."
        );
        delete InternalLoader::get();
        return 1;
    }

    internal_log(Severity::Debug, "Set up loader");
    
    if (InternalMod::get()->getSettingValue<bool>("show-platform-console")) {
        Loader::get()->openPlatformConsole();
    }

    internal_log(Severity::Debug, "Entry done.");

    return 0;
}
