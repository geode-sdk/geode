#include <Windows.h>
#include <string>

// https://github.com/mrexodia/perfect-dll-proxy
#define PROXY_PATH(export) \
    "/export:" #export "=\\\\.\\GLOBALROOT\\SystemRoot\\System32\\dinput8.dll." #export

#pragma comment(linker, PROXY_PATH(DirectInput8Create))
#pragma comment(linker, PROXY_PATH(DllCanUnloadNow))
#pragma comment(linker, PROXY_PATH(DllGetClassObject))
#pragma comment(linker, PROXY_PATH(DllRegisterServer))
#pragma comment(linker, PROXY_PATH(DllUnregisterServer))
#pragma comment(linker, PROXY_PATH(GetdfDIJoystick))

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