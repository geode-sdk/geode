#pragma once

#include <Geode/DefaultInclude.hpp>
#include "Result.hpp"
#include <string>
#include <unordered_set>
#include <vector>
#include <functional>
#include "fs/filesystem.hpp"

namespace geode::utils::clipboard {
    GEODE_DLL bool write(std::string const& data);
    GEODE_DLL std::string read();
}

namespace geode::utils::file {
    GEODE_DLL ghc::filesystem::path geodeRoot();
    GEODE_DLL bool openFolder(ghc::filesystem::path const& path);

    enum class PickMode {
        OpenFile,
        SaveFile,
        OpenFolder,
    };

    struct FilePickOptions {
        struct Filter {
            // Name of the filter
            std::string description;
            // Extensions (*.txt, *.doc, *.mp3, etc.)
            std::unordered_set<std::string> files;
        };
        ghc::filesystem::path defaultPath;
        std::vector<Filter> filters;
    };
    
    GEODE_DLL Result<ghc::filesystem::path> pickFile(
        PickMode mode,
        FilePickOptions const& options
    );
    GEODE_DLL Result<std::vector<ghc::filesystem::path>> pickFiles(
        FilePickOptions const& options
    );
}

namespace geode::utils::web {
    GEODE_DLL void openLinkInBrowser(std::string const& url);
}
