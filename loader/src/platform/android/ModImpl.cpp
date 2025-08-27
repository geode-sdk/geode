#include <Geode/DefaultInclude.hpp>

#include <Geode/loader/Mod.hpp>
#include <loader/ModImpl.hpp>

using namespace geode::prelude;

template <typename T>
T findSymbolOrMangled(void* so, char const* name, char const* mangled) {
    auto res = reinterpret_cast<T>(dlsym(so, name));
    if (!res) {
        res = reinterpret_cast<T>(dlsym(so, mangled));
    }
    return res;
}

Result<> Mod::Impl::loadPlatformBinary() {
    auto so =
        dlopen(utils::string::pathToString(this->getBinaryPath()).c_str(), RTLD_LAZY);
    if (so) {
        if (m_platformInfo) {
            delete m_platformInfo;
        }
        m_platformInfo = new PlatformInfo{so};

        auto geodeImplicitEntry = findSymbolOrMangled<void(*)()>(so, "geodeImplicitEntry", "_Z17geodeImplicitEntryv");
        if (geodeImplicitEntry) {
            geodeImplicitEntry();
        }

        auto geodeCustomEntry = findSymbolOrMangled<void(*)()>(so, "geodeCustomEntry", "_Z15geodeCustomEntryv");
        if (geodeCustomEntry) {
            geodeCustomEntry();
        }

        return Ok();
    }
    std::string err = dlerror();
    log::error("Unable to load the SO: dlerror returned ({})", err);
    return Err("Unable to load the SO: dlerror returned (" + err + ")");
}
