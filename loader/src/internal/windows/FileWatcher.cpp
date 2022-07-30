#include <FileWatcher.hpp>
#include <iostream>
#include <thread>

#ifdef GEODE_IS_WINDOWS

static constexpr const auto notifyAttributes = FILE_NOTIFY_CHANGE_LAST_WRITE | FILE_NOTIFY_CHANGE_ATTRIBUTES | FILE_NOTIFY_CHANGE_SIZE;

FileWatcher::FileWatcher(ghc::filesystem::path const& file, FileWatchCallback callback, ErrorCallback error) {
	this->m_filemode = ghc::filesystem::is_regular_file(file);
	auto handle =  FindFirstChangeNotificationW(
		(this->m_filemode ? file.parent_path() : file).wstring().c_str(),
		false,
		notifyAttributes
	);
	this->m_platform_handle = (void*)handle;

	this->m_file = file;
	this->m_callback = callback;
	this->m_error = error;
	if (handle != INVALID_HANDLE_VALUE) {
		std::thread(&FileWatcher::watch, this).detach();
	} else {
		if (this->m_error) this->m_error("Invalid handle");
	}
}

FileWatcher::~FileWatcher() {
	HANDLE handle = (HANDLE)this->m_platform_handle;
	FindCloseChangeNotification(handle);
	this->m_exiting = true;
}

void FileWatcher::watch() {
	HANDLE handle = (HANDLE)this->m_platform_handle;
	while (WaitForSingleObject(handle, 10000) == WAIT_OBJECT_0) {
		if (this->m_exiting) return;
		if (this->m_callback) {
			if (this->m_filemode) {
				auto file = CreateFileW(
					this->m_file.parent_path().wstring().c_str(),
					FILE_LIST_DIRECTORY,
					FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,
					nullptr,
					OPEN_EXISTING,
					FILE_FLAG_BACKUP_SEMANTICS | FILE_FLAG_OVERLAPPED,
					nullptr
				);
				if (file == INVALID_HANDLE_VALUE) {
					handle = nullptr;
					if (this->m_error) this->m_error("Reading dir failed");
					return;
				}
				OVERLAPPED overlapped;
				overlapped.hEvent = CreateEvent(NULL, FALSE, 0, NULL);
				std::vector<DWORD> buffer;
				buffer.resize(1024);
				if (!ReadDirectoryChangesW(
					file, buffer.data(), buffer.size(), false,
					notifyAttributes,
					nullptr, &overlapped, nullptr
				)) {
					handle = nullptr;
					if (this->m_error) this->m_error("Reading dir changes failed");
					return;
				}
				DWORD result = WaitForSingleObject(overlapped.hEvent, 500);
				if (result != WAIT_OBJECT_0 && result != WAIT_TIMEOUT) {
					handle = nullptr;
					if (this->m_error) this->m_error("Overlap hEvent was not WAIT_OBJECT_0");
					return;
				}
				DWORD bytes_transferred;
				GetOverlappedResult(file, &overlapped, &bytes_transferred, FALSE);
				auto info = reinterpret_cast<FILE_NOTIFY_INFORMATION*>(buffer.data());
				do {
					auto filename = std::wstring(info->FileName, info->FileName + info->FileNameLength / sizeof(wchar_t));
					if (
						ghc::filesystem::exists(this->m_file) &&
						ghc::filesystem::file_size(this->m_file) > 1000 &&
						info->Action == FILE_ACTION_MODIFIED && 
						this->m_file.filename().wstring() == filename
					) {
						this->m_callback(this->m_file);
					}
				} while (info->NextEntryOffset && (info = info + info->NextEntryOffset));
			} else {
				this->m_callback(this->m_file);
			}
		}
		if (!FindNextChangeNotification(handle)) {
			handle = nullptr;
			if (this->m_error) this->m_error("FindNextChangeNotification failed");
			return;
		}
	}
	handle = nullptr;
	if (this->m_error) this->m_error("WaitForSingleObject failed");
}

bool FileWatcher::watching() const {
	HANDLE handle = (HANDLE)this->m_platform_handle;
	return handle != INVALID_HANDLE_VALUE &&
		   handle != nullptr;
}

#endif
