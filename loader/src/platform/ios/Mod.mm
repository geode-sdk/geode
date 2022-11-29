
#ifdef GEODE_IS_IOS

    #include <Geode/loader/Mod.hpp>
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

Result<> Mod::loadPlatformBinary() {
    auto dylib =
        dlopen((this->m_tempDirName / this->m_info.m_binaryName).string().c_str(), RTLD_LAZY);
    if (dylib) {
        this->m_implicitLoadFunc =
            findSymbolOrMangled<geode_load>(dylib, "geode_implicit_load", "_geode_implicit_load");

        if (!this->m_implicitLoadFunc) {
            return Err("Unable to find mod entry point");
        }
        if (this->m_platformInfo) {
            delete this->m_platformInfo;
        }
        this->m_platformInfo = new PlatformInfo { dylib };

        return Ok();
    }
    std::string err = (char const*)dlerror();
    return Err("Unable to load the DYLIB: dlerror returned (" + err + ")");
}

Result<> Mod::unloadPlatformBinary() {
    auto dylib = this->m_platformInfo->m_dylib;
    delete this->m_platformInfo;
    this->m_platformInfo = nullptr;
    if (dlclose(dylib) == 0) {
        this->m_implicitLoadFunc = nullptr;
        return Ok();
    }
    else {
        return Err("Unable to free library");
    }
}

#endif
