#include <Geode/utils/platform.hpp>

#ifdef GEODE_IS_MACOS

USE_GEODE_NAMESPACE();

#include <iostream>
#include <sstream>
#include <Cocoa/Cocoa.h>

bool utils::clipboard::write(std::string const& data) {
	[[NSPasteboard generalPasteboard] clearContents];
	[[NSPasteboard generalPasteboard] setString:[NSString stringWithUTF8String: data.c_str()] forType:NSPasteboardTypeString];

    return true;
}

std::string utils::clipboard::read() {
	auto clipboard = [[[NSPasteboard generalPasteboard] stringForType: NSPasteboardTypeString] UTF8String];
	if (!clipboard) return std::string("");
	return std::string(clipboard);
}

ghc::filesystem::path utils::dirs::geodeRoot() {
	char cwd[PATH_MAX];
	getcwd(cwd, sizeof(cwd));
	// utils::clipboard::write(cwd);
	return ghc::filesystem::path(cwd);
}

bool utils::dirs::openFolder(ghc::filesystem::path const& path) {
	NSURL* fileURL = [NSURL fileURLWithPath: [NSString stringWithUTF8String: path.string().c_str()]];
	NSURL* folderURL = [fileURL URLByDeletingLastPathComponent];
	[[NSWorkspace sharedWorkspace] openURL: folderURL]; 
	return true;
}


void utils::web::openLinkInBrowser(std::string const& url) {
    [[NSWorkspace sharedWorkspace] openURL:[NSURL URLWithString:[NSString stringWithUTF8String: url.c_str()]]];
}

#endif
