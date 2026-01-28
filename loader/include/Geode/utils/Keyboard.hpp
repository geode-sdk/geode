#pragma once
#include <Geode/cocos/robtop/keyboard_dispatcher/CCKeyboardDelegate.h>
#include <Geode/loader/Event.hpp>

namespace geode {
    struct KeyboardInputEvent final : Event {
        enum class Action : uint8_t {
            Press,
            Release,
            Repeat
        };

        enum Modifiers : uint8_t {
            Mods_None    = 0,

            Mods_Shift   = 1 << 0,
            Mods_Control = 1 << 1,
            Mods_Alt     = 1 << 2,
            Mods_Super   = 1 << 3,
        };

        struct Native final {
            uint64_t code;  // Windows: vKey
            uint64_t extra; // Windows: scancode
        };

        Native native;
        double timestamp;
        cocos2d::enumKeyCodes key;
        Action action;
        Modifiers modifiers = Mods_None;

        KeyboardInputEvent(cocos2d::enumKeyCodes key, Action action, Native native, double timestamp, Modifiers mods) noexcept
            : native(native), timestamp(timestamp), key(key), action(action), modifiers(mods) {}
    };

    constexpr KeyboardInputEvent::Modifiers operator|(KeyboardInputEvent::Modifiers a, KeyboardInputEvent::Modifiers b) {
        return static_cast<KeyboardInputEvent::Modifiers>(static_cast<uint8_t>(a) | static_cast<uint8_t>(b));
    }

    constexpr KeyboardInputEvent::Modifiers operator&(KeyboardInputEvent::Modifiers a, KeyboardInputEvent::Modifiers b) {
        return static_cast<KeyboardInputEvent::Modifiers>(static_cast<uint8_t>(a) & static_cast<uint8_t>(b));
    }

    constexpr KeyboardInputEvent::Modifiers& operator|=(KeyboardInputEvent::Modifiers& a, KeyboardInputEvent::Modifiers b) {
        a = a | b;
        return a;
    }

    constexpr KeyboardInputEvent::Modifiers& operator&=(KeyboardInputEvent::Modifiers& a, KeyboardInputEvent::Modifiers b) {
        a = a & b;
        return a;
    }

    constexpr KeyboardInputEvent::Modifiers operator~(KeyboardInputEvent::Modifiers a) {
        return static_cast<KeyboardInputEvent::Modifiers>(~static_cast<uint8_t>(a));
    }

    struct MouseInputEvent final : Event {
        enum class Action {
            Press,
            Release
        };

        enum class Button {
            Left,
            Right,
            Middle,
            Button4,
            Button5
        };

        Button button;
        Action action;
        double timestamp;

        MouseInputEvent(Button button, Action action, double timestamp) noexcept
            : button(button), action(action), timestamp(timestamp) {}
    };

    struct MouseMoveEvent final : Event {
        int32_t x;
        int32_t y;

        MouseMoveEvent(int32_t x, int32_t y) noexcept : x(x), y(y) {}
    };

    // TODO: Add controller/touch input events?
}