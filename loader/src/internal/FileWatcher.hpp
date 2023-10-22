#pragma once

#include <Geode/DefaultInclude.hpp>
//#include <Geode/utils/general.hpp>
#include <Geode/utils/MiniFunction.hpp>
#include <ghc/fs_fwd.hpp>
#include <functional>
#include <string>

class FileWatcher {
public:
    using FileWatchCallback = geode::utils::MiniFunction<void(ghc::filesystem::path)>;
    using ErrorCallback = geode::utils::MiniFunction<void(std::string)>;

protected:
    ghc::filesystem::path m_file;
    FileWatchCallback m_callback;
    ErrorCallback m_error;
    bool m_filemode = false;

    void* m_platformHandle;
    bool m_exiting = false;
    void watch();

public:
    bool watching() const;

    ghc::filesystem::path path() const {
        return m_file;
    }

    FileWatcher(
        ghc::filesystem::path const& file,
        FileWatchCallback callback,
        ErrorCallback error = nullptr
    );
    FileWatcher(FileWatcher const&) = delete;
    inline FileWatcher(FileWatcher&& other)
      : m_file(std::move(other.m_file)),
        m_callback(std::move(other.m_callback)),
        m_error(std::move(other.m_error)),
        m_filemode(other.m_filemode),
        m_platformHandle(other.m_platformHandle),
        m_exiting(other.m_exiting)
    {
        other.m_callback = nullptr;
        other.m_error = nullptr;
        other.m_platformHandle = nullptr;
        other.m_exiting = true;
    }
    ~FileWatcher();
};
