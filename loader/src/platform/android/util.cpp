#include <Geode/DefaultInclude.hpp>

#ifdef GEODE_IS_ANDROID

using namespace geode::prelude;

#include <Geode/utils/cocos.hpp>
#include <Geode/loader/Dirs.hpp>
#include <Geode/utils/web.hpp>
#include <ghc/filesystem.hpp>

ghc::filesystem::path dirs::getGameDir() {
    return ghc::filesystem::path(CCFileUtils::sharedFileUtils()->getWritablePath().c_str());
}

ghc::filesystem::path dirs::getSaveDir() {
    return ghc::filesystem::path(CCFileUtils::sharedFileUtils()->getWritablePath().c_str());
}

void utils::web::openLinkInBrowser(std::string const& url) {
    CCApplication::sharedApplication()->openURL(url.c_str());
}

bool utils::file::openFolder(ghc::filesystem::path const&) {
    return false;
}

geode::Result<ghc::filesystem::path> utils::file::
    pickFile(geode::utils::file::PickMode, geode::utils::file::FilePickOptions const&) {
    return geode::Err("This function is currently unimplemented");
}

#endif
