
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
    // updated for 2.204
    // check xrefs to "GLFWError #%d Happen, %s\n", now there's two functions with the same exact
    // behaviour, one is a member function though... call ds:MessageBoxW
    // patch MessageBoxW to MessageBoxA
    // geode::base::getCocos() + 0x122600 = MessageBoxA in .idata
    // geode::base::getCocos() + 0x1225DC = MessageBoxW in .idata
    if (LoaderImpl::get()->isForwardCompatMode()) return;

#if 0 // TODO: mat GEODE_COMP_GD_VERSION == 22040
    const uint32_t importedMessageBoxA = geode::base::getCocos() + 0x122600;

    ByteVector p = {
        static_cast<unsigned char>((importedMessageBoxA) & 0xff),
        static_cast<unsigned char>((importedMessageBoxA >> 8) & 0xff),
        static_cast<unsigned char>((importedMessageBoxA >> 16) & 0xff),
        static_cast<unsigned char>((importedMessageBoxA >> 24) & 0xff)};

    (void)Mod::get()->patch(reinterpret_cast<void*>(geode::base::getCocos() + 0xC75F9), p);
    (void)Mod::get()->patch(reinterpret_cast<void*>(geode::base::getCocos() + 0xc7651), p);
#else
    #pragma message("Unsupported GD version!")
#endif
}

#endif
