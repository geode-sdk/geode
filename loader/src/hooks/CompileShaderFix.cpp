#include <Geode/Geode.hpp>

// TODO: this affects every platform :P, but i cant be bothered rn
#ifdef GEODE_IS_WINDOWS

#include <loader/LoaderImpl.hpp>

using namespace geode::prelude;

$execute {
    if (LoaderImpl::get()->isForwardCompatMode()) return;

#if GEODE_COMP_GD_VERSION == 22074
    // patch an abort() call to "return false;" in CCGLProgram::compileShader
    // for some reason cocos only properly returns false on winRT, everywhere
    // else it just closes the whole game

    auto addr = reinterpret_cast<uintptr_t>(
        GetProcAddress(
            GetModuleHandle("libcocos2d.dll"), "?compileShader@CCGLProgram@cocos2d@@AEAA_NPEAIIPEBD@Z"
        )
    ) + 0xbb;

    (void) Mod::get()->patch(reinterpret_cast<void*>(addr), {
        0x31, 0xc0, // xor eax, eax
        0xeb, 0x07 // jmp +7 (to a nearby ret)
    });
#else
    #pragma message("Unsupported GD version!")
#endif
};

#endif
