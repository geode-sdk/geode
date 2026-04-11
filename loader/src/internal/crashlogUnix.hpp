#pragma once
#include <Geode/utils/StringBuffer.hpp>
#include <Geode/platform/cplatform.h>
#include <crashlog.hpp>
#include <string_view>
#include <signal.h>
#include <cxxabi.h>
#include <unistd.h>

#ifdef GEODE_IS_ANDROID
# include <sys/uio.h>
#endif

inline std::string_view getSignalCodeString(int signal, siginfo_t* siginfo) {
    switch(signal) {
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
        case SIGBUS: return "SIGBUS: Bus error (bad memory access)";
        default: return "Unknown signal code";
    }
}

inline bool hasSignalDetail(int signal) {
    return signal == SIGILL || signal == SIGFPE || signal == SIGSEGV || signal == SIGBUS || signal == SIGTRAP;
}

#ifdef GEODE_IS_ANDROID
inline ssize_t safeMemoryRead(void* buf, const void* src, size_t size) {
    struct iovec local { buf, size };
    struct iovec remote { const_cast<void*>(src), size };

    return process_vm_readv(getpid(), &local, 1, &remote, 1, 0);
}

inline void describeInstructionBytes(crashlog::Buffer& stream, const uint8_t* bytes, size_t size, size_t instOffset) {
    stream.append(" (bytes: ");
    size_t instEnd = std::min(size, instOffset + 4);

    // write in format
    // 00 11 22 33 [44 55 66 77] 88 99 AA BB

    for (size_t i = 0; i < size; i++) {
        if (i == instOffset) {
            stream.append('[');
        }
        stream.append("{:02X}", bytes[i]);
        if (i == instEnd - 1) {
            stream.append(']');
        }
        if (i != size - 1) {
            stream.append(' ');
        }
    }

    stream.append(')');
}

inline void tryWriteSigillInstruction(crashlog::Buffer& stream, void* address) {
    // read a couple instructions before & after
    uintptr_t addr = reinterpret_cast<uintptr_t>(address);
    uintptr_t readStart = (addr - 8) & 4;
    size_t readSize = (addr - readStart) + 8;

    uint8_t buf[128];
    auto result = safeMemoryRead(buf, reinterpret_cast<void*>(readStart), readSize);
    if (result > 0) {
        describeInstructionBytes(stream, buf, result, addr - readStart);
        return;
    }

    // try to read just the faulting instruction if fails
    result = safeMemoryRead(buf, address, 4);
    if (result > 0) {
        describeInstructionBytes(stream, buf, result, 0);
        return;
    }

    // give up otherwise
}
#endif

inline void writeSignalDetail(crashlog::Buffer& stream, crashlog::CrashContext& ctx, int signal, siginfo_t* siginfo) {
    switch (signal) {
        case SIGILL: {
            stream.append("Illegal instruction was encountered at ");
            ctx.formatAddress(siginfo->si_addr, stream);
#ifdef GEODE_IS_ANDROID
            tryWriteSigillInstruction(stream, siginfo->si_addr);
#endif
        } break;

        case SIGFPE: {
            stream.append("Floating point exception was thrown at ");
            ctx.formatAddress(siginfo->si_addr, stream);
        } break;

        case SIGSEGV: {
            stream.append("Could not access memory at ");
            ctx.formatAddress(siginfo->si_addr, stream);
            stream.append(" (");

            switch (siginfo->si_code) {
                case SEGV_MAPERR: {
                    stream.append("address not mapped to an object");
                } break;

                case SEGV_ACCERR: {
                    stream.append("invalid permissions for mapped object");
                } break;

                default: {
                    stream.append("unknown reason (code {})", siginfo->si_code);
                } break;
            }

            stream.append(')');
        } break;

        case SIGBUS: {
            stream.append("Bus error when trying to access memory at ");
            ctx.formatAddress(siginfo->si_addr, stream);
            stream.append(" (");

            switch (siginfo->si_code) {
                case BUS_ADRALN: {
                    stream.append("invalid address alignment");
                } break;

                case BUS_ADRERR: {
                    stream.append("nonexistent physical address");
                } break;

                case BUS_OBJERR: {
                    stream.append("object-specific hardware error");
                } break;

                default: {
                    stream.append("unknown reason (code {})", siginfo->si_code);
                } break;
            }

            stream.append(')');
        } break;

        case SIGTRAP: {
            stream.append("Breakpoint was hit at ");
            ctx.formatAddress(siginfo->si_addr, stream);
        } break;
    }

}

inline std::string demangle(const char* name) {
    std::string out;
    int status;
    auto demangle = abi::__cxa_demangle(name, 0, 0, &status);
    if (status == 0) {
        out = demangle;
    }
    free(demangle);
    return out;
}
