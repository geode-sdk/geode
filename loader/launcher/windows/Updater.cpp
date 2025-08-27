#include <Windows.h>
#include <iostream>
#include <array>
#include <filesystem>

std::filesystem::path workingDir;
std::filesystem::path geodeDir;
std::filesystem::path updatesDir;
std::filesystem::path resourcesDir;

void showError(std::wstring const& error) {
    MessageBoxW(nullptr, error.c_str(), L"Error Loading Geode", MB_ICONERROR);
}

std::wstring utf8ToWide(std::string const& str) {
    int size = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, nullptr, 0);
    std::wstring wstr(size, L'\0');
    MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, &wstr[0], size);
    return wstr;
}

void showError(std::wstring error, std::error_code ec) {
    error += L" - " + utf8ToWide(ec.message());
    MessageBoxW(nullptr, error.c_str(), L"Error Loading Geode", MB_ICONERROR);
}

bool waitForFile(std::filesystem::path const& path) {
    if (!path.has_filename())
        return false;

    int delay = 10;
    int maxDelayAttempts = 20;
    HANDLE hFile;
    while ((hFile = CreateFileW(path.wstring().c_str(), FILE_GENERIC_WRITE, FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL)) == INVALID_HANDLE_VALUE) {
        if (GetLastError() == ERROR_SHARING_VIOLATION) {
            Sleep(delay);
            // the delay would raise and go up to about 1 second, after which it will start a 20 second countdown
            if (delay < 1024) {
                delay *= 2;
            } else {
                maxDelayAttempts--;
                // delay too long, failed too many times, just give up now
                if (maxDelayAttempts == 0) {
                    hFile = NULL;
                    break;
                }
            }
        } else {
            break;
        }
    }
    if (hFile) {
        CloseHandle(hFile);
    } else {
        showError(L"Unable to update Geode: " + path.filename().wstring() + L" is open by another process.");
        return false;
    }
    return true;
}

bool updateFile(std::string const& name) {
    std::error_code error;
    if (!std::filesystem::exists(updatesDir / name, error) || error)
        return true;
    if (!waitForFile(workingDir / name))
        return false;

    std::filesystem::rename(updatesDir / name, workingDir / name, error);
    if (error) {
        showError(L"Unable to update Geode: Unable to move " + utf8ToWide(name), error);
        return false;
    }
    return true;
}

void removePath(std::filesystem::path const& path) {
    std::error_code error;
    if (!std::filesystem::exists(path, error) || error)
        return;
    if (path.has_filename() && !waitForFile(path))
        return;

    if (std::filesystem::is_directory(path) && !std::filesystem::is_empty(path))
        std::filesystem::remove_all(path, error);
    std::filesystem::remove(path, error);
    if (error) {
        if (path.has_filename())
            showError(L"Unable to update Geode: Unable to remove " + path.filename().wstring(), error);
        else
            showError(L"Unable to update Geode: Unable to remove " + path.wstring(), error);
        return;
    }
}

void updateResources() {
    std::error_code error;
    if (!std::filesystem::exists(updatesDir / "resources", error) || error)
        return;

    std::filesystem::remove_all(resourcesDir / "geode.loader", error);
    if (error) {
        showError(L"Unable to update Geode resources", error);
        return;
    }

    std::filesystem::rename(updatesDir / "resources", resourcesDir / "geode.loader", error);
    if (error) {
        showError(L"Unable to update Geode resources", error);
        return;
    }
}

int main(int argc, char* argv[]) {
    workingDir = std::filesystem::current_path();
    geodeDir = workingDir / "geode";
    updatesDir = geodeDir / "update";
    resourcesDir = geodeDir / "resources";

    if (std::filesystem::exists(workingDir / "GeodeBootstrapper.dll"))
        removePath(workingDir / "GeodeBootstrapper.dll");

    if (std::filesystem::exists(geodeDir) && std::filesystem::exists(updatesDir)) {
        bool updateSuccess = true;
        updateSuccess &= updateFile("XInput1_4.dll");
        updateSuccess &= updateFile("Geode.dll");
        updateSuccess &= updateFile("Geode.pdb");
        updateResources();
        // if couldnt update the files, dont delete the updates folder
        if (updateSuccess)
            removePath(updatesDir);
    }

    if (argc < 2)
        return 0;

    if (!waitForFile(workingDir / argv[1])) {
        showError(L"There was an error restarting GD. Please, restart the game manually.");
        return 0;
    }

    // restart gd using the provided path
    ShellExecuteW(NULL, L"open", (workingDir / argv[1]).wstring().c_str(), L"", workingDir.wstring().c_str(), TRUE);
    return 0;
}
