#include <Geode/DefaultInclude.hpp>

#ifdef GEODE_IS_ANDROID

#include <Geode/loader/Mod.hpp>
#include <loader/ModImpl.hpp>

using namespace geode::prelude;

Result<> Mod::Impl::loadPlatformBinary() {
    auto so = dlopen((m_tempDirName / m_info.binaryName()).string().c_str(), RTLD_LAZY);
    if (so) {
        if (m_platformInfo) {
            delete m_platformInfo;
        }
        m_platformInfo = new PlatformInfo{so};

        return Ok();
    }
    std::string err = dlerror();
    return Err("Unable to load the SO: dlerror returned (" + err + ")");
}

Result<> Mod::Impl::unloadPlatformBinary() {
    auto so = m_platformInfo->m_so;
    delete m_platformInfo;
    m_platformInfo = nullptr;
    if (dlclose(so) == 0) {
        return Ok();
    }
    else {
        return Err("Unable to free library");
    }
}

#endif
