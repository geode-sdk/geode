
#include <Geode/DefaultInclude.hpp>

#ifdef GEODE_IS_WINDOWS

    #include <Geode/loader/Mod.hpp>
    #include <Geode/modify/Modify.hpp>

USE_GEODE_NAMESPACE();

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
    (void)Mod::get()->patch(
        reinterpret_cast<void*>(geode::base::getCocos() + 0x19feec), toByteArray(&fixedErrorHandler)
    );
}

#endif
