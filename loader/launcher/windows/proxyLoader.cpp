#include <Windows.h>
#include <string>

struct XINPUT_STATE;
struct XINPUT_CAPABILITIES;

constexpr static auto MAX_PATH_CHARS = 32768u;

static HMODULE getXInput() {
    static auto xinput = []() -> HMODULE {
        std::wstring path(MAX_PATH_CHARS, L'\0');
        auto size = GetSystemDirectoryW(const_cast<wchar_t*>(path.data()), path.size());
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

static DWORD errorThread(LPVOID param) {
    constexpr wchar_t REDIST_ERROR[] = L"Could not load Geode!\n"
        "This is likely due to an outdated redist package.\n"
        "Do you want to update Microsoft Visual C++ Redistributable 2022 to try to fix this issue?";
    const DWORD error = reinterpret_cast<DWORD64>(param);

    if (error == ERROR_DLL_INIT_FAILED) {
        const auto choice = MessageBoxW(NULL, REDIST_ERROR, L"Load failed", MB_YESNO | MB_ICONWARNING);
            if (choice == IDYES)
                ShellExecuteW(NULL, L"open", L"https://aka.ms/vs/17/release/vc_redist.x64.exe", NULL, NULL, SW_SHOWNORMAL);
    } else {
        MessageBoxW(NULL, getErrorString(error).c_str(), L"Load failed" , MB_OK | MB_ICONWARNING);
    }

    return 0u;
}

BOOL WINAPI DllMain(HINSTANCE module, DWORD reason, LPVOID _) {
    if (reason == DLL_PROCESS_ATTACH) {
        DisableThreadLibraryCalls(module);

        // This is UB.
        if (LoadLibraryW(L"Geode.dll") == NULL) {
            const auto param = reinterpret_cast<LPVOID>(static_cast<DWORD64>(GetLastError()));
            CreateThread(NULL, 0, &errorThread, param, 0, NULL);
        }
    }

    return TRUE;
}