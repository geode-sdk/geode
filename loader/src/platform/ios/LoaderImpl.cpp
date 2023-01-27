#include <loader/LoaderImpl.hpp>

#ifdef GEODE_IS_IOS

    #include <Geode/loader/Dirs.hpp>
    #include <Geode/loader/Loader.hpp>
    #include <Geode/loader/Log.hpp>
    #include <loader/ModImpl.hpp>
    #include <iostream>
    #include <pwd.h>
    #include <sys/types.h>
    #include <unistd.h>

void Loader::Impl::platformMessageBox(char const* title, std::string const& info) {
    std::cout << title << ": " << info << std::endl;
}

void Loader::Impl::openPlatformConsole() {
    ghc::filesystem::path(getpwuid(getuid())->pw_dir);
    freopen(ghc::filesystem::path(dirs::getGeodeDir() / "geode_log.txt").string().c_str(), "w", stdout);
    m_platformConsoleOpen = true;
}

void Loader::Impl::closePlatformConsole() {}

void Loader::Impl::postIPCReply(
    void* rawPipeHandle, std::string const& replyID, json::Value const& data
) {}

void Loader::Impl::setupIPC() {
    #warning "Set up pipes or smth for this platform"
    log::warning("IPC is not supported on this platform");
}

bool Loader::Impl::userTriedToLoadDLLs() const {
    return false;
}

#endif
