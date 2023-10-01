#include <Geode/DefaultInclude.hpp>

#ifdef GEODE_IS_ANDROID

using namespace geode::prelude;

#include <Geode/utils/cocos.hpp>
#include <Geode/loader/Dirs.hpp>
#include <Geode/utils/web.hpp>
#include <ghc/filesystem.hpp>

ghc::filesystem::path dirs::getGameDir() {
    return ghc::filesystem::path(
        "/storage/emulated/0/Android/data/com.geode.launcher/files/game"
        // "/data/user/0/com.geode.launcher/files/"
        /*CCFileUtils::sharedFileUtils()->getWritablePath().c_str()*/
    );
}

ghc::filesystem::path dirs::getSaveDir() {
    return ghc::filesystem::path(
        "/storage/emulated/0/Android/data/com.geode.launcher/files/save"
        // "/data/user/0/com.geode.launcher/files/"
        /*CCFileUtils::sharedFileUtils()->getWritablePath().c_str()*/
    );
}

ghc::filesystem::path dirs::getModRuntimeDir() {
    return ghc::filesystem::path(
        "/data/user/0/com.geode.launcher/files/geode/unzipped"
    );
    // return dirs::getGeodeDir() / "unzipped";
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

void geode::utils::game::launchLoaderUninstaller(bool deleteSaveData) {
    log::error("Launching Geode uninstaller is not supported on android");
}

void geode::utils::game::exit() {
    CCDirector::get()->getActionManager()->addAction(CCSequence::create(
        CCDelayTime::create(0.5f),
        CCCallFunc::create(nullptr, callfunc_selector(MenuLayer::endGame)),
        nullptr
    ), CCDirector::get()->getRunningScene(), false);;
}

void geode::utils::game::restart() {
    // Not implemented
    log::error("Restarting the game is not implemented on android");
}

#endif
