#pragma once

#include <Geode/cocos/robtop/keyboard_dispatcher/CCKeyboardDelegate.h>

namespace geode {
    // Remaps key codes from 2.208 and before
    cocos2d::enumKeyCodes migrateOldKeyCode(int code);
}
