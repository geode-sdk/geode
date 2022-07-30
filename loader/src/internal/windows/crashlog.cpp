#define WIN32_LEAN_AND_MEAN

#include <Geode/DefaultInclude.hpp>

#ifdef GEODE_IS_WINDOWS

#include <Geode/Geode.hpp>

#include "../crashlog.hpp"
#include <Windows.h>
#include <fstream>
#include <iostream>
#include <string>
#include <errhandlingapi.h>
#include <iomanip>
#include <ctime>
#include <chrono>
#include <DbgHelp.h>

USE_GEODE_NAMESPACE();

static bool g_lastLaunchCrashed = false;
static bool g_symbolsInitialized = false;

static std::string getDateString(bool filesafe) {
    const auto now = std::time(nullptr);
	const auto tm = *std::localtime(&now);
    std::ostringstream oss;
    if (filesafe) {
        oss << std::put_time(&tm, "%F_%H-%M-%S");
    } else {
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

static const char* getExceptionCodeString(DWORD code) {
	#define EXP_STR(c) case c: return #c
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

static HMODULE handleFromAddress(const void* addr) {
    HMODULE module = nullptr;
    GetModuleHandleEx(
        GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS |
		GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT,
		(LPCTSTR)addr,
        &module
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

static void printAddr(
    std::ostream& stream, const void* addr, bool fullPath = true
) {
	HMODULE module = nullptr;

	if (GetModuleHandleEx(
        GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS |
		GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT,
		(LPCTSTR)addr,
        &module
    )) {
        // calculate base + [address]
		const auto diff = as<uintptr_t>(addr) - as<uintptr_t>(module);
		stream
            << getModuleName(module, fullPath) << " + "
            << std::hex << diff << std::dec;

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
                proc,
                static_cast<DWORD64>(as<uintptr_t>(addr)),
                &displacement,
                symbolInfo
            )) {
                stream
                    << " ("
                    << std::string(symbolInfo->Name, symbolInfo->NameLen)
                    << " + " << displacement;
                
                IMAGEHLP_LINE64 line;
                line.SizeOfStruct = sizeof(IMAGEHLP_LINE64);

                DWORD displacement2;

                if (SymGetLineFromAddr64(
                    proc,
                    static_cast<DWORD64>(as<uintptr_t>(addr)),
                    &displacement2,
                    &line
                )) {
                    stream
                        << " | " << line.FileName
                        << ":" << line.LineNumber;
                }

                stream
                    << ")";
            }
        }

	} else {
		stream << addr;
	}
}

// https://stackoverflow.com/a/50208684/9124836
static void walkStack(std::ostream& stream, PCONTEXT context) {
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
			IMAGE_FILE_MACHINE_I386,
			process,
			thread,
			&stack,
			context,
			nullptr,
			SymFunctionTableAccess64,
			SymGetModuleBase64,
			nullptr
        )) break;

		stream << " - ";
        printAddr(stream, reinterpret_cast<void*>(stack.AddrPC.Offset));
        stream << std::endl;
    }
}

static void printRegisters(std::ostream& stream, PCONTEXT context) {
    stream
        << std::hex
        << "EAX: " << context->Eax << "\n"
        << "EBX: " << context->Ebx << "\n"
        << "ECX: " << context->Ecx << "\n"
        << "EDX: " << context->Edx << "\n"
        << "EBP: " << context->Ebp << "\n"
        << "ESP: " << context->Esp << "\n"
        << "EDI: " << context->Edi << "\n"
        << "ESI: " << context->Esi << "\n"
        << "EIP: " << context->Eip << "\n"
        << std::dec;
}

static void printInfo(
    std::ostream& stream, LPEXCEPTION_POINTERS info, Mod* faultyMod
) {
    stream
	    << "Faulty Module: " << getModuleName(
            handleFromAddress(info->ExceptionRecord->ExceptionAddress),
            true
        ) << "\n"
        << "Faulty Mod: " << (faultyMod ? faultyMod->getID() : "<Unknown>")
        << "\n"
	    << "Exception Code: "
        << std::hex << info->ExceptionRecord->ExceptionCode
        << " (" << getExceptionCodeString(info->ExceptionRecord->ExceptionCode)
        << ")" << std::dec << "\n"
	    << "Exception Flags: "
        << info->ExceptionRecord->ExceptionFlags << "\n"
	    << "Exception Address: " << info->ExceptionRecord->ExceptionAddress
        << " (";
	printAddr(stream, info->ExceptionRecord->ExceptionAddress, false);
    stream
        << ")" << "\n"
	    << "Number Parameters: "
        << info->ExceptionRecord->NumberParameters << "\n";
}

static void printGeodeInfo(std::ostream& stream) {
    stream
        << "Loader Version: "
        << Loader::get()->getVersion().toString() << " "
        << Loader::get()->getVersionType() << "\n"
        << "Installed mods: " << Loader::get()->getAllMods().size() << "\n"
        << "Failed mods: " << Loader::get()->getFailedMods().size() << "\n";
}

static void printMods(std::ostream& stream) {
    auto mods = Loader::get()->getAllMods();
    if (!mods.size()) {
        stream << "<None>\n";
    }
    for (auto& mod : mods) {
        stream
            << mod->getID() << " | "
            << mod->getDeveloper() << " | "
            << mod->getVersion().toString() << " | "
            << (mod->isEnabled() ? "Enabled" : "Disabled") << " | "
            << (mod->isLoaded() ? "Loaded" : "Unloaded") << " | "
            << "\n";
    }
}

static LONG WINAPI exceptionHandler(LPEXCEPTION_POINTERS info) {
    // make sure crashlog directory exists
    file_utils::createDirectoryAll(crashlog::getCrashLogDirectory());

    // add a file to let Geode know on next launch that it crashed previously
    // this could also be done by saving a loader setting or smth but eh.
    file_utils::writeBinary(
        crashlog::getCrashLogDirectory() + "/last-crashed", {}
    );

    SymSetOptions(SYMOPT_UNDNAME | SYMOPT_DEFERRED_LOADS | SYMOPT_LOAD_LINES);

    std::ofstream file;
    file.open(
        crashlog::getCrashLogDirectory() + "/" + getDateString(true) + ".log",
        std::ios::app
    );

    // init symbols so we can get some juicy debug info
    g_symbolsInitialized = SymInitialize(
        static_cast<HMODULE>(GetCurrentProcess()),
        nullptr,
        true
    );

    auto faultyMod = modFromAddress(info->ExceptionRecord->ExceptionAddress);

    file
        << getDateString(false) << "\n"
	    << std::showbase
	    << "Whoopsies! An exception has occurred while running Geode.\n";
    
    if (faultyMod) {
        file
            << "It appears that the crash occurred while executing code from "
            << "the \"" << faultyMod->getID() << "\" mod. "
            << "Please submit this crash report to its developer ("
            << faultyMod->getDeveloper() << ") for assistance.\n";
    }
    
    // geode info
    file << "\n== Geode Information ==\n";
    printGeodeInfo(file);
    
    // exception info
    file << "\n== Exception Information ==\n";
    printInfo(file, info, faultyMod);
    
    // registers
    file << "\n== Register States ==\n";
    printRegisters(file, info->ContextRecord);
    
    // stack trace
    file << "\n== Stack Trace ==\n";
    walkStack(file, info->ContextRecord);

    // mods
    file << "\n== Installed Mods ==\n";
    printMods(file);

    return EXCEPTION_CONTINUE_SEARCH;
}

bool crashlog::setupPlatformHandler() {
    SetUnhandledExceptionFilter(exceptionHandler);
    auto lastCrashedFile = crashlog::getCrashLogDirectory() + "/last-crashed";
    if (ghc::filesystem::exists(lastCrashedFile)) {
        g_lastLaunchCrashed = true;
        try { ghc::filesystem::remove(lastCrashedFile); } catch(...) {}
    }
    return true;
}

bool crashlog::didLastLaunchCrash() {
    return g_lastLaunchCrashed;
}

std::string crashlog::getCrashLogDirectory() {
    static auto dir = (
        Loader::get()->getGeodeDirectory() / "crashlogs"
    ).string();
    return dir;
}

#endif
