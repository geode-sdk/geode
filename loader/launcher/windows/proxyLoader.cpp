#include <Windows.h>
#include <string>

#define XINPUT_PATH "\\\\.\\GLOBALROOT\\SystemRoot\\System32\\XInput1_4.dll"

// https://github.com/mrexodia/perfect-dll-proxy
#define PROXY_EXPORT(sym) \
    "/export:" #sym "=" XINPUT_PATH "." #sym

#pragma comment(linker, "/export:DllMain")
#pragma comment(linker, PROXY_EXPORT(XInputSetState))
#pragma comment(linker, PROXY_EXPORT(XInputGetCapabilities))
#pragma comment(linker, PROXY_EXPORT(XInputEnable))
#pragma comment(linker, PROXY_EXPORT(XInputGetBatteryInformation))
#pragma comment(linker, PROXY_EXPORT(XInputGetKeystroke))
#pragma comment(linker, PROXY_EXPORT(XInputGetAudioDeviceIds))

struct XINPUT_STATE;

// libcocos2d.dll requires for this function to have ordinal 2.
#pragma comment(linker, "/export:XInputGetState,@2")
extern "C" DWORD XInputGetState(DWORD dwUserIndex, XINPUT_STATE *pState) {
    auto xinput = GetModuleHandleA(XINPUT_PATH);
    if (!xinput)
        xinput = LoadLibraryA(XINPUT_PATH);

    if (xinput) {
        auto fp = GetProcAddress(xinput, "XInputGetState");
        if (fp) {
            using FPType = decltype(&XInputGetState);
            return reinterpret_cast<FPType>(fp)(dwUserIndex, pState);
        } 
    }

    return ERROR_DEVICE_NOT_CONNECTED;
}

static std::wstring getErrorString() {
    return L"Could not load Geode. Error code: " + std::to_wstring(GetLastError());
}

__declspec(dllexport) BOOL WINAPI DllMain(HINSTANCE module, DWORD reason, LPVOID _) {
    if (reason == DLL_PROCESS_ATTACH) {
        DisableThreadLibraryCalls(module);

        // This is UB.
        if (LoadLibraryW(L"Geode.dll") == NULL)
            MessageBoxW(NULL, getErrorString().c_str(), L"Load failed" , MB_OK | MB_ICONWARNING);
    }

    return TRUE;
}