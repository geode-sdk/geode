#include <Windows.h>
#include <stdio.h>

#pragma comment(linker, "/export:XInputGetState=xinput1_3.XInputGetState")
#pragma comment(linker, "/export:XInputSetState=xinput1_3.XInputSetState")
#pragma comment(linker, "/export:XInputGetCapabilities=xinput1_3.XInputGetCapabilities")
#pragma comment(linker, "/export:XInputGetDSoundAudioDeviceGuids=xinput1_3.XInputGetDSoundAudioDeviceGuids")

__declspec(dllimport) void fake();
__declspec(dllexport) void sussy_impostor_sus_red_sus_vote_red_red_was_not_the_impostor() { fake(); }
