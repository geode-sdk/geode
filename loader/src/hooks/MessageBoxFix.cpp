
#include <Geode/DefaultInclude.hpp>

#ifdef GEODE_IS_WINDOWS

    #include <loader/LoaderImpl.hpp>
    #include <Geode/loader/Mod.hpp>
    #include <Geode/modify/Modify.hpp>

using namespace geode::prelude;

// for some reason RobTop uses MessageBoxW in his GLFW error handler.
// no one knows how this is possible (he passes char* to wchar_t*).
// so anyway, here's a fix for it

static void __cdecl fixedErrorHandler(int code, char const* description) {
    log::error("GLFW Error {}: {}", code, description);
    MessageBoxA(
        nullptr,
        fmt::format(
            "GLFWError #{}: {}\nPlease contact the "
            "Geode Development Team for more information.",
            code,
            description
        )
            .c_str(),
        "OpenGL Error",
        MB_ICONERROR
    );
}

$execute {
    // updated for 2.206
    // check xrefs to "GLFWError #%d Happen, %s\n", now there's two functions with the same exact
    // behaviour, one is a member function though... call ds:MessageBoxW
    // patch MessageBoxW to MessageBoxA
    // geode::base::getCocos() + 0xe2670 = MessageBoxA in .idata
    // geode::base::getCocos() + 0xe26b8 = MessageBoxW in .idata
    if (LoaderImpl::get()->isForwardCompatMode()) return;

#if GEODE_COMP_GD_VERSION == 22060
    // in x64 these became rip-relative jmp & call respectively, instead of absolute calls,
    // so this code is a bit more complicated than it used to be

    const uintptr_t importedMessageBoxA = 0xe2670;
    
    const uintptr_t offset1 = 0x75d4a;
    const uint32_t rel1 = static_cast<uint32_t>(importedMessageBoxA - offset1 - 7);
    
    (void) Mod::get()->patch(reinterpret_cast<void*>(geode::base::getCocos() + offset1 + 3), geode::toByteArray(rel1));

    const uintptr_t offset2 = 0x75daf;
    const uint32_t rel2 = static_cast<uint32_t>(importedMessageBoxA - offset2 - 6);

    (void) Mod::get()->patch(reinterpret_cast<void*>(geode::base::getCocos() + offset2 + 2), geode::toByteArray(rel2));
#else
    #pragma message("Unsupported GD version!")
#endif
}

#endif
