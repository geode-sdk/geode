#include "load.hpp"
#include <Geode/platform/cplatform.h>

#ifdef GEODE_IS_MACOS

#include <AppKit/AppKit.h>
#include <loader/LoaderImpl.hpp>
#include <loader/LogImpl.hpp>
#include <CoreGraphics/CoreGraphics.h>
#include <Cocoa/Cocoa.h>

bool safeModeCheck() {
    if (CGEventSourceKeyState(kCGEventSourceStateHIDSystemState, (CGKeyCode)56)) { // 56 is Shift key
        NSAlert *alert = [NSAlert new];
        alert.messageText = @"The shift key was held down. Would you like to enable safe mode?";
        [alert addButtonWithTitle:@"Yes"];
        NSButton *cancelButton = [alert addButtonWithTitle:@"No"];
        alert.window.defaultButtonCell = cancelButton.cell;
        NSModalResponse choice = [alert runModal];
        // if Yes is clicked
        return choice == NSAlertFirstButtonReturn;
    }
    return false;
}

void checkPermissionPlist() {
    std::thread([] {
        // Load the plist and check for microphone key
        std::string plistPath = (dirs::getGameDir() / "Info.plist").string();
        NSString* nsPlistPath = [NSString stringWithUTF8String:plistPath.c_str()];

        NSMutableDictionary* plist = [NSMutableDictionary dictionaryWithContentsOfFile:nsPlistPath];
        if (![plist objectForKey:@"NSMicrophoneUsageDescription"]) {
            // If the microphone key doesn't exist, create it
            [plist setObject:@"A mod you installed is requesting microphone access" forKey:@"NSMicrophoneUsageDescription"];
            [plist writeToFile:nsPlistPath atomically:YES];
        }
    }).detach(); 
}
#endif