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
#include "../../utils/thread.hpp"
#include <arc/sync/oneshot.hpp>

bool utils::clipboard::write(ZStringView data) {
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

// If successful, the bool represents whether a file was picked (true) or dialog was cancelled (false)
static arc::Future<Result<bool>> asyncNfdPick(
    NFDMode mode,
    file::FilePickOptions const& options,
    void* result
) {
    auto [tx, rx] = arc::oneshot::channel<Result<>>();

    auto thread = std::thread([&, tx = std::move(tx)] mutable {
        auto pickresult = nfdPick(mode, options, result);
        (void) tx.send(pickresult);
    });

    // wait for the result, and join the thread
    auto recvresult = co_await rx.recv();
    if (thread.joinable()) thread.join();

    if (!recvresult) {
        co_return Err("Error occurred while picking file");
    }

    auto res = std::move(recvresult).unwrap();
    if (!res) {
        auto err = std::move(res).unwrapErr();
        if (err == "Dialog cancelled") {
            co_return Ok(false);
        }
        co_return Err(std::move(err));
    }
    co_return Ok(true);
}

arc::Future<file::PickResult> file::pick(PickMode mode, FilePickOptions options) {
    #define TURN_INTO_NFDMODE(mode) \
        case file::PickMode::mode: nfdMode = NFDMode::mode; break;

    NFDMode nfdMode;
    switch (mode) {
        TURN_INTO_NFDMODE(OpenFile);
        TURN_INTO_NFDMODE(SaveFile);
        TURN_INTO_NFDMODE(OpenFolder);
        default:
            co_return Err("Invalid pick mode");
    }

    std::filesystem::path path;
    bool picked = ARC_CO_UNWRAP(co_await asyncNfdPick(nfdMode, options, &path));
    if (!picked) {
        co_return Ok(std::nullopt);
    }

    co_return Ok(std::move(path));
}

arc::Future<file::PickManyResult> file::pickMany(FilePickOptions options) {
    std::vector<std::filesystem::path> paths;
    bool picked = ARC_CO_UNWRAP(co_await asyncNfdPick(NFDMode::OpenFiles, options, &paths));
    if (!picked) {
        co_return Ok(std::vector<std::filesystem::path>{});
    }

    co_return Ok(std::move(paths));
}

void utils::web::openLinkInBrowser(ZStringView url) {
    ShellExecuteW(0, 0, utils::string::utf8ToWide(url).c_str(), 0, 0, SW_SHOW);
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
    auto const gdName = L"\"" + std::filesystem::path(buffer).filename().wstring() + L"\"";

    // launch updater
    auto const updaterPath = (workingDir / "GeodeUpdater.exe").wstring();
    ShellExecuteW(nullptr, L"open", updaterPath.c_str(), gdName.c_str(), workingDir.wstring().c_str(), false);

    exit(saveData);
}

void geode::utils::game::launchLoaderUninstaller(bool deleteSaveData) {
    const auto workingDir = dirs::getGameDir();

    if (!exists((workingDir / "GeodeUninstaller.exe"))) {
        log::error("Uninstaller not found! Not launching.");
        return;
    }

    std::wstring params;
    if (deleteSaveData) {
        params = L"\"/DATA=" + dirs::getSaveDir().wstring() + L"\"";
    }

    // launch uninstaller
    auto const uninstallerPath = workingDir / "GeodeUninstaller.exe";
    ShellExecuteW(nullptr, L"open", uninstallerPath.c_str(), params.c_str(), workingDir.wstring().c_str(), false);
}

Result<> geode::hook::addObjcMethod(char const* className, char const* selectorName, void* imp) {
    return Err("Wrong platform");
}
Result<void*> geode::hook::getObjcMethodImp(char const* className, char const* selectorName) {
    return Err("Wrong platform");
}

bool geode::utils::permission::getPermissionStatus(Permission permission) {
    return true; // unimplemented
}

void geode::utils::permission::requestPermission(Permission permission, geode::Function<void(bool)> callback) {
    callback(true); // unimplemented
}

// [Set|Get]ThreadDescription are pretty new, so the user's system might not have them
// or they might only be accessible dynamically (see msdocs link below for more info)
static auto setThreadDesc = reinterpret_cast<decltype(&SetThreadDescription)>(GetProcAddress(GetModuleHandleW(L"Kernel32.dll"), "SetThreadDescription"));
static auto getThreadDesc = reinterpret_cast<decltype(&GetThreadDescription)>(GetProcAddress(GetModuleHandleW(L"Kernel32.dll"), "GetThreadDescription"));

static std::optional<std::string> getNameFromOs() {
    if (!getThreadDesc) {
        return std::nullopt;
    }

    PWSTR wname = nullptr;
    if (!SUCCEEDED(getThreadDesc(GetCurrentThread(), &wname))) {
        return std::nullopt;
    }

    std::string name = utils::string::wideToUtf8(wname);
    LocalFree(wname);

    return name;
}

std::string geode::utils::thread::getDefaultName() {
    // try to request name from the OS first, fallback to a simple format if fails
    if (auto name = getNameFromOs()) {
        return *name;
    }

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

void obliterate(ZStringView name) {
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
void geode::utils::thread::platformSetName(ZStringView name) {
    // SetThreadDescription
    if (setThreadDesc) {
        auto res = setThreadDesc(GetCurrentThread(), string::utf8ToWide(name).c_str());
        if (FAILED(res))
            log::warn("SetThreadDescription failed ({}), using only fallback method.", res);
    }
    obliterate(name);
}

std::string geode::utils::getEnvironmentVariable(ZStringView name) {
    char buffer[1024];
    size_t count = 0;
    if (0 == getenv_s(&count, buffer, name.c_str()) && count != 0) {
        return buffer;
    }

    return "";
}

std::string geode::utils::formatSystemError(int code) {
    wchar_t errorBuf[512]; // enough for most messages

    auto result = FormatMessageW(
        FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
        nullptr, code, MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US), errorBuf, sizeof(errorBuf), nullptr);

    if (result == 0) {
        return fmt::format("Unknown ({})", code);
    } else {
        auto wmsg = std::wstring(errorBuf, errorBuf + result);
        auto msg = utils::string::wideToUtf8(wmsg);

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
