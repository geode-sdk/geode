#include <Windows.h>
#include <iostream>
#include <array>
#include <ghc/filesystem.hpp>

ghc::filesystem::path workingDir;
ghc::filesystem::path geodeDir;
ghc::filesystem::path updatesDir;
ghc::filesystem::path resourcesDir;

void showError(std::string const& error) {
	MessageBoxA(nullptr, error.c_str(), "Error Loading Geode", MB_ICONERROR);
}

bool waitForFile(ghc::filesystem::path const& path) {
	if (!path.has_filename())
		return false;

	int delay = 10;
	HANDLE hFile;
	while ((hFile = CreateFile(path.string().c_str(), FILE_GENERIC_WRITE, FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL)) == INVALID_HANDLE_VALUE) {
		if (GetLastError() == ERROR_SHARING_VIOLATION) {
			Sleep(delay);
			// the delay would raise and go up to about 5 seconds, after which it will fail
			if (delay < 5120)
				delay *= 2;
		}
		else
			break;
	}
	if (hFile)
		CloseHandle(hFile);
	else {
		showError("Unable to update Geode: " + path.filename().string() + " is open by another process.");
		return false;
	}
	return true;
}

void updateFile(std::string const& name) {
	auto error = std::error_code();
	if (!ghc::filesystem::exists(updatesDir / name, error) || error)
		return;
	if (!waitForFile(workingDir / name))
		return;

	ghc::filesystem::rename(updatesDir / name, workingDir / name, error);
	if (error) {
		showError("Unable to update Geode: Unable to move " + name + " - " + error.message());
		return;
	}
}

void removePath(ghc::filesystem::path const& path) {
	auto error = std::error_code();
	if (!ghc::filesystem::exists(path, error) || error)
		return;
	if (path.has_filename() && !waitForFile(path))
		return;

	if (ghc::filesystem::is_directory(path) && !ghc::filesystem::is_empty(path))
		ghc::filesystem::remove_all(path, error);
	ghc::filesystem::remove(path, error);
	if (error) {
		if (path.has_filename())
			showError("Unable to update Geode: Unable to remove " + path.filename().string() + " - " + error.message());
		else
			showError("Unable to update Geode: Unable to remove " + path.string() + " - " + error.message());
		return;
	}
}

void removeDirectory(std::string const& name) {
	auto error = std::error_code();
	if (!ghc::filesystem::exists(workingDir / name, error) || error)
		return;
	if (!waitForFile(workingDir / name))
		return;

	ghc::filesystem::remove(workingDir / name, error);
	if (error) {
		showError("Unable to update Geode: Unable to remove " + name + " - " + error.message());
		return;
	}
}

void updateResources() {
	auto error = std::error_code();
	if (!ghc::filesystem::exists(updatesDir / "resources", error) || error)
		return;

	ghc::filesystem::remove_all(resourcesDir / "geode.loader", error);
	if (error) {
		showError("Unable to update Geode resources:" + error.message());
		return;
	}

	ghc::filesystem::rename(updatesDir / "resources", resourcesDir / "geode.loader", error);
	if (error) {
		showError("Unable to update Geode resources: " + error.message());
		return;
	}
}

int main(int argc, char* argv[]) {
	workingDir = ghc::filesystem::current_path();
	geodeDir = workingDir / "geode";
	updatesDir = geodeDir / "update";
	resourcesDir = geodeDir / "resources";

	if (ghc::filesystem::exists(geodeDir) && ghc::filesystem::exists(updatesDir)) {
		removePath(workingDir / "GeodeBootstrapper.dll");
		updateFile("XInput9_1_0.dll");
		updateFile("Geode.dll");
		updateResources();
		removePath(updatesDir);
	}

	if(argc < 2)
		return 0;

	// restart gd using the provided path
	ShellExecute(NULL, "open", (workingDir / argv[1]).string().c_str(), "", workingDir.string().c_str(), TRUE);

	return 0;
}
