#include <loader/LoaderImpl.hpp>
#include <loader/console.hpp>
#include <loader/IPC.hpp>
#include <loader/updater.hpp>

#include <Geode/loader/IPC.hpp>
#include <Geode/loader/Loader.hpp>
#include <Geode/loader/Log.hpp>
#include <Geode/loader/Mod.hpp>
#include <Geode/utils/JsonValidation.hpp>
#include <loader/LogImpl.hpp>

#include <array>

using namespace geode::prelude;

#include "load.hpp"

$on_mod(Loaded) {
    ipc::listen("ipc-test", [](ipc::IPCEvent* event) -> matjson::Value {
        return "Hello from Geode!";
    });

    ipc::listen("loader-info", [](ipc::IPCEvent* event) -> matjson::Value {
        return Mod::get()->getMetadataRef();
    });

    ipc::listen("list-mods", [](ipc::IPCEvent* event) -> matjson::Value {
        std::vector<matjson::Value> res;

        auto args = *event->messageData;
        auto root = checkJson(args, "[ipc/list-mods]");

        auto includeRunTimeInfo = root.has("include-runtime-info").get<bool>();
        auto dontIncludeLoader = root.has("dont-include-loader").get<bool>();

        if (!dontIncludeLoader) {
            res.push_back(
                includeRunTimeInfo ? Mod::get()->getRuntimeInfo() :
                                     Mod::get()->getMetadataRef().toJSON()
            );
        }

        for (auto& mod : Loader::get()->getAllMods()) {
            res.push_back(includeRunTimeInfo ? mod->getRuntimeInfo() : mod->getMetadataRef().toJSON());
        }

        return res;
    });
}

void tryLogForwardCompat() {
    if (!LoaderImpl::get()->isForwardCompatMode()) return;
    // TODO: change text later
    log::warn("+-----------------------------------------------------------------------------------------------+");
    log::warn("| Geode is running in a newer version of GD than Geode targets.                                 |");
    log::warn("| UI is going to be disabled, platform console is forced on and crashes can be more common.     |");
    log::warn("| However, if your game crashes, it is probably caused by an outdated mod and not Geode itself. |");
    log::warn("+-----------------------------------------------------------------------------------------------+");
}

void tryShowForwardCompat() {
    if (!LoaderImpl::get()->isForwardCompatMode()) return;

    if (Mod::get()->getSavedValue<std::string>("last-forward-compat-warn-popup-ver", "_") ==
        LoaderImpl::get()->getGameVersion())
        return;

    // TODO: change text later
    // console::messageBox(
    //     "Forward Compatibility Warning",
    //     "Geode is running in a newer version of GD than Geode targets.\n"
    //     "UI is going to be disabled, platform console is forced on and crashes can be more common.\n"
    //     "However, if your game crashes, it is probably caused by an outdated mod and not Geode itself.",
    //     Severity::Warning
    // );

    Mod::get()->setSavedValue<std::string>(
        "last-forward-compat-warn-popup-ver",
        LoaderImpl::get()->getGameVersion()
    );
}

#ifdef GEODE_IS_WINDOWS
bool safeModeCheck() {
    // yes this is quite funny
    if (!(GetAsyncKeyState(VK_SHIFT) & (1 << 15))) {
        return false;
    }

    auto choice = MessageBoxW(
        NULL,
        L"(This has been triggered because you were holding SHIFT)\n"
        L"Do you want to activate Geode Safe Mode? This disables loading any mods.",
        L"Attention",
        MB_YESNO | MB_ICONINFORMATION
    );
    return choice == IDYES;
}
#elif !defined(GEODE_IS_MACOS)
// macos is defined in load.mm, this is for android
// on android the launcher just adds the launch args to enable safe mode
bool safeModeCheck() {
    return false;
}
#endif

int geodeEntry(void* platformData) {
    thread::setName("Main");

    console::setup();
    if (LoaderImpl::get()->isForwardCompatMode()) {
        console::openIfClosed();
    }

    if (safeModeCheck()) {
        LoaderImpl::get()->forceSafeMode();
    }

    std::string forwardCompatSuffix;
    if (LoaderImpl::get()->isForwardCompatMode())
        forwardCompatSuffix = " (forward compatibility mode)";

    if (LoaderImpl::get()->getGameVersion().empty()) {
        log::info("Running {} {}{} on {}", Mod::get()->getName(), Mod::get()->getVersion(),
            forwardCompatSuffix, PlatformID::toString(GEODE_PLATFORM_TARGET));
    }
    else {
        log::info("Running {} {} in Geometry Dash v{}{} on {}", Mod::get()->getName(),
            Mod::get()->getVersion(), LoaderImpl::get()->getGameVersion(), forwardCompatSuffix,
            PlatformID::toString(GEODE_PLATFORM_TARGET));
    }

    tryLogForwardCompat();

    auto begin = std::chrono::high_resolution_clock::now();

    // set up internal mod, settings and data
    log::info("Setting up internal mod");
    {
        log::NestScope nest;
        auto internalSetupRes = LoaderImpl::get()->setupInternalMod();
        if (!internalSetupRes) {
            console::messageBox(
                "Unable to Load Geode!",
                "There was a fatal error setting up "
                "the internal mod and Geode can not be loaded: " + internalSetupRes.unwrapErr()
            );
            LoaderImpl::get()->forceReset();
            return 1;
        }
    }

    tryShowForwardCompat();

    if (Mod::get()->getSettingValue<bool>("show-platform-console")) {
        console::openIfClosed();
    }

    // Setup logger here so that internal mod is setup and we can read log level
    // Logging before this point does store the log, and everything gets logged in this setup call
    log::Logger::get()->setup();

    // set up loader, load mods, etc.
    log::info("Setting up loader");
    {
        log::NestScope nest;
        auto setupRes = LoaderImpl::get()->setup();
        if (!setupRes) {
            console::messageBox(
                "Unable to Load Geode!",
                "There was an unknown fatal error setting up "
                "the loader and Geode can not be loaded. "
                "(" + setupRes.unwrapErr() + ")"
            );
            LoaderImpl::get()->forceReset();
            return 1;
        }
    }

    crashlog::setupPlatformHandlerPost();

    // delete old log files

    int logMaxAge = Mod::get()->getSettingValue<int>("log-retention-period");

    // 0 means no deletion
    if (logMaxAge > 0) {
        // put it in a thread so that it doesn't slow down launch times
        std::thread([logMaxAge] {
            log::Logger::get()->deleteOldLogs(std::chrono::days{logMaxAge});
        }).detach();
    }


    log::debug("Setting up IPC");
    {
        log::NestScope nest;
        ipc::setup();
    }

    // download and install new loader update in the background

    if (Mod::get()->getSettingValue<bool>("auto-check-updates")) {
        log::info("Starting loader update check");
        updater::checkForLoaderUpdates();
    }
    else {
        log::info("Skipped loader update check");
    }

    auto end = std::chrono::high_resolution_clock::now();
    auto time = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count();
    log::info("Entry took {}s", static_cast<float>(time) / 1000.f);

    // also log after entry so that users are more likely to notice
    tryLogForwardCompat();

    return 0;
}
