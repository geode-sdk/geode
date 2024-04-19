#include <Geode/DefaultInclude.hpp>

using namespace geode::prelude;

#include <Geode/loader/Dirs.hpp>
#include <UIKit/UIKit.h>
#include <iostream>
#include <sstream>
#include <Geode/utils/web.hpp>
#include <Geode/utils/permission.hpp>
#include <Geode/utils/cocos.hpp>
#include <Geode/binding/GameManager.hpp>
#include <Geode/binding/AppDelegate.hpp>
#include <Geode/binding/MenuLayer.hpp>

bool utils::clipboard::write(std::string const& data) {
    [UIPasteboard generalPasteboard].string = [NSString stringWithUTF8String:data.c_str()];
    return true;
}

std::string utils::clipboard::read() {
    return std::string([[UIPasteboard generalPasteboard].string UTF8String]);
}

void utils::web::openLinkInBrowser(std::string const& url) {
    [[UIApplication sharedApplication]
        openURL:[NSURL URLWithString:[NSString stringWithUTF8String:url.c_str()]]];
}

bool utils::file::openFolder(ghc::filesystem::path const& path) {
    // TODO: maybe we can just copy the one from mac
    return false;
}

void file::pickFile(
    PickMode mode, FilePickOptions const& options,
    MiniFunction<void(ghc::filesystem::path)> callback,
    MiniFunction<void()> failed
) {
    // TODO
}

void geode_nslog(uintptr_t x) {
    NSLog(@"geode %lx", x);
}

// TODO: copied those two from android but idk maybe shouldve copied from mac
void geode::utils::game::exit() {
    // TODO: yeah
    // if (CCApplication::sharedApplication() &&
    //     (GameManager::get()->m_playLayer || GameManager::get()->m_levelEditorLayer)) {
    //     log::error("Cannot exit in PlayLayer or LevelEditorLayer!");
    //     return;
    // }
    AppDelegate::get()->trySaveGame(true);
    // AppDelegate::get()->showLoadingCircle(false, true);

    CCDirector::get()->getActionManager()->addAction(CCSequence::create(
        CCDelayTime::create(0.5f),
        CCCallFunc::create(nullptr, callfunc_selector(MenuLayer::endGame)),
        nullptr
    ), CCDirector::get()->getRunningScene(), false);
}

void geode::utils::game::restart() {
    log::error("Restarting the game is not implemented on iOS");
}

void crashlog::setupPlatformHandlerPost() {}

void geode::utils::game::launchLoaderUninstaller(bool deleteSaveData) {
    log::error("Launching Geode uninstaller is not supported on android");
}

CCPoint cocos::getMousePos() {
    return CCPoint(0, 0);
}

ghc::filesystem::path dirs::getGameDir() {
    return ghc::filesystem::current_path();
}

ghc::filesystem::path dirs::getModRuntimeDir() {
    return dirs::getGeodeDir() / "unzipped";
}

ghc::filesystem::path dirs::getSaveDir() {
    static auto path = [] {
        NSArray *paths = NSSearchPathForDirectoriesInDomains(NSApplicationSupportDirectory, NSUserDomainMask, YES);
        NSString *applicationSupportDirectory = [paths firstObject];

        ghc::filesystem::path supportPath = [applicationSupportDirectory UTF8String];
        auto currentPath = supportPath / "GeometryDash";
        return currentPath;
    }();
    return path;
}

bool geode::utils::permission::getPermissionStatus(Permission permission) {
    return true; // unimplemented
}

void geode::utils::permission::requestPermission(Permission permission, utils::MiniFunction<void(bool)> callback) {
    callback(true); // unimplemented
}


#include "../../utils/thread.hpp"

std::string geode::utils::thread::getDefaultName() {
    uint64_t tid = 0ul;
    pthread_threadid_np(nullptr, &tid);

    return fmt::format("Thread #{}", tid);
}

void geode::utils::thread::platformSetName(std::string const& name) {
    pthread_setname_np(name.c_str());
}
