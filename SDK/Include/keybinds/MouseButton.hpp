#pragma once

namespace geode {
    enum MouseButton {
        kMouseButtonNone        = -1,
        kMouseButtonLeft        = 0,
        kMouseButtonRight       = 1,
        kMouseButtonMiddle      = 2,
        kMouseButtonPrev        = 3,
        kMouseButtonNext        = 4,
        kMouseButtonDoubleClick = 19,
        kMouseButtonScrollUp    = 20,
        kMouseButtonScrollDown  = 21,
    };

    constexpr const char* mouseToString(MouseButton btn) {
        switch (btn) {
            case kMouseButtonNone: return "";
            case kMouseButtonLeft: return "Left Click";
            case kMouseButtonDoubleClick: return "Double Click";
            case kMouseButtonRight: return "Right Click";
            case kMouseButtonMiddle: return "Middle Click";
            case kMouseButtonNext: return "Mouse Forward";
            case kMouseButtonPrev: return "Mouse Back";
            case kMouseButtonScrollUp: return "Scroll Up";
            case kMouseButtonScrollDown: return "Scroll Down";
        }
        return "Mouse Unk";
    }
}

