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


#include <jni.h>
#include <Geode/cocos/platform/android/jni/JniHelper.h>

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

// bool crashlog::setupPlatformHandler() {
//     auto pidFile = crashlog::getCrashLogDirectory() / "last-pid";

//     int lastPid = 0;

//     if (ghc::filesystem::exists(pidFile)) {

//         auto res = file::readString(pidFile);
//         if (!res) {
//             log::warn("Failed to read last-pid file: {}", res.error());
//         }
//         else {
//             lastPid = std::stoi(res.unwrap());
//         }

//         std::error_code ec;
//         ghc::filesystem::remove(pidFile, ec);

//         if (ec) {
//             log::warn("Failed to remove last-pid file: {}", ec.message());
//         }
//     }

//     auto res = file::writeString(pidFile, std::to_string(getpid()));
//     if (!res) {
//         log::warn("Failed to write last-pid file: {}", res.error());
//     }

//     lastPid = 1513;

    
//     if (lastPid == 0) {
//         return true;
//     }

//     // TODO: get logcat crash

//     std::string logcatCrash = R"RAW()RAW";

//     std::string crashTrace;
//     auto findLast = logcatCrash.find_last_of(fmt::format("pid {} (.geode.launcher)", lastPid));
//     if (findLast != std::string::npos) {
//         auto begin = logcatCrash.substr(0, findLast).find_last_of("F/libc");
//         if (begin != std::string::npos) {
//             crashTrace = logcatCrash.substr(begin);
//         }
//     }
//     else {
//         return true;
//     }

//     auto text = crashlog::writeCrashlog(nullptr, "", crashTrace, "");
//     s_lastLaunchCrashed = true;
	
//     auto lastCrashedFile = crashlog::getCrashLogDirectory() / "last-crashed";
//     if (ghc::filesystem::exists(lastCrashedFile)) {
//         std::error_code ec;
//         ghc::filesystem::remove(lastCrashedFile, ec);

//         if (ec) {
//             log::warn("Failed to remove last-crashed file: {}", ec.message());
//         }
//     }
//     return true;
// }

// bool crashlog::didLastLaunchCrash() {
//     return s_lastLaunchCrashed;
// }

// ghc::filesystem::path crashlog::getCrashLogDirectory() {
//     return dirs::getGeodeDir() / "crashlogs";
// }

ghc::filesystem::path crashlog::getCrashLogDirectory() {
    return geode::dirs::getSaveDir();
}

bool crashlog::setupPlatformHandler() {
    auto path = crashlog::getCrashLogDirectory() / (getDateString(true) + ".log");

    JniMethodInfo t;
    if (JniHelper::getStaticMethodInfo(t, "com/geode/launcher/utils/GeodeUtils", "writeLogcatCrashBuffer", "(Ljava/lang/String;)Z")) {
        jstring stringArg1 = t.env->NewStringUTF(path.string().c_str());

        jboolean result = t.env->CallStaticBooleanMethod(t.classID, t.methodID, stringArg1);

        t.env->DeleteLocalRef(stringArg1);
        t.env->DeleteLocalRef(t.classID);
        return result;
    }

    return false;
}

bool crashlog::didLastLaunchCrash() {
    return false;
}

#endif
