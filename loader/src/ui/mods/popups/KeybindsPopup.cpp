#include "KeybindsPopup.hpp"

bool KeybindsPopup::init() {
    if (!GeodePopup::init(400, 270))
        return false;
    
    return true;
}

KeybindsPopup* KeybindsPopup::create() {
    auto ret = new KeybindsPopup();
    if (ret->init()) {
        ret->autorelease();
        return ret;
    }
    delete ret;
    return nullptr;
}
