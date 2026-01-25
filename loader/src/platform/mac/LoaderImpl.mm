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

void console::messageBox(ZStringView title, ZStringView info, Severity) {
    CFStringRef cfTitle = CFStringCreateWithCString(NULL, title.c_str(), kCFStringEncodingUTF8);
    CFStringRef cfMessage = CFStringCreateWithBytes(NULL, (unsigned char*)info.data(), info.size(), kCFStringEncodingUTF8, false);

    CFUserNotificationDisplayNotice(
        0, kCFUserNotificationNoteAlertLevel, NULL, NULL, NULL, cfTitle, cfMessage, NULL
    );
}

void console::log(ZStringView zmsg, Severity severity) {
    auto msg = zmsg.view();

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

        auto handle = dlopen(utils::string::pathToString(entry_path).c_str(), RTLD_LAZY);

        if (!handle) {
            auto err = dlerror();
            log::warn("failed to load native binary at {}: dlerror returned ({})", 
                entry_path, err
            );
            continue;
        }

        dlclose(handle);
    }
}

bool gameVersionIsAmbiguous(std::string gameVersion) {
    return gameVersion == "2.207" || gameVersion == "2.208";
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
        {"620B0C9B-8F75-3043-BD34-3BB9DD201C3A", "2.206"},
        {"48C25B63-0D7C-3F67-B831-DF935524C043", "2.2070"},
        {"4933391F-D6C1-3188-99E8-23D64C674B64", "2.2071"},
        {"9C1D62A7-7C2F-3514-AEFB-D1AB7BBD48FF", "2.2072"},
        {"0B1FCFE4-79E8-3246-8ECB-500FDBDCFD9A", "2.2073"},
        {"27044C8B-76BD-303C-A035-5314AF1D9E6E", "2.2074"},
        {"9E958BAE-B00D-37B1-A75E-6195B0FCAAFD", "2.2080"},
        {"ABCA9EC8-DDEE-317A-BC32-3F41A842311D", "2.2081"}
#elif defined(GEODE_IS_INTEL_MAC)
        {"29549F90-F083-35A8-B917-9962262FE112", "2.200"},
        {"AE6DFCCC-153A-32AB-BFD5-6F2478BC41B6", "2.206"},
        {"D497E431-5C3F-3EB4-9DF7-115B861578EE", "2.2070"},
        {"08E24832-EC11-3637-910E-7CB6C0EF8EC0", "2.2071"},
        {"E53731FD-D1B6-33D2-BFA4-3B5D8D55279F", "2.2072"},
        {"1F4AFF98-DB51-382D-9BB2-59C911B88EB2", "2.2073"},
        {"DB5CADC0-E533-3123-8A63-5A434FE391ED", "2.2074"},
        {"45A69FE9-4478-3D26-B75D-59D07E9261AF", "2.2080"},
        {"3FBF9F72-5A83-3918-A82A-37BD3C019D7A", "2.2081"}
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

bool Loader::Impl::supportsLaunchArguments() const {
    return true;
}

std::string Loader::Impl::getLaunchCommand() const {
    NSArray* arguments = [[NSProcessInfo processInfo] arguments];
    NSString* joinedString = [arguments componentsJoinedByString:@" "];
    std::string fullString([joinedString UTF8String]);
    return fullString;
}

bool Loader::Impl::isModVersionSupported(VersionInfo const& target) {
    return semverCompare(this->getVersion(), target);
}

bool Loader::Impl::isForwardCompatMode() {
    return false;
}