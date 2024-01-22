#include <crashlog.hpp>

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

static std::string getImageName(void const* image) {
    if (image == nullptr) {
        return "<Unknown>";
    }
    std::string imageName;// = image->imageFilePath;
    if (imageName.empty()) {
        imageName = "<Unknown>";
    }
    return imageName;
}

static void const* imageFromAddress(void const* addr) {
    if (addr == nullptr) {
        return nullptr;
    }
    return nullptr;
}

static Mod* modFromAddress(void const* addr) {
    if (addr == nullptr) {
        return nullptr;
    }
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
    }

    free(messages);

    return stacktrace.str();
}

static std::string getRegisters() {
    std::stringstream registers;

    return registers.str();
}

static void handlerThread() {
    std::unique_lock<std::mutex> lock(s_mutex);
    s_cv.wait(lock, [] { return s_signal != 0; });

    auto signalAddress = reinterpret_cast<void*>(s_context->uc_mcontext.fault_address);
    Mod* faultyMod = modFromAddress(signalAddress);

    auto text = crashlog::writeCrashlog(faultyMod, getInfo(signalAddress, faultyMod), getStacktrace(), getRegisters());

    log::error("Geode crashed!\n{}", text);
    
    s_signal = 0;
    s_cv.notify_all();

    log::debug("Notified");
}

static bool s_lastLaunchCrashed = false;

ghc::filesystem::path crashlog::getCrashLogDirectory() {
    return dirs::getGeodeDir() / "crashlogs";
}

int writeAndGetPid() {
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

    return lastPid;
}

void printModsAndroid(std::stringstream& stream) {
    auto mods = Loader::get()->getAllMods();
    if (mods.empty()) {
        stream << "<None>\n";
    }
    using namespace std::string_view_literals;
    for (auto& mod : mods) {
        stream << fmt::format("{} | [{}] {}\n",
            mod->shouldLoad() ? "x"sv : " "sv,
            mod->getVersion().toString(), mod->getID()
        );
    }
}

void printMemoryMappings(std::stringstream& stream) {
    std::ifstream processMappings("/proc/self/maps");

    std::string mapping;
    while (std::getline(processMappings, mapping)) {
        stream << mapping << "\n";
    }
}

static std::string s_result;
bool crashlog::setupPlatformHandler() {
    (void)utils::file::createDirectoryAll(crashlog::getCrashLogDirectory());
    
    JniMethodInfo t;
    
    if (JniHelper::getStaticMethodInfo(t, "com/geode/launcher/utils/GeodeUtils", "getLogcatCrashBuffer", "()Ljava/lang/String;")) {
        jstring stringResult = (jstring)t.env->CallStaticObjectMethod(t.classID, t.methodID);

        s_result = JniHelper::jstring2string(stringResult);

        t.env->DeleteLocalRef(stringResult);
        t.env->DeleteLocalRef(t.classID);

        if (s_result.empty()) {
            return false;
        }
    }
    else return false;

    auto lastPid = writeAndGetPid();

    auto index = s_result.rfind(fmt::format("pid {}", lastPid));
    if (index != std::string::npos) {
        auto begin = s_result.substr(0, index).rfind("F/libc");
        if (begin != std::string::npos) {
            s_result = s_result.substr(begin);
        }
        else {
            s_result = "Logcat buffer is empty.";
        }
        s_lastLaunchCrashed = true;
    }
    else {
        s_result = "Logcat buffer is empty.";
    }

    return true;
}

void crashlog::setupPlatformHandlerPost() {
    if (s_result.empty()) return;

    std::stringstream ss;
    ss << "Geode crashed!\n";
    ss << "Please submit this crash report to the developer of the mod that caused it.\n";
    ss << "\n== Geode Information ==\n";
    crashlog::printGeodeInfo(ss);

    ss << "\n== Installed Mods ==\n";
    printModsAndroid(ss);

    ss << "\n== Crash Report (Logcat) ==\n";
    ss << s_result;

    ss << "\n== Process Mapping ==\n";
    printMemoryMappings(ss);

    std::ofstream actualFile;
    actualFile.open(
        crashlog::getCrashLogDirectory() / (crashlog::getDateString(true) + ".log"), std::ios::app
    );
    actualFile << ss.rdbuf() << std::flush;
    actualFile.close();
}

bool crashlog::didLastLaunchCrash() {
    return s_lastLaunchCrashed;
}
