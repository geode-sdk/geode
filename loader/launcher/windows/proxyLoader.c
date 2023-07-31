#include <Windows.h>
#include <stdio.h>

#pragma comment(linker, "/export:XInputGetState=xinput1_3.XInputGetState")
#pragma comment(linker, "/export:XInputSetState=xinput1_3.XInputSetState")
#pragma comment(linker, "/export:XInputGetCapabilities=xinput1_3.XInputGetCapabilities")
#pragma comment(linker, "/export:XInputGetDSoundAudioDeviceGuids=xinput1_3.XInputGetDSoundAudioDeviceGuids")

__declspec(dllimport) void fake();
void impasta() {
	fake();
}
