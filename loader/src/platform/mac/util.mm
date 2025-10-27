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
#include <string.h>

#define CommentType CommentTypeDummy
#import <Cocoa/Cocoa.h>
#undef CommentType


NSString* intoNS(std::string const& str) {
    return [NSString stringWithUTF8String:str.c_str()];
}


bool utils::clipboard::write(std::string const& data) {
    [[NSPasteboard generalPasteboard] clearContents];
    [[NSPasteboard generalPasteboard] setString:intoNS(data)
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
    NSURL* folderURL = [NSURL fileURLWithPath:intoNS(utils::string::pathToString(path))];
    [[NSWorkspace sharedWorkspace] openURL:folderURL];
    return true;
}

void utils::web::openLinkInBrowser(std::string const& url) {
    [[NSWorkspace sharedWorkspace]
        openURL:[NSURL URLWithString:intoNS(url)]];
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
+(void) dispatchFilePickerWithMode:(file::PickMode)mode options:(file::FilePickOptions const&)options multiple:(bool)mult onCompletion:(void(^)(FileResult&&))onCompletion;
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
        auto path = options.defaultPath.value();

        if (std::filesystem::is_directory(path) || mode == file::PickMode::OpenFolder) {
            auto defaultPath = intoNS(options.defaultPath.value());
            [panel setDirectoryURL: [NSURL fileURLWithPath: defaultPath]];
        } else {
            auto defaultPath = intoNS(options.defaultPath->parent_path());
            auto name = intoNS(options.defaultPath->filename());

            [panel setDirectoryURL: [NSURL fileURLWithPath: defaultPath]];
            [panel setNameFieldStringValue: name];
        }
    }

    // title
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

        if (options.filters.size() > 0) {
            NSMutableArray* allowedFileTypes = [NSMutableArray new];
            for (auto& filter : options.filters) {
                for (auto& ext : filter.files) {
                    if (ext.size() > 2 && strncmp(ext.c_str(), "*.", 2) == 0)
                        [allowedFileTypes addObject: intoNS(ext.substr(2))];
                    else
                        [allowedFileTypes addObject: intoNS(ext)];
                }
            }
            [openPanel setAllowedFileTypes: allowedFileTypes];
        }
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

+(void) dispatchFilePickerWithMode:(file::PickMode)mode options:(file::FilePickOptions const&)options multiple:(bool)mult onCompletion:(void(^)(FileResult&&))onCompletion {
    file::FilePickOptions optionsCopy = options;
    dispatch_async(dispatch_get_main_queue(), ^{
        auto result = [self filePickerWithMode:mode options:optionsCopy multiple:mult];
        onCompletion(std::move(result));
    });
}

@end

GEODE_DLL Task<Result<std::filesystem::path>> file::pick(file::PickMode mode, file::FilePickOptions const& options) {
    using RetTask = Task<Result<std::filesystem::path>>;
    return RetTask::runWithCallback([mode, options](auto resultCallback, auto progress, auto cancelled) {
        [FileDialog dispatchFilePickerWithMode:mode options:options multiple:false onCompletion: ^(FileResult&& result) {
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
        [FileDialog dispatchFilePickerWithMode: file::PickMode::OpenFile options:options multiple:true onCompletion: ^(FileResult&& result) {
            if (cancelled()) {
                resultCallback(RetTask::Cancel());
            } else {
                resultCallback(std::move(result));
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

std::filesystem::path dirs::getResourcesDir() {
    return dirs::getGameDir() / "Resources";
}

void geode::utils::game::exit(bool save) {
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

        void shutdownNoSave() {
            std::exit(0); // i don't know if this is the best
        }
    };

    CCDirector::get()->getActionManager()->addAction(CCSequence::create(
        CCDelayTime::create(0.5f),
        CCCallFunc::create(nullptr, save ? callfunc_selector(Exit::shutdown) : callfunc_selector(Exit::shutdownNoSave)),
        nullptr
    ), CCDirector::get()->getRunningScene(), false);
}

void geode::utils::game::exit() {
    exit(true);
}

void geode::utils::game::restart(bool save) {
    if (CCApplication::sharedApplication() &&
        (GameManager::get()->m_playLayer || GameManager::get()->m_levelEditorLayer)) {
        log::error("Cannot restart in PlayLayer or LevelEditorLayer!");
        return;
    }

    auto restart = +[] {
        log::info("Restarting game...");
        auto gdExec = dirs::getGameDir() / "MacOS" / "Geometry Dash";

        NSTask *task = [NSTask new];
        [task setLaunchPath: intoNS(utils::string::pathToString(gdExec))];
        [task launch];
    };

    std::atexit(restart);
    exit(save);
}

void geode::utils::game::restart() {
    restart(true);
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

bool geode::utils::permission::getPermissionStatus(Permission permission) {
    return true; // unimplemented
}

void geode::utils::permission::requestPermission(Permission permission, std::function<void(bool)> callback) {
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

std::string geode::utils::getEnvironmentVariable(const char* name) {
    auto result = std::getenv(name);
    return result ? result : "";
}

std::string geode::utils::formatSystemError(int code) {
    return strerror(code);
}

cocos2d::CCRect geode::utils::getSafeAreaRect() {
    auto winSize = cocos2d::CCDirector::sharedDirector()->getWinSize();
    return cocos2d::CCRect(0.0f, 0.0f, winSize.width, winSize.height);
}

bool cocos2d::CCImage::saveToFile(const char* pszFilePath, bool bIsToRGB) {
    uint8_t* data = m_pData;
    bool usePNG = std::string_view(pszFilePath).ends_with(".png");
    int channels = !usePNG || bIsToRGB || !m_bHasAlpha ? 3 : 4;
    if (channels == 3) {
        data = new uint8_t[m_nWidth * m_nHeight * 3];
        for (uint32_t i = 0; i < m_nWidth * m_nHeight; i++) {
            data[i * 3] = m_pData[i * 4];
            data[i * 3 + 1] = m_pData[i * 4 + 1];
            data[i * 3 + 2] = m_pData[i * 4 + 2];
        }
    }

    CGDataProviderRef provider = CGDataProviderCreateWithData(nullptr, data, m_nWidth * m_nHeight * channels, nullptr);
    if (!provider) {
        if (data != m_pData) {
            delete[] data;
        }
        return false;
    }

    CGColorSpaceRef colorSpace = CGColorSpaceCreateDeviceRGB();
    if (!colorSpace) {
        CGDataProviderRelease(provider);
        if (data != m_pData) {
            delete[] data;
        }
        return false;
    }

    CGImageRef imageRef = CGImageCreate(
        m_nWidth,
        m_nHeight,
        8,
        channels * 8,
        m_nWidth * channels,
        colorSpace,
        channels == 4 ? kCGImageAlphaPremultipliedLast : kCGImageAlphaNone,
        provider,
        nullptr,
        false,
        kCGRenderingIntentDefault
    );
    if (!imageRef) {
        CGDataProviderRelease(provider);
        CGColorSpaceRelease(colorSpace);
        if (data != m_pData) {
            delete[] data;
        }
        return false;
    }

    CFStringRef file = CFStringCreateWithCString(kCFAllocatorDefault, pszFilePath, kCFStringEncodingUTF8);
    if (!file) {
        CGImageRelease(imageRef);
        CGDataProviderRelease(provider);
        CGColorSpaceRelease(colorSpace);
        if (data != m_pData) {
            delete[] data;
        }
        return false;
    }

    CFURLRef url = CFURLCreateWithFileSystemPath(kCFAllocatorDefault, file, kCFURLPOSIXPathStyle, false);
    CFRelease(file);
    if (!url) {
        CGImageRelease(imageRef);
        CGDataProviderRelease(provider);
        CGColorSpaceRelease(colorSpace);
        if (data != m_pData) {
            delete[] data;
        }
        return false;
    }

    CGImageDestinationRef destination = CGImageDestinationCreateWithURL(url, usePNG ? kUTTypePNG : kUTTypeJPEG, 1, nullptr);
    CFRelease(url);
    if (!destination) {
        CGImageRelease(imageRef);
        CGDataProviderRelease(provider);
        CGColorSpaceRelease(colorSpace);
        if (data != m_pData) {
            delete[] data;
        }
        return false;
    }

    CGImageDestinationAddImage(destination, imageRef, nullptr);
    bool success = CGImageDestinationFinalize(destination);
    CFRelease(destination);
    CGImageRelease(imageRef);
    CGDataProviderRelease(provider);
    CGColorSpaceRelease(colorSpace);
    if (data != m_pData) {
        delete[] data;
    }
    return success;
}
