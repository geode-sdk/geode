#include <Windows.h>
#include <string>

// https://github.com/mrexodia/perfect-dll-proxy
#define PROXY(export, ordinal) \
    "/export:" #export "=\\\\.\\GLOBALROOT\\SystemRoot\\System32\\XInput1_4.dll.#" #ordinal

// This is the only function required by libcocos2d.dll.
#pragma comment(linker, PROXY(XInputGetAudioDeviceIds, 2))
extern "C" void XInputGetAudioDeviceIds() {}

static std::wstring getErrorString() {
    return L"Could not load Geode. Error code: " + std::to_wstring(GetLastError());
}

BOOL WINAPI DllMain(HINSTANCE module, DWORD reason, LPVOID _) {
    if (reason == DLL_PROCESS_ATTACH) {
        DisableThreadLibraryCalls(module);

        // This is UB.
        if (LoadLibraryW(L"Geode.dll") == NULL)
            MessageBoxW(NULL, getErrorString().c_str(), L"Load failed" , MB_OK | MB_ICONWARNING);
    }

    return TRUE;
}