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
static siginfo_t* s_siginfo = nullptr;
static ucontext_t* s_context = nullptr;
static crashlog::CrashContext g_context;
static int s_pipe[2];

std::vector<Image> CrashContext::getImages() {
    std::vector<Image> images;

    dl_iterate_phdr([](struct dl_phdr_info* info, size_t size, void* data) {
        auto& images = *reinterpret_cast<std::vector<Image>*>(data);
        images.push_back({
            .address = info->dlpi_addr,
            .name_ = info->dlpi_name ? info->dlpi_name : "<Unknown>"
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

    __android_log_print(ANDROID_LOG_DEBUG, "Geode", "Base: %p", (void*)geode::base::get());

    unwindstack::Elf* gdElf;
    for (size_t i = 0; i < maps->Total(); ++i) {
        auto map = maps->Get(i);
        std::string_view name{map->name()};
        if (name.starts_with("[") || name.starts_with("/system") || name.starts_with("/apex") || name.starts_with("/dev")) continue;

        if (name.contains("com.robtopx") && !gdElf) {
            gdElf = map->GetElf(mem, unwindstack::ARCH_ARM64);
        }

        __android_log_print(ANDROID_LOG_DEBUG, "Geode", "Map %zu: %s (%p - %p), offset %p", i, name.data(), (void*)map->start(), (void*)map->end(), (void*)map->offset_);
    }

    unwindstack::UnwinderFromPid unwinder(
        MAX_FRAMES,
        getpid(),
        unwindstack::ARCH_ARM64,
        maps.get(),
        mem
    );
    unwinder.SetRegs(unwindstack::Regs::CreateFromUcontext(unwindstack::ARCH_ARM64, s_context));

    unwinder.SetResolveNames(true);
    unwinder.Unwind();

    for (int i = 0; i < unwinder.NumFrames(); ++i) {
        __android_log_print(ANDROID_LOG_DEBUG, "Geode", "Unwinder frame %d: %s", i, unwinder.FormatFrame(i).c_str());

        auto& frame = unwinder.frames()[i];
        std::string symbol;

        if (!frame.function_name.empty()) {
            int status;
            auto demangle = abi::__cxa_demangle(frame.function_name.c_str(), 0, 0, &status);
            if (status == 0) {
                symbol = demangle;
            } else {
                symbol = frame.function_name;
            }
            free(demangle);
        } else if (frame.map_info) {
            // manual fucking
            auto elf = gdElf ?: frame.map_info->GetElf(mem, unwindstack::ARCH_ARM64);
            auto fullOffset = frame.map_info->elf_start_offset() + frame.rel_pc;

            unwindstack::SharedString name;
            uint64_t funcOffset;

            if (elf->GetFunctionName(fullOffset, &name, &funcOffset)) {
                frame.function_name = std::move(name);
            } else {
                __android_log_print(ANDROID_LOG_ERROR, "Geode", "Failed for %d", i);
            }

            __android_log_print(ANDROID_LOG_DEBUG, "Geode", "%p %p %s", (void*)frame.pc, (void*)frame.rel_pc, frame.function_name.c_str());
        }

        frames.push_back({
            .address = frame.pc,
            .image = g_context.imageFromAddress((void*)frame.pc),
            .symbol = std::move(symbol)
        });
    }

    return frames;
}

std::vector<Register> CrashContext::getRegisters() {
    std::vector<Register> registers;
    auto& ss = s_context->uc_mcontext;

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

    return registers;
}

std::string_view CrashContext::getGeodeBinaryName() {
    return "Geode.so";
}

void CrashContext::writeExtraInfo(Buffer& stream) {
    stream.append("\nSignal Code: 0x{:x} ({})\n", s_siginfo->si_code, getSignalCodeString(s_signal, s_siginfo));

    if (hasSignalDetail(s_signal)) {
        stream.append("Signal Detail: ");
        writeSignalDetail(stream, g_context, s_signal, s_siginfo);
        stream.append('\n');
    }
}

extern "C" void signalHandler(int signal, siginfo_t* signalInfo, void* vcontext) {
    s_signal = signal;
    s_siginfo = signalInfo;
    s_context = reinterpret_cast<ucontext_t*>(vcontext);
    char buf = '1';
    write(s_pipe[1], &buf, 1);
}

static void handlerThread() {
    // no more mutex deadlocker
    char buf;
    while (read(s_pipe[0], &buf, 1) != 0) {
        auto signalAddress = reinterpret_cast<void*>(s_context->uc_mcontext.pc);
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

    std::thread(&handlerThread).detach();

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
