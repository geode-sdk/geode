#include <loader/LoaderImpl.hpp>

#include <Geode/loader/IPC.hpp>
#include <Geode/loader/Loader.hpp>
#include <Geode/loader/Log.hpp>
#include <Geode/loader/Mod.hpp>
#include <Geode/loader/SettingEvent.hpp>
#include <Geode/loader/ModJsonTest.hpp>
#include <Geode/utils/JsonValidation.hpp>
#include <loader/LogImpl.hpp>

#include <array>
 
using namespace geode::prelude;

#include "load.hpp"

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
        return Mod::get()->getMetadata();
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
                includeRunTimeInfo ? Mod::get()->getRuntimeInfo() :
                                     Mod::get()->getMetadata().toJSON()
            );
        }

        for (auto& mod : Loader::get()->getAllMods()) {
            res.push_back(includeRunTimeInfo ? mod->getRuntimeInfo() : mod->getMetadata().toJSON());
        }

        return res;
    });
}

int geodeEntry(void* platformData) {
    log::Logger::get()->setup();

    log::info("Running {} {}", Mod::get()->getName(), Mod::get()->getVersion());

    auto begin = std::chrono::high_resolution_clock::now();

    // set up internal mod, settings and data
    log::info("Setting up internal mod");
    log::pushNest();
    auto internalSetupRes = LoaderImpl::get()->setupInternalMod();
    log::popNest();
    if (!internalSetupRes) {
        LoaderImpl::get()->platformMessageBox(
            "Unable to Load Geode!",
            "There was a fatal error setting up "
            "the internal mod and Geode can not be loaded: " + internalSetupRes.unwrapErr()
        );
        LoaderImpl::get()->forceReset();
        return 1;
    }

    // open console
    if (Mod::get()->getSettingValue<bool>("show-platform-console")) {
        log::debug("Opening console");
        Loader::get()->openPlatformConsole();
    }

    // set up loader, load mods, etc.
    log::info("Setting up loader");
    log::pushNest();
    auto setupRes = LoaderImpl::get()->setup();
    log::popNest();
    if (!setupRes) {
        LoaderImpl::get()->platformMessageBox(
            "Unable to Load Geode!",
            "There was an unknown fatal error setting up "
            "the loader and Geode can not be loaded. "
            "(" + setupRes.unwrapErr() + ")"
        );
        LoaderImpl::get()->forceReset();
        return 1;
    }

    crashlog::setupPlatformHandlerPost();

    log::info("Set up loader");

    // download and install new loader update in the background
    if (Mod::get()->getSettingValue<bool>("auto-check-updates")) {
        log::info("Starting loader update check");
        LoaderImpl::get()->checkForLoaderUpdates();
    }
    else {
        log::info("Skipped loader update check");
    }

    auto end = std::chrono::high_resolution_clock::now();
    auto time = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count();
    log::info("Entry took {}s", static_cast<float>(time) / 1000.f);

    return 0;
}
