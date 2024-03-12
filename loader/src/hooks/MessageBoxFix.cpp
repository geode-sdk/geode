#include <Geode/DefaultInclude.hpp>

#ifdef GEODE_IS_WINDOWS

#include <loader/LoaderImpl.hpp>
#include <Geode/loader/Mod.hpp>
#include <Geode/modify/Modify.hpp>

using namespace geode::prelude;

static void __cdecl fixedErrorHandler(int code, const char* description) {
    log::error("GLFW Error {}: {}", code, description);
    MessageBoxA(
        nullptr,
        fmt::format(
            "GLFWError #{}: {}\nPlease contact the "
            "Geode Development Team for more information.",
            code,
            description
        ).c_str(),
        "OpenGL Error",
        MB_ICONERROR
    );
}

$execute {
    if (LoaderImpl::get()->isForwardCompatMode())
        return;

    const uint32_t importedMessageBoxA = geode::base::getCocos() + 0x122600;

    ByteVector p;
    for (int i = 0; i < 4; ++i) {
        p.push_back(static_cast<unsigned char>((importedMessageBoxA >> (8 * i)) & 0xff));
    }

    const uint32_t baseAddress = geode::base::getCocos();
    (void)Mod::get()->patch(reinterpret_cast<void*>(baseAddress + 0xC75F9), p);
    (void)Mod::get()->patch(reinterpret_cast<void*>(baseAddress + 0xC7651), p);
}

#endif
