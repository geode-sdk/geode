#include <Geode/loader/IPC.hpp>
#include <Geode/loader/Log.hpp>
#include <iostream>
#include <loader/LoaderImpl.hpp>
#include <loader/ModImpl.hpp>
#import <Foundation/Foundation.h>
#include <sys/stat.h>

using namespace geode::prelude;

struct MacConsoleData {
    std::string logFile;
    std::string scriptFile;
    int logFd;
};

void Loader::Impl::platformMessageBox(char const* title, std::string const& info) {
    CFStringRef cfTitle = CFStringCreateWithCString(NULL, title, kCFStringEncodingUTF8);
    CFStringRef cfMessage = CFStringCreateWithCString(NULL, info.c_str(), kCFStringEncodingUTF8);

    CFUserNotificationDisplayNotice(
        0, kCFUserNotificationNoteAlertLevel, NULL, NULL, NULL, cfTitle, cfMessage, NULL
    );
}

void Loader::Impl::logConsoleMessageWithSeverity(std::string const& msg, Severity severity) {
    if (m_platformConsoleOpen) {
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

void Loader::Impl::openPlatformConsole() {
    if (m_platformConsoleOpen) return;

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

        m_platformData = new MacConsoleData {
            outFile,
            script,
            outFd
        };
    }

    m_platformConsoleOpen = true;

    for (auto const& log : log::Logger::list()) {
        this->logConsoleMessageWithSeverity(log->toString(true), log->getSeverity());
    }
}

void Loader::Impl::closePlatformConsole() {
    if (m_platformData) {
        auto consoleData = reinterpret_cast<MacConsoleData*>(m_platformData);
        close(consoleData->logFd);
        unlink(consoleData->logFile.c_str());
        unlink(consoleData->scriptFile.c_str());

        delete consoleData;
        m_platformData = nullptr;
    }

    m_platformConsoleOpen = false;
}

CFDataRef msgPortCallback(CFMessagePortRef port, SInt32 messageID, CFDataRef data, void* info) {
    if (!CFDataGetLength(data)) return NULL;

    std::string cdata(reinterpret_cast<char const*>(CFDataGetBytePtr(data)), CFDataGetLength(data));

    std::string reply = LoaderImpl::get()->processRawIPC(port, cdata).dump();
    return CFDataCreate(NULL, (UInt8 const*)reply.data(), reply.size());
}

void Loader::Impl::setupIPC() {
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
