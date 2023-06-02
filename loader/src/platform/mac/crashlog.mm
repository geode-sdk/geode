#include <crashlog.hpp>

#ifdef GEODE_IS_MACOS

#include <array>
#include <ghc/fs_fwd.hpp>

#import <Foundation/Foundation.h>

// https://gist.github.com/jvranish/4441299

static std::string_view getSignalCodeString(int sig, siginfo_t* siginfo) {
    switch(sig) {
        case SIGSEGV: return "SIGSEGV: Segmentation Fault";
        case SIGINT: return "SIGINT: Interactive attention signal, (usually ctrl+c)";
        case SIGFPE:
            switch(siginfo->si_code) {
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
            switch(siginfo->si_code) {
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
        default: return "Unknown signal code";
    }
}

static std::string getInfo(int sig, siginfo_t* siginfo, ucontext_t* context, Mod* faultyMod) {
    std::stringstream stream;
    void* address = reinterpret_cast<void*>(context->uc_mcontext->__ss.__rip);
    stream // << "Faulty Lib: " << getModuleName(handleFromAddress(address), true) << "\n"
           << "Faulty Mod: " << (faultyMod ? faultyMod->getID() : "<Unknown>") << "\n"
           << "Signal Code: " << std::hex << sig << " (" << getSignalCodeString(sig, siginfo) << ")" << std::dec << "\n"
    return stream.str();
}

extern "C" void signalHandler(int signal, siginfo_t* signal_info, void* vcontext) {
	auto context = reinterpret_cast<ucontext_t*>(vcontext);

	auto array = std::array<void*, 20>();
	auto size = backtrace(array.data(), 20);

#ifdef __APPLE__
	array[2] = reinterpret_cast<void*>(context->uc_mcontext->__ss.__rip);
#else
	array[1] = reinterpret_cast<void*>(context->uc_mcontext.gregs[REG_RIP]);
#endif

	if (size < 20) {
		array[size] = nullptr;
	}

	auto messages = backtrace_symbols(array.data(), size);
	if (size < 20) {
		messages[size] = nullptr;
	}

	ServerLoop::get()->m_panicBacktrace.store(messages);

	ServerLoop::get()->m_signalExit.notify();

	ServerLoop::get()->m_signalFinish.wait();

	free(messages);

	std::abort();
}

bool crashlog::setupPlatformHandler() {
    return true;
}

bool crashlog::didLastLaunchCrash() {
    return false;
}

ghc::filesystem::path crashlog::getCrashLogDirectory() {
    std::array<char, 1024> path;
    CFStringGetCString(
        (CFStringRef)NSHomeDirectory(), path.data(), path.size(), kCFStringEncodingUTF8
    );
    auto crashlogDir =
        ghc::filesystem::path(path.data()) / "Library" / "Logs" / "DiagnosticReports";
    return crashlogDir.string();
}

#endif