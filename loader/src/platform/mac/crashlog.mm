#include <crashlogApple.hpp>

#include <thread>
#include <filesystem>

using namespace geode::prelude;
using namespace crashlog;

// https://gist.github.com/jvranish/4441299

static std::mutex s_mutex;
static std::condition_variable s_cv;
static int s_crashIter = 0;


std::string_view CrashContext::getGeodeBinaryName() {
    return "Geode.dylib";
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
    if (s_crashIter > 3) {
        // the crashlog crashed too much so maybe we shouldn't try
        std::abort();
    }
    s_crashIter++;

	auto context = reinterpret_cast<ucontext_t*>(vcontext);
	s_backtraceSize = backtrace(s_backtrace.data(), s_backtrace.size());

    // for some reason this is needed, dont ask me why
	#ifdef GEODE_IS_INTEL_MAC
	s_backtrace[2] = reinterpret_cast<void*>(context->uc_mcontext->__ss.__rip);
	#else
	s_backtrace[2] = reinterpret_cast<void*>(context->uc_mcontext->__ss.__pc);
	#endif
	if (s_backtraceSize < s_backtrace.size()) {
		s_backtrace[s_backtraceSize] = nullptr;
	}

    // erase first frame since it's always the signal handler and we don't care about it
    for (size_t i = 1; i < s_backtraceSize; i++) {
        s_backtrace[i - 1] = s_backtrace[i];
    }
    if (s_backtraceSize > 0) s_backtraceSize--;

    {
        std::unique_lock<std::mutex> lock(s_mutex);
        s_signal = signal;
        s_siginfo = signalInfo;
        s_context = context;
    }

    s_cv.notify_all();
    std::unique_lock<std::mutex> lock(s_mutex);
    s_cv.wait(lock, [] { return s_signal == 0; });

    s_crashIter--;
	std::_Exit(EXIT_FAILURE);
}

static void handlerThread() {
    std::unique_lock<std::mutex> lock(s_mutex);
    s_cv.wait(lock, [] { return s_signal != 0; });

    #ifdef GEODE_IS_INTEL_MAC
    auto signalAddress = reinterpret_cast<void*>(s_context->uc_mcontext->__ss.__rip);
    #else // m1
    auto signalAddress = reinterpret_cast<void*>(s_context->uc_mcontext->__ss.__pc);
    #endif

    g_context.initialize(signalAddress);

    auto text = crashlog::writeCrashlog(g_context);

    log::error("Geode crashed!\n{}", text);

    s_signal = 0;
    s_cv.notify_all();

    log::debug("Notified");
}

static bool s_lastLaunchCrashed;

bool crashlog::setupPlatformHandler() {
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
        std::error_code ec;
        std::filesystem::remove(lastCrashedFile, ec);
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
