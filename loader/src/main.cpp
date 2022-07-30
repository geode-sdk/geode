#include <Geode/loader/Mod.hpp>
#include <Geode/loader/Loader.hpp>
#include <InternalLoader.hpp>
#include <InternalMod.hpp>
#include <Geode/loader/Log.hpp>
#include <Geode/loader/Interface.hpp>
#include "../core/Core.hpp"

int geodeEntry(void* platformData);
// platform-specific entry points

#if defined(GEODE_IS_IOS) || defined(GEODE_IS_MACOS)
#include <mach-o/dyld.h>
#include <unistd.h>

std::length_error::~length_error() _NOEXCEPT {} // do not ask...

__attribute__((constructor)) void _entry() {
    char gddir[PATH_MAX];
    uint32_t out = PATH_MAX;
    _NSGetExecutablePath(gddir, &out);

    ghc::filesystem::path gdpath = gddir;
    ghc::filesystem::current_path(gdpath.parent_path().parent_path());

    geodeEntry(nullptr);
}
#elif defined(GEODE_IS_WINDOWS)
#include <Windows.h>

DWORD WINAPI loadThread(void* arg) {
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

    InternalMod::get()->log()
        << Severity::Debug
        << "Loaded internal Geode class";

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

    InternalMod::get()->log()
        << Severity::Debug
        << "Set up loader";

    // debugging console
    #ifdef GEODE_PLATFORM_CONSOLE
    InternalMod::get()->log()
        << Severity::Debug
        << "Loading Console...";

    InternalLoader::get()->setupPlatformConsole();
    InternalLoader::get()->awaitPlatformConsole();
    InternalLoader::get()->closePlatformConsole();

    InternalMod::get()->log()
        << Severity::Debug
        << "Cleaning up...";

    //delete InternalLoader::get();
    #endif

    InternalMod::get()->log()
        << Severity::Debug
        << "Entry done.";
    return 0;
}
