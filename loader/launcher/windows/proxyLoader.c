#include <Windows.h>
#include <Xinput.h>
#include <stdio.h>
#include <inttypes.h>

#ifndef MAX_PATH
    #define MAX_PATH 260
#endif

HMODULE xinput = NULL;
DWORD(WINAPI* getState)(DWORD, XINPUT_STATE*) = NULL;
DWORD(WINAPI* setState)(DWORD, XINPUT_VIBRATION*) = NULL;
DWORD(WINAPI* getCapabilities)(DWORD, DWORD, XINPUT_CAPABILITIES*) = NULL;
DWORD(WINAPI* getDSoundAudioDeviceGuids)(DWORD, GUID*, GUID*) = NULL;
void lazyLoadXInput() {
    if (xinput)
        return;
    char path[MAX_PATH];
    GetSystemDirectoryA(path, MAX_PATH);
    strcat_s(path, MAX_PATH, "\\XInput9_1_0.dll");
    xinput = LoadLibraryA(path);
    getState = (DWORD(WINAPI*)(DWORD, XINPUT_STATE*))GetProcAddress(xinput, "XInputGetState");
    setState = (DWORD(WINAPI*)(DWORD, XINPUT_VIBRATION*))GetProcAddress(xinput, "XInputSetState");
    getCapabilities = (DWORD(WINAPI*)(DWORD, DWORD, XINPUT_CAPABILITIES*))GetProcAddress(xinput, "XInputGetCapabilities");
    getDSoundAudioDeviceGuids = (DWORD(WINAPI*)(DWORD, GUID*, GUID*))GetProcAddress(xinput, "XInputGetDSoundAudioDeviceGuids");
}

DWORD WINAPI xinputGetState(DWORD dwUserIndex, XINPUT_STATE* pState) {
    lazyLoadXInput();
    return getState(dwUserIndex, pState);
}
DWORD WINAPI xinputSetState(DWORD dwUserIndex, XINPUT_VIBRATION* pVibration) {
    lazyLoadXInput();
    return setState(dwUserIndex, pVibration);
}
DWORD WINAPI xinputGetCapabilities(DWORD dwUserIndex, DWORD dwFlags, XINPUT_CAPABILITIES* pCapabilities) {
    lazyLoadXInput();
    return getCapabilities(dwUserIndex, dwFlags, pCapabilities);
}
DWORD WINAPI xinputGetDSoundAudioDeviceGuids(DWORD dwUserIndex, GUID* pDSoundRenderGuid, GUID* pDSoundCaptureGuid) {
    lazyLoadXInput();
    return getDSoundAudioDeviceGuids(dwUserIndex, pDSoundRenderGuid, pDSoundCaptureGuid);
}

// https://github.com/mrexodia/perfect-dll-proxy
#if defined(_WIN64)
#define PROXY_PATH(export) \
    "/export:" #export "=\\\\.\\GLOBALROOT\\SystemRoot\\System32\\XInput9_1_0.dll." #export
#else
#define PROXY_PATH(export) \
    "/export:" #export "=\\\\.\\GLOBALROOT\\SystemRoot\\SysWOW64\\XInput9_1_0.dll." #export
#endif

#pragma comment(linker, PROXY_PATH(XInputGetState))
#pragma comment(linker, PROXY_PATH(XInputSetState))
#pragma comment(linker, PROXY_PATH(XInputGetCapabilities))
#pragma comment(linker, PROXY_PATH(XInputGetDSoundAudioDeviceGuids))

BOOL fileExists(char const* path) {
    DWORD attrib = GetFileAttributesA(path);
    return (attrib != INVALID_FILE_ATTRIBUTES && !(attrib & FILE_ATTRIBUTE_DIRECTORY));
}

BOOL WINAPI DllMain(HINSTANCE module, DWORD reason, LPVOID _) {
    if (reason != DLL_PROCESS_ATTACH)
        return TRUE;
    DisableThreadLibraryCalls(module);

    if (fileExists("Geode.dll")) {
        // somehow, this works fine inside of dllmain :-)
        // yes, even on wine.
        /* * * * * * * * * * * * * * * * * *\
         * The Shadows Shall Smite You 01  *
         * - ConfiG                        *
        \* * * * * * * * * * * * * * * * * */
        LoadLibraryA("Geode.dll");
    }

    return TRUE;
}
