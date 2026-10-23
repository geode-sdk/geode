#include <Geode/DefaultInclude.hpp>

#include <Geode/loader/Mod.hpp>
#include <loader/ModImpl.hpp>
#include <dlfcn.h>

using namespace geode::prelude;

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
        dlopen(utils::string::pathToString(this->getBinaryPath()).c_str(), RTLD_LAZY);
    if (dylib) {
        if (m_platformInfo) {
            delete m_platformInfo;
        }
        m_platformInfo = new PlatformInfo { dylib };

        auto geodeImplicitEntry = findSymbolOrMangled<void(*)()>(dylib, "geodeImplicitEntry", "_Z17geodeImplicitEntryv");
        if (geodeImplicitEntry) {
            geodeImplicitEntry();
        }

        auto geodeCustomEntry = findSymbolOrMangled<void(*)()>(dylib, "geodeCustomEntry", "_Z15geodeCustomEntryv");
        if (geodeCustomEntry) {
            geodeCustomEntry();
        }

        return Ok();
    }
    std::string err = (char const*)dlerror();
    return Err("Unable to load the DYLIB: dlerror returned (" + err + ")");
}

void Mod::Impl::addNativeBinariesPath(std::filesystem::path const& path) {
    // this takes advantage of dyld using already loaded binaries when loading relative shared libraries
    // however, this also means that the binaries are loaded, which could have some weird side effects
    // but if you could use dlopen (and thus control when libraries are loaded), then you wouldn't be using this, would you?

    for (const auto& entry : std::filesystem::directory_iterator(path)) {
        if (!entry.is_regular_file()) {
            continue;
        }

        auto& entry_path = entry.path();

        if (entry_path.extension() != ".dylib") {
            continue;
        }

        auto handle = dlopen(utils::string::pathToString(entry_path).c_str(), RTLD_LAZY);

        if (!handle) {
            auto err = dlerror();
            log::warn("failed to load native binary at {}: dlerror returned ({})", 
                entry_path, err
            );
            continue;
        }

        dlclose(handle);
    }
}