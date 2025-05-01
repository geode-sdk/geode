#include <Geode/loader/IPC.hpp>
#include <Geode/loader/Log.hpp>
#include <loader/ModImpl.hpp>
#include <loader/LoaderImpl.hpp>
#include <Geode/utils/string.hpp>
#include <processenv.h>

using namespace geode::prelude;

#include <Psapi.h>

#include "gdTimestampMap.hpp"
std::string Loader::Impl::getGameVersion() {
    if (m_gdVersion.empty()) {
        auto dosHeader = reinterpret_cast<IMAGE_DOS_HEADER*>(geode::base::get());
        auto ntHeader = reinterpret_cast<PIMAGE_NT_HEADERS>(geode::base::get() + dosHeader->e_lfanew);
        auto timestamp = ntHeader->FileHeader.TimeDateStamp;
        m_gdVersion = timestampToVersion(timestamp);
    }
    return m_gdVersion;
}

bool Loader::Impl::userTriedToLoadDLLs() const {
    static std::unordered_set<std::string> KNOWN_MOD_DLLS {
        "betteredit-v4.0.5.dll",
        "betteredit-v4.0.5-min.dll",
        "betteredit-v4.0.3.dll",
        "betteredit.dll",
        "gdshare-v0.3.4.dll",
        "gdshare-v0.3.5.dll",
        "gdshare.dll",
        "hackpro.dll",
        "hackproldr.dll",
        "quickldr.dll",
        "minhook.x32.dll",
        "iconsave.dll",
        "menuanim.dll",
        "volumecontrol.dll",
        "customsplash.dll",
        "scrollanyinput-v1.1.dll",
        "alttabfix-v1.0.dll",
        "sceneswitcher-v1.1.dll",
        "gdantialiasing.dll",
        "textureldr.dll",
        "run-info.dll",
        "roastedmarshmellow.dll",
        "toastedmarshmellow.dll",
        "gdh.dll",
        "mimalloc.dll",
    };

    bool triedToLoadDLLs = false;

    // Check for .DLLs in mods dir
    if (auto files = file::readDirectory(dirs::getModsDir(), true)) {
        for (auto& file : files.unwrap()) {
            if (file.extension() == ".dll") {
                triedToLoadDLLs = true;
            }
        }
    }

    // Check all loaded DLLs in the process
    std::array<HMODULE, 1024> mods;
    DWORD needed;
    auto process = GetCurrentProcess();

    if (EnumProcessModules(process, mods.data(), mods.size(), &needed)) {
        for (auto i = 0; i < (needed / sizeof(HMODULE)); i++) {
            std::array<char, MAX_PATH> modName;
            if (GetModuleFileNameExA(process, mods[i], modName.data(), modName.size())) {
                if (KNOWN_MOD_DLLS.count(string::trim(string::toLower(
                    std::filesystem::path(modName.data()).filename().string()
                )))) {
                    triedToLoadDLLs = true;
                }
            }
        }
    }

    return triedToLoadDLLs;
}

void Loader::Impl::addNativeBinariesPath(std::filesystem::path const& path) {
    // https://learn.microsoft.com/en-us/windows/win32/api/libloaderapi/nf-libloaderapi-adddlldirectory#remarks
    static auto runOnce = [] {
        SetDefaultDllDirectories(LOAD_LIBRARY_SEARCH_DEFAULT_DIRS);
        return 0;
    }();
    AddDllDirectory(path.wstring().c_str());
}

bool Loader::Impl::supportsLaunchArguments() const {
    return true;
}

std::string Loader::Impl::getLaunchCommand() const {
    return GetCommandLineA();
}
