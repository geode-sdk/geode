#include <Windows.h>
#include <iostream>
#include <fs/filesystem.hpp>

void showError(std::string const& error) {
	MessageBoxA(nullptr, error.c_str(), "Error Loading Geode", MB_ICONERROR);
}

int loadGeode(PVOID module) {
	if (!LoadLibraryW(L"Geode.dll")) {
		auto code = GetLastError();
		showError("Unable to load Geode (code " + std::to_string(code) + ")");
		return code;
	}
	FreeLibraryAndExitThread(static_cast<HINSTANCE>(module), 0);
	return 0;
}

DWORD WINAPI load(PVOID module) {
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
			showError("Unable to update Geode: Unable to move Geode.dll - " + error.message());
			return error.value();
		}
	}

	if (ghc::filesystem::exists(updatesDir / "resources", error) && !error) {
		ghc::filesystem::remove_all(resourcesDir / "geode.loader", error);

		if (error) {
			showError("Unable to update Geode resources: " + error.message());
		} else {
			ghc::filesystem::rename(
				updatesDir / "resources", 
				resourcesDir / "geode.loader", error
			);
			if (error) {
				showError("Unable to update Geode resources: " + error.message());
			}
		}
	}

	return loadGeode(module);
}

BOOL WINAPI DllMain(HINSTANCE module, DWORD reason, LPVOID) {
	if (reason == DLL_PROCESS_ATTACH) {
		HANDLE handle = CreateThread(NULL, 0, load, module, 0, NULL);
		if (handle) {
			CloseHandle(handle);
		} else {
			return FALSE;
		}
	}
	return TRUE;
}