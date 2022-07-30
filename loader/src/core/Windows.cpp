#include "Windows.hpp"
#include "Core.hpp"

#ifdef GEODE_IS_WINDOWS

#include <Windows.h>

using namespace geode::core::hook;
using namespace geode::core::impl;

namespace {
    LONG WINAPI signalHandler(EXCEPTION_POINTERS* info) {

	#if defined(_WIN64)
        auto current = reinterpret_cast<void*>(info->ContextRecord->Rip);
    #elif defined(_WIN32)
        auto current = reinterpret_cast<void*>(info->ContextRecord->Eip);
    #endif

        // handleContext(reinterpret_cast<void*>(info), current);

        return EXCEPTION_CONTINUE_EXECUTION;
    }
}

void* Windows::allocateVM(size_t size) {
	return VirtualAlloc(
		nullptr, size,
		MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE
	);
}

std::vector<std::byte> Windows::jump(void* from, void* to) {
	std::vector<std::byte> ret;
    ret.resize(5, std::byte(0u));
	ret[0] = std::byte(0xe9);
    
    // target - src - 5

    intptr_t offset = reinterpret_cast<intptr_t>(to) - reinterpret_cast<intptr_t>(from) - 5;
    std::byte data[4];
    std::memcpy(data, &offset, 4);
    ret[1] = data[0];
    ret[2] = data[1];
    ret[3] = data[2];
    ret[4] = data[3];

	return ret;
}

bool Windows::enableSingleStep(void* vcontext) {
	auto info = reinterpret_cast<EXCEPTION_POINTERS*>(vcontext);
#if defined(_WIN64)
    info->ContextRecord->RFlags |= ((QWORD)0x100);
#elif defined(_WIN32)
    info->ContextRecord->EFlags |= ((DWORD)0x100);
#endif
	return true;
}

bool Windows::disableSingleStep(void* vcontext) {
	auto info = reinterpret_cast<EXCEPTION_POINTERS*>(vcontext);
#if defined(_WIN64)
    info->ContextRecord->RFlags &= ~((QWORD)0x100);
#elif defined(_WIN32)
    info->ContextRecord->EFlags &= ~((DWORD)0x100);
#endif
	return true;
}

bool Windows::writeMemory(void* to, void* from, size_t size) {
    DWORD old;
    VirtualProtect(to, size, PAGE_EXECUTE_READWRITE, &old);
    auto res = WriteProcessMemory(GetCurrentProcess(), to, from, size, nullptr);
    VirtualProtect(to, size, old, &old);
    return res;
}

bool Windows::initialize() {
    // return AddVectoredExceptionHandler(true, signalHandler);
    return true;
}

#endif
