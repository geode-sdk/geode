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

bool gameVersionIsAmbiguous(std::string gameVersion) {
    return gameVersion == "2.207";
}

std::array<std::uint8_t, 16> getBinaryUUID() {
    auto imageCount = _dyld_image_count();

    auto binaryHeader = _dyld_get_image_header(0);
    for (auto i = 0u; i < imageCount; i++) {
        auto headerCandidate = _dyld_get_image_header(i);
        if (headerCandidate->filetype == MH_EXECUTE) {
            auto imageName = _dyld_get_image_name(i);

            binaryHeader = headerCandidate;
            break;
        }
    }

    auto commandCount = binaryHeader->ncmds;
    auto commandSize = binaryHeader->sizeofcmds;

    // for whatever reason _dyld_get_image_header returns a 32bit mach header, not a 64bit one
    // that's okay but we do have to account for that issue here
    auto commands = reinterpret_cast<const std::uint8_t*>(binaryHeader) + sizeof(struct mach_header_64);

    for (auto i = 0u; i < commandCount; i++) {
        auto cCommand = reinterpret_cast<const struct load_command*>(commands);
        if (cCommand->cmd == LC_UUID) {
            auto uuidCommand = reinterpret_cast<const struct uuid_command*>(commands);
            return std::to_array(uuidCommand->uuid);
        }

        commands += cCommand->cmdsize;
    }

    return {};
}

const auto& uuidToVersionMap() {
    // we only need to define versions that are ambiguous
    // so for now, 2.207
    // you can get these hashes from otool, just look for the LC_UUID load command!
    static std::unordered_map<std::string, std::string> uuidToVersionName{
#if defined(GEODE_IS_ARM_MAC)
        {"27044C8B-76BD-303C-A035-5314AF1D9E6E", "2.2074"},
#elif defined(GEODE_IS_INTEL_MAC)
        {"DB5CADC0-E533-3123-8A63-5A434FE391ED", "2.2074"}
#endif
    };

    return uuidToVersionName;
}

std::string Loader::Impl::getGameVersion() {
    NSBundle* mainBundle = [NSBundle mainBundle];
    NSDictionary* infoDictionary = [mainBundle infoDictionary];

    NSString *version = infoDictionary[@"CFBundleShortVersionString"];

    auto versionStr = std::string([version UTF8String]);

    if (gameVersionIsAmbiguous(versionStr)) {
        static std::string manualVersionStr = []() -> std::string {
            auto uuid = getBinaryUUID();
            if (uuid.empty()) {
                // i wonder if this might cause issues
                return {};
            }

            auto uuidStr = fmt::format("{:02X}{:02X}{:02X}{:02X}-{:02X}{:02X}-{:02X}{:02X}-{:02X}{:02X}-{:02X}{:02X}{:02X}{:02X}{:02X}{:02X}",
                uuid[0], uuid[1], uuid[2], uuid[3], uuid[4], uuid[5], uuid[6], uuid[7],
                uuid[8], uuid[9], uuid[10], uuid[11], uuid[12], uuid[13], uuid[14], uuid[15]
            );

            auto& versionMap = uuidToVersionMap();
            auto versionElem = versionMap.find(uuidStr);
            if (versionElem != versionMap.end()) {
                return versionElem->second;
            }

            // return the uuid as a fallback
            // in this situation, geode (and any mods) won't load, so it's not really a big deal
            return uuidStr;
        }();

        return manualVersionStr;
    }

    return versionStr;
}

// TODO
bool Loader::Impl::supportsLaunchArguments() const {
    return false;
}

std::string Loader::Impl::getLaunchCommand() const {
    return std::string(); // Empty
}
