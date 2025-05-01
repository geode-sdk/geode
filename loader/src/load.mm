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

#endif