#pragma once

#include <Geode/DefaultInclude.hpp>
#include "Result.hpp"
#include <string>
#include <vector>
#include <functional>
#include "fs/filesystem.hpp"

namespace geode::utils::clipboard {
    GEODE_DLL bool write(std::string const& data);
    GEODE_DLL std::string read();
}

namespace geode::utils::dirs {
    GEODE_DLL ghc::filesystem::path geodeRoot();
    GEODE_DLL bool openFolder(ghc::filesystem::path const& path);
}

namespace geode::utils::web {
    GEODE_DLL void openLinkInBrowser(std::string const& url);
}
