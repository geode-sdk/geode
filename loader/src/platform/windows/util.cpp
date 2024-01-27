#include <Geode/DefaultInclude.hpp>

using namespace geode::prelude;

#include <Geode/loader/Dirs.hpp>
#include "nfdwin.hpp"
#include <ghc/fs_fwd.hpp>
#include <Windows.h>
#include <iostream>
#include <ShlObj.h>
#include <shlwapi.h>
#include <shobjidl.h>
#include <sstream>
#include <Geode/utils/web.hpp>
#include <Geode/utils/cocos.hpp>

#include <filesystem>

bool utils::clipboard::write(std::string const& data) {
    if (!OpenClipboard(nullptr)) return false;
    if (!EmptyClipboard()) {
        CloseClipboard();
        return false;
    }

    HGLOBAL hg = GlobalAlloc(GMEM_MOVEABLE, data.size() + 1);

    if (!hg) {
        CloseClipboard();
        return false;
    }

    auto dest = GlobalLock(hg);

    if (!dest) {
        CloseClipboard();
        return false;
    }

    memcpy(dest, data.c_str(), data.size() + 1);

    GlobalUnlock(hg);

    SetClipboardData(CF_TEXT, hg);
    CloseClipboard();

    GlobalFree(hg);

    return true;
}

std::string utils::clipboard::read() {
    if (!OpenClipboard(nullptr)) return "";

    HANDLE hData = GetClipboardData(CF_TEXT);
    if (hData == nullptr) {
        CloseClipboard();
        return "";
    }

    char* pszText = static_cast<char*>(GlobalLock(hData));
    if (pszText == nullptr) {
        CloseClipboard();
        return "";
    }

    std::string text(pszText);

    GlobalUnlock(hData);
    CloseClipboard();

    return text;
}

bool utils::file::openFolder(ghc::filesystem::path const& path) {
    auto success = false;
    if (CoInitializeEx(nullptr, COINIT_MULTITHREADED) == S_OK) {
        if (auto id = ILCreateFromPathW(path.wstring().c_str())) {
            ghc::filesystem::path selectPath = path / ".";
            std::error_code whatever;
            if (!ghc::filesystem::is_directory(path, whatever)) {
                selectPath = path;
            }
            auto selectEntry = ILCreateFromPathW(selectPath.wstring().c_str());
            if (SHOpenFolderAndSelectItems(id, 1, (PCUITEMID_CHILD_ARRAY)(&selectEntry), 0) == S_OK) {
                success = true;
            }
            ILFree(id);
        }
        CoUninitialize();
    }
    return success;
}

Result<ghc::filesystem::path> utils::file::pickFile(
    file::PickMode mode, file::FilePickOptions const& options
) {
    #define TURN_INTO_NFDMODE(mode) \
        case file::PickMode::mode: nfdMode = NFDMode::mode; break;

    NFDMode nfdMode;
    switch (mode) {
        TURN_INTO_NFDMODE(OpenFile);
        TURN_INTO_NFDMODE(SaveFile);
        TURN_INTO_NFDMODE(OpenFolder);
        default: return Err<std::string>("Unknown open mode");
    }
    ghc::filesystem::path path;
    GEODE_UNWRAP(nfdPick(nfdMode, options, &path));
    return Ok(path);
}

GEODE_DLL void file::pickFile(
    PickMode mode, FilePickOptions const& options,
    MiniFunction<void(ghc::filesystem::path)> callback,
    MiniFunction<void()> failed
) {
    auto result = file::pickFile(mode, options);

    if (result.isOk()) {
        callback(std::move(result.unwrap()));
    } else {
        failed();
    }
}

Result<std::vector<ghc::filesystem::path>> utils::file::pickFiles(
    file::FilePickOptions const& options
) {
    std::vector<ghc::filesystem::path> paths;
    GEODE_UNWRAP(nfdPick(NFDMode::OpenFolder, options, &paths));
    return Ok(paths);
}

GEODE_DLL void file::pickFiles(
    FilePickOptions const& options,
    MiniFunction<void(std::vector<ghc::filesystem::path>)> callback,
    MiniFunction<void()> failed
) {
    auto result = file::pickFiles(options);

    if (result.isOk()) {
        callback(std::move(result.unwrap()));
    } else {
        failed();
    }
}

void utils::web::openLinkInBrowser(std::string const& url) {
    ShellExecuteA(0, 0, url.c_str(), 0, 0, SW_SHOW);
}

CCPoint cocos::getMousePos() {
    auto* director = CCDirector::get();
    auto* gl = director->getOpenGLView();
    auto winSize = director->getWinSize();
    auto frameSize = gl->getFrameSize();
    auto mouse = gl->getMousePosition() / frameSize;
    return ccp(mouse.x, 1.f - mouse.y) * winSize;
}

ghc::filesystem::path dirs::getGameDir() {
    // only fetch the path once, since ofc it'll never change
    // throughout the execution
    static const auto path = [] {
        std::array<WCHAR, MAX_PATH> buffer;
        GetModuleFileNameW(NULL, buffer.data(), MAX_PATH);

        const ghc::filesystem::path path(buffer.data());
        return std::filesystem::weakly_canonical(path.parent_path().wstring()).wstring();
    }();

    return path;
}

ghc::filesystem::path dirs::getSaveDir() {
    // only fetch the path once, since ofc it'll never change
    // throughout the execution
    static const auto path = [] {
        std::array<WCHAR, MAX_PATH + 1> buffer;
        GetModuleFileNameW(NULL, buffer.data(), MAX_PATH + 1);

        auto executablePath = ghc::filesystem::path(buffer.data());
        auto executableName = executablePath.filename().wstring();
        executableName = executableName.substr(0, executableName.find_last_of(L"."));

        if (SHGetFolderPathW(NULL, CSIDL_LOCAL_APPDATA, NULL, SHGFP_TYPE_CURRENT, buffer.data()) >= 0) {
            auto appdataPath = ghc::filesystem::path(buffer.data());
            auto savePath = appdataPath / executableName;

            if (SHCreateDirectoryExW(NULL, savePath.wstring().c_str(), NULL) >= 0) {
                return std::filesystem::weakly_canonical(savePath.wstring()).wstring();
            }
        }

        return std::filesystem::weakly_canonical(executablePath.parent_path().wstring()).wstring();
    }();

    return path;
}

ghc::filesystem::path dirs::getModRuntimeDir() {
    return dirs::getGeodeDir() / "unzipped";
}

void geode::utils::game::exit() {
    // TODO: mat
    #if 0
    if (CCApplication::sharedApplication() &&
        (GameManager::get()->m_playLayer || GameManager::get()->m_levelEditorLayer)) {
        log::error("Cannot exit in PlayLayer or LevelEditorLayer!");
        return;
    }
    #endif

    if (CCApplication::sharedApplication())
        // please forgive me..
        // manually set the closed flag
        // TODO: actually call glfwSetWindowShouldClose
        *reinterpret_cast<bool*>(reinterpret_cast<uintptr_t>(CCEGLView::sharedOpenGLView()->getWindow()) + 0xa) = true;
    else
        std::exit(0);
}

void geode::utils::game::restart() {
    // TODO: mat
    #if 0
    if (CCApplication::sharedApplication() &&
        (GameManager::get()->m_playLayer || GameManager::get()->m_levelEditorLayer)) {
        log::error("Cannot restart in PlayLayer or LevelEditorLayer!");
        return;
    }
    #endif

    const auto workingDir = dirs::getGameDir();

    wchar_t buffer[MAX_PATH];
    GetModuleFileNameW(nullptr, buffer, MAX_PATH);
    const auto gdName = ghc::filesystem::path(buffer).filename().string();

    // launch updater
    const auto updaterPath = (workingDir / "GeodeUpdater.exe").string();
    ShellExecuteA(nullptr, "open", updaterPath.c_str(), gdName.c_str(), workingDir.string().c_str(), false);

    exit();
}

void geode::utils::game::launchLoaderUninstaller(bool deleteSaveData) {
    const auto workingDir = dirs::getGameDir();

    if (!exists((workingDir / "GeodeUninstaller.exe"))) {
        log::error("Uninstaller not found! Not launching.");
        return;
    }

    std::string params;
    if (deleteSaveData) {
        params = "\"/DATA=" + dirs::getSaveDir().string() + "\"";
    }

    // launch uninstaller
    const auto uninstallerPath = (workingDir / "GeodeUninstaller.exe").string();
    ShellExecuteA(nullptr, "open", uninstallerPath.c_str(), params.c_str(), workingDir.string().c_str(), false);
}

Result<> geode::hook::addObjcMethod(std::string const& className, std::string const& selectorName, void* imp) {
    return Err("Wrong platform");
}
Result<void*> geode::hook::getObjcMethodImp(std::string const& className, std::string const& selectorName) {
    return Err("Wrong platform");
}

bool geode::utils::permission::getPermissionStatus(Permission permission) {
    return true; // unimplemented
}

void geode::utils::permission::requestPermission(Permission permission, utils::MiniFunction<void(bool)> callback) {
    callback(true); // unimplemented
}
