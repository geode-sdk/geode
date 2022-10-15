#include <Geode/utils/platform.hpp>
#include <fs/filesystem.hpp>

#ifdef GEODE_IS_WINDOWS

USE_GEODE_NAMESPACE();

#include <iostream>
#include <sstream>
#include <Windows.h>
#include <shobjidl.h>
#include <shlwapi.h>
#include "nfdwin.hpp"

bool utils::clipboard::write(std::string const& data) {
    if (!OpenClipboard(nullptr))
        return false;
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
    if (!OpenClipboard(nullptr))
        return "";
    
    HANDLE hData = GetClipboardData(CF_TEXT);
    if (hData == nullptr) {
        CloseClipboard();
        return "";
    }

    char * pszText = static_cast<char*>(GlobalLock(hData));
    if (pszText == nullptr) {
        CloseClipboard();
        return "";
    }

    std::string text(pszText);

    GlobalUnlock(hData);
    CloseClipboard();

    return text;
}

ghc::filesystem::path utils::file::geodeRoot() {
    return ghc::filesystem::path(CCFileUtils::sharedFileUtils()->getWritablePath2().c_str());
}

bool utils::file::openFolder(ghc::filesystem::path const& path) {
	ShellExecuteA(NULL, "open", path.string().c_str(), NULL, NULL, SW_SHOWDEFAULT);
	return true;
}

Result<ghc::filesystem::path> utils::file::pickFile(
    file::PickMode mode,
    file::FilePickOptions const& options
) {
    #define TURN_INTO_NFDMODE(mode) \
        case file::PickMode::mode: nfdMode = NFDMode::mode; break;

    NFDMode nfdMode;
    switch (mode) {
        TURN_INTO_NFDMODE(OpenFile);
        TURN_INTO_NFDMODE(SaveFile);
        TURN_INTO_NFDMODE(OpenFolder);
        default: return Err("Unknown open mode");
    }
    ghc::filesystem::path path;
    auto r = nfdPick(nfdMode, options, &path);
    if (!r) {
        return Err(r.error());
    }
    return Ok(path);
}

Result<std::vector<ghc::filesystem::path>> utils::file::pickFiles(
    file::FilePickOptions const& options
) {
    std::vector<ghc::filesystem::path> paths;
    auto r = nfdPick(NFDMode::OpenFolder, options, &paths);
    if (!r) {
        return Err(r.error());
    }
    return Ok(paths);
}

void utils::web::openLinkInBrowser(std::string const& url) {
    ShellExecuteA(0, 0, url.c_str(), 0, 0, SW_SHOW);
}

#endif
