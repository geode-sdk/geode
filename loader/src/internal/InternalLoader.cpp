#include <fmt/format.h>
#include <hash.hpp>
#include <iostream>
#include <sstream>
#include <string>
#include <thread>
#include <vector>

#include <Geode/loader/Loader.hpp>
#include <Geode/loader/IPC.hpp>
#include <Geode/loader/Log.hpp>
#include <Geode/utils/web.hpp>
#include <Geode/utils/file.hpp>

#include "InternalLoader.hpp"
#include "InternalMod.hpp"
#include "resources.hpp"

InternalLoader::InternalLoader() : Loader() {}

InternalLoader::~InternalLoader() {
    this->closePlatformConsole();
}

InternalLoader* InternalLoader::get() {
    static auto g_geode = new InternalLoader;
    return g_geode;
}

bool InternalLoader::setup() {
    log::log(Severity::Debug, InternalMod::get(), "Set up internal mod representation");
    log::log(Severity::Debug, InternalMod::get(), "Loading hooks... ");

    if (!this->loadHooks()) {
        log::log(
            Severity::Error, InternalMod::get(),
            "There were errors loading some hooks, see console for details"
        );
    }

    log::log(Severity::Debug, InternalMod::get(), "Loaded hooks");

    log::log(Severity::Debug, InternalMod::get(), "Setting up IPC...");

    this->setupIPC();

    return true;
}

bool InternalLoader::isReadyToHook() const {
    return m_readyToHook;
}

void InternalLoader::addInternalHook(Hook* hook, Mod* mod) {
    m_internalHooks.push_back({hook, mod});
}

bool InternalLoader::loadHooks() {
    m_readyToHook = true;
    auto thereWereErrors = false;
    for (auto const& hook : m_internalHooks) {
        auto res = hook.second->addHook(hook.first);
        if (!res) {
            log::log(Severity::Error, hook.second, "{}", res.unwrapErr());
            thereWereErrors = true;
        }
    }
    // free up memory
    m_internalHooks.clear();
    return !thereWereErrors;
}

void InternalLoader::queueInGDThread(ScheduledFunction func) {
    std::lock_guard<std::mutex> lock(m_gdThreadMutex);
    m_gdThreadQueue.push_back(func);
}

void InternalLoader::executeGDThreadQueue() {
    // copy queue to avoid locking mutex if someone is
    // running addToGDThread inside their function
    m_gdThreadMutex.lock();
    auto queue = m_gdThreadQueue;
    m_gdThreadQueue.clear();
    m_gdThreadMutex.unlock();

    // call queue
    for (auto const& func : queue) {
        func();
    }
}

void InternalLoader::logConsoleMessage(std::string const& msg) {
    if (m_platformConsoleOpen) {
        // TODO: make flushing optional
        std::cout << msg << '\n' << std::flush;
    }
}

bool InternalLoader::platformConsoleOpen() const {
    return m_platformConsoleOpen;
}

bool InternalLoader::shownInfoAlert(std::string const& key) {
    if (m_shownInfoAlerts.count(key)) {
        return true;
    }
    m_shownInfoAlerts.insert(key);
    return false;
}

void InternalLoader::saveInfoAlerts(nlohmann::json& json) {
    json["alerts"] = m_shownInfoAlerts;
}

void InternalLoader::loadInfoAlerts(nlohmann::json& json) {
    m_shownInfoAlerts = json["alerts"].get<std::unordered_set<std::string>>();
}

void InternalLoader::downloadLoaderResources(IndexUpdateCallback callback) {
    auto version = this->getVersion().toString();
    auto tempResourcesZip = this->getGeodeDirectory() / GEODE_RESOURCE_DIRECTORY / "new.zip";
    auto resourcesDir =
        this->getGeodeDirectory() / GEODE_RESOURCE_DIRECTORY / InternalMod::get()->getID();

    web::AsyncWebRequest()
        .join("update-geode-loader-resources")
        .fetch(fmt::format(
            "https://github.com/geode-sdk/geode/releases/download/{}/resources.zip", version
        ))
        .into(tempResourcesZip)
        .then([tempResourcesZip, resourcesDir, callback](auto) {
            // unzip resources zip
            auto unzip = file::unzipTo(tempResourcesZip, resourcesDir);
            if (!unzip) {
                if (callback)
                    callback(
                        UpdateStatus::Failed, "Unable to unzip new resources: " + unzip.unwrapErr(), 0
                    );
                return;
            }
            // delete resources zip
            try {
                ghc::filesystem::remove(tempResourcesZip);
            }
            catch (...) {
            }

            if (callback) callback(UpdateStatus::Finished, "Resources updated", 100);
        })
        .expect([callback](std::string const& info) {
            if (callback) callback(UpdateStatus::Failed, info, 0);
        })
        .progress([callback](auto&, double now, double total) {
            if (callback)
                callback(
                    UpdateStatus::Progress, "Downloading resources",
                    static_cast<uint8_t>(now / total * 100.0)
                );
        });
}

bool InternalLoader::verifyLoaderResources(IndexUpdateCallback callback) {
    static std::optional<bool> CACHED = std::nullopt;
    if (CACHED.has_value()) {
        return CACHED.value();
    }

    // geode/resources/geode.loader
    auto resourcesDir =
        this->getGeodeDirectory() / GEODE_RESOURCE_DIRECTORY / InternalMod::get()->getID();

    // if the resources dir doesn't exist, then it's probably incorrect
    if (!(ghc::filesystem::exists(resourcesDir) && ghc::filesystem::is_directory(resourcesDir))) {
        this->downloadLoaderResources(callback);
        return false;
    }

    // make sure every file was covered
    size_t coverage = 0;

    // verify hashes
    for (auto& file : ghc::filesystem::directory_iterator(resourcesDir)) {
        auto name = file.path().filename().string();
        // skip unknown files
        if (!LOADER_RESOURCE_HASHES.count(name)) {
            continue;
        }
        // verify hash
        auto hash = calculateSHA256(file.path());
        if (hash != LOADER_RESOURCE_HASHES.at(name)) {
            log::debug(
                "compare {} {} {}", file.path().string(), hash, LOADER_RESOURCE_HASHES.at(name)
            );
            this->downloadLoaderResources(callback);
            return false;
        }
        coverage += 1;
    }

    // make sure every file was found
    if (coverage != LOADER_RESOURCE_HASHES.size()) {
        this->downloadLoaderResources(callback);
        return false;
    }

    return true;
}

std::string InternalLoader::processRawIPC(void* rawHandle, std::string const& buffer) {
    std::string reply;

    try {
        std::optional<std::string> replyID = std::nullopt;

        // parse received message
        auto json = nlohmann::json::parse(buffer);
        if (!json.contains("mod") || !json["mod"].is_string()) {
            log::warn("Received IPC message without 'mod' field");
            return reply;
        }
        if (!json.contains("message") || !json["message"].is_string()) {
            log::warn("Received IPC message without 'message' field");
            return reply;
        }
        if (json.contains("reply") && json["reply"].is_string()) {
            replyID = json["reply"];
        }
        nlohmann::json data;
        if (json.contains("data")) {
            data = json["data"];
        }
        // log::debug("Posting IPC event");
        // ! warning: if the event system is ever made asynchronous this will break!
        IPCEvent(rawHandle, json["mod"], json["message"], data, &reply).post();
    } catch(...) {
        log::warn("Received IPC message that isn't valid JSON");
    }

    return reply;
}
