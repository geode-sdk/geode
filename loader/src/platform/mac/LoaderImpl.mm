#include <Geode/loader/IPC.hpp>
#include <Geode/loader/Log.hpp>
#include <iostream>
#include <loader/LoaderImpl.hpp>
#include <loader/console.hpp>
#include <loader/IPC.hpp>
#include <loader/ModImpl.hpp>
#import <Foundation/Foundation.h>
#include <sys/stat.h>
#include <loader/LogImpl.hpp>

using namespace geode::prelude;

struct MacConsoleData {
    std::string logFile;
    std::string scriptFile;
    int logFd;
};

bool s_isOpen = false;
MacConsoleData s_platformData;

void console::messageBox(char const* title, std::string const& info, Severity) {
    CFStringRef cfTitle = CFStringCreateWithCString(NULL, title, kCFStringEncodingUTF8);
    CFStringRef cfMessage = CFStringCreateWithCString(NULL, info.c_str(), kCFStringEncodingUTF8);

    CFUserNotificationDisplayNotice(
        0, kCFUserNotificationNoteAlertLevel, NULL, NULL, NULL, cfTitle, cfMessage, NULL
    );
}

void console::log(std::string const& msg, Severity severity) {
    if (s_isOpen) {
        int colorcode = 0;
        switch (severity) {
            case Severity::Debug: colorcode = 36; break;
            case Severity::Info: colorcode = 34; break;
            case Severity::Warning: colorcode = 33; break;
            case Severity::Error: colorcode = 31; break;
            default: colorcode = 35; break;
        }
        auto newMsg = "\033[1;" + std::to_string(colorcode) + "m" + msg.substr(0, 8) + "\033[0m" + msg.substr(8);

        std::cout << newMsg << "\n" << std::flush;
    }
}


void console::open() {
    if (s_isOpen) return;

    std::string outFile = "/tmp/command_output_XXXXXX";
    int outFd = mkstemp(&outFile[0]);

    auto script = outFile + ".command";
    auto scriptContent = fmt::format(R"(
        #!/bin/sh
        echo -n -e "\033]0;Geode Console {}\007"
        tail -f {} &
        trap "" SIGINT
        lsof -p {} +r 1 &>/dev/null
        pkill -P $$
        osascript -e 'tell application "Terminal"
            close (every window whose name contains "Geode Console {}")
            if (count windows) is 0 then quit
        end tell' &
        exit
    )", getpid(), outFile, getpid(), getpid());

    if (file::writeString(script, scriptContent)) {
        chmod(script.c_str(), 0777);
        dup2(outFd, 1);
        dup2(outFd, 2);

        NSTask* task = [[NSTask alloc] init];
        task.launchPath = @"/usr/bin/open";
        task.arguments = @[[NSString stringWithUTF8String:script.c_str()]];
        [task launch];

        s_platformData = MacConsoleData {
            outFile,
            script,
            outFd
        };
    }

    s_isOpen = true;

    for (auto const& log : log::Logger::get()->list()) {
        console::log(log.toString(true), log.getSeverity());
    }
}

void console::close() {
    if (s_isOpen) {
        ::close(s_platformData.logFd);
        unlink(s_platformData.logFile.c_str());
        unlink(s_platformData.scriptFile.c_str());
    }

    s_isOpen = false;
}

CFDataRef msgPortCallback(CFMessagePortRef port, SInt32 messageID, CFDataRef data, void* info) {
    if (!CFDataGetLength(data)) return NULL;

    std::string cdata(reinterpret_cast<char const*>(CFDataGetBytePtr(data)), CFDataGetLength(data));

    std::string reply = geode::ipc::processRaw(port, cdata).dump();
    return CFDataCreate(NULL, (UInt8 const*)reply.data(), reply.size());
}

void geode::ipc::setup() {
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

bool Loader::Impl::userTriedToLoadDLLs() const {
    return false;
}

void Loader::Impl::addNativeBinariesPath(ghc::filesystem::path const& path) {
    log::warn("LoaderImpl::addNativeBinariesPath not implement on this platform, not adding path {}", path.string());
}

std::string Loader::Impl::getGameVersion() {
    return GEODE_STR(GEODE_GD_VERSION); // TODO implement
}
