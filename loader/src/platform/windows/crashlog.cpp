#define WIN32_LEAN_AND_MEAN

#include <Geode/DefaultInclude.hpp>

#ifdef GEODE_IS_WINDOWS

#include <crashlog.hpp>
#include <Geode/loader/Dirs.hpp>
#include <Geode/loader/Loader.hpp>
#include <Geode/loader/Mod.hpp>
#include <DbgHelp.h>
#include <Geode/utils/casts.hpp>
#include <Geode/utils/file.hpp>
#include <Windows.h>
#include <chrono>
#include <ctime>
#include <errhandlingapi.h>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>
#include <fmt/core.h>
#include "ehdata_structs.hpp"

using namespace geode::prelude;

static bool g_lastLaunchCrashed = false;
static bool g_symbolsInitialized = false;

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
    return ghc::filesystem::path(buffer).filename().string();
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

static void printAddr(std::ostream& stream, void const* addr, bool fullPath = true) {
    HMODULE module = nullptr;

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

            auto proc = GetCurrentProcess();

            if (SymFromAddr(
                    proc, static_cast<DWORD64>(reinterpret_cast<uintptr_t>(addr)), &displacement,
                    symbolInfo
                )) {
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
    }
}

// https://stackoverflow.com/a/50208684/9124836
static std::string getStacktrace(PCONTEXT context) {
    std::stringstream stream;
    STACKFRAME64 stack;
    memset(&stack, 0, sizeof(STACKFRAME64));

    auto process = GetCurrentProcess();
    auto thread = GetCurrentThread();
    stack.AddrPC.Offset = context->Eip;
    stack.AddrPC.Mode = AddrModeFlat;
    stack.AddrStack.Offset = context->Esp;
    stack.AddrStack.Mode = AddrModeFlat;
    stack.AddrFrame.Offset = context->Ebp;
    stack.AddrFrame.Mode = AddrModeFlat;

    // size_t frame = 0;
    while (true) {
        if (!StackWalk64(
                IMAGE_FILE_MACHINE_I386, process, thread, &stack, context, nullptr,
                SymFunctionTableAccess64, SymGetModuleBase64, nullptr
            ))
            break;

        stream << " - ";
        printAddr(stream, reinterpret_cast<void*>(stack.AddrPC.Offset));
        stream << std::endl;
    }
    return stream.str();
}

static std::string getRegisters(PCONTEXT context) {
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
}

static std::string getInfo(LPEXCEPTION_POINTERS info, Mod* faultyMod) {
    std::stringstream stream;

    if (info->ExceptionRecord->ExceptionCode == EH_EXCEPTION_NUMBER) {
        // This executes when a C++ exception was thrown and not handled.
        // https://devblogs.microsoft.com/oldnewthing/20100730-00/?p=13273

        // since you can throw virtually anything, we need to figure out if it's an std::exception* or not
        bool isStdException = false;

        auto* exceptionRecord = info->ExceptionRecord;
        auto exceptionObject = exceptionRecord->ExceptionInformation[1];

        auto* throwInfo = reinterpret_cast<_ThrowInfo*>(exceptionRecord->ExceptionInformation[2]);
        auto* catchableTypeArray = reinterpret_cast<_CatchableTypeArray*>(throwInfo->pCatchableTypeArray);
        auto ctaSize = catchableTypeArray->nCatchableTypes;

        for (int i = 0; i < ctaSize; i++) {
            auto* catchableType = reinterpret_cast<_CatchableType*>(catchableTypeArray->arrayOfCatchableTypes[i]);
            auto* ctDescriptor = reinterpret_cast<_TypeDescriptor*>(catchableType->pType);
            const char* classname = ctDescriptor->name;

            if (strcmp(classname, ".?AVexception@std@@") == 0) {
                isStdException = true;
                break;
            }
        }

        if (isStdException) {
            std::exception* excObject = reinterpret_cast<std::exception*>(exceptionObject);
            // stream << "C++ Exception Type: " << typeid(excObject).name() << "\n"; // always const std::exception *
            stream << "C++ Exception: " << excObject->what() << "\n";
        } else {
            stream << "C++ Exception: <Unknown type>\n";
        }

        stream << "Faulty Mod: " << (faultyMod ? faultyMod->getID() : "<Unknown>") << "\n";
    } else {
        stream << "Faulty Module: "
            << getModuleName(handleFromAddress(info->ExceptionRecord->ExceptionAddress), true)
            << "\n"
            << "Faulty Mod: " << (faultyMod ? faultyMod->getID() : "<Unknown>") << "\n"
            << "Exception Code: " << std::hex << info->ExceptionRecord->ExceptionCode << " ("
            << getExceptionCodeString(info->ExceptionRecord->ExceptionCode) << ")" << std::dec
            << "\n"
            << "Exception Flags: " << info->ExceptionRecord->ExceptionFlags << "\n"
            << "Exception Address: " << info->ExceptionRecord->ExceptionAddress << " (";
        printAddr(stream, info->ExceptionRecord->ExceptionAddress, false);
        stream << ")"
            << "\n"
            << "Number Parameters: " << info->ExceptionRecord->NumberParameters << "\n";
    }
    return stream.str();
}

static LONG WINAPI exceptionHandler(LPEXCEPTION_POINTERS info) {

    SymSetOptions(SYMOPT_UNDNAME | SYMOPT_DEFERRED_LOADS | SYMOPT_LOAD_LINES);

    // init symbols so we can get some juicy debug info
    g_symbolsInitialized = SymInitialize(static_cast<HMODULE>(GetCurrentProcess()), nullptr, true);

    auto faultyMod = modFromAddress(info->ExceptionRecord->ExceptionAddress);

    auto text = crashlog::writeCrashlog(faultyMod, getInfo(info, faultyMod), getStacktrace(info->ContextRecord), getRegisters(info->ContextRecord));
    
    // show message box on debug mode
    #ifdef GEODE_DEBUG
    MessageBoxA(nullptr, text.c_str(), "Geode Crashed", MB_ICONERROR);
    #endif

    return EXCEPTION_CONTINUE_SEARCH;
}

static LONG WINAPI exceptionHandlerDummy(LPEXCEPTION_POINTERS info) {
    SetUnhandledExceptionFilter(exceptionHandler);
    return EXCEPTION_CONTINUE_SEARCH;
}

bool crashlog::setupPlatformHandler() {
    // for some reason, on exceptions windows seems to clear SetUnhandledExceptionFilter
    // so we attach a VE handler (which runs *earlier*) and inside set our crash handler
    AddVectoredExceptionHandler(0, exceptionHandlerDummy);
    SetUnhandledExceptionFilter(exceptionHandler);

    auto lastCrashedFile = crashlog::getCrashLogDirectory() / "last-crashed";
    if (ghc::filesystem::exists(lastCrashedFile)) {
        g_lastLaunchCrashed = true;
        try {
            ghc::filesystem::remove(lastCrashedFile);
        }
        catch (...) {
        }
    }
    return true;
}

bool crashlog::didLastLaunchCrash() {
    return g_lastLaunchCrashed;
}

ghc::filesystem::path crashlog::getCrashLogDirectory() {
    return dirs::getGeodeDir() / "crashlogs";
}

#endif
