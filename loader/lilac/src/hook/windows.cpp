#include "windows.hpp"

#include "impl.hpp"

#include <Windows.h>
// temporary
#include <cassert>

using namespace lilac::hook;

namespace {
    LONG WINAPI handler(EXCEPTION_POINTERS* info) {
#if defined(_WIN64)
        void* ret = *reinterpret_cast<void**>(info->ContextRecord->Rsp);
        const void*& address = reinterpret_cast<const void*&>(info->ContextRecord->Rip);
#elif defined(_WIN32)
        void* ret = *reinterpret_cast<void**>(info->ContextRecord->Esp);
        const void*& address = reinterpret_cast<const void*&>(info->ContextRecord->Eip);
#endif

        Exception exception = { address, ret };

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
    auto result = WriteProcessMemory(GetCurrentProcess(), to, from, size, nullptr);
    VirtualProtect(to, size, old, &old);
    return result;
}

bool WindowsX86::initialize() {
    return AddVectoredExceptionHandler(true, handler);
}