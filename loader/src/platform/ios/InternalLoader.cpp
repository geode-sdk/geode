#include <InternalLoader.hpp>
#include <Geode/loader/Log.hpp>
#include <iostream>
#include <InternalMod.hpp>

#ifdef GEODE_IS_IOS

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

void InternalLoader::postIPCReply(
    void* rawPipeHandle,
    std::string const& replyID,
    nlohmann::json const& data
) {}

void InternalLoader::setupIPC() {
    #warning "Set up pipes or smth for this platform"
    log::log(Severity::Warning, InternalMod::get(), "IPC is not supported on this platform");
}

#endif
