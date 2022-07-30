#include <Windows.h>

#pragma comment(linker, "/export:XInputGetState=xinput1_4.XInputGetState")
#pragma comment(linker, "/export:XInputSetState=xinput1_4.XInputSetState")
#pragma comment(linker, "/export:XInputGetCapabilities=xinput1_4.XInputGetCapabilities")

DWORD XInputGetDSoundAudioDeviceGuids(DWORD user, GUID* render, GUID* capture) {
	return ERROR_BAD_ARGUMENTS;
}

#pragma comment(linker, "/export:XInputGetDSoundAudioDeviceGuids=_XInputGetDSoundAudioDeviceGuids")

DWORD WINAPI load(PVOID _) {
	LoadLibraryA("Geode.dll");
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