// this is mostly copied from macos
#include <crashlog.hpp>
#include <crashlogApple.hpp>

#include <Geode/utils/string.hpp>
#include <Geode/utils/ZStringView.hpp>
#include <array>
#include <thread>
#include <execinfo.h>
#include <dlfcn.h>
#include <cxxabi.h>
#include <algorithm>

#include <mach-o/dyld_images.h>
#include <mach-o/dyld.h>
#include <unistd.h>
#include <fcntl.h>
#import <Foundation/Foundation.h>

using namespace geode::prelude;
using namespace crashlog;

// https://gist.github.com/jvranish/4441299

static int s_pipe[2];

std::string_view CrashContext::getGeodeBinaryName() {
    return "Geode.ios.dylib";
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
}

std::vector<StackFrame> CrashContext::getStacktrace() {
    std::vector<StackFrame> frames;

    auto messages = backtrace_symbols(s_backtrace.data(), s_backtraceSize);
    if (s_backtraceSize < FRAME_SIZE) {
        messages[s_backtraceSize] = nullptr;
    }

    std::stringstream lines(addr2Line());

    for (int i = 0; i < s_backtraceSize; ++i) {
        int index;
        std::string binary;
        uintptr_t address;
        std::string function;
        uintptr_t offset;
        std::string line;
        size_t cutoff;

        auto message = std::string(i == 0 ? "" : messages[i]);
        auto stream = std::stringstream(message);

        if (i > 0) {
            stream >> index;

            if (!lines.eof()) {
                std::getline(lines, line);
            }
            std::getline(stream, binary);
            cutoff = binary.find("0x");
            stream = std::stringstream(binary.substr(cutoff));
            binary = geode::utils::string::trim(binary.substr(0, cutoff));
            stream >> std::hex >> address >> std::dec;
        } else {
            // first entry in the stacktrace, not present in `messages`
            address = (uintptr_t) s_backtrace[0];
            index = 0;
        }

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

            // TODO: also parse this properly?
            if (frame.symbol.empty()) {
                frame.description = line;
            }
        }
        else {
            if (i > 0) {
                std::getline(stream, function);

                cutoff = function.find("+");
                stream = std::stringstream(function.substr(cutoff + 1));
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
            }

            // don't display the (function + offset) part if it will be bogus.
            // the first case (0x0) happens with hook handlers, while the second happens because
            // GD exports a few fmt symbols, so backtrace_symbols thinks every function in GD is the last fmt symbol in the binary
            if (function == "0x0" || (binary == "GeometryJump" && offset > 0x1000)) {
                function = "";
            }

            frame.symbol = std::move(function);
            frame.offset = offset;
        }

        frames.push_back(std::move(frame));
    }

    free(messages);

    return frames;
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
