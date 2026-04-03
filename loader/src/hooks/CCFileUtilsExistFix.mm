#include <Geode/Geode.hpp>
#include <Geode/modify/CCFileUtils.hpp>
#include <Geode/utils/VMTHookManager.hpp>

#define CommentType CommentTypeDummy
#import <Foundation/Foundation.h>
#undef CommentType

using namespace geode::prelude;

// CCFileUtils::isFileExist on MacOS/iOS will always return false if the path is a
// relative path that does not contain any slashes, so we reimplement it properly.
// Thanks cocos!

static bool isFileExistImpl(geode::ZStringView path) {
    if (path.empty()) return false;

    auto view = path.view();

    if (view[0] == '/') {
        // absolute path
        return [[NSFileManager defaultManager] fileExistsAtPath:[NSString stringWithUTF8String:path.c_str()]];
    }

    StringBuffer<512> pathBuf;
    StringBuffer<512> fileBuf;
    size_t pos = view.find_last_of('/');
    if (pos != std::string::npos) {
        fileBuf.append(view.substr(pos + 1));
        pathBuf.append(view.substr(0, pos));
    } else {
        fileBuf.append(view);
    }

    NSString* nspath = pathBuf.size() == 0 ? nil : [NSString stringWithUTF8String:pathBuf.c_str()];
    NSString* nsfile = [NSString stringWithUTF8String:fileBuf.c_str()];
    NSString* fullpath = [[NSBundle mainBundle] pathForResource:nsfile ofType:nil inDirectory:nspath];

    return fullpath != nil;
}

#ifdef GEODE_IS_MACOS
class $modify(CCFileUtilsExistFix, CCFileUtilsMac) {
    bool isFileExist(const gd::string& path) {
        return isFileExistImpl(path);
    }
};
#endif

#ifdef GEODE_IS_IOS
class $modify(CCFileUtilsExistFix, CCFileUtilsIOS) {
    bool isFileExist(const gd::string& path) {
        return isFileExistImpl(path);
    }
};
#endif

