#include <Geode/DefaultInclude.hpp>

using namespace geode::prelude;

#include <Geode/loader/Dirs.hpp>
#include <UIKit/UIKit.h>
#include <UniformTypeIdentifiers/UniformTypeIdentifiers.h>
#include <AVFoundation/AVFoundation.h>
#include <iostream>
#include <sstream>
#include <Geode/utils/web.hpp>
#include <Geode/utils/permission.hpp>
#include <Geode/utils/cocos.hpp>
#include <Geode/binding/GameManager.hpp>
#include <Geode/binding/AppDelegate.hpp>
#include <Geode/binding/MenuLayer.hpp>
#include <Geode/binding/FLAlertLayer.hpp>
#include <Geode/Utils.hpp>
#include <objc/runtime.h>
#include <stdlib.h>

using geode::utils::permission::Permission;

bool utils::clipboard::write(std::string const& data) {
    [UIPasteboard generalPasteboard].string = [NSString stringWithUTF8String:data.c_str()];
    return true;
}

std::string utils::clipboard::read() {
    return std::string([[UIPasteboard generalPasteboard].string UTF8String]);
}

void utils::web::openLinkInBrowser(std::string const& url) {
    [[UIApplication sharedApplication]
        openURL:[NSURL URLWithString:[NSString stringWithUTF8String:url.c_str()]] options:{} completionHandler:nil];
}

#pragma region Folder Pick Delegate

@interface PickerDelegate : NSObject <UIDocumentPickerDelegate>
@property (nonatomic, copy) void (^completion)(NSArray<NSURL*>* urls, NSError* error);
- (instancetype)initWithCompletion:(void (^)(NSArray<NSURL*>* urls, NSError* error))completion;
@end

@implementation PickerDelegate
- (instancetype)initWithCompletion:(void (^)(NSArray<NSURL*>* urls, NSError* error))completion {
    self = [super init];
    if (self) {
        _completion = [completion copy];
    }
    return self;
}

- (void)documentPicker:(UIDocumentPickerViewController*)controller didPickDocumentsAtURLs:(NSArray<NSURL*>*)urls {
    if (self.completion) {
        self.completion(urls, nil);
    }
}

- (void)documentPickerWasCancelled:(UIDocumentPickerViewController*)controller {
    if (self.completion) {
        self.completion(nil, [NSError errorWithDomain:NSCocoaErrorDomain code:NSUserCancelledError userInfo:nil]);
    }
}

@end

PickerDelegate* PickerDelegate_instance = nil;

#pragma endregion

UIViewController* getCurrentViewController() {
    UIWindow *window = [[UIApplication sharedApplication] keyWindow];
    UIViewController *rootViewController = window.rootViewController;

    while (rootViewController.presentedViewController) {
        rootViewController = rootViewController.presentedViewController;
    }

    return rootViewController;
}

bool utils::file::openFolder(std::filesystem::path const& path) {
    std::string newPath = fmt::format("{}://{}", getenv("GEODEINJECT_LOADED") ? "filza" : "shareddocuments", path);
    NSURL *url = [NSURL URLWithString:[NSString stringWithUTF8String:newPath.c_str()]];
    if ([[UIApplication sharedApplication] canOpenURL:url]) {
        [[UIApplication sharedApplication] openURL:url options:@{} completionHandler:nil];
        return true;
    }
    return false;
}

GEODE_DLL Task<Result<std::filesystem::path>> file::pick(file::PickMode mode, file::FilePickOptions const& options) {
    using RetTask = Task<Result<std::filesystem::path>>;
    return RetTask::runWithCallback([mode, options](auto resultCallback, auto progress, auto cancelled) {

        NSMutableArray<UTType*> *documentTypes = [NSMutableArray array];
        for (const auto& filter : options.filters) {
            for (const auto& file : filter.files) {
                UTType* uti = [UTType typeWithFilenameExtension:@(file.c_str())];
                if (uti) {
                    [documentTypes addObject:uti];
                }
            }
        }

        NSURL *FileURL = [NSURL fileURLWithPath:[NSTemporaryDirectory() stringByAppendingPathComponent:@"temp.file"]];
        if (options.defaultPath && !options.defaultPath->parent_path().empty()) {
            FileURL = [NSURL fileURLWithPath:[NSString stringWithUTF8String:options.defaultPath->c_str()]];
        }
        else if (options.defaultPath) {
            auto FileExtension = [NSString stringWithUTF8String:options.defaultPath->c_str()];
            FileURL = [NSURL fileURLWithPath:[NSTemporaryDirectory() stringByAppendingPathComponent:FileExtension]];
        }
        
        // just for the picker not to crash, it gotta have a file to "save" then the writing is handled in the mod once we save the file somewhere
        [@"" writeToURL:FileURL atomically:NO encoding:NSUTF8StringEncoding error:nil];

        if (documentTypes.count == 0) {
            [documentTypes addObject:UTTypeItem]; // Default to any file type if no filters are provided
        }

        UIDocumentPickerViewController *picker;
        switch (mode) {
            case file::PickMode::OpenFile:
                picker = [[UIDocumentPickerViewController alloc]
                    initForOpeningContentTypes:documentTypes
                    asCopy:YES];
                break;
            case file::PickMode::SaveFile:
                picker = [[UIDocumentPickerViewController alloc]
                    initForExportingURLs:@[FileURL]
                    asCopy:YES];
                break;
            case file::PickMode::OpenFolder:
                picker = [[UIDocumentPickerViewController alloc]
                    initForOpeningContentTypes:@[UTTypeFolder]
                    asCopy:YES];
                break;
        }
        picker.allowsMultipleSelection = NO;
        picker.shouldShowFileExtensions = YES;

        PickerDelegate_instance = [[PickerDelegate alloc] initWithCompletion:^(NSArray<NSURL*>* urls, NSError* error) {
            PickerDelegate_instance = nil;

            if (urls && urls.count > 0)
            {
                std::filesystem::path paths;

                for (NSURL* url : urls)
                {
                    if (url && url.path)
                    {
                        std::string pathStr = std::string([url.path UTF8String]);
                        auto path = std::filesystem::path(pathStr);

                        paths = path;
                    }
                }

                resultCallback(Ok(paths));

                for (NSURL* url : urls)
                {
                    if (url && url.path)
                    {
                        [url stopAccessingSecurityScopedResource];
                    }
                }
            }
            else if (cancelled()) {
                resultCallback(RetTask::Cancel());
            } else if (error) {
                resultCallback(Err(std::string([[error localizedDescription] UTF8String])));
            } else {
                resultCallback(RetTask::Cancel());
            }
        }];

        picker.delegate = PickerDelegate_instance;

        dispatch_async(dispatch_get_main_queue(), ^{
            UIViewController *currentViewController = getCurrentViewController();
            [currentViewController presentViewController:picker animated:YES completion:nil];
        });
    });
}

GEODE_DLL Task<Result<std::vector<std::filesystem::path>>> file::pickMany(file::FilePickOptions const& options) {
    using RetTask = Task<Result<std::vector<std::filesystem::path>>>;
    return RetTask::runWithCallback([options](auto resultCallback, auto progress, auto cancelled) {
        NSMutableArray<NSString*> *documentTypes = [NSMutableArray array];
        for (const auto& filter : options.filters) {
            for (const auto& file : filter.files) {
                NSString* uti = [UTType typeWithFilenameExtension:@(file.c_str())].identifier;
                if (uti) {
                    [documentTypes addObject:uti];
                }
            }
        }
        if (documentTypes.count == 0) {
            [documentTypes addObject:(NSString*)UTTypeItem.identifier]; // Default to any file type if no filters are provided
        }

        UIDocumentPickerViewController* picker = [[UIDocumentPickerViewController alloc] initWithDocumentTypes:documentTypes inMode:UIDocumentPickerModeOpen];
        picker.allowsMultipleSelection = true;

        PickerDelegate_instance = [[PickerDelegate alloc] initWithCompletion:^(NSArray<NSURL*>* urls, NSError* error) {
            PickerDelegate_instance = nil;

            if (urls && urls.count > 0)
            {
                std::vector<std::filesystem::path> paths;

                for (NSURL* url : urls)
                {
                    if (url && url.path)
                    {
                        std::string pathStr = std::string([url.path UTF8String]);

                        if ([url startAccessingSecurityScopedResource])
                        {
                            auto path = std::filesystem::path(pathStr);

                            paths.push_back(path);
                        }
                        else
                        {
                            resultCallback(Err("Failed to access security-scoped resource: {}", pathStr));
                        }
                    }
                }

                resultCallback(Ok(paths));

                for (NSURL* url : urls)
                {
                    if (url && url.path)
                    {
                        [url stopAccessingSecurityScopedResource];
                    }
                }
            }
            else if (cancelled()) {
                resultCallback(RetTask::Cancel());
            } else if (error) {
                resultCallback(Err(std::string([[error localizedDescription] UTF8String])));
            } else {
                resultCallback(RetTask::Cancel());
            }
        }];

        picker.delegate = PickerDelegate_instance;

        dispatch_async(dispatch_get_main_queue(), ^{
            UIViewController *currentViewController = getCurrentViewController();
            [currentViewController presentViewController:picker animated:YES completion:nil];
        });
    });
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

    class Exit : public CCObject {
        public:
        void shutdown() {
            // someone please look into this, I'm unsure if this will cause issues with saving!
            std::exit(0);
        }
    };

    CCDirector::get()->getActionManager()->addAction(CCSequence::create(
        CCDelayTime::create(0.5f),
        CCCallFunc::create(nullptr, callfunc_selector(Exit::shutdown)),
        nullptr
    ), CCDirector::get()->getRunningScene(), false);
}

void geode::utils::game::restart() {
    AppDelegate::get()->trySaveGame(true);

    class Exit : public CCObject {
        public:
        void shutdown() {
            NSURL* url = [NSURL URLWithString:@"geode://relaunch"];
            if ([[UIApplication sharedApplication] canOpenURL:url]) {
                [[UIApplication sharedApplication] openURL:url options:@{} completionHandler:nil];
            } else {
                // this would only happen if you don't have the launcher
                FLAlertLayer::create(
                     "Unavailable",
                     "Restarting is currently <cr>unavailable</c>. Please <cy>restart the game</c> manually.",
                     "OK"
                 )->show();
            }
        }
    };

    CCDirector::get()->getActionManager()->addAction(CCSequence::create(
        CCDelayTime::create(0.5f),
        CCCallFunc::create(nullptr, callfunc_selector(Exit::shutdown)),
        nullptr
    ), CCDirector::get()->getRunningScene(), false);
}

void geode::utils::game::launchLoaderUninstaller(bool deleteSaveData) {
    log::error("Launching Geode uninstaller is not supported on iOS");
}

CCPoint cocos::getMousePos() {
    return CCPoint(0, 0);
}

namespace {
    std::string s_savedBaseDir = "";

    std::filesystem::path getBaseDir() {
        if (!s_savedBaseDir.empty()) {
            return std::filesystem::path(s_savedBaseDir);
        }

        NSArray *paths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
        NSString *documentsDirectory = [paths firstObject];

        std::filesystem::path documentsPath = [documentsDirectory UTF8String];

        s_savedBaseDir = documentsPath;
        return std::filesystem::path(documentsPath);
    }
}

std::filesystem::path dirs::getGameDir() {
    return getBaseDir() / "game";
}

std::filesystem::path dirs::getModRuntimeDir() {
    return dirs::getGeodeDir() / "unzipped";
}

std::filesystem::path dirs::getSaveDir() {
    return getBaseDir() / "save";
}

bool geode::utils::permission::getPermissionStatus(Permission permission) {
    switch (permission) {
        case Permission::RecordAudio: 
            return [[AVAudioSession sharedInstance] recordPermission] == AVAudioSessionRecordPermissionGranted;
        default: 
            return false;
    }
}

void geode::utils::permission::requestPermission(Permission permission, std::function<void(bool)> callback) {
    switch (permission) {
        case Permission::RecordAudio: 
            return [[AVAudioSession sharedInstance] requestRecordPermission:^(BOOL granted) {
                callback(granted == YES);
            }];
        default: // ios doesnt have a "access all files" permission
            return callback(false);
    }
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


Result<> geode::hook::addObjcMethod(std::string const& className, std::string const& selectorName, void* imp) {
    auto cls = objc_getClass(className.c_str());
    if (!cls)
        return Err("Class not found");

    auto sel = sel_registerName(selectorName.c_str());

    class_addMethod(cls, sel, (IMP)imp, "v@:");

    return Ok();
}
Result<void*> geode::hook::getObjcMethodImp(std::string const& className, std::string const& selectorName) {
    auto cls = objc_getClass(className.c_str());
    if (!cls)
        return Err("Class not found");

    auto sel = sel_registerName(selectorName.c_str());

    auto method = class_getInstanceMethod(cls, sel);
    if (!method)
        return Err("Method not found");

    return Ok((void*)method_getImplementation(method));
}

Result<void*> geode::hook::replaceObjcMethod(std::string const& className, std::string const& selectorName, void* imp) {
    auto cls = objc_getClass(className.c_str());
    if (!cls)
        return Err("Class not found");

    auto sel = sel_registerName(selectorName.c_str());

    auto method = class_getInstanceMethod(cls, sel);
    if (!method)
        return Err("Method not found");

    auto oldImp = method_setImplementation(method, (IMP)imp);

    return Ok((void*)oldImp);
}
