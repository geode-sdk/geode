// this is mostly copied from macos
#include <crashlog.hpp>
#include <crashlogUnix.hpp>

#include <Geode/utils/string.hpp>
#include <Geode/utils/ZStringView.hpp>
#include <thread>
#include <dlfcn.h>
#include <cxxabi.h>

#include <sys/ucontext.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <link.h>
#include <unwind.h>
#include <unwindstack/Maps.h>
#include <unwindstack/Unwinder.h>

using namespace geode::prelude;
using namespace crashlog;

// https://gist.github.com/jvranish/4441299

static constexpr auto crashIndicatorFilename = "lastSessionDidCrash";

static int s_signal = 0;
static std::atomic<int> s_reentrancy{0};
static std::atomic<bool> s_skipSymbols{false};
static siginfo_t* s_siginfo = nullptr;
static ucontext_t* s_context = nullptr;
static crashlog::CrashContext g_context;
static int s_pipe[2];

static constexpr auto CURRENT_ARCH =
    GEODE_ANDROID32(unwindstack::ARCH_ARM)
    GEODE_ANDROID64(unwindstack::ARCH_ARM64);

std::vector<Image> CrashContext::getImages() {
    std::vector<Image> images;

    dl_iterate_phdr([](struct dl_phdr_info* info, size_t size, void* data) {
        auto& images = *reinterpret_cast<std::vector<Image>*>(data);

        uintptr_t lowAddr = (uintptr_t)-1;
        uintptr_t highAddr = 0;

        for (size_t i = 0; i < info->dlpi_phnum; i++) {
            if (info->dlpi_phdr[i].p_type == PT_LOAD) {
                uintptr_t start = info->dlpi_phdr[i].p_vaddr;
                uintptr_t end = start + info->dlpi_phdr[i].p_memsz;


                lowAddr = std::min(lowAddr, start);
                highAddr = std::max(highAddr, end);
            }
        }


        images.push_back({
            info->dlpi_addr,
            info->dlpi_name ? info->dlpi_name : "<Unknown>",
            highAddr - lowAddr
        });

        return 0;
    }, &images);

    return images;
}

std::vector<StackFrame> CrashContext::getStacktrace() {
    std::vector<StackFrame> frames;

    auto mem = unwindstack::Memory::CreateProcessMemoryCached(getpid());
    auto maps = std::make_unique<unwindstack::LocalMaps>();
    if (!maps->Parse()) {
        __android_log_print(ANDROID_LOG_ERROR, "Geode", "Failed to parse memory maps for unwinder");
        return {};
    }

    unwindstack::UnwinderFromPid unwinder(
        MAX_FRAMES,
        getpid(),
        CURRENT_ARCH,
        maps.get(),
        mem
    );
    unwinder.SetRegs(unwindstack::Regs::CreateFromUcontext(CURRENT_ARCH, s_context));

    unwinder.SetResolveNames(!s_skipSymbols);
    unwinder.Unwind();

    for (int i = 0; i < unwinder.NumFrames(); ++i) {
        __android_log_print(ANDROID_LOG_DEBUG, "Geode", "Unwinder frame %d: %s", i, unwinder.FormatFrame(i).c_str());

        auto& frame = unwinder.frames()[i];
        std::string symbol;
        uintptr_t offset = 0;

        if (!frame.function_name.empty()) {
            offset = frame.function_offset;

            int status;
            auto demangle = abi::__cxa_demangle(frame.function_name.c_str(), 0, 0, &status);
            if (status == 0) {
                symbol = demangle;
            } else {
                symbol = frame.function_name;
            }
            free(demangle);
        }

        frames.push_back({
            .address = (uintptr_t)frame.pc,
            .image = frame.map_info ? g_context.imageFromAddress((void*)frame.pc) : nullptr,
            .symbol = std::move(symbol),
            .offset = offset
        });
    }

    return frames;
}

std::vector<Register> CrashContext::getRegisters() {
    std::vector<Register> registers;
    auto& ss = s_context->uc_mcontext;

#ifdef GEODE_IS_ANDROID64
    registers.push_back({ "x0", ss.regs[0] });
    registers.push_back({ "x1", ss.regs[1] });
    registers.push_back({ "x2", ss.regs[2] });
    registers.push_back({ "x3", ss.regs[3] });
    registers.push_back({ "x4", ss.regs[4] });
    registers.push_back({ "x5", ss.regs[5] });
    registers.push_back({ "x6", ss.regs[6] });
    registers.push_back({ "x7", ss.regs[7] });
    registers.push_back({ "x8", ss.regs[8] });
    registers.push_back({ "x9", ss.regs[9] });
    registers.push_back({ "x10", ss.regs[10] });
    registers.push_back({ "x11", ss.regs[11] });
    registers.push_back({ "x12", ss.regs[12] });
    registers.push_back({ "x13", ss.regs[13] });
    registers.push_back({ "x14", ss.regs[14] });
    registers.push_back({ "x15", ss.regs[15] });
    registers.push_back({ "x16", ss.regs[16] });
    registers.push_back({ "x17", ss.regs[17] });
    registers.push_back({ "x18", ss.regs[18] });
    registers.push_back({ "x19", ss.regs[19] });
    registers.push_back({ "x20", ss.regs[20] });
    registers.push_back({ "x21", ss.regs[21] });
    registers.push_back({ "x22", ss.regs[22] });
    registers.push_back({ "x23", ss.regs[23] });
    registers.push_back({ "x24", ss.regs[24] });
    registers.push_back({ "x25", ss.regs[25] });
    registers.push_back({ "x26", ss.regs[26] });
    registers.push_back({ "x27", ss.regs[27] });
    registers.push_back({ "x28", ss.regs[28] });
    registers.push_back({ "fp", ss.regs[29] });
    registers.push_back({ "lr", ss.regs[30] });
    registers.push_back({ "sp", ss.sp });
    registers.push_back({ "pc", ss.pc });
    registers.push_back({ "cpsr", ss.pstate });
#else
    registers.push_back({ "r0", ss.arm_r0 });
    registers.push_back({ "r1", ss.arm_r1 });
    registers.push_back({ "r2", ss.arm_r2 });
    registers.push_back({ "r3", ss.arm_r3 });
    registers.push_back({ "r4", ss.arm_r4 });
    registers.push_back({ "r5", ss.arm_r5 });
    registers.push_back({ "r6", ss.arm_r6 });
    registers.push_back({ "r7", ss.arm_r7 });
    registers.push_back({ "r8", ss.arm_r8 });
    registers.push_back({ "r9", ss.arm_r9 });
    registers.push_back({ "r10", ss.arm_r10 });
    registers.push_back({ "r11", ss.arm_fp });
    registers.push_back({ "r12", ss.arm_ip });
    registers.push_back({ "sp", ss.arm_sp });
    registers.push_back({ "lr", ss.arm_lr });
    registers.push_back({ "pc", ss.arm_pc });
    registers.push_back({ "cpsr", ss.arm_cpsr });
#endif
    return registers;
}

std::string_view CrashContext::getGeodeBinaryName() {
    return "Geode.so";
}

void CrashContext::writeInfo(Buffer& stream) {
    auto image = this->imageFromAddress(crashAddr);

    stream.append("Faulty Lib: {}\n", image ? image->name() : "<Unknown>");
    stream.append("Faulty Mod: {}\n", faultyMod ? faultyMod->getID() : "<Unknown>");
    stream.append("Instruction Address: ");
    this->formatAddress(crashAddr, infoStream);

    stream.append("\nSignal Code: 0x{:x} ({})\n", s_siginfo->si_signo, getSignalCodeString(s_signal, s_siginfo));

    if (hasSignalDetail(s_signal)) {
        stream.append("Signal Detail: ");
        writeSignalDetail(stream, g_context, s_signal, s_siginfo);
        stream.append('\n');
    }
}

extern "C" void signalHandler(int signal, siginfo_t* signalInfo, void* vcontext) {
    __android_log_print(ANDROID_LOG_ERROR, "Geode", "Signal handler called with signal %d", signal);

    int reentrancy = s_reentrancy.fetch_add(1) + 1;
    if (reentrancy > 3) {
        // crash handler crashed 3 times, give up and die
        __android_log_print(ANDROID_LOG_ERROR, "Geode", "Signal handler reentrancy level %d, giving up", reentrancy);
        std::_Exit(EXIT_FAILURE);
    }

    // if the crash handler itself has already crashed processing its own crash, skip extra processing to try and avoid another crash
    s_skipSymbols = reentrancy > 2;

    s_signal = signal;
    s_siginfo = signalInfo;
    s_context = reinterpret_cast<ucontext_t*>(vcontext);

    write(s_pipe[1], "1", 1);

    // wait for the death to come
    while (true) {
        usleep(100000);
    }
}

static void handlerThread() {
    // no more mutex deadlocker
    char buf;
    while (read(s_pipe[0], &buf, 1) != 0) {
#ifdef GEODE_IS_ANDROID64
        auto signalAddress = reinterpret_cast<void*>(s_context->uc_mcontext.pc);
#else
        auto signalAddress = reinterpret_cast<void*>(s_context->uc_mcontext.arm_pc);
#endif
        g_context.initialize(signalAddress);

        auto text = crashlog::writeCrashlog(g_context);
        log::error("Geode crashed!\n{}", text);
        std::_Exit(EXIT_FAILURE);
        //s_signal = 0;
    }
}

static bool s_lastLaunchCrashed;

bool crashlog::setupPlatformHandler() {
    // for whatever reason, i can't just do int*
    if (pipe(s_pipe) != 0) return false;
    fcntl(s_pipe[0], F_SETFD, FD_CLOEXEC);
    fcntl(s_pipe[1], F_SETFD, FD_CLOEXEC);
    struct sigaction action;
    action.sa_sigaction = &signalHandler;
    action.sa_flags = SA_SIGINFO;
    sigemptyset(&action.sa_mask);
    sigaction(SIGSEGV, &action, nullptr);
    // I'd rather not track interrupt lol
    // sigaction(SIGINT, &action, nullptr);
    sigaction(SIGFPE, &action, nullptr);
    sigaction(SIGILL, &action, nullptr);
    sigaction(SIGTERM, &action, nullptr);
    sigaction(SIGABRT, &action, nullptr);
    sigaction(SIGBUS, &action, nullptr);


    // spawn multiple to handle reentrant crashes, cases where one handler thread died
    for (size_t i = 0; i < 3; i++) {
        std::thread(&handlerThread).detach();
    }

    auto crashIndicatorPath = crashlog::getCrashLogDirectory() / crashIndicatorFilename;
    if (std::filesystem::exists(crashIndicatorPath)) {
        s_lastLaunchCrashed = true;
        std::filesystem::remove(crashIndicatorPath);
    }

    return true;
}

void crashlog::setupPlatformHandlerPost() {}

bool crashlog::didLastLaunchCrash() {
    return s_lastLaunchCrashed;
}

std::filesystem::path crashlog::getCrashLogDirectory() {
    return dirs::getGeodeDir() / "crashlogs";
}
