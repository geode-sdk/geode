#include <FileWatcher.hpp>

FileWatcher::FileWatcher(
    std::filesystem::path const& file, FileWatchCallback callback, ErrorCallback error
) {
    m_filemode = std::filesystem::is_regular_file(file);

    m_platformHandle = nullptr;
    m_file = file;
    m_callback = callback;
    m_error = error;
}

FileWatcher::~FileWatcher() {
}

void FileWatcher::watch() {
	// TODO: setup inotify or something
}

bool FileWatcher::watching() const {
    return false;
}
