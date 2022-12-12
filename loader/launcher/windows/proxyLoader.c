#include <Windows.h>
#include <stdio.h>

#pragma comment(linker, "/export:XInputGetState=xinput1_4.XInputGetState")
#pragma comment(linker, "/export:XInputSetState=xinput1_4.XInputSetState")
#pragma comment(linker, "/export:XInputGetCapabilities=xinput1_4.XInputGetCapabilities")

DWORD XInputGetDSoundAudioDeviceGuids(DWORD user, GUID* render, GUID* capture) {
	return ERROR_BAD_ARGUMENTS;
}

#pragma comment(linker, "/export:XInputGetDSoundAudioDeviceGuids=_XInputGetDSoundAudioDeviceGuids")

DWORD WINAPI load(PVOID _) {
	if (!LoadLibraryW(L"GeodeBootstrapper.dll")) {
		char msg[256];
		sprintf(msg,
			"Unable to load Geode: Unable to load "
			"bootstrapper (error code %d)", GetLastError()
		);
		MessageBoxA(NULL, msg, "Error Loading Geode", MB_ICONERROR);
	}
	return 0;
}

BOOL WINAPI DllMain(HINSTANCE module, DWORD reason, LPVOID _) {
	if (reason == DLL_PROCESS_ATTACH) {
		HANDLE handle = CreateThread(NULL, 0, load, module, 0, NULL);
		if (handle)
			CloseHandle(handle);
		else
			return FALSE;
	}
	return TRUE;
}
