#include <Geode/utils/platform.hpp>

#ifdef GEODE_IS_IOS

#include <Geode/Geode.hpp>
#include <dlfcn.h>

USE_GEODE_NAMESPACE();

template<typename T>
T findSymbolOrMangled(void* dylib, const char* name, const char* mangled) {
	auto res = reinterpret_cast<T>(dlsym(dylib, name));
	if (!res) {
		res = reinterpret_cast<T>(dlsym(dylib,mangled));
	}
	return res;
}

Result<> Mod::loadPlatformBinary() {
	auto dylib = dlopen((this->m_tempDirName / this->m_info.m_binaryName).string().c_str(), RTLD_LAZY);
	if (dylib) {
		this->m_implicitLoadFunc = findSymbolOrMangled<geode_load>(dylib, "geode_implicit_load", "_geode_implicit_load");
		this->m_loadFunc = findSymbolOrMangled<geode_load>(dylib, "geode_load", "_geode_load");
		this->m_unloadFunc = findSymbolOrMangled<geode_unload>(dylib, "geode_unload", "_geode_unload");
		this->m_enableFunc = findSymbolOrMangled<geode_enable>(dylib, "geode_enable", "_geode_enable");
		this->m_disableFunc = findSymbolOrMangled<geode_disable>(dylib, "geode_disable", "_geode_disable");
		this->m_saveDataFunc = findSymbolOrMangled<geode_save_data>(dylib, "geode_save_data", "_geode_save_data");
		this->m_loadDataFunc = findSymbolOrMangled<geode_load_data>(dylib, "geode_load_data", "_geode_load_data");
		this->m_settingUpdatedFunc = findSymbolOrMangled<geode_setting_updated>(dylib, "geode_setting_updated", "_geode_setting_updated");

		if (!this->m_implicitLoadFunc && !this->m_loadFunc) {
			return Err<>("Unable to find mod entry point (lacking both implicit & explicit definition)");
		}

		if (this->m_platformInfo) {
			delete this->m_platformInfo;
		}
		this->m_platformInfo = new PlatformInfo { dylib };

		return Ok<>();
	}
	std::string err = (char const*)dlerror();
	return Err<>("Unable to load the DYLIB: dlerror returned (" + err + ")");
}

Result<> Mod::unloadPlatformBinary() {
	auto dylib = this->m_platformInfo->m_dylib;
	delete this->m_platformInfo;
	this->m_platformInfo = nullptr;
	if (dlclose(dylib) == 0) {
		this->m_unloadFunc = nullptr;
		this->m_loadFunc = nullptr;
		this->m_implicitLoadFunc = nullptr;
		this->m_enableFunc = nullptr;
		this->m_disableFunc = nullptr;
		this->m_saveDataFunc = nullptr;
		this->m_loadDataFunc = nullptr;
		this->m_settingUpdatedFunc = nullptr;
		return Ok<>();
	} else {
		return Err<>("Unable to free library");
	}
}

#endif
