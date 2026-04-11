#include <Windows.h>
#include <string>
#include <array>
#include <vector>
#include <filesystem>

struct XINPUT_STATE;
struct XINPUT_CAPABILITIES;
struct XINPUT_VIBRATION;

constexpr static auto MAX_PATH_CHARS = 32768u;

constexpr wchar_t REDIST_ERROR[] = L"Could not load Geode!\n"
    "This is likely due to an outdated redist package.\n"
    "Do you want to update Microsoft Visual C++ Redistributable to try to fix this issue?";
constexpr wchar_t ALT_REDIST_ERROR[] = L"Could not load Geode!\n\n"
    "Please **delete** the following files from your Geometry Dash directory and try again: ";
constexpr wchar_t OUTDATED_REDIST[] = L"Your installed Microsoft Visual C++ Redistributable is outdated.\n"
    "This can cause random crashes and Geode might not work at all.\n\n"
    "Do you want to update it to fix this issue?";
constexpr wchar_t GEODE_NOT_FOUND_ERROR[] = L"Could not find Geode.dll!\n"
    "To fix this issue, please download the installer again and re-install Geode.\n"
    "Also make sure your antivirus is not blocking the file.\n\n"
    "Open the download page?";
constexpr wchar_t BAD_EXE_FORMAT_ERROR[] = L"Your installation of Geode is corrupted.\n"
    "To fix this issue, please download the installer again and re-install Geode.\n\n"
    "Open the download page?";

static HMODULE getXInput() {
    static auto xinput = []() -> HMODULE {
        std::wstring path(MAX_PATH_CHARS, L'\0');
        auto size = GetSystemDirectoryW(path.data(), path.size());
        if (size) {
            path.resize(size);
            return LoadLibraryW((path + L"\\XInput1_4.dll").c_str());
        }
        return NULL;
    }();

    return xinput;
}

static FARPROC getFP(const std::string& sym) {
    if (auto xinput = getXInput())
        return GetProcAddress(xinput, sym.c_str());

    return NULL;
}

#pragma comment(linker, "/export:XInputGetState,@2")
extern "C" DWORD XInputGetState(DWORD dwUserIndex, XINPUT_STATE *pState) {
    static auto fp = getFP("XInputGetState");
    if (fp) {
        using FPType = decltype(&XInputGetState);
        return reinterpret_cast<FPType>(fp)(dwUserIndex, pState);
    }

    return ERROR_DEVICE_NOT_CONNECTED;
}

#pragma comment(linker, "/export:XInputSetState,@3")
extern "C" DWORD XInputSetState(DWORD dwUserIndex, XINPUT_VIBRATION* pVibration) {
    static auto fp = getFP("XInputSetState");
    if (fp) {
        using FPType = decltype(&XInputSetState);
        return reinterpret_cast<FPType>(fp)(dwUserIndex, pVibration);
    }

    return ERROR_DEVICE_NOT_CONNECTED;
}

#pragma comment(linker, "/export:XInputGetCapabilities,@4")
extern "C" DWORD XInputGetCapabilities(DWORD dwUserIndex, DWORD dwFlags, XINPUT_CAPABILITIES *pCapabilities) {
    static auto fp = getFP("XInputGetCapabilities");
    if (fp) {
        using FPType = decltype(&XInputGetCapabilities);
        return reinterpret_cast<FPType>(fp)(dwUserIndex, dwFlags, pCapabilities);
    }

    return ERROR_DEVICE_NOT_CONNECTED;
}

static std::wstring getErrorString(DWORD error) {
    return L"Could not load Geode! Error code: " + std::to_wstring(error);
}

void downloadRedist() {
    std::wstring cmdLine = L"GeodeUpdater.exe /redist";
    std::vector<wchar_t> cmdBuffer(cmdLine.begin(), cmdLine.end());
    cmdBuffer.push_back(L'\0');

    STARTUPINFOW si = { sizeof(si) };
    PROCESS_INFORMATION pi = { 0 };

    if (CreateProcessW(
        NULL,
        cmdBuffer.data(),
        NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi
    )) {
        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);
        ExitProcess(0);
    } else {
        ShellExecuteW(NULL, L"open", L"https://aka.ms/vc14/vc_redist.x64.exe", NULL, NULL, SW_SHOWNORMAL);
        ExitProcess(0);
    }
}

void openDownloadPage() {
    ShellExecuteW(NULL, L"open", L"https://geode-sdk.org/install", NULL, NULL, SW_SHOWNORMAL);
    ExitProcess(0);
}

static DWORD errorThread(LPVOID param) {
    const DWORD error = reinterpret_cast<DWORD64>(param);

    if (error == ERROR_DLL_INIT_FAILED) {

        std::array<std::wstring, 4> msvcpDlls = {
            L"msvcp140.dll",
            L"msvcp140d.dll",
            L"vcruntime140.dll",
            L"vcruntime140d.dll"
        };

        std::vector<std::wstring> foundDlls;
        for(auto dll : msvcpDlls) {
            if(std::filesystem::exists(dll)) {
                foundDlls.push_back(dll);
            }
        }

        if(foundDlls.empty()) {
            const auto choice = MessageBoxW(NULL, REDIST_ERROR, L"Load failed", MB_YESNO | MB_ICONWARNING);
            if (choice == IDYES) {
                downloadRedist();
            }
        } else {
            std::wstring files = ALT_REDIST_ERROR;
            bool first = true;
            for(auto dll : foundDlls) {
                if(!first) files += L", ";
                files += dll;
                first = false;
            }
            const auto choice = MessageBoxW(NULL, files.c_str(), L"Load failed", MB_OK | MB_ICONWARNING);
        }

    } else if (error == ERROR_MOD_NOT_FOUND) {
        if(!std::filesystem::exists(L"Geode.dll")) {
            if(MessageBoxW(NULL, GEODE_NOT_FOUND_ERROR, L"Load failed (error code: 126)", MB_YESNO | MB_ICONWARNING) == IDYES) {
                openDownloadPage();
            }
        } else {
            if(MessageBoxW(NULL, REDIST_ERROR, L"Load failed (error code: 126)", MB_YESNO | MB_ICONWARNING) == IDYES) {
                downloadRedist();
            }
        }
    } else if (error == ERROR_BAD_EXE_FORMAT) {
        if(MessageBoxW(NULL, BAD_EXE_FORMAT_ERROR, L"Load failed (error code: 193)", MB_YESNO | MB_ICONWARNING) == IDYES) {
            openDownloadPage();
        }
    } else {
        MessageBoxW(NULL, getErrorString(error).c_str(), L"Load failed" , MB_OK | MB_ICONWARNING);
    }

    return 0u;
}

bool isRedistUpdated() {
    HMODULE hMod = GetModuleHandleW(L"vcruntime140.dll");
    if(!hMod) return true;

    WCHAR szPath[MAX_PATH];
    GetModuleFileNameW(hMod, szPath, MAX_PATH);

    DWORD dummy;
    DWORD dwSize = GetFileVersionInfoSizeW(szPath, &dummy);
    if (dwSize == 0) return true;

    std::vector<BYTE> data(dwSize);
    if (!GetFileVersionInfoW(szPath, 0, dwSize, data.data())) return true;

    VS_FIXEDFILEINFO* pFileInfo = nullptr;
    UINT len = 0;
    if (!VerQueryValueW(data.data(), L"\\", (LPVOID*)&pFileInfo, &len)) return true;

    return HIWORD(pFileInfo->dwFileVersionMS) == 14 && LOWORD(pFileInfo->dwFileVersionMS) >= 44;
}

BOOL WINAPI DllMain(HINSTANCE module, DWORD reason, LPVOID _) {
    if (reason == DLL_PROCESS_ATTACH) {
        DisableThreadLibraryCalls(module);

        if (!isRedistUpdated()) {
            if (MessageBoxW(NULL, OUTDATED_REDIST, L"Warning", MB_YESNO | MB_ICONWARNING) == IDYES) {
                downloadRedist();
            }
        }

        // This is UB.
        if (LoadLibraryW(L"Geode.dll") == NULL) {
            const auto param = reinterpret_cast<LPVOID>(static_cast<DWORD64>(GetLastError()));
            CreateThread(NULL, 0, &errorThread, param, 0, NULL);
        }
    }

    return TRUE;
}