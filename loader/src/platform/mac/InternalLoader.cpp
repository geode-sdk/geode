#include <Geode/loader/IPC.hpp>
#include <Geode/loader/Log.hpp>
#include <InternalLoader.hpp>
#include <InternalMod.hpp>
#include <iostream>

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

CFDataRef msgPortCallback(CFMessagePortRef port, SInt32 messageID, CFDataRef data, void* info) {
    if (!CFDataGetLength(data)) return NULL;

    std::string cdata(reinterpret_cast<char const*>(CFDataGetBytePtr(data)), CFDataGetLength(data));

    std::string reply = InternalLoader::processRawIPC(port, cdata);
    return CFDataCreate(NULL, (UInt8 const*)reply.data(), reply.size());
}

void InternalLoader::setupIPC() {
    std::thread([]() {
        CFStringRef portName = CFStringCreateWithCString(NULL, IPC_PORT_NAME, kCFStringEncodingUTF8);

        CFMessagePortRef localPort =
            CFMessagePortCreateLocal(NULL, portName, msgPortCallback, NULL, NULL);
        if (localPort == NULL) {
            log::warn("Unable to create port, quitting IPC");
            return;
        }
        CFRunLoopSourceRef runLoopSource = CFMessagePortCreateRunLoopSource(NULL, localPort, 0);

        if (runLoopSource == NULL) {
            log::warn("Unable to create loop source, quitting IPC");
            return;
        }

        CFRunLoopAddSource(CFRunLoopGetCurrent(), runLoopSource, kCFRunLoopCommonModes);
        CFRunLoopRun();
        CFRelease(localPort);
    }).detach();
    log::debug("IPC set up");
}

#endif
