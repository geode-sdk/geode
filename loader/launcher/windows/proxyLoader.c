#include <Windows.h>
#include <Xinput.h>

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

#pragma comment(linker, "/export:XInputGetState=_xinputGetState@8")
#pragma comment(linker, "/export:XInputSetState=_xinputSetState@8")
#pragma comment(linker, "/export:XInputGetCapabilities=_xinputGetCapabilities@12")
#pragma comment(linker, "/export:XInputGetDSoundAudioDeviceGuids=_xinputGetDSoundAudioDeviceGuids@12")

unsigned int getExeTimestamp() {
    HANDLE hMod = GetModuleHandleA(NULL);
    PIMAGE_DOS_HEADER dosHeader = (PIMAGE_DOS_HEADER)hMod;

    if (dosHeader->e_magic == IMAGE_DOS_SIGNATURE) {
        PIMAGE_NT_HEADERS ntHeader = (PIMAGE_NT_HEADERS)((uintptr_t)(hMod) + dosHeader->e_lfanew);

        if (ntHeader->Signature == IMAGE_NT_SIGNATURE) {
            return ntHeader->FileHeader.TimeDateStamp;
        }
    }
    return 0;
}

BOOL fileExists(char const* path) {
    DWORD attrib = GetFileAttributesA(path);

    return (attrib != INVALID_FILE_ATTRIBUTES && !(attrib & FILE_ATTRIBUTE_DIRECTORY));
}

// Table originally from this gist by absolute:
// https://gist.github.com/absoIute/ebe5da42d118109a03632c9751d86e19

#define TIMESTAMP_FOR_1_900 1419173053
#define TIMESTAMP_FOR_1_910 1419880840
#define TIMESTAMP_FOR_1_920 1421745341
#define TIMESTAMP_FOR_2_000 1440638199
#define TIMESTAMP_FOR_2_001 1440643927
#define TIMESTAMP_FOR_2_010 1443053232
#define TIMESTAMP_FOR_2_011 1443077847
#define TIMESTAMP_FOR_2_020 1443077847
#define TIMESTAMP_FOR_2_100 1484612867
#define TIMESTAMP_FOR_2_101 1484626658
#define TIMESTAMP_FOR_2_102 1484737207
#define TIMESTAMP_FOR_2_110 1510526914
#define TIMESTAMP_FOR_2_111 1510538091
#define TIMESTAMP_FOR_2_112 1510619253
#define TIMESTAMP_FOR_2_113 1511220108
#define TIMESTAMP_FOR_2_200 1702921605
#define TIMESTAMP_FOR_2_201 1704582672
#define TIMESTAMP_FOR_2_202 1704601266

#define GEODE_WRAPPER_CONCAT(x, y) x##y
#define GEODE_CONCAT(x, y) GEODE_WRAPPER_CONCAT(x, y)

// Hello future person. if this is erroring then you need to add
// the exe timestamp for whatever you set GEODE_GD_VERSION to. hope that helps!
#define TIMESTAMP GEODE_CONCAT(TIMESTAMP_FOR, GEODE_GD_VERSION_IDENTIFIER)

BOOL WINAPI DllMain(HINSTANCE module, DWORD reason, LPVOID _) {
    if (reason != DLL_PROCESS_ATTACH)
        return TRUE;
    DisableThreadLibraryCalls(module);

    if (fileExists("Geode.dll")) {
        if (getExeTimestamp() == TIMESTAMP) {
            // somehow, this works fine inside of dllmain :-)
            // yes, even on wine.
            LoadLibraryA("Geode.dll");
        } else {
            MessageBoxA(NULL, "GD version mismatch, not loading Geode.", "Unable to load Geode!", MB_OK | MB_ICONWARNING);
        }
    }

    return TRUE;
}