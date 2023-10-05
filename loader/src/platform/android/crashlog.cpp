#include <crashlog.hpp>

#ifdef GEODE_IS_ANDROID

using namespace geode::prelude;

#include <Geode/utils/string.hpp>
#include <array>
#include <thread>
#include <ghc/fs_fwd.hpp>
#include <execinfo.h>
#include <dlfcn.h>
#include <cxxabi.h>
#include <algorithm>
#include <link.h>
#include <unwind.h>

#include "backtrace/execinfo.hpp"

static constexpr size_t FRAME_SIZE = 64;
static std::mutex s_mutex;
static std::condition_variable s_cv;
static int s_signal = 0;
static siginfo_t* s_siginfo = nullptr;
static ucontext_t* s_context = nullptr;
static size_t s_backtraceSize = 0;
static std::array<void*, FRAME_SIZE> s_backtrace;

static std::string_view getSignalCodeString() {
    switch(s_signal) {
        case SIGSEGV: return "SIGSEGV: Segmentation Fault";
        case SIGINT: return "SIGINT: Interactive attention signal, (usually ctrl+c)";
        case SIGFPE:
            switch(s_siginfo->si_code) {
                case FPE_INTDIV: return "SIGFPE: (integer divide by zero)";
                case FPE_INTOVF: return "SIGFPE: (integer overflow)";
                case FPE_FLTDIV: return "SIGFPE: (floating-point divide by zero)";
                case FPE_FLTOVF: return "SIGFPE: (floating-point overflow)";
                case FPE_FLTUND: return "SIGFPE: (floating-point underflow)";
                case FPE_FLTRES: return "SIGFPE: (floating-point inexact result)";
                case FPE_FLTINV: return "SIGFPE: (floating-point invalid operation)";
                case FPE_FLTSUB: return "SIGFPE: (subscript out of range)";
                default: return "SIGFPE: Arithmetic Exception";
            }
        case SIGILL:
            switch(s_siginfo->si_code) {
                case ILL_ILLOPC: return "SIGILL: (illegal opcode)";
                case ILL_ILLOPN: return "SIGILL: (illegal operand)";
                case ILL_ILLADR: return "SIGILL: (illegal addressing mode)";
                case ILL_ILLTRP: return "SIGILL: (illegal trap)";
                case ILL_PRVOPC: return "SIGILL: (privileged opcode)";
                case ILL_PRVREG: return "SIGILL: (privileged register)";
                case ILL_COPROC: return "SIGILL: (coprocessor error)";
                case ILL_BADSTK: return "SIGILL: (internal stack error)";
                default: return "SIGILL: Illegal Instruction";
            }
        case SIGTERM: return "SIGTERM: a termination request was sent to the program";
        case SIGABRT: return "SIGABRT: usually caused by an abort() or assert()";
        case SIGBUS: return "SIGBUS: Bus error (bad memory access)";
        default: return "Unknown signal code";
    }
}

static std::string getImageName(Elf32_Phdr const* image) {
    if (image == nullptr) {
        return "<Unknown>";
    }
    std::string imageName;// = image->imageFilePath;
    if (imageName.empty()) {
        imageName = "<Unknown>";
    }
    return imageName;
}

// static std::vector<struct dyld_image_info const*> getAllImages() {
//     std::vector<struct dyld_image_info const*> images;
//     struct task_dyld_info dyldInfo;
//     mach_msg_type_number_t count = TASK_DYLD_INFO_COUNT;
//     if (task_info(mach_task_self(), TASK_DYLD_INFO, (task_info_t)&dyldInfo, &count) == KERN_SUCCESS) {
//         struct dyld_all_image_infos* imageInfos = (struct dyld_all_image_infos*)dyldInfo.all_image_info_addr;

//         for (size_t i = 0; i < imageInfos->infoArrayCount; ++i) {
//             images.push_back(&imageInfos->infoArray[i]);
//         }
//     }

//     return images;
// }

static Elf32_Phdr const* imageFromAddress(void const* addr) {
    if (addr == nullptr) {
        return nullptr;
    }

    // auto loadedImages = getAllImages();
    // std::sort(loadedImages.begin(), loadedImages.end(), [](auto const a, auto const b) {
    //     return (uintptr_t)a->imageLoadAddress < (uintptr_t)b->imageLoadAddress;
    // });
    // auto iter = std::upper_bound(loadedImages.begin(), loadedImages.end(), addr, [](auto const addr, auto const image) {
    //     return (uintptr_t)addr < (uintptr_t)image->imageLoadAddress;
    // });

    // if (iter == loadedImages.begin()) {
    //     return nullptr;
    // }
    // --iter;

    // auto image = *iter;
    // // auto imageSize = getImageSize((struct mach_header_64 const*)image->imageLoadAddress);
    // auto imageAddress = (uintptr_t)image->imageLoadAddress;
    // if ((uintptr_t)addr >= imageAddress/* && (uintptr_t)addr < imageAddress + imageSize*/) {
    //     return image;
    // }
    return nullptr;
}

static Mod* modFromAddress(void const* addr) {
    if (addr == nullptr) {
        return nullptr;
    }
    // auto image = imageFromAddress(addr);
    // if (image == nullptr) {
    //     return nullptr;
    // }

    // ghc::filesystem::path imagePath = getImageName(image);
    // if (!ghc::filesystem::exists(imagePath)) {
    //     return nullptr;
    // }
    // auto geodePath = dirs::getGameDir() / "Frameworks" / "Geode.dylib";
    // if (ghc::filesystem::equivalent(imagePath, geodePath)) {
    //     return Mod::get();
    // }

    // for (auto& mod : Loader::get()->getAllMods()) {
    //     if (!mod->isEnabled() || !ghc::filesystem::exists(mod->getBinaryPath())) {
    //         continue;
    //     }
    //     if (ghc::filesystem::equivalent(imagePath, mod->getBinaryPath())) {
    //         return mod;
    //     }
    // }
    return nullptr;
}

static std::string getInfo(void* address, Mod* faultyMod) {
    std::stringstream stream;
    stream << "Faulty Lib: " << getImageName(imageFromAddress(address)) << "\n";
    stream << "Faulty Mod: " << (faultyMod ? faultyMod->getID() : "<Unknown>") << "\n";
    stream << "Instruction Address: " << address << "\n";
    stream << "Signal Code: " << std::hex << s_signal << " (" << getSignalCodeString() << ")" << std::dec << "\n";
    return stream.str();
}

static struct sigaction oldActionSEGV;
static struct sigaction oldActionBUS;
static struct sigaction oldActionILL;

extern "C" void signalHandler(int signal, siginfo_t* signalInfo, void* vcontext) {
    auto context = reinterpret_cast<ucontext_t*>(vcontext);
    s_backtraceSize = 0;
    // s_backtraceSize = backtrace(s_backtrace.data(), FRAME_SIZE);

    // for some reason this is needed, dont ask me why
    // s_backtrace[1] = reinterpret_cast<void*>(context->uc_mcontext.fault_address);
    // if (s_backtraceSize < FRAME_SIZE) {
    //     s_backtrace[s_backtraceSize] = nullptr;
    // }

    {
        std::unique_lock<std::mutex> lock(s_mutex);
        s_signal = signal;
        s_siginfo = signalInfo;
        s_context = context;
    }

    s_cv.notify_all();
    std::unique_lock<std::mutex> lock(s_mutex);
    s_cv.wait(lock, [] { return s_signal == 0; });
    // std::_Exit(EXIT_FAILURE);

    switch (signal) {
        case SIGSEGV:
            sigaction(signal, &oldActionSEGV, nullptr);
            oldActionSEGV.sa_sigaction(signal, signalInfo, vcontext);
            break;
        case SIGBUS:
            sigaction(signal, &oldActionBUS, nullptr);
            oldActionBUS.sa_sigaction(signal, signalInfo, vcontext);
            break;
        case SIGILL:
            sigaction(signal, &oldActionILL, nullptr);
            oldActionILL.sa_sigaction(signal, signalInfo, vcontext);
            break;
    }
}

static std::string getStacktrace() {
    std::stringstream stacktrace;

    if (s_backtraceSize == 0) {
        return stacktrace.str();
    }

    auto messages = backtrace_symbols(s_backtrace.data(), s_backtraceSize);
	if (s_backtraceSize < FRAME_SIZE) {
		messages[s_backtraceSize] = nullptr;
	}


    for (int i = 1; i < s_backtraceSize; ++i) {
        auto message = std::string(messages[i]);

        // TODO: parse the message
        stacktrace << message << "\n";


        // auto stream = std::stringstream(message);
        // int index;
        // std::string binary;
        // uintptr_t address;
        // std::string function;
        // uintptr_t offset;
        // std::string line;

        // stream >> index;

        // if (!lines.eof()) {
        //     std::getline(lines, line);
        // }
        // std::getline(stream, binary);
        // auto cutoff = binary.find("0x");
        // stream = std::stringstream(binary.substr(cutoff));
        // binary = geode::utils::string::trim(binary.substr(0, cutoff));
        // stream >> std::hex >> address >> std::dec;

        // if (!line.empty()) {
        //     // log::debug("address: {}", address);
        //     auto image = imageFromAddress(reinterpret_cast<void*>(address));
        //     // log::debug("image: {}", image);
        //     stacktrace << " - " << std::showbase << std::hex;

        //     if (image) {
        //         auto baseAddress = image->imageLoadAddress;
        //         auto imageName = getImageName(image);
        //         stacktrace << imageName << " + " << (address - (uintptr_t)baseAddress);   
        //     }
        //     else {
        //         stacktrace << address;
        //     }
        //     stacktrace << std::dec;
        //     stacktrace << ": " << line << "\n";
        // }
        // else {
        //     std::getline(stream, function);
        //     cutoff = function.find("+");
        //     stream = std::stringstream(function.substr(cutoff));
        //     stream >> offset;
        //     function = geode::utils::string::trim(function.substr(0, cutoff));

        //     {
        //         int status;
        //         auto demangle = abi::__cxa_demangle(function.c_str(), 0, 0, &status);
        //         if (status == 0) {
        //             function = demangle;
        //         }
        //         free(demangle);
        //     }
            
        //     stacktrace << "- " << binary;
        //     stacktrace << " @ " << std::showbase << std::hex << address << std::dec;
        //     stacktrace << " (" << function << " + " << offset << ")\n";
        // }
    }

    free(messages);

    return stacktrace.str();
}

static std::string getRegisters() {
    std::stringstream registers;

    auto context = s_context;
    auto& ctx = context->uc_mcontext;

    // geez
    registers << std::showbase << std::hex /*<< std::setfill('0') << std::setw(16) */;
    registers << "r0: " << ctx.arm_r0 << "\n";
    registers << "r1: " << ctx.arm_r1 << "\n";
    registers << "r2: " << ctx.arm_r2 << "\n";
    registers << "r3: " << ctx.arm_r3 << "\n";
    registers << "r4: " << ctx.arm_r4 << "\n";
    registers << "r5: " << ctx.arm_r5 << "\n";
    registers << "r6: " << ctx.arm_r6 << "\n";
    registers << "r7: " << ctx.arm_r7 << "\n";
    registers << "r8: " << ctx.arm_r8 << "\n";
    registers << "r9: " << ctx.arm_r9 << "\n";
    registers << "r10: " << ctx.arm_r10 << "\n";
    registers << "r11: " << ctx.arm_fp << "\n";
    registers << "r12: " << ctx.arm_ip << "\n";
    registers << "sp: " << ctx.arm_sp << "\n";
    registers << "lr: " << ctx.arm_lr << "\n";
    registers << "pc: " << ctx.arm_pc << "\n";
    registers << "cpsr: " << ctx.arm_cpsr << "\n";

    return registers.str();
}

static void handlerThread() {
    std::unique_lock<std::mutex> lock(s_mutex);
    s_cv.wait(lock, [] { return s_signal != 0; });

    auto signalAddress = reinterpret_cast<void*>(s_context->uc_mcontext.fault_address);
    // Mod* faultyMod = nullptr;
    // for (int i = 1; i < s_backtraceSize; ++i) {
    //     auto mod = modFromAddress(s_backtrace[i]);
    //     if (mod != nullptr) {
    //         faultyMod = mod;
    //         break;
    //     }
    // }
    Mod* faultyMod = modFromAddress(signalAddress);

    auto text = crashlog::writeCrashlog(faultyMod, getInfo(signalAddress, faultyMod), getStacktrace(), getRegisters());

    log::error("Geode crashed!\n{}", text);
    
    s_signal = 0;
    s_cv.notify_all();

    log::debug("Notified");
}

static bool s_lastLaunchCrashed;

bool crashlog::setupPlatformHandler() {
    auto pidFile = crashlog::getCrashLogDirectory() / "last-pid";

    int lastPid = 0;

    if (ghc::filesystem::exists(pidFile)) {

        auto res = file::readString(pidFile);
        if (!res) {
            log::warn("Failed to read last-pid file: {}", res.error());
        }
        else {
            lastPid = std::stoi(res.unwrap());
        }

        std::error_code ec;
        ghc::filesystem::remove(pidFile, ec);

        if (ec) {
            log::warn("Failed to remove last-pid file: {}", ec.message());
        }
    }

    auto res = file::writeString(pidFile, std::to_string(getpid()));
    if (!res) {
        log::warn("Failed to write last-pid file: {}", res.error());
    }

    lastPid = 1513;

    
    if (lastPid == 0) {
        return true;
    }

    // TODO: get logcat crash

    std::string logcatCrash = R"RAW(
F/libc    (31506): Fatal signal 11 (SIGSEGV), code 1 (SEGV_MAPERR), fault addr 0xccb00000 in tid 31584 (GLThread 215445), pid 31506 (.geode.launcher)
F/DEBUG   (31618): *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** ***
F/DEBUG   (31618): Build fingerprint: 'samsung/j7y17ltexx/j7y17lte:9/PPR1.180610.011/J730FXWU8CUG1:user/release-keys'
F/DEBUG   (31618): Revision: '7'
F/DEBUG   (31618): ABI: 'arm'
F/DEBUG   (31618): pid: 31506, tid: 31584, name: GLThread 215445  >>> com.geode.launcher <<<
F/DEBUG   (31618): signal 11 (SIGSEGV), code 1 (SEGV_MAPERR), fault addr 0xccb00000
F/DEBUG   (31618):     r0  ccafffe0  r1  ccafffe4  r2  ffefb73c  r3  00000000
F/DEBUG   (31618):     r4  00000000  r5  c7e4af50  r6  c7e49400  r7  3c888889
F/DEBUG   (31618):     r8  c2dcf000  r9  cf8a7000  r10 cd1c6800  r11 cf8a7000
F/DEBUG   (31618):     ip  eff2b5f0  sp  cd1c6680  lr  c921095d  pc  efe9ef2c
F/DEBUG   (31618): 
F/DEBUG   (31618): backtrace:
F/DEBUG   (31618):     #00 pc 00019f2c  /system/lib/libc.so (memcpy+96)
F/DEBUG   (31618):     #01 pc 00336959  /data/app/com.robtopx.geometryjump-xzzAmj1_bAwPLWKGJk5srA==/lib/arm/libcocos2dcpp.so (offset 0x336000) (GJEffectManager::updateSpawnTriggers(float)+64)
F/DEBUG   (31618):     #02 pc 0021b279  /data/app/com.robtopx.geometryjump-xzzAmj1_bAwPLWKGJk5srA==/lib/arm/libcocos2dcpp.so (offset 0x219000) (PlayLayer::update(float)+480)
F/DEBUG   (31618):     #03 pc 00000617  <anonymous:ee06a000>
F/libc    (  836): Fatal signal 11 (SIGSEGV), code 1 (SEGV_MAPERR), fault addr 0xcd680000 in tid 901 (GLThread 215582), pid 836 (.geode.launcher)
F/DEBUG   (  933): *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** ***
F/DEBUG   (  933): Build fingerprint: 'samsung/j7y17ltexx/j7y17lte:9/PPR1.180610.011/J730FXWU8CUG1:user/release-keys'
F/DEBUG   (  933): Revision: '7'
F/DEBUG   (  933): ABI: 'arm'
F/DEBUG   (  933): pid: 836, tid: 901, name: GLThread 215582  >>> com.geode.launcher <<<
F/DEBUG   (  933): signal 11 (SIGSEGV), code 1 (SEGV_MAPERR), fault addr 0xcd680000
F/DEBUG   (  933):     r0  cd67ffd0  r1  cd67fff4  r2  ffe935fc  r3  00000000
F/DEBUG   (  933):     r4  00000000  r5  cc6fa350  r6  cc6f8800  r7  3c888889
F/DEBUG   (  933):     r8  c2990300  r9  ed1e0600  r10 caec3800  r11 ed1e0600
F/DEBUG   (  933):     ip  eff2b5f0  sp  caec3680  lr  c94fe95d  pc  efe9ef30
F/DEBUG   (  933): 
F/DEBUG   (  933): backtrace:
F/DEBUG   (  933):     #00 pc 00019f30  /system/lib/libc.so (memcpy+100)
F/DEBUG   (  933):     #01 pc 00336959  /data/app/com.robtopx.geometryjump-xzzAmj1_bAwPLWKGJk5srA==/lib/arm/libcocos2dcpp.so (offset 0x336000) (GJEffectManager::updateSpawnTriggers(float)+64)
F/DEBUG   (  933):     #02 pc 0021b279  /data/app/com.robtopx.geometryjump-xzzAmj1_bAwPLWKGJk5srA==/lib/arm/libcocos2dcpp.so (offset 0x219000) (PlayLayer::update(float)+480)
F/DEBUG   (  933):     #03 pc 00000617  <anonymous:ee06a000>
F/libc    ( 1071): Fatal signal 11 (SIGSEGV), code 1 (SEGV_MAPERR), fault addr 0xc9c00000 in tid 1228 (GLThread 215607), pid 1071 (.geode.launcher)
F/DEBUG   ( 1377): *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** ***
F/DEBUG   ( 1377): Build fingerprint: 'samsung/j7y17ltexx/j7y17lte:9/PPR1.180610.011/J730FXWU8CUG1:user/release-keys'
F/DEBUG   ( 1377): Revision: '7'
F/DEBUG   ( 1377): ABI: 'arm'
F/DEBUG   ( 1377): pid: 1071, tid: 1228, name: GLThread 215607  >>> com.geode.launcher <<<
F/DEBUG   ( 1377): signal 11 (SIGSEGV), code 1 (SEGV_MAPERR), fault addr 0xc9c00000
F/DEBUG   ( 1377):     r0  c9bfffe0  r1  c9bfffe4  r2  ffb862fc  r3  00000000
F/DEBUG   ( 1377):     r4  00000000  r5  c1bf2b50  r6  c1bf1000  r7  3c888889
F/DEBUG   ( 1377):     r8  c7d34200  r9  ed1e6e00  r10 cd0d2800  r11 ed1e6e00
F/DEBUG   ( 1377):     ip  eff2b5f0  sp  cd0d2680  lr  c900c95d  pc  efe9ef2c
F/DEBUG   ( 1377): 
F/DEBUG   ( 1377): backtrace:
F/DEBUG   ( 1377):     #00 pc 00019f2c  /system/lib/libc.so (memcpy+96)
F/DEBUG   ( 1377):     #01 pc 00336959  /data/app/com.robtopx.geometryjump-xzzAmj1_bAwPLWKGJk5srA==/lib/arm/libcocos2dcpp.so (offset 0x336000) (GJEffectManager::updateSpawnTriggers(float)+64)
F/DEBUG   ( 1377):     #02 pc 0021b279  /data/app/com.robtopx.geometryjump-xzzAmj1_bAwPLWKGJk5srA==/lib/arm/libcocos2dcpp.so (offset 0x219000) (PlayLayer::update(float)+480)
F/DEBUG   ( 1377):     #03 pc 00000617  <anonymous:edfa0000>
F/libc    ( 1513): Fatal signal 11 (SIGSEGV), code 1 (SEGV_MAPERR), fault addr 0xc8500000 in tid 1658 (GLThread 215649), pid 1513 (.geode.launcher)
F/DEBUG   ( 1752): *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** ***
F/DEBUG   ( 1752): Build fingerprint: 'samsung/j7y17ltexx/j7y17lte:9/PPR1.180610.011/J730FXWU8CUG1:user/release-keys'
F/DEBUG   ( 1752): Revision: '7'
F/DEBUG   ( 1752): ABI: 'arm'
F/DEBUG   ( 1752): pid: 1513, tid: 1658, name: GLThread 215649  >>> com.geode.launcher <<<
F/DEBUG   ( 1752): signal 11 (SIGSEGV), code 1 (SEGV_MAPERR), fault addr 0xc8500000
F/DEBUG   ( 1752):     r0  c84fffc0  r1  c84fffe4  r2  ffb13634  r3  00000008
F/DEBUG   ( 1752):     r4  00000000  r5  c83e0750  r6  c83dec00  r7  3c888889
F/DEBUG   ( 1752):     r8  e41cad00  r9  ed9fbc00  r10 cd38f800  r11 ed9fbc00
F/DEBUG   ( 1752):     ip  00000000  sp  cd38f680  lr  c951195d  pc  efe9ef30
F/DEBUG   ( 1752): 
F/DEBUG   ( 1752): backtrace:
F/DEBUG   ( 1752):     #00 pc 00019f30  /system/lib/libc.so (memcpy+100)
F/DEBUG   ( 1752):     #01 pc 00336959  /data/app/com.robtopx.geometryjump-xzzAmj1_bAwPLWKGJk5srA==/lib/arm/libcocos2dcpp.so (offset 0x336000) (GJEffectManager::updateSpawnTriggers(float)+64)
F/DEBUG   ( 1752):     #02 pc 0021b279  /data/app/com.robtopx.geometryjump-xzzAmj1_bAwPLWKGJk5srA==/lib/arm/libcocos2dcpp.so (offset 0x219000) (PlayLayer::update(float)+480)
F/DEBUG   ( 1752):     #03 pc 00000617  <anonymous:ee06a000>
    )RAW";

    std::string crashTrace;
    auto findLast = logcatCrash.find_last_of(fmt::format("pid {} (.geode.launcher)", lastPid));
    if (findLast != std::string::npos) {
        auto begin = logcatCrash.substr(0, findLast).find_last_of("F/libc");
        if (begin != std::string::npos) {
            crashTrace = logcatCrash.substr(begin);
        }
    }
    else {
        return true;
    }

    auto text = crashlog::writeCrashlog(nullptr, "", crashTrace, "");
    s_lastLaunchCrashed = true;
	
    auto lastCrashedFile = crashlog::getCrashLogDirectory() / "last-crashed";
    if (ghc::filesystem::exists(lastCrashedFile)) {
        std::error_code ec;
        ghc::filesystem::remove(lastCrashedFile, ec);

        if (ec) {
            log::warn("Failed to remove last-crashed file: {}", ec.message());
        }
    }
    return true;
}

bool crashlog::didLastLaunchCrash() {
    return s_lastLaunchCrashed;
}

ghc::filesystem::path crashlog::getCrashLogDirectory() {
    return dirs::getGeodeDir() / "crashlogs";
}

// ghc::filesystem::path crashlog::getCrashLogDirectory() {
//     return geode::dirs::getSaveDir();
// }

// bool crashlog::setupPlatformHandler() {
//     return false;
// }

// bool crashlog::didLastLaunchCrash() {
//     return false;
// }

#endif
