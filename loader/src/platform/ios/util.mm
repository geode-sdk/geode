
#include <Geode/DefaultInclude.hpp>

#ifdef GEODE_IS_IOS

USE_GEODE_NAMESPACE();

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

#endif
