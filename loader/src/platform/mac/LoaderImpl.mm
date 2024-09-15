#import <Foundation/Foundation.h>
#include <Geode/loader/IPC.hpp>
#include <Geode/loader/Log.hpp>
#include <iostream>
#include <loader/LoaderImpl.hpp>
#include <loader/console.hpp>
#include <loader/IPC.hpp>
#include <loader/ModImpl.hpp>
#include <sys/stat.h>
#include <loader/LogImpl.hpp>
#include <dlfcn.h>

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


void console::setup() { }
void console::openIfClosed() {
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
        console::log(log.toString(), log.getSeverity());
    }
}

CFDataRef msgPortCallback(CFMessagePortRef port, SInt32 messageID, CFDataRef data, void* info) {
    if (!CFDataGetLength(data)) return NULL;

    std::string cdata(reinterpret_cast<char const*>(CFDataGetBytePtr(data)), CFDataGetLength(data));

    std::string reply = geode::ipc::processRaw(port, cdata).dump();
    return CFDataCreate(NULL, (UInt8 const*)reply.data(), reply.size());
}

void geode::ipc::setup() {
    std::thread([]() {
        thread::setName("Geode Main IPC");

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

void Loader::Impl::addNativeBinariesPath(std::filesystem::path const& path) {
    // this takes advantage of dyld using already loaded binaries when loading relative shared libraries
    // however, this also means that the binaries are loaded, which could have some weird side effects
    // but if you could use dlopen (and thus control when libraries are loaded), then you wouldn't be using this, would you?

    for (const auto& entry : std::filesystem::directory_iterator(path)) {
        if (!entry.is_regular_file()) {
            continue;
        }

        auto& entry_path = entry.path();

        if (entry_path.extension() != ".dylib") {
            continue;
        }

        auto handle = dlopen(entry_path.string().c_str(), RTLD_LAZY);

        if (!handle) {
            auto err = dlerror();
            log::warn("failed to load native binary at {}: dlerror returned ({})", entry_path.string(), err);
            continue;
        }

        dlclose(handle);
    }
}

std::string Loader::Impl::getGameVersion() {
    NSBundle* mainBundle = [NSBundle mainBundle];
    NSDictionary* infoDictionary = [mainBundle infoDictionary];

    NSString *version = infoDictionary[@"CFBundleShortVersionString"];

    return std::string([version UTF8String]);
}

// TODO
bool Loader::Impl::supportsLaunchArguments() const {
    return false;
}

std::string Loader::Impl::getLaunchCommand() const {
    return std::string(); // Empty
}
