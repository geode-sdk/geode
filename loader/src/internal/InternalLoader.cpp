#include "InternalLoader.hpp"

#include "InternalMod.hpp"
#include "resources.hpp"

#include <Geode/loader/Loader.hpp>
#include <Geode/loader/Log.hpp>
#include <Geode/utils/fetch.hpp>
#include <Geode/utils/file.hpp>
#include <fmt/format.h>
#include <hash.hpp>
#include <iostream>
#include <sstream>
#include <string>
#include <thread>
#include <vector>

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

    return true;
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
                        UpdateStatus::Failed, "Unable to unzip new resources: " + unzip.error(), 0
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

#if defined(GEODE_IS_WINDOWS)
void InternalLoader::platformMessageBox(char const* title, std::string const& info) {
    MessageBoxA(nullptr, info.c_str(), title, MB_ICONERROR);
}

void InternalLoader::openPlatformConsole() {
    if (m_platformConsoleOpen) return;
    if (AllocConsole() == 0) return;
    SetConsoleCP(CP_UTF8);
    // redirect console output
    freopen_s(reinterpret_cast<FILE**>(stdout), "CONOUT$", "w", stdout);
    freopen_s(reinterpret_cast<FILE**>(stdin), "CONIN$", "r", stdin);

    m_platformConsoleOpen = true;

    for (auto const& log : Loader::get()->getLogs()) {
        std::cout << log->toString(true) << "\n";
    }
}

void InternalLoader::closePlatformConsole() {
    if (!m_platformConsoleOpen) return;

    fclose(stdin);
    fclose(stdout);
    FreeConsole();

    m_platformConsoleOpen = false;
}

#elif defined(GEODE_IS_MACOS)
    #include <CoreFoundation/CoreFoundation.h>

void InternalLoader::platformMessageBox(char const* title, std::string const& info) {
    CFStringRef cfTitle = CFStringCreateWithCString(NULL, title, kCFStringEncodingUTF8);
    CFStringRef cfMessage = CFStringCreateWithCString(NULL, info.c_str(), kCFStringEncodingUTF8);

    CFUserNotificationDisplayNotice(
        0, kCFUserNotificationNoteAlertLevel, NULL, NULL, NULL, cfTitle, cfMessage, NULL
    );
}

void InternalLoader::openPlatformConsole() {
    m_platformConsoleOpen = true;

    for (auto const& log : Loader::get()->getLogs()) {
        std::cout << log->toString(true) << "\n";
    }
}

void InternalLoader::closePlatformConsole() {
    m_platformConsoleOpen = false;
}

#elif defined(GEODE_IS_IOS)

    #include <pwd.h>
    #include <sys/types.h>
    #include <unistd.h>

void InternalLoader::platformMessageBox(char const* title, std::string const& info) {
    std::cout << title << ": " << info << std::endl;
}

void InternalLoader::openPlatformConsole() {
    ghc::filesystem::path(getpwuid(getuid())->pw_dir);
    freopen(
        ghc::filesystem::path(utils::file::geodeRoot() / "geode_log.txt").string().c_str(), "w",
        stdout
    );
    InternalLoader::m_platformConsoleOpen = true;
}

void InternalLoader::closePlatformConsole() {}
#endif
