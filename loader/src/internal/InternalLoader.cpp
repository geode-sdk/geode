#include "InternalLoader.hpp"
#include <vector>
#include <string>
#include <sstream>
#include <iostream>
#include "InternalMod.hpp"
#include <Geode/loader/Log.hpp>
#include <Geode/loader/Loader.hpp>
#include <Geode/utils/fetch.hpp>
#include <thread>

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
            Severity::Error,
            InternalMod::get(),
            "There were errors loading some hooks, see console for details"
        );
    }

    log::log(Severity::Debug, InternalMod::get(), "Loaded hooks");

    return true;
}

void InternalLoader::queueInGDThread(ScheduledFunction func) {
    std::lock_guard<std::mutex> lock(m_gdThreadMutex);
    this->m_gdThreadQueue.push_back(func);
}

void InternalLoader::executeGDThreadQueue() {
    m_gdThreadMutex.lock();
    auto queue = std::move(m_gdThreadQueue);
    m_gdThreadMutex.unlock();
    for (auto const& func : queue) {
        func();
    }
    m_gdThreadMutex.lock();
    m_gdThreadQueue.clear();
    m_gdThreadMutex.unlock();
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

#if defined(GEODE_IS_WINDOWS)
void InternalLoader::platformMessageBox(const char* title, std::string const& info) {
    MessageBoxA(nullptr, info.c_str(), title, MB_ICONERROR);
}

void InternalLoader::openPlatformConsole() {
    if (m_platformConsoleOpen) return;
    if (AllocConsole() == 0)   return;
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

void InternalLoader::platformMessageBox(const char* title, std::string const& info) {
	 CFStringRef cfTitle = CFStringCreateWithCString(NULL, title, kCFStringEncodingUTF8);
    CFStringRef cfMessage = CFStringCreateWithCString(NULL, info.c_str(), kCFStringEncodingUTF8);

    CFUserNotificationDisplayNotice(0, kCFUserNotificationNoteAlertLevel, NULL, NULL, NULL, cfTitle, cfMessage, NULL);
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

#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>

void InternalLoader::platformMessageBox(const char* title, std::string const& info) {
    std::cout << title << ": " << info << std::endl;
}

void InternalLoader::openPlatformConsole() {
    ghc::filesystem::path(getpwuid(getuid())->pw_dir);
    freopen(ghc::filesystem::path(
        utils::file::geodeRoot() / "geode_log.txt"
    ).string().c_str(),"w",stdout);
    InternalLoader::
    m_platformConsoleOpen = true;
}

void InternalLoader::closePlatformConsole() {
}
#endif

