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

#include <tlhelp32.h>

using namespace geode::prelude;
using namespace crashlog;

static bool g_lastLaunchCrashed = false;
static bool g_symbolsInitialized = false;
static std::wstring g_unzippedSearchPaths;
static crashlog::CrashContext g_context;
static LPEXCEPTION_POINTERS g_excPtr;

PVOID GeodeFunctionTableAccess64(HANDLE hProcess, DWORD64 AddrBase);

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

std::vector<Image> CrashContext::getImages() {
    HANDLE snap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, GetCurrentProcessId());

    if (snap == INVALID_HANDLE_VALUE) {
        return {};
    }

    MODULEENTRY32W me32;
    me32.dwSize = sizeof(MODULEENTRY32W);

    std::vector<Image> images;
    if (Module32FirstW(snap, &me32)) {
        do {
            images.push_back({
                reinterpret_cast<uintptr_t>(me32.modBaseAddr),
                utils::string::wideToUtf8(me32.szExePath)
            });
        } while (Module32NextW(snap, &me32));
    }

    CloseHandle(snap);
    return images;
}

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

static std::string findSymbolNameFromRVA(HMODULE module, DWORD rva) {
    if (!module) return {};

    auto base = reinterpret_cast<uintptr_t>(module);
    auto dosHeader = reinterpret_cast<PIMAGE_DOS_HEADER>(base);

    if (dosHeader->e_magic != IMAGE_DOS_SIGNATURE) return {};

    auto ntHeaders = reinterpret_cast<PIMAGE_NT_HEADERS>(base + dosHeader->e_lfanew);
    auto& exportDirAttr = ntHeaders->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT];

    if (exportDirAttr.Size == 0) return {};

    auto exports = reinterpret_cast<PIMAGE_EXPORT_DIRECTORY>(base + exportDirAttr.VirtualAddress);
    auto functions = reinterpret_cast<DWORD*>(base + exports->AddressOfFunctions);
    auto names = reinterpret_cast<DWORD*>(base + exports->AddressOfNames);
    auto ordinals = reinterpret_cast<WORD*>(base + exports->AddressOfNameOrdinals);

    for (DWORD i = 0; i < exports->NumberOfFunctions; i++) {
        DWORD funcRVA = functions[i];
        if (funcRVA == rva) {
            for (DWORD j = 0; j < exports->NumberOfNames; j++) {
                if (ordinals[j] == i) {
                    char demangledBuf[512];
                    auto symbol = reinterpret_cast<const char*>(base + names[j]);
                    size_t written = UnDecorateSymbolName(
                        symbol, demangledBuf, 512,
                        UNDNAME_NO_ACCESS_SPECIFIERS | UNDNAME_NO_ALLOCATION_MODEL |
                        UNDNAME_NO_THISTYPE | UNDNAME_NO_MS_KEYWORDS |
                        UNDNAME_NO_FUNCTION_RETURNS
                    );
                    if (written) return {demangledBuf};
                    return symbol;
                }
            }
        }
    }

    return {};
}

static StackFrame getFrame(void const* addr, bool fullPath = true) {
    HMODULE module = nullptr;
    auto proc = GetCurrentProcess();

    StackFrame frame{};
    frame.address = reinterpret_cast<uintptr_t>(addr);

    if (!GetModuleHandleEx(
        GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT,
        (LPCTSTR)addr, &module
    )) {
        return frame;
    }

    frame.image = g_context.imageFromAddress(addr);

    auto const diff = reinterpret_cast<uintptr_t>(addr) - reinterpret_cast<uintptr_t>(module);

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
                    return frame;
                }
            }

            frame.symbol = std::string(symbolInfo->Name, symbolInfo->NameLen);
            frame.offset = displacement;

            IMAGEHLP_LINE64 line;
            line.SizeOfStruct = sizeof(IMAGEHLP_LINE64);

            DWORD displacement2;

            if (SymGetLineFromAddr64(
                    proc, static_cast<DWORD64>(reinterpret_cast<uintptr_t>(addr)),
                    &displacement2, &line
                )) {
                frame.file = line.FileName;
                frame.line = line.LineNumber;
            }
        } else {
            // handle GeometryDash.exe bindings and libcocos2d.dll missing symbols on wine
            bool isGD = (uintptr_t)module == base::get();
            bool isCocos = (uintptr_t)module == base::getCocos();
            if (!isGD && !isCocos) return frame;

            DWORD64 dwAddr = reinterpret_cast<uintptr_t>(addr);
            if (auto entry = SymFunctionTableAccess64(proc, dwAddr)) {
                auto moduleBase = SymGetModuleBase64(proc, dwAddr);
                auto runtimeFunction = static_cast<PRUNTIME_FUNCTION>(entry);

                if (moduleBase) {
                    uintptr_t funcAddr = moduleBase + runtimeFunction->BeginAddress;
                    uintptr_t diff = reinterpret_cast<uintptr_t>(addr) - funcAddr;

                    if (isGD) {
                        auto funcName = crashlog::lookupFunctionByOffset(runtimeFunction->BeginAddress);
                        if (!funcName.empty()) {
                            frame.symbol = funcName;
                            frame.offset = diff;
                            return frame;
                        }
                    } else {
                        auto funcName = findSymbolNameFromRVA(module, runtimeFunction->BeginAddress);
                        if (!funcName.empty()) {
                            frame.symbol = funcName;
                            frame.offset = diff;
                            return frame;
                        }
                    }

                    // unnamed function
                    frame.symbol = fmt::format("sub_{:x}", runtimeFunction->BeginAddress);
                    return frame;
                }
            }

            // fallback (usually leaf functions)
            if (isGD) {
                uintptr_t offset = diff;
                auto funcName = crashlog::lookupClosestFunction(offset);
                if (!funcName.empty()) {
                    frame.symbol = funcName;
                    frame.offset = offset;
                }
            }
        }
    }

    return frame;
}

std::vector<StackFrame> CrashContext::getStacktrace() {
    std::vector<StackFrame> frames;

    static STACKFRAME64 stack;
    memset(&stack, 0, sizeof(STACKFRAME64));

    auto ctx = g_excPtr->ContextRecord;
    auto process = GetCurrentProcess();
    auto thread = GetCurrentThread();
#ifdef GEODE_IS_X86
    stack.AddrPC.Offset = ctx->Eip;
    stack.AddrStack.Offset = ctx->Esp;
    stack.AddrFrame.Offset = ctx->Ebp;
#else
    stack.AddrPC.Offset = ctx->Rip;
    stack.AddrStack.Offset = ctx->Rsp;
    stack.AddrFrame.Offset = ctx->Rdi;
#endif

    stack.AddrPC.Mode = AddrModeFlat;
    stack.AddrStack.Mode = AddrModeFlat;
    stack.AddrFrame.Mode = AddrModeFlat;

    // size_t frame = 0;
    while (true) {
        if (!StackWalk64(
                IMAGE_FILE_MACHINE_AMD64, process, thread, &stack, ctx, nullptr,
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

        void* addr = reinterpret_cast<void*>(stack.AddrPC.Offset);
        frames.push_back(getFrame(addr));
    }

    return frames;
}

std::vector<Register> CrashContext::getRegisters() {
    std::vector<Register> registers;
    auto& ss = g_excPtr->ContextRecord;

#ifdef GEODE_IS_X86
    registers.push_back({ "EAX", ss->Eax });
    registers.push_back({ "EBX", ss->Ebx });
    registers.push_back({ "ECX", ss->Ecx });
    registers.push_back({ "EDX", ss->Edx });
    registers.push_back({ "EBP", ss->Ebp });
    registers.push_back({ "ESP", ss->Esp });
    registers.push_back({ "EDI", ss->Edi });
    registers.push_back({ "ESI", ss->Esi });
    registers.push_back({ "EIP", ss->Eip });
#else
    registers.push_back({ "RAX", ss->Rax });
    registers.push_back({ "RBX", ss->Rbx });
    registers.push_back({ "RCX", ss->Rcx });
    registers.push_back({ "RDX", ss->Rdx });
    registers.push_back({ "RBP", ss->Rbp });
    registers.push_back({ "RSP", ss->Rsp });
    registers.push_back({ "RDI", ss->Rdi });
    registers.push_back({ "RSI", ss->Rsi });
    registers.push_back({ "RIP", ss->Rip });
    registers.push_back({ "R8",  ss->R8  });
    registers.push_back({ "R9",  ss->R9  });
    registers.push_back({ "R10", ss->R10 });
    registers.push_back({ "R11", ss->R11 });
    registers.push_back({ "R12", ss->R12 });
    registers.push_back({ "R13", ss->R13 });
    registers.push_back({ "R14", ss->R14 });
    registers.push_back({ "R15", ss->R15 });
#endif

    return registers;
}

std::string_view CrashContext::getGeodeBinaryName() {
    return "Geode.dll";
}

static HMODULE handleFromAddress(void const* addr) {
    HMODULE module = nullptr;
    GetModuleHandleEx(
        GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT,
        (LPCTSTR)addr, &module
    );
    return module;
}

static void printExtraParameters(Buffer& stream, DWORD code, ULONG_PTR* params, size_t count) {
    switch (code) {
        case EXCEPTION_ACCESS_VIOLATION: {
            std::string_view what;
            switch (params[0]) {
                case 0: what = "read from memory"; break;
                case 1: what = "write to memory"; break;
                case 8: what = "execute memory (DEP violation)"; break;
                default: what = "???"; break;
            }

            stream.append(
                "Exception Details: Failed to {} at 0x{:X}\n",
                what, params[1]
            );
        } break;

        default: {
            // if we can't deduce any useful information, just print the number of parameters
            stream.append("Number Parameters: {}\n", count);
        } break;
    }
}

template <typename T, typename U>
static std::add_const_t<std::decay_t<T>> rebaseAndCast(intptr_t base, U value) {
    // U value -> const T* (base+value)
    return reinterpret_cast<std::add_const_t<std::decay_t<T>>>(base + (ptrdiff_t)(value));
}

static std::string demangleSymbol(const char* symbol, bool isClassName) {
    char demangledBuf[512];

    DWORD flags = 0;
    if (isClassName) {
        symbol += 1; // i know.
        flags = UNDNAME_NO_ARGUMENTS;
    }

    size_t written = UnDecorateSymbolName(symbol, demangledBuf, 512, flags);
    if (written == 0) {
        // return mangled
        return std::string(symbol);
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

void CrashContext::writeInfo(Buffer& stream) {
    // the error code wine raises when a non-existent imported function gets invoked
    constexpr DWORD EXCEPTION_WINE_STUB = 0x80000100;

    DWORD code = g_excPtr->ExceptionRecord->ExceptionCode;

    auto makeFaultyModString = [](Mod* mod) -> std::string {
        if (!mod) return "Faulty Mod: <Unknown>";

        return fmt::format("Faulty Mod: {} {} ({})", mod->getName(), mod->getVersion().toVString(), mod->getID());
    };

    if (code == EXCEPTION_NUMBER) {
        stream.append(parseCppException(g_excPtr));
        stream.append('\n');
        stream.append(makeFaultyModString(faultyMod));
        stream.append('\n');
    }
    else if (isGeodeExceptionCode(code)) {
        stream.append(
            "A mod has deliberately asked the game to crash.\n"
            "Reason: {}\n{}\n",
            reinterpret_cast<const char*>(g_excPtr->ExceptionRecord->ExceptionInformation[0]),
            makeFaultyModString(reinterpret_cast<Mod*>(g_excPtr->ExceptionRecord->ExceptionInformation[1]))
        );
    }
    else if (code == EXCEPTION_WINE_STUB) {
        auto* dll = reinterpret_cast<const char*>(g_excPtr->ExceptionRecord->ExceptionInformation[0]);
        auto* function = reinterpret_cast<const char*>(g_excPtr->ExceptionRecord->ExceptionInformation[1]);

        stream.append("Attempted to invoke a non-existent function: {} (not found in {})\n", demangleSymbol(function, false), dll);
        stream.append(makeFaultyModString(faultyMod));
        stream.append('\n');
    }
    else {
        auto rec = g_excPtr->ExceptionRecord;
        auto image = this->imageFromAddress(this->crashAddr);

        stream.append("Faulty Module: {}\n", image ? image->name() : "<Unknown>");
        stream.append("{}\n", makeFaultyModString(faultyMod));
        stream.append("Exception Code: {:x} ({})\n", rec->ExceptionCode, getExceptionCodeString(rec->ExceptionCode));
        stream.append("Exception Flags: {}\n", rec->ExceptionFlags);
        stream.append("Instruction Address: ");
        this->formatAddress(rec->ExceptionAddress, stream);
        stream.append('\n');


        printExtraParameters(
            stream,
            rec->ExceptionCode,
            rec->ExceptionInformation,
            rec->NumberParameters
        );
    }

    // show the thread that crashed
    stream.append("Crashed thread: {}\n", thread::getName());
}

static void handleException(LPEXCEPTION_POINTERS info) {
    std::string text;
    std::filesystem::path crashlogPath;
    g_excPtr = info;

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
            if (std::array<wchar_t, 4096> searchPathBuffer;
                SymGetSearchPathW(static_cast<HMODULE>(GetCurrentProcess()), searchPathBuffer.data(), searchPathBuffer.size())) {
                std::wstring searchPath(searchPathBuffer.data());
                searchPath += L";" + g_unzippedSearchPaths;
                SymSetSearchPathW(static_cast<HMODULE>(GetCurrentProcess()), searchPath.c_str());
            }
        }

        g_context.initialize(info->ExceptionRecord->ExceptionAddress);

        text = crashlog::writeCrashlog(g_context, crashlogPath);

        if (g_symbolsInitialized) {
            SymCleanup(GetCurrentProcess());
            g_symbolsInitialized = false;
        }
    }

    // defined in crashlogWindow.cpp
    extern bool showCustomCrashlogWindow(std::string text, std::filesystem::path const& crashlogPath);

    if (!showCustomCrashlogWindow(text, crashlogPath)) {
        // if the window fails to show, we show a message box instead
        MessageBoxW(nullptr, utils::string::utf8ToWide(text).c_str(), L"Geometry Dash Crashed", MB_ICONERROR);
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
        std::error_code ec;
        std::filesystem::remove(lastCrashedFile, ec);
    }
    return true;
}

bool crashlog::didLastLaunchCrash() {
    return g_lastLaunchCrashed;
}

void crashlog::setupPlatformHandlerPost() {
    g_unzippedSearchPaths.clear();
    for (auto& mod : Loader::get()->getAllMods()) {
        g_unzippedSearchPaths.append(mod->getTempDir().native());
        g_unzippedSearchPaths.push_back(L';');
    }
}

std::filesystem::path crashlog::getCrashLogDirectory() {
    return dirs::getGeodeDir() / "crashlogs";
}
