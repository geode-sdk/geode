#include <Geode/Geode.hpp>

// TODO: this affects every platform :P, but i cant be bothered rn
#include <loader/LoaderImpl.hpp>

using namespace geode::prelude;

$execute {
    if (LoaderImpl::get()->isForwardCompatMode()) return;

#if GEODE_COMP_GD_VERSION == 22074
    // patch an abort() call to "return false;" in CCGLProgram::compileShader
    // for some reason cocos only properly returns false on winRT, everywhere
    // else it just closes the whole game

#ifdef GEODE_IS_WINDOWS
    auto addr = reinterpret_cast<uintptr_t>(
        GetProcAddress(
            GetModuleHandle("libcocos2d.dll"), "?compileShader@CCGLProgram@cocos2d@@AEAA_NPEAIIPEBD@Z"
        )
    ) + 0xbb;

    (void) Mod::get()->patch(reinterpret_cast<void*>(addr), {
        0x31, 0xc0, // xor eax, eax
        0xeb, 0x07 // jmp +7 (to a nearby ret)
    });
#elif defined(GEODE_IS_ANDROID64)
    auto addr = reinterpret_cast<uintptr_t>(
        dlsym(RTLD_DEFAULT, "_ZN7cocos2d11CCGLProgram13compileShaderEPjjPKc")
    ) + 0x74;

    (void) Mod::get()->patch(reinterpret_cast<void*>(addr), {
        0x1f, 0x20, 0x03, 0xd5 // nop (skip if statement)
    });
#elif defined(GEODE_IS_ANDROID32)
    auto addr = reinterpret_cast<uintptr_t>(
        dlsym(RTLD_DEFAULT, "_ZN7cocos2d11CCGLProgram13compileShaderEPjjPKc")
    );
    log::info("Patching CCGLProgram::compileShader at 0x{:x} + 0x44", addr);
    addr += 0x44;

    (void) Mod::get()->patch(reinterpret_cast<void*>(addr), {
        0x14, 0xe0 // b +2c (skip if statement)
    });
#endif
#else
    #pragma message("Unsupported GD version!")
#endif
};
