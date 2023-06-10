#include <Windows.h>
#include <stdio.h>

#pragma comment(linker, "/export:XInputGetState=xinput1_4.XInputGetState")
#pragma comment(linker, "/export:XInputSetState=xinput1_4.XInputSetState")
#pragma comment(linker, "/export:XInputGetCapabilities=xinput1_4.XInputGetCapabilities")

DWORD XInputGetDSoundAudioDeviceGuids(DWORD user, GUID* render, GUID* capture) {
	return ERROR_BAD_ARGUMENTS;
}

#pragma comment(linker, "/export:XInputGetDSoundAudioDeviceGuids=_XInputGetDSoundAudioDeviceGuids")

__declspec(dllimport) DWORD WINAPI loadGeode(void*);
BOOL WINAPI DllMain(HINSTANCE module, DWORD reason, LPVOID _) {
	if (reason == DLL_PROCESS_ATTACH) {
		// Prevents threads from notifying this DLL on creation or destruction.
		// Kind of redundant for a game that isn't multi-threaded but will provide
		// some slight optimizations if a mod frequently creates and deletes threads.
		DisableThreadLibraryCalls(module);

		DWORD code = loadGeode(module);
		if (code != 0)
			return FALSE;
	}
	return TRUE;
}
