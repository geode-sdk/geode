#include <loader/LoaderImpl.hpp>

#include <Geode/loader/Dirs.hpp>
#include <Geode/loader/Loader.hpp>
#include <Geode/loader/Log.hpp>
#include <Geode/utils/file.hpp>
#include <loader/ModImpl.hpp>
#include <loader/IPC.hpp>
#include <loader/console.hpp>
#include <iostream>
#include <pwd.h>
#include <sys/types.h>
#include <unistd.h>
#import <Foundation/Foundation.h>

#include <loader/LogImpl.hpp>

using namespace geode::prelude;

bool s_isOpen = false;

void console::messageBox(ZStringView title, ZStringView info, Severity severity) {
    // TODO: implement
    console::log(info, severity);
}

void console::log(ZStringView zmsg, Severity severity) {
    auto msg = zmsg.view();
    NSLog(@"%s", zmsg.c_str());

    if (s_isOpen) {
        int colorcode = 0;
        switch (severity) {
            case Severity::Debug: colorcode = 36; break;
            case Severity::Info: colorcode = 34; break;
            case Severity::Warning: colorcode = 33; break;
            case Severity::Error: colorcode = 31; break;
            default: colorcode = 35; break;
        }
        
        auto newMsg = fmt::format(
            "\033[1;{}m{}\033[0m{}",
            colorcode,
            msg.substr(0, 8),
            msg.substr(8)
        );

        std::cout << newMsg << "\n" << std::flush;
    }
}

void console::openIfClosed() {
    if (s_isOpen) return;

    std::filesystem::path(getpwuid(getuid())->pw_dir);
    freopen(utils::string::pathToString(dirs::getGeodeDir() / "geode_log.txt").c_str(), "w", stdout);
    s_isOpen = true;
}

void console::setup() {}

// void Loader::Impl::postIPCReply(
//     void* rawPipeHandle, std::string const& replyID, matjson::Value const& data
// ) {}

void geode::ipc::setup() {
    #warning "Set up pipes or smth for this platform"
    log::warn("IPC is not supported on this platform");
}

bool Loader::Impl::userTriedToLoadDLLs() const {
    return false;
}

bool Loader::Impl::supportsLaunchArguments() const {
    return true;
}

std::string Loader::Impl::getLaunchCommand() const {
    auto launchArgsFile = dirs::getModRuntimeDir() / "launch-args.txt";
    if (std::filesystem::exists(launchArgsFile)) {
        log::debug("Reading launch arguments from {}", utils::string::pathToString(launchArgsFile));
        auto content = file::readString(launchArgsFile);
        if (content.isOk()) {
            std::filesystem::remove(launchArgsFile);
            return content.unwrap();
        }
    }

    return (getenv("LAUNCHARGS")) ? getenv("LAUNCHARGS") : std::string();
}

void Loader::Impl::addNativeBinariesPath(std::filesystem::path const& path) {
    log::warn("LoaderImpl::addNativeBinariesPath not implement on this platform, not adding path {}", path);
}

std::string Loader::Impl::getGameVersion() {
    NSBundle* mainBundle = [NSBundle mainBundle];
    NSDictionary* infoDictionary = [mainBundle infoDictionary];

    std::string version = [infoDictionary[@"CFBundleShortVersionString"] UTF8String];

    // temporary workaround - the bundle version is 2.207 although the actual game is 2.2074
    if (version == "2.207") return "2.2074";
    if (version == "2.208") return "2.2081";

    return version;
}

/**
 * iOS Jitless is implemented with Geode 4.6.0, so we kinda did a major change
 * but who cares about semver, right?
 */
bool Loader::Impl::isModVersionSupported(VersionInfo const& target) {
    return semverCompare(this->getVersion(), target);
}

bool Loader::Impl::isForwardCompatMode() {
    return false;
}
