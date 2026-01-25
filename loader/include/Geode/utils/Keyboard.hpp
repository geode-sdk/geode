#pragma once
#include <Geode/cocos/robtop/keyboard_dispatcher/CCKeyboardDelegate.h>
#include <Geode/loader/Event.hpp>

namespace geode {
    struct KeyboardInputEvent final : Event {
        enum class Action {
            Press,
            Release,
            Repeat
        };

        struct Native final {
            uint64_t code;  // Windows: lParam
            uint64_t extra; // Windows: wParam
        };

        cocos2d::enumKeyCodes key;
        Action action;
        Native native;
        std::chrono::steady_clock::time_point timestamp;

        KeyboardInputEvent(
            cocos2d::enumKeyCodes key, Action action, Native native,
            std::chrono::steady_clock::time_point timestamp
        ) noexcept : key(key), action(action), native(native), timestamp(timestamp) {}
    };

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
        std::chrono::steady_clock::time_point timestamp;

        MouseInputEvent(
            Button button, Action action,
            std::chrono::steady_clock::time_point timestamp
        ) noexcept : button(button), action(action), timestamp(timestamp) {}
    };

    struct MouseMoveEvent final : Event {
        int32_t x;
        int32_t y;

        MouseMoveEvent(int32_t x, int32_t y) noexcept : x(x), y(y) {}
    };

    // TODO: Add controller/touch input events?
}