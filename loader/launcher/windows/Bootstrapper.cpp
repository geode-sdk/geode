#include <Windows.h>
#include "../../../filesystem/fs/filesystem.hpp"

DWORD WINAPI load(PVOID _) {
	auto workingDir = ghc::filesystem::current_path();
	auto updatesDir = workingDir / "geode" / "update";
	auto resourcesDir = workingDir / "geode" / "resources";

	auto error = std::error_code();

	if (ghc::filesystem::exists(updatesDir / "Geode.dll", error) && !error) {
        ghc::filesystem::rename(
            updatesDir / "Geode.dll", 
            workingDir / "Geode.dll", error
        );
        if (error) return error.value();
    }

    if (ghc::filesystem::exists(updatesDir / "resources", error) && !error) {
        ghc::filesystem::rename(
            updatesDir / "resources", 
            resourcesDir / "geode.loader", error
        );
        if (error) return error.value();
    }
    
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