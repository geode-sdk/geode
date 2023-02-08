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

    ghc::filesystem::path path() {
        return m_file;
    }

    FileWatcher(
        ghc::filesystem::path const& file, FileWatchCallback callback, ErrorCallback error = nullptr
    );
    ~FileWatcher();
};
