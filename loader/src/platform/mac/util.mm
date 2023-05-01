
#include <Geode/DefaultInclude.hpp>

#ifdef GEODE_IS_MACOS

using namespace geode::prelude;

#include <Geode/loader/Dirs.hpp>
#import <AppKit/AppKit.h>
#include <Geode/utils/web.hpp>
#include <Geode/utils/file.hpp>
#include <Geode/utils/cocos.hpp>

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

bool utils::file::openFolder(ghc::filesystem::path const& path) {
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
+ (Result<ghc::filesystem::path>)importDocumentWithMode:(file::PickMode)mode options:(file::FilePickOptions const&)options mult:(bool)mult;
+ (Result<std::vector<ghc::filesystem::path>>)importDocumentsWithOptions:(file::FilePickOptions const&)options;
@end

@implementation FileDialog

+ (Result<ghc::filesystem::path>)importDocumentWithMode:(file::PickMode)mode options:(file::FilePickOptions const&)options mult:(bool)mult {
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
        return Ok(ghc::filesystem::path(path));
    } else {
        return Err(result);
    }
}

@end*/


@interface FileDialog : NSObject
+(Result<std::vector<ghc::filesystem::path>>) filePickerWithMode:(file::PickMode)mode options:(file::FilePickOptions const&)options multiple:(bool)mult;
@end

@implementation FileDialog
+(Result<std::vector<ghc::filesystem::path>>) filePickerWithMode:(file::PickMode)mode options:(file::FilePickOptions const&)options multiple:(bool)mult {
    NSOpenPanel* panel = [NSOpenPanel openPanel];

    // allowed files
    NSMutableArray* allowed = [NSMutableArray array];

    for (auto& f : options.filters) {
        for (auto& i : f.files) {
            auto nsstr = [NSString stringWithUTF8String: i.c_str()];

            if (![allowed containsObject: nsstr])
                [allowed addObject: nsstr];
        }
    }

    if (options.filters.size())
        [panel setAllowedFileTypes: allowed];

    // multiple
    [panel setAllowsMultipleSelection: mult];

    // default path
    if (options.defaultPath) {
        auto defaultPath = [NSString stringWithUTF8String:options.defaultPath->c_str()];
        [panel setDirectoryURL: [NSURL URLWithString: defaultPath]];
    }

    // other
    [panel setCanChooseDirectories: NO];
    [panel setCanChooseFiles: YES];

    // run thing

    int result = [panel runModal];

    if (result == NSModalResponseOK) {
        std::vector<ghc::filesystem::path> fileURLs;

        for (NSURL* i in panel.URLs) {
            fileURLs.push_back(std::string(i.path.UTF8String));
        }

        return Ok(fileURLs);
    } else {
        return Err("File picker cancelled");
    }
}
@end

Result<ghc::filesystem::path> utils::file::pickFile(
    file::PickMode mode, file::FilePickOptions const& options
) {
    auto result = [FileDialog filePickerWithMode:mode options:options multiple: false];

    if (result.isOk()) {
        return Ok<ghc::filesystem::path>(std::move(result.unwrap()[0]));
    } else {
        return Err<>(result.unwrapErr());
    }
}

Result<std::vector<ghc::filesystem::path>> utils::file::pickFiles(
    file::FilePickOptions const& options
) {
    //return Err("utils::file::pickFiles is not implemented");
    return [FileDialog filePickerWithMode: file::PickMode::OpenFile options:options multiple:true];
}

CCPoint cocos::getMousePos() {
    auto frame = NSApp.mainWindow.frame;
    auto scaleFactor = CCPoint(CCDirector::get()->getWinSize()) / ccp(frame.size.width, frame.size.height);
    auto mouse = [NSEvent mouseLocation];
    return ccp(mouse.x - frame.origin.x, mouse.y - frame.origin.y) * scaleFactor;
}

ghc::filesystem::path dirs::getGameDir() {
    std::array<char, PATH_MAX> gddir;

    uint32_t out = PATH_MAX;
    _NSGetExecutablePath(gddir.data(), &out);

    ghc::filesystem::path gdpath = gddir.data();
    auto currentPath = gdpath.parent_path().parent_path();    
    return currentPath;
}

#endif
