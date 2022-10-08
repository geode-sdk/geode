#include <Windows.h>
#include <iostream>
#include "../../../filesystem/fs/filesystem.hpp"

int loadGeode() {
    auto dll = LoadLibraryA("Geode.dll");
    if (!dylib) {
    	std::cout << "Couldn't open Geode: " << GetLastError() << std::endl;
    	return GetLastError();
    }
    return 0;
}

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
        if (error) {
            std::cout << "Couldn't update Geode: " << error.message() << std::endl;
            return loadGeode();
        }
    }

    if (ghc::filesystem::exists(updatesDir / "resources", error) && !error) {
    	std::filesystem::remove_all(resourcesDir / "geode.loader", error);

        if (error) {
            std::cout << "Couldn't update Geode resources: " << error.message() << std::endl;
            return loadGeode();
        }

        ghc::filesystem::rename(
            updatesDir / "resources", 
            resourcesDir / "geode.loader", error
        );
        if (error) {
            std::cout << "Couldn't update Geode resources: " << error.message() << std::endl;
            return loadGeode();
        }
    }
    
	return loadGeode();
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