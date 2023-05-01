
#include <Geode/DefaultInclude.hpp>

#ifdef GEODE_IS_WINDOWS

using namespace geode::prelude;
#include <Geode/loader/Dirs.hpp>
#include "nfdwin.hpp"
#include <ghc/fs_fwd.hpp>
#include <Windows.h>
#include <iostream>
#include <shlwapi.h>
#include <shobjidl.h>
#include <sstream>
#include <Geode/utils/web.hpp>
#include <Geode/utils/cocos.hpp>

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
    ShellExecuteA(NULL, "open", path.string().c_str(), NULL, NULL, SW_SHOWDEFAULT);
    return true;
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

Result<std::vector<ghc::filesystem::path>> utils::file::pickFiles(
    file::FilePickOptions const& options
) {
    std::vector<ghc::filesystem::path> paths;
    GEODE_UNWRAP(nfdPick(NFDMode::OpenFolder, options, &paths));
    return Ok(paths);
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
        return path.parent_path();
    }();

    return path;
}

#endif
