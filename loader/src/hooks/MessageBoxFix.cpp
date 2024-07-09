
#include <Geode/DefaultInclude.hpp>

#ifdef GEODE_IS_WINDOWS

    #include <loader/LoaderImpl.hpp>
    #include <Geode/loader/Mod.hpp>
    #include <Geode/modify/Modify.hpp>

using namespace geode::prelude;

// for some reason RobTop uses MessageBoxW in his GLFW error handler.
// no one knows how this is possible (he passes char* to wchar_t*).
// so anyway, here's a fix for it

static void __cdecl fixedErrorHandler2(int code, char const* description) {
    log::error("GLFW Error {}: {}", code, description);
    MessageBoxA(
        nullptr,
        fmt::format(
            "GLFWError #{}: {}",
            code,
            description
        )
            .c_str(),
        "OpenGL Error",
        MB_ICONERROR
    );

    std::abort();
}

static void __cdecl fixedErrorHandler(CCEGLView*, int code, char const* description) {
    fixedErrorHandler2(code, description);
}

$execute {
    // updated for 2.206
    // check xrefs to "GLFWError #%d Happen, %s\n", now there's two functions with the same exact
    // behaviour, one is a member function though... 
    // hook them to call our own handler
    if (LoaderImpl::get()->isForwardCompatMode()) return;

#if GEODE_COMP_GD_VERSION == 22060
    const uintptr_t offset1 = 0x75d00; // member function in CCEGLView
    const uintptr_t offset2 = 0x75d60; // static function
    
    (void) Mod::get()->hook(
        reinterpret_cast<void*>(geode::base::getCocos() + offset1),
        fixedErrorHandler,
        "onGLFWError"
    );
    
    (void) Mod::get()->hook(
        reinterpret_cast<void*>(geode::base::getCocos() + offset2),
        fixedErrorHandler2,
        "onGLFWError2"
    );

#else
    #pragma message("Unsupported GD version!")
#endif
}

#endif
