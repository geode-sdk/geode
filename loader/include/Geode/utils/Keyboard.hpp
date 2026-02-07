#pragma once
#include <Geode/cocos/robtop/keyboard_dispatcher/CCKeyboardDelegate.h>
#include <Geode/loader/Event.hpp>

namespace geode {
    struct KeyboardInputData final {
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

        KeyboardInputData(cocos2d::enumKeyCodes key, Action action, Native native, double timestamp, Modifiers mods) noexcept
            : native(native), timestamp(timestamp), key(key), action(action), modifiers(mods) {}
    };

    struct KeyboardInputEvent final : GlobalEvent<KeyboardInputEvent, bool(KeyboardInputData&), cocos2d::enumKeyCodes> {
        using GlobalEvent::GlobalEvent;
    };

    constexpr KeyboardInputData::Modifiers operator|(KeyboardInputData::Modifiers a, KeyboardInputData::Modifiers b) {
        return static_cast<KeyboardInputData::Modifiers>(static_cast<uint8_t>(a) | static_cast<uint8_t>(b));
    }

    constexpr KeyboardInputData::Modifiers operator&(KeyboardInputData::Modifiers a, KeyboardInputData::Modifiers b) {
        return static_cast<KeyboardInputData::Modifiers>(static_cast<uint8_t>(a) & static_cast<uint8_t>(b));
    }

    constexpr KeyboardInputData::Modifiers& operator|=(KeyboardInputData::Modifiers& a, KeyboardInputData::Modifiers b) {
        a = a | b;
        return a;
    }

    constexpr KeyboardInputData::Modifiers& operator&=(KeyboardInputData::Modifiers& a, KeyboardInputData::Modifiers b) {
        a = a & b;
        return a;
    }

    constexpr KeyboardInputData::Modifiers operator~(KeyboardInputData::Modifiers a) {
        return static_cast<KeyboardInputData::Modifiers>(~static_cast<uint8_t>(a));
    }

    struct MouseInputData final {
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

        MouseInputData(Button button, Action action, double timestamp) noexcept
            : button(button), action(action), timestamp(timestamp) {}
    };

    struct MouseInputEvent final : SimpleEvent<MouseInputEvent, MouseInputData&> {
        using SimpleEvent::SimpleEvent;
    };

    struct MouseMoveEvent final : SimpleEvent<MouseMoveEvent, int32_t, int32_t> {
        using SimpleEvent::SimpleEvent;
    };

    struct ScrollWheelEvent final : SimpleEvent<MouseMoveEvent, double, double> {
        using SimpleEvent::SimpleEvent;
    };
}