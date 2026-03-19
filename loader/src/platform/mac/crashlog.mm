#include <crashlog.hpp>
#include <crashlogApple.hpp>

#include <Geode/utils/string.hpp>
#include <array>
#include <thread>
#include <filesystem>
#include <execinfo.h>
#include <dlfcn.h>
#include <cxxabi.h>
#include <algorithm>

#include <mach-o/dyld_images.h>
#include <mach-o/dyld.h>
#define CommentType CommentTypeDummy
#import <Foundation/Foundation.h>
#undef CommentType

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
    stream.append("Faulty Lib: {}\n", image->name());
    stream.append("Faulty Mod: {}\n", faultyMod ? faultyMod->getID() : "<Unknown>");
    stream.append("Instruction Address: ");
    this->formatAddress(crashAddr, infoStream);

    stream.append("\nSignal Code: 0x{:x} ({})\n", s_siginfo->si_code, getSignalCodeString(s_signal, s_siginfo));

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

std::vector<StackFrame> CrashContext::getStacktrace() {
    // this function is evil
    std::vector<StackFrame> frames;

    auto messages = backtrace_symbols(s_backtrace.data(), s_backtraceSize);

    auto gameDir = geode::utils::string::pathToString(dirs::getGameDir());
    std::stringstream lines(addr2Line());

    for (int i = 1; i < s_backtraceSize; ++i) {
        auto message = std::string(messages[i]);

        auto stream = std::stringstream(message);
        int index;
        std::string binary;
        uintptr_t address;
        std::string function;
        uintptr_t offset;
        std::string line;

        stream >> index;

        if (!lines.eof()) {
            std::getline(lines, line);
        }
        std::getline(stream, binary);
        auto cutoff = binary.find("0x");
        stream = std::stringstream(binary.substr(cutoff));
        binary = geode::utils::string::trim(binary.substr(0, cutoff));
        stream >> std::hex >> address >> std::dec;

        StackFrame frame{};
        frame.address = address;
        auto image = g_context.imageFromAddress(reinterpret_cast<void*>(address));
        frame.image = image;

        if (!line.empty()) {
            // log::debug("address: {}", address);
            // log::debug("image: {}", image);

            if (image) {
                auto baseAddress = image->address;

                if (base::get() == (uintptr_t)baseAddress) {
                    // find closest function start
                    auto const& funcs = getFunctionStarts();
                    auto iter = std::upper_bound(funcs.begin(), funcs.end(), offset);
                    if (iter != funcs.begin()) {
                        --iter;
                        auto funcOffset = *iter;
                        auto funcName = crashlog::lookupFunctionByOffset(funcOffset);
                        frame.offset = offset - funcOffset;

                        if (funcName.empty()) {
                            frame.symbol = fmt::format("sub_{:x}", funcOffset);
                        } else {
                            frame.symbol = funcName;
                        }
                    }
                }
            }

            if (frame.symbol.empty()) {
                // try to extract the symbol name
                auto endSymName = line.find(" (in ");
                if (endSymName != std::string::npos && !line.starts_with("0x")) {
                    frame.symbol = line.substr(0, endSymName);
                }

                // now if that succeeded, try to extract the file/line information
                auto lastParen = line.rfind(')');
                auto lineInfoSplit = line.rfind(':', lastParen);
                auto startParen = line.rfind('(', lineInfoSplit);

                if (startParen != std::string::npos && lastParen != std::string::npos && lineInfoSplit != std::string::npos) {
                    auto inner = std::string_view{line}.substr(startParen + 1, lastParen - startParen - 1);
                    auto colon = inner.rfind(':');
                    auto file = inner.substr(0, colon);
                    auto lineNum = inner.substr(colon + 1);

                    frame.file = file;
                    frame.line = utils::numFromString<uint32_t>(lineNum).unwrapOr(0);
                }
            }
        }
        else {
            std::getline(stream, function);
            cutoff = function.find("+");
            stream = std::stringstream(function.substr(cutoff));
            stream >> offset;
            function = geode::utils::string::trim(function.substr(0, cutoff));

            {
                int status;
                auto demangle = abi::__cxa_demangle(function.c_str(), 0, 0, &status);
                if (status == 0) {
                    function = demangle;
                }
                free(demangle);
            }

            frame.symbol = function;
            frame.offset = offset;
        }

        frames.push_back(std::move(frame));
    }

    free(messages);

    return frames;
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
