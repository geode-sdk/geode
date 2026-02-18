#pragma once
#include <Geode/cocos/robtop/keyboard_dispatcher/CCKeyboardDelegate.h>
#include <Geode/loader/Event.hpp>
#include <matjson.hpp>

namespace geode {
    struct KeyboardModifier {
        enum : uint8_t {
            None    = 0,

            Shift   = 1 << 0,
            Control = 1 << 1,
            Alt     = 1 << 2,
            Super   = 1 << 3,
        };

        constexpr KeyboardModifier() = default;
        constexpr KeyboardModifier(uint8_t value) : value(value) {}

        constexpr operator uint8_t() const { return value; }
        constexpr operator int() const { return value; }
        constexpr operator bool() const { return value != None; }

        constexpr KeyboardModifier operator&(uint8_t other) const {
            return KeyboardModifier(value & other);
        }

        constexpr KeyboardModifier operator|(uint8_t other) const {
            return KeyboardModifier(value | other);
        }

        constexpr KeyboardModifier operator~() const {
            return KeyboardModifier(~value);
        }

        constexpr KeyboardModifier& operator|=(uint8_t other) {
            value |= other;
            return *this;
        }

        constexpr KeyboardModifier& operator&=(uint8_t other) {
            value &= other;
            return *this;
        }

        constexpr bool operator==(KeyboardModifier other) const {
            return value == other.value;
        }

        constexpr bool operator!=(KeyboardModifier other) const {
            return value != other.value;
        }

        constexpr bool operator==(uint8_t other) const {
            return value == other;
        }

        constexpr bool operator!=(uint8_t other) const {
            return value != other;
        }

        uint8_t value = None;
    };

    struct KeyboardInputData final {
        enum class Action : uint8_t {
            Press,
            Release,
            Repeat,
        };

        struct Native final {
            uint64_t code;  // Windows: vKey
            uint64_t extra; // Windows: scancode
        };

        Native native;
        double timestamp;
        cocos2d::enumKeyCodes key;
        Action action;
        KeyboardModifier modifiers = KeyboardModifier::None;

        KeyboardInputData(cocos2d::enumKeyCodes key, Action action, Native native, double timestamp, KeyboardModifier mods) noexcept
            : native(native), timestamp(timestamp), key(key), action(action), modifiers(mods) {}
    };

    struct KeyboardInputEvent final : GlobalEvent<KeyboardInputEvent, bool(KeyboardInputData&), cocos2d::enumKeyCodes> {
        using GlobalEvent::GlobalEvent;
    };

    struct MouseInputData final {
        enum class Action {
            Press,
            Release,
        };

        enum class Button {
            Left,
            Right,
            Middle,
            Button4,
            Button5,
        };

        Button button;
        Action action;
        double timestamp;
        KeyboardModifier modifiers = KeyboardModifier::None;

        // having this here is incredibly silly but oh well
        inline static cocos2d::enumKeyCodes buttonToKeyCode(Button button) {
            switch (button) {
                // These ones aren't representable as keycodes
                default:
                case Button::Left: return cocos2d::enumKeyCodes::KEY_None;
                case Button::Right: return cocos2d::enumKeyCodes::KEY_None;
                case Button::Middle: return cocos2d::enumKeyCodes::KEY_None;

                case Button::Button4: return cocos2d::enumKeyCodes::MOUSE_4;
                case Button::Button5: return cocos2d::enumKeyCodes::MOUSE_5;
            }
        }

        MouseInputData(Button button, Action action, double timestamp, KeyboardModifier mods) noexcept
            : timestamp(timestamp), button(button), action(action), modifiers(mods) {}
    };

    struct MouseInputEvent final : Event<MouseInputEvent, bool(MouseInputData&)> {
        using Event::Event;
    };

    struct MouseMoveEvent final : Event<MouseMoveEvent, bool(int32_t, int32_t)> {
        using Event::Event;
    };

    struct ScrollWheelEvent final : Event<MouseMoveEvent, bool(double, double)> {
        using Event::Event;
    };

    struct Keybind final {
        using Modifiers = KeyboardModifier;

        static constexpr Modifiers Mods_None = Modifiers::None;
        static constexpr Modifiers Mods_Shift = Modifiers::Shift;
        static constexpr Modifiers Mods_Control = Modifiers::Control;
        static constexpr Modifiers Mods_Alt = Modifiers::Alt;
        static constexpr Modifiers Mods_Super = Modifiers::Super;

        cocos2d::enumKeyCodes key = cocos2d::KEY_None;
        Modifiers modifiers = Mods_None;

        Keybind() = default;
        Keybind(cocos2d::enumKeyCodes key, Modifiers modifiers) noexcept
            : key(key), modifiers(modifiers) {}

        static GEODE_DLL Result<Keybind> fromString(std::string_view str);
        GEODE_DLL std::string toString() const;
        GEODE_DLL cocos2d::CCNode* createNode() const;

        bool operator==(Keybind const& other) const noexcept {
            return key == other.key && modifiers == other.modifiers;
        }
    };
}

template <>
struct matjson::Serialize<geode::Keybind> {
    static GEODE_DLL geode::Result<geode::Keybind> fromJson(matjson::Value const& value);
    static GEODE_DLL matjson::Value toJson(geode::Keybind const& value);
};
