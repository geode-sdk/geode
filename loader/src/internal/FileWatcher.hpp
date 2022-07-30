#pragma once

#include <functional>
#include <string>
#include <fs/filesystem.hpp>
#include <Geode/DefaultInclude.hpp>
#include <Geode/utils/types.hpp>

class FileWatcher {
public:
	using FileWatchCallback = std::function<void(ghc::filesystem::path)>;
	using ErrorCallback = std::function<void(std::string)>;

protected:
	ghc::filesystem::path m_file;
	FileWatchCallback m_callback;
	ErrorCallback m_error;
	bool m_filemode = false;

	void* m_platform_handle;
	bool m_exiting = false;
	void watch();

public:
	bool watching() const;

	ghc::filesystem::path path() {
		return m_file;
	}

	FileWatcher(ghc::filesystem::path const& file, FileWatchCallback callback, ErrorCallback error = nullptr);
	~FileWatcher();
};
