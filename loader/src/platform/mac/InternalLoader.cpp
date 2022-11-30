#include <InternalLoader.hpp>
#include <Geode/loader/Log.hpp>
#include <iostream>
#include <InternalMod.hpp>

#ifdef GEODE_IS_MACOS

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

    for (auto const& log : log::Logs::list()) {
        std::cout << log->toString(true) << "\n";
    }
}

void InternalLoader::closePlatformConsole() {
    m_platformConsoleOpen = false;
}

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
