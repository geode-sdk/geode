// Based off https://gist.github.com/absoIute/ce6b463cb1985723dab108a52f5e8815
// with permission

#include <Windows.h>
#include <Xinput.h>
#include <stdio.h>

HINSTANCE s_instance = NULL;

// GD only uses these two functions, and this one in particular is imported by ordinal 2,
// so we make sure to set that up in the .def file
typedef DWORD(WINAPI* f_XInputGetState)(DWORD, XINPUT_STATE*);
f_XInputGetState s_XInputGetState = NULL;

typedef DWORD(WINAPI* f_XInputGetCapabilities)(DWORD, DWORD, XINPUT_CAPABILITIES*);
f_XInputGetCapabilities s_XInputGetCapabilities = NULL;

void load() {
    WCHAR path[MAX_PATH];
    GetSystemDirectoryW(path, MAX_PATH);
    wcscat_s(path, MAX_PATH, L"\\XInput1_4.dll");
    s_instance = LoadLibraryW(path);

    if (!s_instance)
        return;

    s_XInputGetState = (f_XInputGetState)(GetProcAddress(s_instance, "XInputGetState"));
    s_XInputGetCapabilities = (f_XInputGetCapabilities)(GetProcAddress(s_instance, "XInputGetCapabilities"));
}


DWORD WINAPI XInputGetState(DWORD dwUserIndex, XINPUT_STATE* pState) {
    if (!s_XInputGetState)
        load();

    return s_XInputGetState(dwUserIndex, pState);
}

DWORD WINAPI XInputGetCapabilities(DWORD dwUserIndex, DWORD dwFlags, XINPUT_CAPABILITIES* pCapabilities) {
    if (!s_XInputGetCapabilities)
        load();

    return s_XInputGetCapabilities(dwUserIndex, dwFlags, pCapabilities);
}

BOOL WINAPI DllMain(HINSTANCE module, DWORD reason, LPVOID _) {
    if (reason == DLL_PROCESS_ATTACH) {
        DisableThreadLibraryCalls(module);

        // This is UB, but works fine in practice on both Windows and wine
        if (LoadLibraryW(L"Geode.dll") == NULL) {
            char buffer[256];
            sprintf_s(buffer, 256, "Failed to load Geode.dll. Error code: %d", GetLastError());
            MessageBoxA(NULL, buffer, "Load failed" , MB_OK | MB_ICONWARNING);
        }
    }

    return TRUE;
}
