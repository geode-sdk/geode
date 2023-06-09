#include <Windows.h>
#include <iostream>
#include <array>
#include <ghc/filesystem.hpp>

void showError(std::string const& error) {
	MessageBoxA(nullptr, error.c_str(), "Error Loading Geode", MB_ICONERROR);
}

int main(int argc, char* argv[]) {
	std::array<WCHAR, MAX_PATH> szFileName;
	GetModuleFileNameW(NULL, szFileName.data(), MAX_PATH);

	const ghc::filesystem::path path(szFileName.data());
	auto workingDir = path.parent_path();
	auto updatesDir = workingDir / "geode" / "update";
	auto resourcesDir = workingDir / "geode" / "resources";

	// wait for geode.dll to be writable
	int delay = 10;
	HANDLE hFile;
	while ((hFile = CreateFile((workingDir / "Geode.dll").string().c_str(), FILE_GENERIC_WRITE, FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL)) == INVALID_HANDLE_VALUE) {
		if (GetLastError() == ERROR_SHARING_VIOLATION) {
			Sleep(delay);
			if (delay < 5120)
				delay *= 2;
		}
		else
			break;
	}
	if (hFile)
		CloseHandle(hFile);
	else {
		showError("Unable to update Geode: Geode.dll is open by another process.");
		return 1;
	}

	auto error = std::error_code();

	if (ghc::filesystem::exists(updatesDir / "Geode.dll", error) && !error) {
		ghc::filesystem::rename(updatesDir / "Geode.dll", workingDir / "Geode.dll", error);
		if (error) {
			showError("Unable to update Geode: Unable to move Geode.dll - " + error.message());
			return error.value();
		}
	}

	if (ghc::filesystem::exists(updatesDir / "XInput9_1_0.dll", error) && !error) {
		ghc::filesystem::rename(updatesDir / "XInput9_1_0.dll", workingDir / "XInput9_1_0.dll", error);
		if (error) {
			showError("Unable to update Geode: Unable to move XInput9_1_0.dll - " + error.message());
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

	if(argc < 2)
		return 0;

	// restart gd using the provided path
	ShellExecute(NULL, "open", (workingDir / argv[1]).string().c_str(), "", workingDir.string().c_str(), TRUE);

	return 0;
}
