
#include <Geode/DefaultInclude.hpp>

#ifdef GEODE_IS_MACOS

USE_GEODE_NAMESPACE();

    #include <Cocoa/Cocoa.h>
    #include <functional>
    #include <iostream>
    #include <sstream>
    #include <Geode/utils/web.hpp>
    #include <Geode/utils/file.hpp>

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

ghc::filesystem::path utils::file::geodeRoot() {
    char cwd[PATH_MAX];
    getcwd(cwd, sizeof(cwd));
    // utils::clipboard::write(cwd);
    return ghc::filesystem::path(cwd);
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

// @interface FileDialog : NSObject
// + (Result<ghc::filesystem::path>)importDocumentWithMode:(file::PickMode)mode
// options:(file::FilePickOptions const&)options;
// + (Result<std::vector<ghc::filesystem::path>>)importDocumentsWithOptions:(file::FilePickOptions
// const&)options
// @end

// @implementation FileDialog

// + (Result<ghc::filesystem::path>)importDocumentWithMode:(file::PickMode)mode
// options:(file::FilePickOptions const&)options { 	NSOpenPanel* panel = [NSOpenPanel
// openPanel];
// 	// TODO: [panel setAllowedFileTypes:@[]];

// 	auto defaultPath = [NSString stringWithCString:options.defaultPath.c_str()
// encoding:NSUTF8StringEncoding]; 	[panel setDirectoryURL: [NSURL fileURLWithPath:
// defaultPath]]; 	[panel beginWithCompletionHandler:^(NSInteger result){ 		if (result ==
// NSFileHandlingPanelOKButton) { 			auto fileUrl = [[panel URLs] objectAtIndex:0]; 			auto path =
// std::string([[fileUrl path] UTF8String], [[fileUrl path]
// lengthOfBytesUsingEncoding:NSUTF8StringEncoding]); 			return Ok(path);
// 		}
// 		else {
// 			return Err(result);
// 		}
// 	}];
// }

// @end

Result<ghc::filesystem::path> utils::file::pickFile(
    file::PickMode mode, file::FilePickOptions const& options
) {
    return Err<std::string>("utils::file::pickFile is not implemented");
    // return [FileDialog importDocumentWithMode:mode options:options];
}

Result<std::vector<ghc::filesystem::path>> utils::file::pickFiles(
    file::FilePickOptions const& options
) {
    return Err("utils::file::pickFiles is not implemented");
    // return [FileDialog importDocumentWithOptions:options];
}

#endif
