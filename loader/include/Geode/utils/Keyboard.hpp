#pragma once

#include <Geode/loader/Event.hpp>
#include "Geode/cocos/robtop/keyboard_dispatcher/CCKeyboardDelegate.h"

namespace geode {
    /** Called every time a key is pressed. */
    class GEODE_DLL KeyDispatchEvent final : public Event {
    protected:
        cocos2d::enumKeyCodes m_key;
        bool m_down;
        bool m_repeat;

    public:
        KeyDispatchEvent(cocos2d::enumKeyCodes key, bool down, bool repeat);
        cocos2d::enumKeyCodes getKey() const;
        bool isDown() const;
        bool isRepeat() const;
    };

    /** Called when modifier keys change. */
    class GEODE_DLL ModifierKeyEvent final : public Event {
    protected:
        bool m_shift;
        bool m_ctrl;
        bool m_alt;
        bool m_cmd;

    public:
        ModifierKeyEvent(bool shift, bool ctrl, bool alt, bool cmd);
        bool shiftPressed() const;
        bool controlPressed() const;
        bool altPressed() const;
        bool commandPressed() const;
    };
}
