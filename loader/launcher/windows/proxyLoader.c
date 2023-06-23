#include <Windows.h>
#include <stdio.h>

#pragma comment(linker, "/export:XInputGetState=xinput1_4.XInputGetState")
#pragma comment(linker, "/export:XInputSetState=xinput1_4.XInputSetState")
#pragma comment(linker, "/export:XInputGetCapabilities=xinput1_4.XInputGetCapabilities")

__declspec(dllimport) void fake();
DWORD XInputGetDSoundAudioDeviceGuids(DWORD user, GUID* render, GUID* capture) {
	fake();
	return ERROR_BAD_ARGUMENTS;
}

#pragma comment(linker, "/export:XInputGetDSoundAudioDeviceGuids=_XInputGetDSoundAudioDeviceGuids")
