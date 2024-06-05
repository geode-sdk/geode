#include <Geode/DefaultInclude.hpp>

using namespace geode::prelude;

#include <Geode/loader/Dirs.hpp>
#include <UIKit/UIKit.h>
#include <iostream>
#include <sstream>
#include <Geode/utils/web.hpp>

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

void geode_nslog(uintptr_t x) {
    NSLog(@"geode %lx", x);
}


std::filesystem::path dirs::getGameDir() {
    return std::filesystem::current_path();
}

std::filesystem::path dirs::getSaveDir() {
    return weaklyCanonical(CCFileUtils::sharedFileUtils()->getWritablePath().c_str());
}

bool geode::utils::permission::getPermissionStatus(Permission permission) {
    return true; // unimplemented
}

void geode::utils::permission::requestPermission(Permission permission, utils::MiniFunction<void(bool)> callback) {
    callback(true); // unimplemented
}
