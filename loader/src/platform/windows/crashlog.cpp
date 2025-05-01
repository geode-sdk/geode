#define WIN32_LEAN_AND_MEAN

#include <Geode/DefaultInclude.hpp>

#include <crashlog.hpp>
#include <Geode/loader/Dirs.hpp>
#include <Geode/loader/Loader.hpp>
#include <Geode/loader/Mod.hpp>
#include <DbgHelp.h>
#include <Geode/utils/casts.hpp>
#include <Geode/utils/file.hpp>
#include <Geode/utils/terminate.hpp>
#include <Windows.h>
#include <ctime>
#include <errhandlingapi.h>
#include <iomanip>
#include <iostream>
#include <string>
#include <fmt/core.h>
#include "ehdata_structs.hpp"

using namespace geode::prelude;

static bool g_lastLaunchCrashed = false;
static bool g_symbolsInitialized = false;
static std::string g_unzippedSearchPaths;

static std::string getDateString(bool filesafe) {
    auto const now = std::time(nullptr);
    auto const tm = *std::localtime(&now);
    std::ostringstream oss;
    if (filesafe) {
        oss << std::put_time(&tm, "%F_%H-%M-%S");
    }
    else {
        oss << std::put_time(&tm, "%FT%T%z"); // ISO 8601
    }
    return oss.str();
}

static std::string getModuleName(HMODULE module, bool fullPath = true) {
    char buffer[MAX_PATH];
    if (!GetModuleFileNameA(module, buffer, MAX_PATH)) {
        return "<Unknown>";
    }
    if (fullPath) {
        return buffer;
    }
    return std::filesystem::path(buffer).filename().string();
}

static char const* getExceptionCodeString(DWORD code) {
    #define EXP_STR(c) \
        case c: return #c
    switch (code) {
        EXP_STR(EXCEPTION_ACCESS_VIOLATION);
        EXP_STR(EXCEPTION_ARRAY_BOUNDS_EXCEEDED);
        EXP_STR(EXCEPTION_STACK_OVERFLOW);
        EXP_STR(EXCEPTION_ILLEGAL_INSTRUCTION);
        EXP_STR(EXCEPTION_IN_PAGE_ERROR);
        EXP_STR(EXCEPTION_BREAKPOINT);
        EXP_STR(EXCEPTION_DATATYPE_MISALIGNMENT);
        EXP_STR(EXCEPTION_FLT_DENORMAL_OPERAND);
        EXP_STR(EXCEPTION_FLT_DIVIDE_BY_ZERO);
        EXP_STR(EXCEPTION_FLT_INEXACT_RESULT);
        EXP_STR(EXCEPTION_FLT_INVALID_OPERATION);
        EXP_STR(EXCEPTION_FLT_OVERFLOW);
        EXP_STR(EXCEPTION_INT_DIVIDE_BY_ZERO);
        EXP_STR(GEODE_TERMINATE_EXCEPTION_CODE);
        EXP_STR(GEODE_UNREACHABLE_EXCEPTION_CODE);
        default: return "<Unknown>";
    }
    #undef EXP_STR
}

static HMODULE handleFromAddress(void const* addr) {
    HMODULE module = nullptr;
    GetModuleHandleEx(
        GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT,
        (LPCTSTR)addr, &module
    );
    return module;
}

static Mod* modFromAddress(PVOID exceptionAddress) {
    auto modulePath = getModuleName(handleFromAddress(exceptionAddress), true);
    for (auto& mod : Loader::get()->getAllMods()) {
        if (mod->getBinaryPath() == modulePath) {
            return mod;
        }
    }
    return nullptr;
}

PVOID GeodeFunctionTableAccess64(HANDLE hProcess, DWORD64 AddrBase);

typedef union _UNWIND_CODE {
    struct {
        uint8_t CodeOffset;
        uint8_t UnwindOp : 4;
        uint8_t OpInfo   : 4;
    };
    uint16_t FrameOffset;
} UNWIND_CODE, *PUNWIND_CODE;

typedef struct _UNWIND_INFO {
    uint8_t Version       : 3;
    uint8_t Flags         : 5;
    uint8_t SizeOfProlog;
    uint8_t CountOfCodes;
    uint8_t FrameRegister : 4;
    uint8_t FrameOffset   : 4;
    UNWIND_CODE UnwindCode[1];
/*  UNWIND_CODE MoreUnwindCode[((CountOfCodes + 1) & ~1) - 1];
*   union {
*       OPTIONAL ULONG ExceptionHandler;
*       OPTIONAL ULONG FunctionEntry;
*   };
*   OPTIONAL ULONG ExceptionData[]; */
} UNWIND_INFO, *PUNWIND_INFO;

static void printAddr(std::ostream& stream, void const* addr, bool fullPath = true) {
    HMODULE module = nullptr;
    auto proc = GetCurrentProcess();

    if (GetModuleHandleEx(
            GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT,
            (LPCTSTR)addr, &module
        )) {
        // calculate base + [address]
        auto const diff = reinterpret_cast<uintptr_t>(addr) - reinterpret_cast<uintptr_t>(module);
        stream << getModuleName(module, fullPath) << " + " << std::hex << diff << std::dec;

        // log symbol if possible
        if (g_symbolsInitialized) {
            // https://docs.microsoft.com/en-us/windows/win32/debug/retrieving-symbol-information-by-address

            DWORD64 displacement;

            char buffer[sizeof(SYMBOL_INFO) + MAX_SYM_NAME * sizeof(TCHAR)];
            PSYMBOL_INFO symbolInfo = reinterpret_cast<PSYMBOL_INFO>(buffer);

            symbolInfo->SizeOfStruct = sizeof(SYMBOL_INFO);
            symbolInfo->MaxNameLen = MAX_SYM_NAME;

            if (SymFromAddr(
                    proc, static_cast<DWORD64>(reinterpret_cast<uintptr_t>(addr)), &displacement,
                    symbolInfo
                )) {
                if (auto entry = SymFunctionTableAccess64(proc, static_cast<DWORD64>(reinterpret_cast<uintptr_t>(addr)))) {
                    auto moduleBase = SymGetModuleBase64(proc, static_cast<DWORD64>(reinterpret_cast<uintptr_t>(addr)));
                    auto runtimeFunction = static_cast<PRUNTIME_FUNCTION>(entry);
                    auto unwindInfo = reinterpret_cast<PUNWIND_INFO>(moduleBase + runtimeFunction->UnwindInfoAddress);

                    // This is a chain of unwind info structures, so we traverse back to the first one
                    while (unwindInfo->Flags & UNW_FLAG_CHAININFO) {
                        runtimeFunction = (PRUNTIME_FUNCTION)&(unwindInfo->UnwindCode[( unwindInfo->CountOfCodes + 1 ) & ~1]);
                        unwindInfo = reinterpret_cast<PUNWIND_INFO>(moduleBase + runtimeFunction->UnwindInfoAddress);
                    }

                    if (moduleBase + runtimeFunction->BeginAddress != symbolInfo->Address) {
                        // the symbol address is not the same as the function address
                        return;
                    }
                }
                stream << " (" << std::string(symbolInfo->Name, symbolInfo->NameLen) << " + "
                       << displacement;

                IMAGEHLP_LINE64 line;
                line.SizeOfStruct = sizeof(IMAGEHLP_LINE64);

                DWORD displacement2;

                if (SymGetLineFromAddr64(
                        proc, static_cast<DWORD64>(reinterpret_cast<uintptr_t>(addr)),
                        &displacement2, &line
                    )) {
                    stream << " | " << line.FileName << ":" << line.LineNumber;
                }

                stream << ")";
            }
        }
    }
    else {
        stream << addr;

        if (GeodeFunctionTableAccess64(proc, reinterpret_cast<DWORD64>(addr))) {
            stream << " (Hook handler)";
        }
    }
}

static void printExtraParameters(std::ostream& stream, DWORD code, ULONG_PTR* params, size_t count) {
    switch (code) {
        case EXCEPTION_ACCESS_VIOLATION: {
            const char* what;
            switch (params[0]) {
                case 0: what = "read from memory"; break;
                case 1: what = "write to memory"; break;
                case 8: what = "execute memory (DEP violation)"; break;
                default: what = "???"; break;
            }

            stream << fmt::format(
                "Exception Details: Failed to {} at 0x{:X}",
                what, params[1]
            ) << "\n";
        } break;

        default: {
            // if we can't deduce any useful information, just print the number of parameters
            stream << "Number Parameters: " << count << "\n";
        } break;
    }
}

// https://stackoverflow.com/a/50208684/9124836
static std::string getStacktrace(PCONTEXT context, Mod*& suspectedFaultyMod) {
    std::stringstream stream;
    static STACKFRAME64 stack;
    static PCONTEXT pcontext = context;
    memset(&stack, 0, sizeof(STACKFRAME64));

    auto process = GetCurrentProcess();
    auto thread = GetCurrentThread();
#ifdef GEODE_IS_X86
    stack.AddrPC.Offset = context->Eip;
    stack.AddrStack.Offset = context->Esp;
    stack.AddrFrame.Offset = context->Ebp;
#else
    stack.AddrPC.Offset = context->Rip;
    stack.AddrStack.Offset = context->Rsp;
    stack.AddrFrame.Offset = context->Rdi;
#endif

    stack.AddrPC.Mode = AddrModeFlat;
    stack.AddrStack.Mode = AddrModeFlat;
    stack.AddrFrame.Mode = AddrModeFlat;

    // size_t frame = 0;
    while (true) {
        if (!StackWalk64(
                IMAGE_FILE_MACHINE_AMD64, process, thread, &stack, context, nullptr,
                +[](HANDLE hProcess, DWORD64 AddrBase) {
                    auto ret = GeodeFunctionTableAccess64(hProcess, AddrBase);
                    if (ret) {
                        return ret;
                    }
                    return SymFunctionTableAccess64(hProcess, AddrBase);
                }, 
                +[](HANDLE hProcess, DWORD64 dwAddr) -> DWORD64 {
                    auto ret = GeodeFunctionTableAccess64(hProcess, dwAddr);
                    if (ret) {
                        return dwAddr & (~0xffffull);
                    }
                    return SymGetModuleBase64(hProcess, dwAddr);
                }
                , nullptr
            ))
            break;

        stream << " - ";

        void* addr = reinterpret_cast<void*>(stack.AddrPC.Offset);
        printAddr(stream, addr);

        stream << std::endl;

        // set the suspected faulty mod to the first entry in the stack trace that belongs to a mod
        if (!suspectedFaultyMod) {
            suspectedFaultyMod = modFromAddress(addr);
        }
    }
    return stream.str();
}

static std::string getRegisters(PCONTEXT context) {
#ifdef GEODE_IS_X86
    return fmt::format(
        "EAX: {:08x}\n"
        "EBX: {:08x}\n"
        "ECX: {:08x}\n"
        "EDX: {:08x}\n"
        "EBP: {:08x}\n"
        "ESP: {:08x}\n"
        "EDI: {:08x}\n"
        "ESI: {:08x}\n"
        "EIP: {:08x}\n",
        context->Eax,
        context->Ebx,
        context->Ecx,
        context->Edx,
        context->Ebp,
        context->Esp,
        context->Edi,
        context->Esi,
        context->Eip
    );
#else
    return fmt::format(
        "RAX: {:016x}\n"
        "RBX: {:016x}\n"
        "RCX: {:016x}\n"
        "RDX: {:016x}\n"
        "RBP: {:016x}\n"
        "RSP: {:016x}\n"
        "RDI: {:016x}\n"
        "RSI: {:016x}\n"
        "RIP: {:016x}\n"
        "R8:  {:016x}\n"
        "R9:  {:016x}\n"
        "R10: {:016x}\n"
        "R11: {:016x}\n"
        "R12: {:016x}\n"
        "R13: {:016x}\n"
        "R14: {:016x}\n"
        "R15: {:016x}\n",
        context->Rax,
        context->Rbx,
        context->Rcx,
        context->Rdx,
        context->Rbp,
        context->Rsp,
        context->Rdi,
        context->Rsi,
        context->Rip,
        context->R8,
        context->R9,
        context->R10,
        context->R11,
        context->R12,
        context->R13,
        context->R14,
        context->R15
    );
#endif
}

template <typename T, typename U>
static std::add_const_t<std::decay_t<T>> rebaseAndCast(intptr_t base, U value) {
    // U value -> const T* (base+value)
    return reinterpret_cast<std::add_const_t<std::decay_t<T>>>(base + (ptrdiff_t)(value));
}

static std::string demangleSymbol(const char* symbol, bool isClassName) {
    char demangledBuf[256];

    DWORD flags = 0;
    if (isClassName) {
        symbol += 1; // i know.
        flags = UNDNAME_NO_ARGUMENTS;
    }

    size_t written = UnDecorateSymbolName(symbol, demangledBuf, 256, flags);
    if (written == 0) {
        return "";
    } else {
        return std::string(demangledBuf, demangledBuf + written);
    }
}

// Parses an unhandled C++ exception from an exception pointers struct.
static std::string parseCppException(LPEXCEPTION_POINTERS info) {
    if (info->ExceptionRecord->ExceptionCode != EXCEPTION_NUMBER) {
        throw std::runtime_error("exception handler precondition violated: wrong exception code in c++ exception handler");
    }

    // This executes when a C++ exception was thrown and not handled.
    // https://devblogs.microsoft.com/oldnewthing/20100730-00/?p=13273
    // handling code is partially taken from https://github.com/gnustep/libobjc2/blob/377a81d23778400b5306ee490451ed68b6e8db81/eh_win32_msvc.cc#L244

    // since you can throw virtually anything, we need to figure out if it's an std::exception* or not
    bool isStdException = false;

    auto* exceptionRecord = info->ExceptionRecord;
    auto exceptionObject = exceptionRecord->ExceptionInformation[1];

    // 0 on 32-bit, dll offset on 64-bit
    intptr_t imageBase = exceptionRecord->NumberParameters >= 4 ? static_cast<intptr_t>(exceptionRecord->ExceptionInformation[3]) : 0;

    auto* throwInfo = reinterpret_cast<_MSVC_ThrowInfo*>(exceptionRecord->ExceptionInformation[2]);

    std::string excString;
    if (!throwInfo || !throwInfo->pCatchableTypeArray) {
        excString = "C++ exception: <no SEH data available about the thrown exception>";
    } else {
        auto* catchableTypeArray = rebaseAndCast<_MSVC_CatchableTypeArray*>(imageBase, throwInfo->pCatchableTypeArray);
        auto ctaSize = catchableTypeArray->nCatchableTypes;
        const char* targetName = nullptr;

        for (int i = 0; i < ctaSize; i++) {
            auto* catchableType = rebaseAndCast<_MSVC_CatchableType*>(imageBase, catchableTypeArray->arrayOfCatchableTypes[i]);
            auto* ctDescriptor = rebaseAndCast<_MSVC_TypeDescriptor*>(imageBase, catchableType->pType);
            const char* classname = ctDescriptor->name;

            if (i == 0) {
                targetName = classname;
            }

            if (strcmp(classname, ".?AVexception@std@@") == 0) {
                isStdException = true;
                break;
            }
        }

        // demangle the name of the thrown object
        std::string demangledName;

        if (targetName && targetName[0] != '\0' && targetName[1] != '\0') {
            demangledName = demangleSymbol(targetName, true);
        }

        if (demangledName.empty()) {
            demangledName = "<Unknown type>";
        }

        if (isStdException) {
            std::exception* excObject = reinterpret_cast<std::exception*>(exceptionObject);
            excString = fmt::format("C++ Exception: {}(\"{}\")", demangledName, excObject->what());
        } else {
            excString = fmt::format("C++ Exception: type '{}'", demangledName);
        }
    }

    return excString;
}

static std::string getInfo(LPEXCEPTION_POINTERS info, Mod* faultyMod, Mod* suspectedFaultyMod) {
    // the error code wine raises when a non-existent imported function gets invoked
    constexpr DWORD EXCEPTION_WINE_STUB = 0x80000100;

    std::stringstream stream;

    DWORD code = info->ExceptionRecord->ExceptionCode;

    auto makeFaultyModString = [](Mod* mod) -> std::string {
        if (!mod) return "Faulty Mod: <Unknown>";

        return fmt::format("Faulty Mod: {} {} ({})", mod->getName(), mod->getVersion().toVString(), mod->getID());
    };

    if (code == EXCEPTION_NUMBER) {
        if (!faultyMod) {
            faultyMod = suspectedFaultyMod;
        }

        stream << parseCppException(info) << "\n";
        stream << makeFaultyModString(faultyMod) << "\n";
    }
    else if (isGeodeExceptionCode(code)) {
        stream
            << "A mod has deliberately asked the game to crash.\n"
            << "Reason: " << reinterpret_cast<const char*>(info->ExceptionRecord->ExceptionInformation[0]) << "\n"
            << makeFaultyModString(reinterpret_cast<Mod*>(info->ExceptionRecord->ExceptionInformation[1])) << "\n";
    }
    else if (code == EXCEPTION_WINE_STUB) {
        auto* dll = reinterpret_cast<const char*>(info->ExceptionRecord->ExceptionInformation[0]);
        auto* function = reinterpret_cast<const char*>(info->ExceptionRecord->ExceptionInformation[1]);

        if (!faultyMod) {
            faultyMod = suspectedFaultyMod;
        }

        stream << fmt::format("Attempted to invoke a non-existent function: {} (not found in {})\n", demangleSymbol(function, false), dll);
        stream << makeFaultyModString(faultyMod) << "\n";
    }
    else {
        stream << "Faulty Module: "
            << getModuleName(handleFromAddress(info->ExceptionRecord->ExceptionAddress), true)
            << "\n"
            << makeFaultyModString(faultyMod) << "\n"
            << "Exception Code: " << std::hex << info->ExceptionRecord->ExceptionCode << " ("
            << getExceptionCodeString(info->ExceptionRecord->ExceptionCode) << ")" << std::dec
            << "\n"
            << "Exception Flags: " << info->ExceptionRecord->ExceptionFlags << "\n"
            << "Instruction Address: " << info->ExceptionRecord->ExceptionAddress << " (";
        printAddr(stream, info->ExceptionRecord->ExceptionAddress, false);
        stream << ")\n";

        printExtraParameters(
            stream,
            info->ExceptionRecord->ExceptionCode,
            info->ExceptionRecord->ExceptionInformation,
            info->ExceptionRecord->NumberParameters
        );
    }

    // show the thread that crashed
    stream << "Crashed thread: " << utils::thread::getName() << "\n";

    return stream.str();
}

static void handleException(LPEXCEPTION_POINTERS info) {
    std::string text;
    std::filesystem::path crashlogPath;

    // calling SymInitialize from multiple threads can have unexpected behavior, so synchronize this part
    static std::mutex symMutex;
    {
        std::lock_guard lock(symMutex);

        SymSetOptions(SYMOPT_UNDNAME | SYMOPT_DEFERRED_LOADS | SYMOPT_LOAD_LINES);

        // init symbols so we can get some juicy debug info
        g_symbolsInitialized = SymInitialize(static_cast<HMODULE>(GetCurrentProcess()), nullptr, true);
        if (!g_symbolsInitialized) {
            log::warn("Failed to initialize debug symbols: Error {}", GetLastError());
        }
        else {
            // set the search path to include the mods' temp directories
            if (std::array<char, 4096> searchPathBuffer; 
                SymGetSearchPath(static_cast<HMODULE>(GetCurrentProcess()), searchPathBuffer.data(), searchPathBuffer.size())) {
                std::string searchPath(searchPathBuffer.data());
                searchPath += ";" + g_unzippedSearchPaths;
                SymSetSearchPath(static_cast<HMODULE>(GetCurrentProcess()), searchPath.c_str());
            }
        }
        
        // in some cases, we can be pretty certain that the first mod found while unwinding
        // is the one that caused the crash, so using `suspectedFaultyMod` is safe and correct.
        //
        // however, for most cases there's no such guarantee, and for them only top stack entry is checked.
        Mod* faultyMod = modFromAddress(info->ExceptionRecord->ExceptionAddress);
        Mod* suspectedFaultyMod = nullptr;

        auto stacktrace = getStacktrace(info->ContextRecord, suspectedFaultyMod);
        auto crashInfo = getInfo(info, faultyMod, suspectedFaultyMod);

        text = crashlog::writeCrashlog(
            faultyMod,
            crashInfo,
            stacktrace,
            getRegisters(info->ContextRecord),
            crashlogPath
        );

        if (g_symbolsInitialized) {
            SymCleanup(GetCurrentProcess());
        }
    }

    // defined in crashlogWindow.cpp
    extern bool showCustomCrashlogWindow(std::string text, std::filesystem::path const& crashlogPath);

    if (!showCustomCrashlogWindow(text, crashlogPath)) {
        // if the window fails to show, we show a message box instead
        MessageBoxA(nullptr, text.c_str(), "Geometry Dash Crashed", MB_ICONERROR);
    }
}

static LONG WINAPI exceptionHandler(LPEXCEPTION_POINTERS info) {
    handleException(info);

    // continue searching, which usually just ends up terminating the program (exactly what we need)
    return EXCEPTION_CONTINUE_SEARCH;
}

bool crashlog::setupPlatformHandler() {
    SetUnhandledExceptionFilter(exceptionHandler);

    auto lastCrashedFile = crashlog::getCrashLogDirectory() / "last-crashed";
    if (std::filesystem::exists(lastCrashedFile)) {
        g_lastLaunchCrashed = true;
        try {
            std::filesystem::remove(lastCrashedFile);
        }
        catch (...) {
        }
    }
    return true;
}

bool crashlog::didLastLaunchCrash() {
    return g_lastLaunchCrashed;
}

void crashlog::setupPlatformHandlerPost() {
    g_unzippedSearchPaths.clear();
    for (auto& mod : Loader::get()->getAllMods()) {
        g_unzippedSearchPaths += mod->getTempDir().string() + ";";
    }
}

std::filesystem::path crashlog::getCrashLogDirectory() {
    return dirs::getGeodeDir() / "crashlogs";
}
