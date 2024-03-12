#pragma once

#include <Geode/DefaultInclude.hpp>
#include <Geode/utils/MiniFunction.hpp>
#include <ghc/fs_fwd.hpp>
#include <functional>
#include <string>
#include <memory>

class FileWatcher {
public:
    using FileWatchCallback = geode::utils::MiniFunction<void(ghc::filesystem::path)>;
    using ErrorCallback = geode::utils::MiniFunction<void(std::string)>;

protected:
    ghc::filesystem::path m_file;
    FileWatchCallback m_callback;
    ErrorCallback m_error;
    bool m_filemode = false;

    std::unique_ptr<void, void(*)(void*)> m_platformHandle;
    bool m_exiting = false;
    void watch();

public:
    bool watching() const;

    const ghc::filesystem::path& path() const {
        return m_file;
    }

    FileWatcher(
        const ghc::filesystem::path& file,
        FileWatchCallback callback,
        ErrorCallback error = nullptr
    ) : m_file(file),
        m_callback(std::move(callback)),
        m_error(std::move(error)),
        m_platformHandle(nullptr, nullptr),
        m_exiting(false)
    {
        watch();
    }

    FileWatcher(const FileWatcher&) = delete;
    FileWatcher(FileWatcher&& other) noexcept
        : m_file(std::move(other.m_file)),
          m_callback(std::move(other.m_callback)),
          m_error(std::move(other.m_error)),
          m_platformHandle(std::move(other.m_platformHandle)),
          m_exiting(other.m_exiting)
    {
        other.m_exiting = true;
    }

    ~FileWatcher();
};
