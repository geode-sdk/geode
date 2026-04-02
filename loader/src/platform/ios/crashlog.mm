#include <crashlogApple.hpp>

#include <thread>

#include <unistd.h>
#include <fcntl.h>

using namespace geode::prelude;
using namespace crashlog;

// https://gist.github.com/jvranish/4441299

static int s_pipe[2];

std::string_view CrashContext::getGeodeBinaryName() {
    return "Geode.ios.dylib";
}

void CrashContext::writeInfo(Buffer& stream) {
    auto image = this->imageFromAddress(crashAddr);
    stream.append("Faulty Lib: {}\n", image ? image->name() : "<Unknown>");
    stream.append("Faulty Mod: {} ({})\n", faultyMod ? faultyMod->getID() : "<Unknown>", faultyMod ? faultyMod->getHash() : "N/A");
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
    /*auto context = reinterpret_cast<ucontext_t*>(vcontext);
    s_backtraceSize = backtrace(s_backtrace.data(), FRAME_SIZE);

    // for some reason this is needed, dont ask me why
    s_backtrace[2] = reinterpret_cast<void*>(context->uc_mcontext->__ss.__pc);
    if (s_backtraceSize < FRAME_SIZE) {
        s_backtrace[s_backtraceSize] = nullptr;
    }*/

    s_signal = signal;
    s_siginfo = signalInfo;
    s_context = reinterpret_cast<ucontext_t*>(vcontext);
    char buf = '1';
    write(s_pipe[1], &buf, 1);

    // wait for the death to come
    while (true) {
        usleep(100000);
    }
}

static void handlerThread() {
    // no more mutex deadlocker
    char buf;
    while (read(s_pipe[0], &buf, 1) != 0) {
        auto signalAddress = reinterpret_cast<void*>(s_context->uc_mcontext->__ss.__pc);
        // as you can tell, i moved code from signalHandler to here
        if (s_context) {
            //s_backtraceSize = backtrace(s_backtrace.data(), FRAME_SIZE);
            // i can't use 2 because then it'll show the actual stacktrace to be lower than what it actually is
            s_backtrace[s_backtraceSize++] = signalAddress;
            void* current_fp = reinterpret_cast<void*>(s_context->uc_mcontext->__ss.__fp);
            /*
            if (s_backtraceSize < FRAME_SIZE) {
                s_backtrace[s_backtraceSize] = nullptr;
            }
            */
            while (s_backtraceSize < FRAME_SIZE && current_fp) {
                void** frame = reinterpret_cast<void**>(current_fp);
                void* next_fp = frame[0];
                void* lr = frame[1];

                if (next_fp == current_fp || lr == nullptr) break;

                s_backtrace[s_backtraceSize++] = lr;
                current_fp = next_fp;
            }
        }

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

    auto lastCrashedFile = crashlog::getCrashLogDirectory() / "last-crashed";
    if (std::filesystem::exists(lastCrashedFile)) {
        s_lastLaunchCrashed = true;
        std::filesystem::remove(lastCrashedFile);
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
