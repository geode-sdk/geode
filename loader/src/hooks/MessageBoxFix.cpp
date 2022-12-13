
#include <Geode/DefaultInclude.hpp>

#ifdef GEODE_IS_WINDOWS

#include <Geode/modify/InternalMacros.hpp>
#include <Geode/loader/Mod.hpp>

USE_GEODE_NAMESPACE();
using geode::core::meta::x86::Thiscall;

// for some reason RobTop uses MessageBoxW in his GLFW error handler.
// no one knows how this is possible (he passes char* to wchar_t*).
// so anyway, here's a fix for it

static auto CCEGLVIEW_CON_ADDR = reinterpret_cast<void*>(base::getCocos() + 0xc2860);

static void __cdecl fixedErrorHandler(int code, char const* description) {
    log::error("GLFW Error {}: {}", code, description);
    MessageBoxA(
        nullptr,
        fmt::format(
            "GLFWError #{}: {}\nPlease contact the "
            "Geode Development Team for more information.",
            code, description
        ).c_str(),
        "OpenGL Error", MB_ICONERROR
    );
}

static CCEGLView* CCEGLView_CCEGLView(CCEGLView* self) {
    // you will never have to make a manual hook with Geode again, they said
    // it will be fun, they said
    reinterpret_cast<CCEGLView*(__thiscall*)(CCEGLView*)>(CCEGLVIEW_CON_ADDR)(self);
    static auto p = Mod::get()->patch(
        reinterpret_cast<void*>(geode::base::getCocos() + 0x19feec),
        to_byte_array(&fixedErrorHandler)
    );
    return self;
}

$execute {
    (void)Mod::get()->addHook<&CCEGLView_CCEGLView, Thiscall>(
        "CCEGLView::CCEGLView", CCEGLVIEW_CON_ADDR
    );
}

#endif
