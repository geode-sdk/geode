#include "InternalLoader.hpp"
#include <vector>
#include <string>
#include <sstream>
#include <iostream>
#include "InternalMod.hpp"
#include <Geode/loader/Log.hpp>
#include <Geode/loader/Loader.hpp>
#include <Geode/Geode.hpp>
#include <thread>

InternalLoader::InternalLoader() : Loader() {
    #ifdef GEODE_PLATFORM_CONSOLE
    this->setupPlatformConsole();
    #endif
}

InternalLoader::~InternalLoader() {
    #ifdef GEODE_PLATFORM_CONSOLE
    this->closePlatformConsole();
    #endif
}

InternalLoader* InternalLoader::get() {
    static auto g_geode = new InternalLoader;
    return g_geode;
}

bool InternalLoader::setup() {
    InternalMod::get()->log()
        << Severity::Debug << "Set up internal mod representation";

    InternalMod::get()->log()
        << Severity::Debug << "Loading hooks... ";

    if (!this->loadHooks()) {
        InternalMod::get()->log()
            << "There were errors loading some hooks, "
            "see console for details";
    }

    InternalMod::get()->log()
        << Severity::Debug << "Loaded hooks";

    return true;
}

void InternalLoader::queueInGDThread(std::function<void GEODE_CALL()> func) {
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

void InternalLoader::queueConsoleMessage(LogPtr* msg) {
    this->m_logQueue.push_back(msg);
}

bool InternalLoader::platformConsoleReady() const {
    return m_platformConsoleReady;
}

#if defined(GEODE_IS_WINDOWS)
void InternalLoader::platformMessageBox(const char* title, const char* info) {
    MessageBoxA(nullptr, title, info, MB_OK);
}

void InternalLoader::setupPlatformConsole() {
    if (m_platformConsoleReady) return;
    if (AllocConsole() == 0)    return;
    // redirect console output
    freopen_s(reinterpret_cast<FILE**>(stdout), "CONOUT$", "w", stdout);
    freopen_s(reinterpret_cast<FILE**>(stdin), "CONIN$", "r", stdin);

    m_platformConsoleReady = true;
}

void InternalLoader::awaitPlatformConsole() {
    if (!m_platformConsoleReady) return;

    for (auto const& log : this->m_logQueue) {
        std::cout << log->toString(true) << "\n";
        this->m_logQueue.clear();
    }

    std::string inp;
    getline(std::cin, inp);
    std::string inpa;
    std::stringstream ss(inp);
    std::vector<std::string> args;

    while (ss >> inpa) args.push_back(inpa);
    ss.clear();
    
    if (inp != "e") this->awaitPlatformConsole();
}

void InternalLoader::closePlatformConsole() {
    if (!m_platformConsoleReady) return;

    fclose(stdin);
    fclose(stdout);
    FreeConsole();
}

#elif defined(GEODE_IS_MACOS)
#include <iostream>

void InternalLoader::platformMessageBox(const char* title, const char* info) {
	std::cout << title << ": " << info << std::endl;
}

void InternalLoader::setupPlatformConsole() {
    m_platformConsoleReady = true;
}

void InternalLoader::awaitPlatformConsole() {
	
}

void InternalLoader::closePlatformConsole() {
}

#elif defined(GEODE_IS_IOS)

#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>

void InternalLoader::platformMessageBox(const char* title, const char* info) {
    std::cout << title << ": " << info << std::endl;
}

void InternalLoader::setupPlatformConsole() {
    ghc::filesystem::path(getpwuid(getuid())->pw_dir);
    freopen(ghc::filesystem::path(utils::dirs::geodeRoot() / "geode_log.txt").string().c_str(),"w",stdout);
    InternalLoader::
    m_platformConsoleReady = true;
}

void InternalLoader::awaitPlatformConsole() {
}

void InternalLoader::closePlatformConsole() {
}
#endif

