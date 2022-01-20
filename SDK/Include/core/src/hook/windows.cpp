#include "windows.hpp"
#include "impl.hpp"

#include <Windows.h>

using namespace geode::core::hook;

namespace {
    LONG WINAPI handler(EXCEPTION_POINTERS* info) {
    #if defined(_WIN64)
        const void* ret = *reinterpret_cast<void**>(info->ContextRecord->Rsp);
        const void** current = reinterpret_cast<const void**>(&info->ContextRecord->Rip);
    #elif defined(_WIN32)
        const void* ret = *reinterpret_cast<void**>(info->ContextRecord->Esp);
        const void** current = reinterpret_cast<const void**>(&info->ContextRecord->Eip);
    #endif

        Exception exception = {
            info->ExceptionRecord->ExceptionAddress,
             ret,
             *current
        };
    

        if (HookManager::handler(exception)) {
            return EXCEPTION_CONTINUE_EXECUTION;
        }
        else {
            return EXCEPTION_CONTINUE_SEARCH;
        }
    }
}

bool WindowsX86::write_memory(void* to, const void* from, size_t size) {
    DWORD old;
    VirtualProtect(to, size, PAGE_EXECUTE_READWRITE, &old);
    auto res = WriteProcessMemory(GetCurrentProcess(), to, from, size, nullptr);
    VirtualProtect(to, size, old, &old);
    return res;
}

bool WindowsX86::read_memory(void* addr, void* to, size_t size) {
    return ReadProcessMemory(GetCurrentProcess(), addr, to, size, nullptr);
}

bool WindowsX86::initialize() {
    return AddVectoredExceptionHandler(true, handler);
}