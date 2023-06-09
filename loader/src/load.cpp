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

    // open console
    if (Mod::get()->getSettingValue<bool>("show-platform-console")) {
        Loader::get()->openPlatformConsole();
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

    // download and install new loader update in the background
    if (Mod::get()->getSettingValue<bool>("auto-check-updates")) {
        LoaderImpl::get()->checkForLoaderUpdates();
    }

    log::debug("Entry done.");

    return 0;
}
