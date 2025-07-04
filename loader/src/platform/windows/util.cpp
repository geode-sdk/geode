#include <Geode/DefaultInclude.hpp>

using namespace geode::prelude;

#include <Geode/loader/Dirs.hpp>
#include <Geode/binding/AppDelegate.hpp>
#include "nfdwin.hpp"
#include <Windows.h>
#include <processthreadsapi.h>
#include <ShlObj.h>
#include <shlwapi.h>
#include <shobjidl.h>
#include <Geode/utils/web.hpp>
#include <Geode/utils/cocos.hpp>
#include <Geode/loader/Log.hpp>
#include <filesystem>
#include <Geode/utils/permission.hpp>
#include <Geode/utils/ObjcHook.hpp>
#include <Geode/utils/string.hpp>

bool utils::clipboard::write(std::string const& data) {
    if (!OpenClipboard(nullptr)) return false;
    if (!EmptyClipboard()) {
        CloseClipboard();
        return false;
    }

    std::wstring wData = string::utf8ToWide(data);
    auto const size = (wData.size() + 1) * sizeof(wchar_t);

    HGLOBAL hg = GlobalAlloc(GMEM_MOVEABLE, size);

    if (!hg) {
        CloseClipboard();
        return false;
    }

    auto dest = GlobalLock(hg);

    if (!dest) {
        CloseClipboard();
        return false;
    }

    memcpy(dest, wData.c_str(), size);

    GlobalUnlock(hg);

    SetClipboardData(CF_UNICODETEXT, hg);
    CloseClipboard();

    GlobalFree(hg);

    return true;
}

std::string utils::clipboard::read() {
    if (!OpenClipboard(nullptr)) return "";

    HANDLE hData = GetClipboardData(CF_UNICODETEXT);
    if (hData == nullptr) {
        CloseClipboard();
        return "";
    }

    auto pszText = static_cast<wchar_t*>(GlobalLock(hData));
    if (pszText == nullptr) {
        CloseClipboard();
        return "";
    }

    std::string text = string::wideToUtf8(pszText);

    GlobalUnlock(hData);
    CloseClipboard();

    return text;
}

bool utils::file::openFolder(std::filesystem::path const& path) {
    // mods can (and do) keep CoInitializeEx initialized on the main thread
    // which results in this function just not doing anything
    // which is why we're using a separate thread
    // feel free to rework later, im just tired of reports of this not working
    auto success = false;
    auto thread = std::thread([](auto const& path, bool& success) {
        if (CoInitializeEx(nullptr, COINIT_MULTITHREADED) == S_OK) {
            // Don't try to open up a file >:(
            std::error_code whatever;
            std::filesystem::path dir = path;
            if (!std::filesystem::is_directory(dir, whatever)) {
                dir = dir.parent_path();
            }
            if (auto id = ILCreateFromPathW(dir.wstring().c_str())) {
                std::filesystem::path selectPath = path / ".";
                if (!std::filesystem::is_directory(path, whatever)) {
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
    }, path, std::ref(success));
    thread.join();
    return success;
}

Task<Result<std::filesystem::path>> file::pick(PickMode mode, FilePickOptions const& options) {
    using RetTask = Task<Result<std::filesystem::path>>;
    #define TURN_INTO_NFDMODE(mode) \
        case file::PickMode::mode: nfdMode = NFDMode::mode; break;

    NFDMode nfdMode;
    switch (mode) {
        TURN_INTO_NFDMODE(OpenFile);
        TURN_INTO_NFDMODE(SaveFile);
        TURN_INTO_NFDMODE(OpenFolder);
        default:
            return RetTask::immediate(Err<std::string>("Invalid pick mode"));
    }
    std::filesystem::path path;
    auto pickresult = nfdPick(nfdMode, options, &path);
    if (pickresult.isErr()) {
        if (pickresult.unwrapErr() == "Dialog cancelled") {
            return RetTask::cancelled();
        }
        return RetTask::immediate(Err(pickresult.unwrapErr()));
    } else {
        return RetTask::immediate(Ok(path));
    }
}

Task<Result<std::vector<std::filesystem::path>>> file::pickMany(FilePickOptions const& options) {
    using RetTask = Task<Result<std::vector<std::filesystem::path>>>;

    std::vector<std::filesystem::path> paths;
    auto pickResult = nfdPick(NFDMode::OpenFiles, options, &paths);
    if (pickResult.isErr()) {
        return RetTask::immediate(Err(pickResult.err().value()));
    } else {
        return RetTask::immediate(Ok(paths));
    }
    // return Task<Result<std::vector<std::filesystem::path>>>::immediate(std::move(file::pickFiles(options)));
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

std::filesystem::path dirs::getGameDir() {
    // only fetch the path once, since ofc it'll never change
    // throughout the execution
    static const auto path = [] {
        std::array<WCHAR, MAX_PATH> buffer;
        GetModuleFileNameW(NULL, buffer.data(), MAX_PATH);

        const std::filesystem::path path(buffer.data());
        return std::filesystem::weakly_canonical(path.parent_path().wstring()).wstring();
    }();

    return path;
}

std::filesystem::path dirs::getSaveDir() {
    // only fetch the path once, since ofc it'll never change
    // throughout the execution
    static const auto path = [] {
        std::array<WCHAR, MAX_PATH + 1> buffer;
        GetModuleFileNameW(NULL, buffer.data(), MAX_PATH + 1);

        auto executablePath = std::filesystem::path(buffer.data());
        auto executableName = executablePath.filename().wstring();
        executableName = executableName.substr(0, executableName.find_last_of(L"."));

        if (SHGetFolderPathW(NULL, CSIDL_LOCAL_APPDATA, NULL, SHGFP_TYPE_CURRENT, buffer.data()) >= 0) {
            auto appdataPath = std::filesystem::path(buffer.data());
            auto savePath = appdataPath / executableName;

            if (SHCreateDirectoryExW(NULL, savePath.wstring().c_str(), NULL) >= 0) {
                return std::filesystem::weakly_canonical(savePath.wstring()).wstring();
            }
        }

        return std::filesystem::weakly_canonical(executablePath.parent_path().wstring()).wstring();
    }();

    return path;
}

std::filesystem::path dirs::getModRuntimeDir() {
    return dirs::getGeodeDir() / "unzipped";
}

std::filesystem::path dirs::getResourcesDir() {
    return dirs::getGameDir() / "Resources";
}

void geode::utils::game::exit(bool saveData) {
    // TODO: mat
    #if 0
    if (CCApplication::sharedApplication() &&
        (GameManager::get()->m_playLayer || GameManager::get()->m_levelEditorLayer)) {
        log::error("Cannot exit in PlayLayer or LevelEditorLayer!");
        return;
    }
    #endif

    // If this breaks down the read, uhhh blame Cvolton or something
    if (saveData) {
        if (AppDelegate::get()) {
            AppDelegate::get()->trySaveGame(true);
        }
    }

    std::exit(0);
}

void geode::utils::game::exit() {
    exit(true);
}

void geode::utils::game::restart(bool saveData) {
    // TODO: mat
    // TODO: be VERY careful before enabling this again, this function is called in platform/windows/main.cpp,
    // before we even check if we are in forward compatibility mode or not.
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
    const auto gdName = fmt::format("\"{}\"", std::filesystem::path(buffer).filename().string());

    // launch updater
    const auto updaterPath = (workingDir / "GeodeUpdater.exe").string();
    ShellExecuteA(nullptr, "open", updaterPath.c_str(), gdName.c_str(), workingDir.string().c_str(), false);

    exit(saveData);
}

void geode::utils::game::restart() {
    restart(true);
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

void geode::utils::permission::requestPermission(Permission permission, std::function<void(bool)> callback) {
    callback(true); // unimplemented
}

#include "../../utils/thread.hpp"

std::string geode::utils::thread::getDefaultName() {
    return fmt::format("Thread #{}", GetCurrentThreadId());
}

// https://learn.microsoft.com/en-us/visualstudio/debugger/how-to-set-a-thread-name-in-native-code?view=vs-2022
#pragma pack(push,8)
typedef struct tagTHREADNAME_INFO {
    DWORD dwType; // Must be 0x1000.
    LPCSTR szName; // Pointer to name (in user addr space).
    DWORD dwThreadID; // Thread ID (-1=caller thread).
    DWORD dwFlags; // Reserved for future use, must be zero.
} THREADNAME_INFO;
#pragma pack(pop)

// SetThreadDescription is pretty new, so the user's system might not have it
// or it might only be accessible dynamically (see msdocs link above for more info)
auto setThreadDesc = reinterpret_cast<decltype(&SetThreadDescription)>(GetProcAddress(GetModuleHandleA("Kernel32.dll"), "SetThreadDescription"));
void obliterate(std::string const& name) {
    // exception
    THREADNAME_INFO info;
    info.dwType = 0x1000;
    info.szName = name.c_str();
    info.dwThreadID = GetCurrentThreadId();
    info.dwFlags = 0;
#pragma warning(push)
#pragma warning(disable: 6320 6322)
    __try {
        RaiseException(0x406d1388, 0, sizeof(info) / sizeof(ULONG_PTR), reinterpret_cast<ULONG_PTR*>(&info));
    }
    __except (EXCEPTION_EXECUTE_HANDLER) { }
#pragma warning(pop)
}
void geode::utils::thread::platformSetName(std::string const& name) {
    // SetThreadDescription
    if (setThreadDesc) {
        auto res = setThreadDesc(GetCurrentThread(), string::utf8ToWide(name).c_str());
        if (FAILED(res))
            log::warn("SetThreadDescription failed ({}), using only fallback method.", res);
    }
    obliterate(name);
}

std::string geode::utils::getEnvironmentVariable(const char* name) {
    char buffer[1024];
    size_t count = 0;
    if (0 == getenv_s(&count, buffer, name) && count != 0) {
        return buffer;
    }

    return "";
}

std::string geode::utils::formatSystemError(int code) {
    char errorBuf[512]; // enough for most messages

    auto result = FormatMessageA(
        FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
        nullptr, code, MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US), errorBuf, sizeof(errorBuf), nullptr);

    if (result == 0) {
        return fmt::format("Unknown ({})", code);
    } else {
        std::string msg = std::string(errorBuf, errorBuf + result);

        // the string sometimes includes a crlf, strip it, also remove unprintable chars
        msg.erase(std::find_if(msg.rbegin(), msg.rend(), [](unsigned char ch) {
            return ch != '\r' && ch != '\n' && ch < 127;
        }).base(), msg.end());

        return msg;
    }
}

cocos2d::CCRect geode::utils::getSafeAreaRect() {
    auto winSize = cocos2d::CCDirector::sharedDirector()->getWinSize();
    return cocos2d::CCRect(0.0f, 0.0f, winSize.width, winSize.height);
}
