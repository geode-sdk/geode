#include "Geode/utils/file.hpp"
#include <Geode/DefaultInclude.hpp>

using namespace geode::prelude;

#include <Geode/loader/Dirs.hpp>
#import <AppKit/AppKit.h>
#include <Geode/Utils.hpp>
#include <Geode/binding/GameManager.hpp>
#include <objc/runtime.h>
#include <Geode/utils/web.hpp>
#include <Geode/utils/Task.hpp>

#define CommentType CommentTypeDummy
#import <Cocoa/Cocoa.h>
#undef CommentType

bool utils::clipboard::write(std::string const& data) {
    [[NSPasteboard generalPasteboard] clearContents];
    [[NSPasteboard generalPasteboard] setString:[NSString stringWithUTF8String:data.c_str()]
                                        forType:NSPasteboardTypeString];

    return true;
}

std::string utils::clipboard::read() {
    auto clipboard =
        [[[NSPasteboard generalPasteboard] stringForType:NSPasteboardTypeString] UTF8String];
    if (!clipboard) return std::string("");
    return std::string(clipboard);
}

bool utils::file::openFolder(std::filesystem::path const& path) {
    NSURL* fileURL = [NSURL fileURLWithPath:[NSString stringWithUTF8String:path.string().c_str()]];
    NSURL* folderURL = [fileURL URLByDeletingLastPathComponent];
    [[NSWorkspace sharedWorkspace] openURL:folderURL];
    return true;
}

void utils::web::openLinkInBrowser(std::string const& url) {
    [[NSWorkspace sharedWorkspace]
        openURL:[NSURL URLWithString:[NSString stringWithUTF8String:url.c_str()]]];
}

/*@interface FileDialog : NSObject
+ (Result<std::filesystem::path>)importDocumentWithMode:(file::PickMode)mode options:(file::FilePickOptions const&)options mult:(bool)mult;
+ (Result<std::vector<std::filesystem::path>>)importDocumentsWithOptions:(file::FilePickOptions const&)options;
@end

@implementation FileDialog

+ (Result<std::filesystem::path>)importDocumentWithMode:(file::PickMode)mode options:(file::FilePickOptions const&)options mult:(bool)mult {
    NSOpenPanel* panel = [NSOpenPanel openPanel];
    // TODO: [panel setAllowedFileTypes:@[]];

    if (options.defaultPath) {
        auto defaultPath = [NSString stringWithCString:options.defaultPath->c_str() encoding:NSUTF8StringEncoding];
        [panel setDirectoryURL: [NSURL fileURLWithPath: defaultPath]];
    }

    panel.allowsMultipleSelection = mult;

    int result = [panel runModal];
    if (result == NSFileHandlingPanelOKButton) {
        auto fileUrl = [[panel URLs] objectAtIndex:0];
        auto path = std::string([[fileUrl path] UTF8String], [[fileUrl path] lengthOfBytesUsingEncoding:NSUTF8StringEncoding]);
        return Ok(std::filesystem::path(path));
    } else {
        return Err(result);
    }
}

@end*/

namespace {
    using FileResult = Result<std::vector<std::filesystem::path>>;
}

@interface FileDialog : NSObject
+(Result<std::vector<std::filesystem::path>>) filePickerWithMode:(file::PickMode)mode options:(file::FilePickOptions const&)options multiple:(bool)mult;
+(void) dispatchFilePickerWithMode:(file::PickMode)mode options:(file::FilePickOptions const&)options multiple:(bool)mult onCompletion:(void(^)(FileResult))onCompletion;
@end

@implementation FileDialog
+(Result<std::vector<std::filesystem::path>>) filePickerWithMode:(file::PickMode)mode options:(file::FilePickOptions const&)options multiple:(bool)mult {
    NSSavePanel* panel;
    if (mode == file::PickMode::SaveFile)
        panel = [NSSavePanel savePanel];
    else
        panel = [NSOpenPanel openPanel];

    [panel setCanCreateDirectories: TRUE];


    // default path
    if (options.defaultPath) {
        auto defaultPath = [NSString stringWithUTF8String:options.defaultPath->c_str()];
        [panel setDirectoryURL: [NSURL URLWithString: defaultPath]];
    }

    // other
    if (mode != file::PickMode::SaveFile) {
        auto openPanel = (NSOpenPanel*)panel;

        if (mode == file::PickMode::OpenFile){
            [openPanel setCanChooseDirectories: NO];
            [openPanel setCanChooseFiles: YES];
        }
        else {
            [openPanel setCanChooseDirectories: YES];
            [openPanel setCanChooseFiles: NO];
        }

        [openPanel setAllowsMultipleSelection: mult];

        // allowed files
        // TODO: allowed files using the NSOpenSavePanelDelegate xd
        // NSMutableArray* allowed = [NSMutableArray array];

        // for (auto& f : options.filters) {
        //     for (auto& i : f.files) {
        //         auto nsstr = [NSString stringWithUTF8String: i.c_str()];

        //         if (![allowed containsObject: nsstr])
        //             [allowed addObject: nsstr];
        //     }
        // }

        // if (options.filters.size())
        //     [panel setAllowedFileTypes: allowed];
    }

    // run thing

    int result = [panel runModal];

    if (result == NSModalResponseOK) {
        std::vector<std::filesystem::path> fileURLs;
        if (mode == file::PickMode::SaveFile) {
            fileURLs.push_back(std::string([[[panel URL] path] UTF8String]));
        }
        else {
            auto openPanel = (NSOpenPanel*)panel;

            for (NSURL* i in openPanel.URLs) {
                fileURLs.push_back(std::string(i.path.UTF8String));
            }
        }
        return Ok(fileURLs);
    } else {
        return Err("File picker cancelled");
    }
}

+(void) dispatchFilePickerWithMode:(file::PickMode)mode options:(file::FilePickOptions const&)options multiple:(bool)mult onCompletion:(void(^)(FileResult))onCompletion {
    dispatch_async(dispatch_get_main_queue(), ^{
        auto result = [self filePickerWithMode:mode options:options multiple:mult];
        onCompletion(result);
    });
}

@end

GEODE_DLL Task<Result<std::filesystem::path>> file::pick(file::PickMode mode, file::FilePickOptions const& options) {
    using RetTask = Task<Result<std::filesystem::path>>;
    return RetTask::runWithCallback([mode, options](auto resultCallback, auto progress, auto cancelled) {
        [FileDialog dispatchFilePickerWithMode:mode options:options multiple:false onCompletion: ^(FileResult result) {
            if (cancelled()) {
                resultCallback(RetTask::Cancel());
            } else {
                if (result.isOk()) {
                    std::filesystem::path path = result.unwrap()[0];
                    resultCallback(Ok(path));
                } else {
                    resultCallback(Err(result.err().value()));
                }
            }
        }];
    });
}

GEODE_DLL Task<Result<std::vector<std::filesystem::path>>> file::pickMany(file::FilePickOptions const& options) {
    using RetTask = Task<Result<std::vector<std::filesystem::path>>>;
    return RetTask::runWithCallback([options](auto resultCallback, auto progress, auto cancelled) {
        [FileDialog dispatchFilePickerWithMode: file::PickMode::OpenFile options:options multiple:true onCompletion: ^(FileResult result) {
            if (cancelled()) {
                resultCallback(RetTask::Cancel());
            } else {
                resultCallback(result);
            }
        }];
    });
}

CCPoint cocos::getMousePos() {
    auto windowFrame = NSApp.mainWindow.frame;
    auto viewFrame = NSApp.mainWindow.contentView.frame;
    auto scaleFactor = CCPoint(CCDirector::get()->getWinSize()) / ccp(viewFrame.size.width, viewFrame.size.height);
    auto mouse = [NSEvent mouseLocation];
    return ccp(mouse.x - windowFrame.origin.x, mouse.y - windowFrame.origin.y) * scaleFactor;
}

std::filesystem::path dirs::getGameDir() {
    static auto path = [] {
        std::array<char, PATH_MAX> gddir;

        uint32_t out = PATH_MAX;
        _NSGetExecutablePath(gddir.data(), &out);

        std::filesystem::path gdpath = gddir.data();
        auto currentPath = std::filesystem::canonical(gdpath.parent_path().parent_path());
        return currentPath;
    }();

    return path;
}

std::filesystem::path dirs::getSaveDir() {
    static auto path = [] {
        NSArray *paths = NSSearchPathForDirectoriesInDomains(NSApplicationSupportDirectory, NSUserDomainMask, YES);
        NSString *applicationSupportDirectory = [paths firstObject];

        std::filesystem::path supportPath = [applicationSupportDirectory UTF8String];
        auto currentPath = supportPath / "GeometryDash";
        return currentPath;
    }();

    return path;
}

std::filesystem::path dirs::getModRuntimeDir() {
    return dirs::getGeodeDir() / "unzipped";
}

void geode::utils::game::exit() {
    if (CCApplication::sharedApplication() &&
        (GameManager::get()->m_playLayer || GameManager::get()->m_levelEditorLayer)) {
        log::error("Cannot exit in PlayLayer or LevelEditorLayer!");
        return;
    }

    class Exit : public CCObject {
    public:
        void shutdown() {
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wobjc-method-access"
            [[[NSClassFromString(@"AppControllerManager") sharedInstance] controller] shutdownGame];
#pragma clang diagnostic pop
        }
    };

    CCDirector::get()->getActionManager()->addAction(CCSequence::create(
        CCDelayTime::create(0.5f),
        CCCallFunc::create(nullptr, callfunc_selector(Exit::shutdown)),
        nullptr
    ), CCDirector::get()->getRunningScene(), false);
}

void geode::utils::game::restart() {
    if (CCApplication::sharedApplication() &&
        (GameManager::get()->m_playLayer || GameManager::get()->m_levelEditorLayer)) {
        log::error("Cannot restart in PlayLayer or LevelEditorLayer!");
        return;
    }

    auto restart = +[] {
        log::info("Restarting game...");
        auto gdExec = dirs::getGameDir() / "MacOS" / "Geometry Dash";

        NSTask *task = [NSTask new];
        [task setLaunchPath: [NSString stringWithUTF8String: gdExec.string().c_str()]];
        [task launch];
    };

    std::atexit(restart);
    exit();
}

void geode::utils::game::launchLoaderUninstaller(bool deleteSaveData) {
    log::error("Launching Geode uninstaller is not supported on macOS");
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

float geode::utils::getDisplayFactor() {
    float displayScale = 1.f;
    if ([[NSScreen mainScreen] respondsToSelector:@selector(backingScaleFactor)]) {
        NSArray* screens = [NSScreen screens];
        for (int i = 0; i < screens.count; i++) {
            float s = [screens[i] backingScaleFactor];
            if (s > displayScale)
                displayScale = s;
        }
    }
    return displayScale;
}