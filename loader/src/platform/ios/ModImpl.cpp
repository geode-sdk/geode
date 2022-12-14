#include <Geode/DefaultInclude.hpp>

#ifdef GEODE_IS_IOS

    #include <Geode/loader/Mod.hpp>
    #include <loader/ModImpl.hpp>
    #include <dlfcn.h>

USE_GEODE_NAMESPACE();

template <typename T>
T findSymbolOrMangled(void* dylib, char const* name, char const* mangled) {
    auto res = reinterpret_cast<T>(dlsym(dylib, name));
    if (!res) {
        res = reinterpret_cast<T>(dlsym(dylib, mangled));
    }
    return res;
}

Result<> Mod::Impl::loadPlatformBinary() {
    auto dylib =
        dlopen((this->m_tempDirName / this->m_info.binaryName).string().c_str(), RTLD_LAZY);
    if (dylib) {
        if (this->m_platformInfo) {
            delete this->m_platformInfo;
        }
        this->m_platformInfo = new PlatformInfo { dylib };

        return Ok();
    }
    std::string err = (char const*)dlerror();
    return Err("Unable to load the DYLIB: dlerror returned (" + err + ")");
}

Result<> Mod::Impl::unloadPlatformBinary() {
    auto dylib = this->m_platformInfo->m_dylib;
    delete this->m_platformInfo;
    this->m_platformInfo = nullptr;
    if (dlclose(dylib) == 0) {
        return Ok();
    }
    else {
        return Err("Unable to free library");
    }
}

#endif
